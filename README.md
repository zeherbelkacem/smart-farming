# IoT Based Smart Farming  Monitoring & Control System

[[_TOC_]]

## 1- Project Introduction

The goal of this project is to realize an IoT based smart farming monitoring and control system. In other terms, built a smart irrigation system capable of activating the irrigation process,
monitoring and data analyzing some climate conditions (temperature, moisture, color...) from an user interface. This project is intended especially for amateur gardeners and can be improved 
for farming needs in general. 

The project can also be considered as a **skeleton** representing the major components of Internet of Things (and can be adapted for other projects using ESP8266 as a Gateway):
- **Firmware** (* device or thing): based STM32 NUCLEO-G071RB Bord
- **Gateway**: An **ESP8266** is used
- **Cloud**: the project **back-end** is entirely powered by Amazon Web Services for IoT
- **Analytic** (TODO)
- **User interface**: Web page or Mobile application 

## 2- Global scheme
![Semantic description of image](/images/initial-global-scheme.png "Initial global scheme")

## 3- Technical scheme (wiring)
![Semantic description of image](/images/initial-wiring-scheme.png "Initial wiring scheme")

## 4- Firmware

> All the firmware was coded in **C language***

### 4.1. Hardware components

- **Board**: [STM32 Nucleo-G071rb ](https://www.st.com/en/evaluation-tools/nucleo-g071rb.html)
- [**Breadboard & jumper wires**](https://www.amazon.fr/wire-jumper/s?k=wire+jumper)
- [**RGB sensor TCS34725**](https://www.adafruit.com/product/1334)
- [**DHT11**](https://www.amazon.fr/Module-Capteur-dhumidit%C3%A9-Temp%C3%A9rature-Arduino/dp/B00HCAC95K)
- [**Water Level Sensor**](https://fr.aliexpress.com/item/32705326396.html)
- [**MH sensor series (moisture)**](https://www.amazon.fr/Moisture-contr%C3%B4leur-d%C3%A9tection-darrosage-automatiquement/dp/B072VDXH4K)
- [**RGB Led**](https://www.amazon.fr/Gratuit-KY-016-couleur-Module-Arduino/dp/B00VAPB47M)
- [**12V water pump**](https://www.amazon.fr/gp/product/B07XJ4L81H/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
- [**9V Duracell Battery**](https://www.amazon.fr/pile-9v-duracell/s?k=pile+9v+duracell)
- [**KY-019 Relay**](https://www.amazon.fr/WINGONEER-KY-019-Bouclier-module-darduino/dp/B06XHJ2PBJ)
- [**Power Supply Module (3.3V & 5V) MB102**](https://www.amazon.fr/s?k=mb102&i=electronics&__mk_fr_FR=%C3%85M%C3%85%C5%BD%C3%95%C3%91&ref=nb_sb_noss_1)

### 4.2. Software tools
- [**Stm32CubeIde for Ubuntu**](https://www.st.com/en/development-tools/stm32cubeide.html)
- [**CuteCom Console for Ubuntu**](https://help.ubuntu.com/community/Cutecom)

### 4.3. CubeMX configuration
**Pinout Configuration**

![Semantic description of image](/images/pinout-configuration.png "pinout configuration")

**Clock Configuration**

![Semantic description of image](/images/clock-configuration.png "clokc configuration")

### 4.5. Implementation 
<p>I'll just show how to implement the STM32 ADC Multi-channels use and the <strong>12V pump  driver</strong> </br>
For the rest, I'll let you take a look at the <strong>/device-chef-d-oeuvre/stm32-chef-d-oeuvre</strong> directory</p>

### 4.5.1 ADC Multi-channels method (PollforConversion)
<p>We need to set up the channels we want to read under ADC (the alone ADC1 of Nucleo). Here We will read Channels IN0 (water level)and channel IN1 (moisture state).</p>

* Set AD1 configuration, see the last configuration section
* Start ADC: `HAL_ADC_Start(&hadc1);`
* Read water level input:
```
HAL_ADC_PollForConversion(&hadc1, 100);
waterLevelInput = HAL_ADC_GetValue(&hadc1);
```
* Read moisture input: 
```
HAL_ADC_PollForConversion(&hadc1, 100);
moistureInput = HAL_ADC_GetValue(&hadc1);
```
* Stop ADC: `HAL_ADC_Stop(&hadc1);`

### 4.5.2. Pump action driver
> Just pay attention to the inverted logic when using the stm32 to write to the gpio outputs (0V, 5V)

```
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 0 ); //put GPIO_PIN_3 to 5V because inverse logic
...
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1 );//put GPIO_PIN_3 to 0V because inverse logic
```
### 4.5 The final firmware STATE MACHINE
```
/* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  switch (FLAG) {
	  	  case PERIODIC_DATA:
	  		  dbg_log("periodic data %lu\n", HAL_GetTick() - periodic_time );
	  		  periodic_time = HAL_GetTick();
	  		  readSend_all_sensor_data();
	  		  FLAG = CYCLE;//wait
		  break;
		  case IRRIGATE:
			  pump_action_start();
			  timer_start(&SINGLE_HANDLER);
			  start_time = HAL_GetTick();
			  //dbg_log(" START TIME %lu", start_time);
			  FLAG = CYCLE;
			  break;
		  case STOP_PUMP:
			 pump_action_stop();
			 timer_stop(&SINGLE_HANDLER);
			 stop_time = HAL_GetTick();
			 //dbg_log(" STOP TIME %lu", stop_time);
			 printf(" PUMP TIME %lu\n", stop_time-start_time);
			 FLAG = CYCLE;
			  break;
		  case REQUEST_DATA:
			  readSend_all_sensor_data();
			  FLAG = CYCLE;
			  break;

	  	  default:
	  		  break;
	  }
  }

  /* USER CODE END 3 */
```


## 5- Gateway
<p>To interface between the device (embedded part) and the cloud (aws), I used WEMOS D1 mini Pro board (ESP8266). It reads and writes (data) on the Nucleo using </br>
the UART protocol (RxTx). To send data to the <strong>cloud</strong> (AWS), a WIFI connection and an <strong>MQTT</strong> (Subscribe & publish) protocol are needed.</br> 
To use <strong>MQTT</strong> protocol with AWS <strong>account</strong>, the provided <strong>security keys</strong> and <strong>Endpoint adress</strong> are also needed </br>
(see the registering device part in the next AWS IoT chapter).</p>

![Semantic description of image](/images/nucleo-esp-aws.png "nucleo esp8266AWS ")

> All the gateway was coded in **C++ language**

### 5.1. Hardware components & Software tools
* [Visual Studio Code (VSCode) for Ubuntu](https://code.visualstudio.com/)
* [PlatformIO for VSCode](https://platformio.org/platformio-ide), to be installed directly on VSCode
* As gateway module (board), [ESP8266: Wemos D1 mini pro](https://www.amazon.fr/Wemos-D1-Mini-Pro-Esp8266-Carte-d%C3%A9veloppement/dp/B06XWDVGJ5) was used (see wiring )

### 5.2. The used Librairies 

| Nucleo-ESP8266 Interfacing                    | ESP8266-AWS IoT Interfacing                    |
| --------------------------------------------- | ---------------------------------------------- |
| **SoftwareSerial**: for UART connection       | **ESP8266WIFI**: WiFi connection               |
| **Arduino**: for serial monitor               | **PubSubClient**:  Mqtt topic subscribe/publish|
|                                               | **ArduinoJson**: JSON format                   |
|                                               | **libb64/decode**: AWS certificats decoding    |
|                                               | **ctime**: Unix time                           | 

### 5.3. Implementation

See the [esp8266-chef-d-ouevre directory](https://gitlab.com/simplon-fad-iot-labege-1/chef-doeuvre/iot-based-smart-farming-belkacem/-/tree/master/device-chef-d-oeuvre/esp8266-chef-d-oeuvre)

### 5.4. Gateway STATE MACHINE
```
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
```

## 6- AWS
<p>For this section, we'll need the following steps:</p>

### 6.1. Sign to the AWS IoT Console

Go to: [Aws IoT Console ](https://aws.amazon.com/fr/console/)

### 6.2. **Create thing** 

<p> AWS IoT is used to communicate with the your board (<strong>Nucleo-G071rb</strong> in my case) as well as process data coming from the device. Data is automatically processed by AWS IoT Rules Engine .</br>
To connect our device to the AWS IoT (in my case, i used <strong>MQTT</strong> Protocole), at least, we need 4 elements (use case in <strong>GateWay</strong> section):</p>

1. Thing Certificate (ends with .pem.crt)
1. Private key security (ends with.key)
1. A root CA key (ends with .pem)
1. An endpoint Url (ends with amazonaws.com) 

Once in your __Aws Console__, go to **Services** and choose **Aws IoT Core**. 

On the left column, proceed as follows:

* **Manage**, **Create**, **Create a single thing** 
* Give a **name** thing ("smart-farming" in my case), add **type** and **group** (optional) ("*arming* and *famningGroup* in my case) and press **Next**
* **Create a Certificat**, download the **certificat** and the **2 keys**: ...cert.pem, ...public.key and ...private.key and save them somewhere in your loacl machine
* Before pressing **activate**, press *download*, in new page download and save the **RSA 2048 bit key: _Amazon Root CA 1_** in the same folder as the previous keys
* Press **Attach a policy** and **Save thing**
>Communication between your device and AWS IoT Core is protected through the use of X.509 certificates. AWS IoT Core can generate a certificate for you or you can use your own X.509 certificate.
In my case, I used AWS IoT Core's X.509 certificate. Certificates must be activated prior to use. 

![Semantic description of image](/images/aws-certificates.png "AWS Certificates")  

* On the left column, press **Security** then **Policy**, **Create**
* Give **name** policy, for _action_: **iot:*, for _ressources ARN_: * and press **Create**
* On the left column, press **Security** then **Certificats**, and choose the certificat previuosly created
* For the certificat, **Attach** the **policy** and the **thing** newly created
* On the left column, go to **manage**/**thing** and chosse your created thing
* In **Details**/**Interact** save somewhere the **endpoint url** to use as an MQTT Brocker for the next steps

![Semantic description of image](/images/aws-mqttbrocker.png "Mqqt Brocker") 
> For more details, see: [Create thing and and register a device ](https://docs.aws.amazon.com/iot/latest/developerguide/create-aws-thing.html)

### 6.3. **Configure rules** 

> The AWS IoT rules engine listens for incoming (from the device) MQTT messages that match a rule. When a matching message is received, the rule takes some action
with the data in the MQTT message (in this project :writing (INSERT) data to an Amazon DynamonDB). In this step, you create and configure a rule to send the data 
received from a device to an Amazon DynamoDB:

* On the left colunm of your IoT Core, go to **Act** and **rules** then **Create**
* Give a **name** rule, and **description** (optional)
* Choose a **Topic** name where the rule is listening (subcribing) when messages comming from the device (publishing)
* In my case, I named it *farming Topic*.  ![Semantic description of image](/images/aws-rule.png "aws-rules")
* Press **Add an action**, choose the first list action **Insert data in dynamodDB Table** and press **Configure an action**
* Select your table, in the **Partion key value** put **${your parttion Key}** and in the **sort key value** put **${your sort key value}**
* For **write message data**, put **payload**, for **operation**, put **INSERT** and press **Create a role**

![Semantic description of image](/images/aws-action.png "aws-action")

* Give a name role, press **Create role**, press **add action** and finaly press **Create rule**
* 
> for more details, see [aws dynamodb rule](https://docs.aws.amazon.com/iot/latest/developerguide/iot-ddb-rule.html)

### 6.4. **Create dynamonDB table(s)**

> I need two tables for this project:

1. **weathermap**: to store weather data fetched from **openweathermap.com**
2. **final_project**: to store sensors data of device the device 
> Go to amozon **Services** and select **Dynamodb**, then **Create a new Table**.
> Give a name table, a **primary key** and **sort key** (optinal). Once created, go to the table presentation and save somewhere the table **ARN**

### 6.5. **Build lambda functions and API Gateway(s)**

> Three lambda functions were built for this project:

##### 6.5.1. **fetchweather-lambda function**: 
> To fetch weather data from openweathermap.com and store the data in dynamoDB (triggered with a periodic AWS **CloudWatch rule** (*alarm*))

**Step 1 -Create lambda function**

* Go to amozon **Services**, select **Lambda**, **Create a function** and by default **Create from zero**
* Give a **name** function, **Runtime** select **Python 3.6**. For **role**, select **Create a new lambda basic execution role** and press **Create a function**
* In **Code entry type**, select **upload a .zip file** (in mu case [lambda.zip](https://gitlab.com/simplon-fad-iot-labege-1/chef-doeuvre/iot-based-smart-farming-belkacem/-/blob/belkacem/device-chef-d-oeuvre/aws/lambda.zip))
* In **Handler Info**, put **fetchweather.lambda_funct**

![Semantic description of image](/images/aws-handler.png "handler function")

* Inside the zip file, fetchweather.py file is used as an API REST (GET method) to fetch station weather data
> To fetch from [openweathermap.com](https://openweathermap.com), you have to **sign in** to get an account associated to an **API key** allowing the python file to call openweathermap.com API.
    - To process the JSON data that we have recovered, we must look at their form as it is returned by Openweathermap
    - The weather forecast can be found in the **List** key. The value associated with this key is a list of 40 items.
    - In each forecast, the date is found by the key **dt**
    - The temperature is accessible by the **main** key then **temp**
    - To process all the temperature forecasts, we will therefore browse each of the 40 elements of the **List** list using a Python loop: `for prev in content ["list"]`
> For more details in order to understand AWS Lambda Deployment Package in Python, see [](https://www.youtube.com/watch?v=rDbxCeTzw_k)

**Step 2 -Add an inline policy for lambda**

* Select **Authorizations** and press **the created lambda role**
* Press add a **new inline policy**
* Select the **DynamoDB** service, open **write** section and check **PutItem**
* For **add ARN**, go to the dynamodb table presentation and copy/paste the ARN, the press **Add**
* In the next window, give a **name** policy and press **Create policy**

**Step 3 -CloudWatch to Schedule AWS Lambda**

* In the created lambda function, go to the **Designer** section and press ** add trigger**
* 
![Semantic description of image](/images/weather-lambda.png "cloudWatch")

* Select **CloudWatch Events/EventBridge**. For **rule** select **Create a new rule**
* Give a **name** rule and for **Expression of planification**, put the period you want (for me: 1 day)
> In my case, this means that lambda function will be triggered to fetch data from **openweathermap.com** station every **day** 

##### 6.5.2. **get-data-lambda-function**: 
> To publish a message action (get data from device) in a desired topic where device is subcribing 

* **Create** a new lambda function, **name** it, **python 3.6** for **Runtime** and **default role** for lambda **role**
* Copy the following python code content and paste it lambda functio code section 
```
import boto3
import json

def lambda_handler(event, context):
        client = boto3.client('iot-data', region_name='eu-west-2')
        
        # Change topic, qos and payload
        response = client.publish(
                topic='irrigateTopic',
                qos=1,
                payload=json.dumps({"command":"irrigate\r"})
            )`
```

* Select **Authorizations** and press **the created lambda role** and in service, select **IoT** 
* Press add a **new inline policy** and in service, select **IoT**
* Open **write** and check **publish** 
* For **ARN**, add the **region** (eu-west-2) and give the name of your desired **topic** (irrigateTopic for me), the press **add**
* In the next window, give a **name** for the policiy and press **add policy**
* To trigger lambda functio, select **API Gateway** and choose **Create API**
* Select **API REST** for the **API Gateway** creation
* Give a **name** API and for **pont Type...**, select **Regional**
* In *Ressources* **actions**, select **create a ressource**, give a **name** (getdata for me) and check **active CORS API Gateway**
* In *ressources* **action**, select **cretae method** and select **ANY**
* Press **ANY** and at the right select **integration request**
* **lambda function** for **integration type**, check **activate proxy**, select your **region**, give the correspondind **lambda function** name and check **use delay...**
* In **Ressouces action**, select **deploy API**, select **new stage** and give a description (**getdata**, for me), then press **deploy**
* In the top page you can see the API Url (something like this for me: **https://xxxxxxxxxx.execute-api.eu-west-2.amazonaws.com/getdata**)
* To trigger the created lambda function, call the the **API Url** (*https://xxxxxxxxxx.execute-api.eu-west-2.amazonaws.com/getdata/getdata*)

![Semantic description of image](/images/apigateway.png "API Gateway trigger")

##### 6.5.3. **irrigate-lambda-function**: 
> The same as the second function, the difference is in the action (here: water pump action). The both are triggered by an API REST (**AWS API GateWay**)

> In lamnbda python code, change **{'command': 'get data\r'}** by **{'command': 'irrigate\r'}** 
> To this function, you can use the same **API Gateway** previously created, ad**a new ressource**, **a new method** and match it with this lambda function
> Do the same as previously in deploying the **API REST**: **API Url** (*https://xxxxxxxxxx.execute-api.eu-west-2.amazonaws.com/irrigate/irrigate*)
> To test your lambda function, go to **aws Services**/**IoT Core**/**Test**/**Subscribe to a topic** ((ex: irrigateTopic)/Invoke the trigger **API Url** and see the results

### 6.6. **Host dashboard in Aws S3**

> In this part, I will create an S3 **Bucket** to host my **dashboard** (see the next chapter). I will need to set some authorizations (policies) and get my the dashboard **Url adress**

#### 6.6.1. Create an AWS S3 Bucket
* Go to AWS Services, select **S3** and press **Create a bucket**
* Give a **name** bucket, select your **region** and press **Next**
* In the step, leave everythings by default and press **Next**
* By default, press **Next** and press **Create a bucket**

#### 6.6.2. Upload Web Site files (Dashboard) in Bucket
* To avoid problems in this step, choose the last **Chrome** version
* In AWS **S3**, press on your created bucket **name**
* Go to your loacl machine where your web site **folder** is saved and open it
* Select all files and subdirectories inside your web side folder
* Drag and drop the selected files from your local machine to the content displayed on the Preview tab of the S3 console.

![Semantic description of image](/images/s3-files.png "buckets files") 

#### 6.6.3. Security
* In S3 console, open **permissions** and press **Modify**
* Unchek the two last points: <<*Block new public bucket policies*>> and <<*Block access to the public and to cross accounts if the sub-fund has public policies*>>
* Press **save** and go to **Bucket policies**
* Past the following policy with changing *BUCKETNAME* with your **bucket name** then press **save**
```
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Principal": "*",
            "Action": "s3:GetObject",
            "Resource": "arn:aws:s3:::BUCKETNAME/*"
        }
    ]
}
```
#### 6.6.4. Get site Endpoint (Url)'

* In S3 console, select **properties** and open **static web site hosting**
* Check **Use this bucket to host your web site**
* For **Index document**, choose your **index.html**, then **save**
* In my case, my endpoint is: http://farmingdashboard.s3-website.eu-west-2.amazonaws.com 

## 7- Dashboard
### 7.1. Software Tools and programming language 
> **Sublime Text**, **HTML**, **CSS**, **JavaScrip**, **Ajax**, **jQuery**, **chart.js**, **nvd3**

### 7.1. The Pool Identity
<p>With an identity pool, users can obtain temporary AWS credentials to access AWS services, such as Amazon S3 and DynamoDB.</p>
<p>In my case, to build my dashboard, I need to have access to my AWS account services (dynamoDB, S3, API Gateway, lambda....</br>
So, an <strong>Identity Pool Id</strong> and a <strong>Region</strong> must be provided (for JAVASCRIPT). Also, a policies (for each service)</br>
must be atteched to the <strong>Unanthentified Groups </strong> <strong>Role</strong>  : </p>

* Go to the **Amazon Cognito Console** and **Create an identity Pool**
* Give a **name pool**, check **acticate access to unanthentified pools** and press **Create a pool**
* Press **Authorize**
* In platform, select **JavaScript** and copy your identifiers (Identity Pool Id & region)
```
// Initialiser le fournisseur d'informations d'identification Amazon Cognito
AWS.config.region = 'eu-west-2'; // Région
AWS.config.credentials = new AWS.CognitoIdentityCredentials({
    IdentityPoolId: 'eu-west-2:xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxx', //Replace with yours 
});
```
* Past the last code in the head your JavaScript **file** (refresh.js in my case)
* Go to **IAM Console** and select **Roles**
* Serach for your **Cognito_YOURGROUPNAMEGrpIdUnauth_Role** and click on it
* Press **add inline policies** and select the concerned services
* In my case, I need policies to access to **dynamoDB services** (GetItems, Query...)

### 7.2. Dashbord components 
![Semantic description of image](/images/dashboard.png "Initial wiring scheme")

## 8- Android

> To be completed

## 9- Small Demonstration 
### 9.1. Demo

[Video Demo](https://youtu.be/_paBueTVGjk)

### How to use the project 

* Download the project (and software tools and components: boards, sensors...)
* For **Firmware** in **Stm32CubeIde**:
- [ ]  Files/Open Project/...device-chef-d-oeuvre/stm32-chef-d-oeuvre
- [ ] - Project/generate code
- [ ] - Project/built project
- [ ] - Run/debug As... (ensure that your NUCLEO is connect in Usb)
* For **GateWay** in **Vscode**
- [ ] - Install **PlatformIo** and in platfomIO install all the **librairies** used in the **GateWay section above**
- [ ] - In platformIO: Open project/...device-chef-d-oeuvre/ESP8266-chef-d-oeuvre
- [ ] - In **secret_keys.h file**, replace all your personnal identifiers (wifi, aws...)
- [ ] - In the bottom blue strip: PlatformIo:Build then PlatformIo:Upload (with connecting your ESP8266 in usb)
* For **AWS**: see the **AWS section** to create account, create a thing, have different identifiers...
* For **Dashboard**: In **refresh.js file**, replace your **region** and **IdentityPoolId**
* Pay attention that the **wiring above** is well done 


