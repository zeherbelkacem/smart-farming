#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <secret_keys.h>
extern "C" {
#include "libb64/cdecode.h"
}
#include<string.h>

#define SOFTWBUFFCAPACITY 128

//SoftwareSerial swSer(14, 12, false, 256);
SoftwareSerial espRxTx(13, 15);// The esp8266 RX TX used to match with the nucleo bord 
void callback(char* topic, byte* payload, unsigned int len);// to get message from AWS
void connectToAws(); // to connect esp8266 to AWS core (mqtt protocol)
void connectToWifi(); // connect esp8266 to the local wifi network
String getRxdata(const char * request); //to get sensor data from Nucleo board 
void setCurrentTime(); // to be informed on the certification validation...
void sendDatatoAws(String jsonData); // post data in json format to aws dynamodb
int b64decode(String b64Text, uint8_t* output);
WiFiClientSecure wiFiClient; //WIFI Client
PubSubClient pubSubClient(AWS_IOT_ENDPOINT, 8883, callback, wiFiClient); //MQTT Client

const char *requestPost = "get data\r"; //post request to the nucleo board
String rxData = ""; //get rxData as response (sensor data) from nucleo board 

/*****==================================================================*****/
void setup() {
  Serial.begin(115200); 
  espRxTx.begin(115200);
  //s1.begin(SWSERIAL_5N1)
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
  //if(//request from aws){
  rxData = getRxdata(requestPost);
  sendDatatoAws(rxData);
  //}
  delay(2000); 
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
      pubSubClient.connect("dht11");
    }
    Serial.println(" connected");
    pubSubClient.subscribe(MQTT_PUB_TOPIC);
  }
  pubSubClient.loop();
}

/*****==================================================================*****/
String getRxdata(const char * request)
{
  String resp = "";
  espRxTx.write(request); //write a post to TX_PIN to get data
  while(espRxTx.available()>0) //
  {
    resp += (char)espRxTx.read();//read() read from RX_PIN  character by character 
    //Serial.write((char)espRxTx.read());
  }
  Serial.println(resp);
  Serial.println(strlen(resp.c_str()));
  return resp;

}

/*****==================================================================*****/
void sendDatatoAws(String jsonData)
{ 
  //The model we got from nucleo UART
 // String jsonData = "{\"Id\": 4, \"PT\": 602, \"Dt\": {\"AH\": 49.00, \"Tp\": 20.00, 
  //\"Ms\": 56.00, \"Clr\": red, \"Gs\": 32.00, \"WL\": empty, \"RGB\": [0,0,0]}}"; 
  
  DynamicJsonDocument doc(1024); //Allocate Json document 
  deserializeJson(doc, jsonData); //Deserialize the jsonData input (here a String)
  //Fetch values
  int Id = doc["Id"];
  int PT = doc["PT"];
  double AIrHumidity = doc["Dt"]["AH"];
  double Temperat = doc["Dt"]["Tp"];
  double Moistur = doc["Dt"]["Ms"];
  String Colors = doc["Dt"]["Clr"];
  double Gz = doc["Dt"]["Gs"];//Serial.println(Gz);
  String WaterLev = doc["Dt"]["WL"];
  int r = doc["Dt"]["RGB"][0];
  int g = doc["Dt"]["RGB"][1];
  int b = doc["Dt"]["RGB"][2];

  //Json document construction
  JsonObject root = doc.to<JsonObject>();
  root["DeviceId"] = Id;
  root["PosixTime"] = PT; 
  JsonObject Data = root.createNestedObject("Data");
  Data["AirHumidity"] = AIrHumidity; 
  Data["Temperature"] = Temperat; 
  Data["Moisture"] = Moistur; 
  Data["Color"] = Colors; 
  Data["Gas"] = Gz; 
  Data["WaterLevel"] = WaterLev; 
  JsonArray RGB_Color = Data.createNestedArray("RGB_Color");
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

/*****==================================================================*****/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on "); Serial.print(topic); Serial.print(": ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
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