#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <secret_keys.h>
extern "C" {
#include "libb64/cdecode.h"
}
#include<string.h>
#include<stdio.h>
#include<ctime>

#define SOFTWBUFFCAPACITY 128
long int unix_timestamp();
SoftwareSerial espRxTx(13, 15);// The esp8266 RX TX used to match with the nucleo bord 
void callback(char* topic, byte* payload, unsigned int len);// to get message from AWS
void connectToAws(); // to connect esp8266 to AWS core (mqtt protocol)
void connectToWifi(); // connect esp8266 to the local wifi network
const char * getRxdata(const char * request); //to get sensor data from Nucleo board 
void setCurrentTime(); // to be informed on the certification validation...
void sendDatatoAws(const char * jsonData); // post data in json format to aws dynamodb
int b64decode(String b64Text, uint8_t* output);
WiFiClientSecure wiFiClient; //WIFI Client
PubSubClient pubSubClient(AWS_IOT_ENDPOINT, 8883, callback, wiFiClient); //MQTT Client

const char* getData = "get data\r";
const char* irrigate = "irrigate\r";
typedef enum {
	CYCLE,
	REQUEST_DATA,
	IRRIGATE
} STATE_MACHINE;

STATE_MACHINE FLAG = CYCLE;

/*****==================================================================*****/
void setup() {
  Serial.begin(115200); 
  espRxTx.begin(115200);
  Serial.println("ESP8266: Get Nucleo sensor data and post it to AWS IoT Core");

  connectToWifi();
  /*BEGIN: Copied from https://github.com/HarringayMakerSpace/awsiot/blob/master/Esp8266AWSIoTExample/Esp8266AWSIoTExample.ino*/
  setCurrentTime();//  // get current time, otherwise certificates are flagged as expired

  uint8_t binaryCert[AWS_CERT_CRT.length() * 3 / 4];
  int len = b64decode(AWS_CERT_CRT, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[AWS_CERT_PRIVATE.length() * 3 / 4];
  len = b64decode(AWS_CERT_PRIVATE, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[AWS_CERT_CA.length() * 3 / 4];
  len = b64decode(AWS_CERT_CA, binaryCA);
  wiFiClient.setCACert(binaryCA, len);
  /*END: ...*/
}
 

/*****==================================================================*****/
void loop() {
  connectToAws();
  pubSubClient.loop();
  if(espRxTx.available()) {
      char buffer[256]={0};
      size_t count = espRxTx.readBytes(buffer, sizeof(buffer));
      Serial.print(" --- buffer : ");
      Serial.println(buffer);
      sendDatatoAws(buffer);
  }
  else{
    switch (FLAG)
    {
    case REQUEST_DATA:
      espRxTx.write(getData);
      FLAG = CYCLE;
      break;
    case IRRIGATE:
        espRxTx.write(irrigate);
      FLAG = CYCLE;
      break;
    
    default:
      break;
    }
  }
     
}

/*****==================================================================*****/
void connectToWifi(){
  Serial.print("Connecting to "); Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());
}

/*****==================================================================*****/
void connectToAws() {
  if ( ! pubSubClient.connected()) {
    Serial.print("PubSubClient connecting to: "); Serial.print(AWS_IOT_ENDPOINT);
    while ( ! pubSubClient.connected()) {
      Serial.print(".");
      pubSubClient.connect(THINGNAME);
    }
    Serial.println(" connected");
    pubSubClient.subscribe(MQTT_SUB_TOPIC);
  }

  }


/*****==================================================================*****/
const char * getRxdata(const char * request)
{
  const char * resp ;
  size_t len = 0;
  char rxBuffer[256] = {0};
  /*if(espRxTx.availableForWrite())
  {
    espRxTx.write(request, sizeof(request)); //or write(co...) inste
  }*/
  espRxTx.write(request);
  
  if(espRxTx.available()) //
  {
    //read() read from RX_PIN  character by character  // rxBuffer[i] = (char)espRxTx.read(); // i++;
    len = espRxTx.readBytes(rxBuffer, sizeof(rxBuffer));
    Serial.print("len: ");
    Serial.println(len);
    resp = rxBuffer;
    Serial.print("response ");
    Serial.println(rxBuffer);
  }
  Serial.print("rxbuffer ");
    Serial.println(rxBuffer);
    Serial.print("response ");
    Serial.println(resp);
  return resp;

}

/*****==================================================================*****/
void sendDatatoAws(const char * jsonData)
{
 // Serial.print("jsondata ");
 // Serial.println(jsonData);
  
  //The model we got from nucleo UART
  // String jsonData =  {"Id": 6619235, "AH": 60.00, "Tp": 20.00,  "Ms": "out", "Pwr": "full", "WL": "Low","R": "127", "G":85, "B": 42} 

  DynamicJsonDocument doc(1024); //Allocate Json document 
  deserializeJson(doc, jsonData); //Deserialize the jsonData input (here a String)
  //Fetch values
  int DevicId = doc["Id"]; Serial.println(DevicId);
  long AIrHumidity = doc["AH"];Serial.println(AIrHumidity);
  long Temperat = doc["Tp"];Serial.println(Temperat);
  const char* Moistur = doc["Ms"];Serial.println(Moistur);
  const char* Powe =  doc["Pwr"];Serial.println(Powe);
  const char* WaterLev = doc["WL"];Serial.println(WaterLev);
  int r = doc["R"];Serial.println(r);
  int g = doc["G"];Serial.println(g);
  int b = doc["B"];Serial.println(b);
  
  if(DevicId != 0)
  {
    //Json document construction
    JsonObject root = doc.to<JsonObject>();
    root["DeviceId"] = DevicId;
    root["PosixTime"] = unix_timestamp(); 
    JsonObject Data = root.createNestedObject("Data");
    Data["AH"] = AIrHumidity; 
    Data["Tp"] = Temperat; 
    Data["Ms"] = Moistur; 
    Data["Pw"] = Powe; 
    Data["WL"] = WaterLev; 
    JsonArray RGB_Color = Data.createNestedArray("RGB");
    RGB_Color.add(r);
    RGB_Color.add(g);
    RGB_Color.add(b);
  
    Serial.printf("Sending  [%s]: ", MQTT_PUB_TOPIC);
    serializeJson(root, Serial); //Json doc serialisation
    Serial.println();
    char shadow[256];
      
    serializeJson(root, shadow, sizeof(shadow));
    if (!pubSubClient.publish(MQTT_PUB_TOPIC, shadow, false))// send json data to dynamoDbB topic
    Serial.println("ERROR??? :"); Serial.println(pubSubClient.state()); //Connected '0'
  }
  
    
  
}

/*****==================================================================*****/
void callback(char* topic, byte* payload, unsigned int length) {
  String  requestPost = "";
  
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (unsigned int i = 0; i < length; i++) {
    requestPost+=  (char)payload[i];//{"command": "get data\r"}
  }
  Serial.print("from irrigateTopic: ");
  Serial.println(requestPost);

  DynamicJsonDocument doc(128); //Allocate Json document 
  deserializeJson(doc, requestPost); //Deserialize the jsonData input (here a String)
  //Fetch values
  const char* command = doc["command"];
  Serial.print("command: "); Serial.println(command);
  if(strcmp((const char*)command, "irrigate\r") == 0)
    FLAG = IRRIGATE;
  else if(strcmp(command, "get data\r") == 0)
    FLAG = REQUEST_DATA;
  else
    Serial.println("ERROR GATEWAY REQUEST");
  
}

/*****==================================================================*****/
int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

/*****==================================================================*****/
void setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}
long int unix_timestamp()
{
    time_t t = std::time(0);
    long int now = static_cast<long int> (t);
    return now;
}