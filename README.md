### iot-based-smart-farming-belkacem

# IoT Based Smart Farming  Monitoring & Control System

[[_TOC_]]

## 1- Project Intruction

The goal of this project is to realize an IoT based smart farming monitoring and control system. In other terms, built a smart irrigation system capable of activating the irrigation process,
monitoring and data analyzing some climate conditions (temperature, moisture, color...) from an user interface. This project is intended especially for amateur gardeners and can be improved 
for farming needs in general. 

The project can also be considered as a skeleton representing the major components of Internet of Things:
- **Firmware** (* device or thing): based STM32 NUCLEO-G071RB Bord
- **Gateway**: An **ESP8266** is used
- **Cloud**: the project **back-end** is entirely powered by Amazon Web Services for IoT
- **Analytic** (TODO)
- **User interface**: Web page or Mobile application 

## 2- Initial global scheme
![Semantic description of image](/images/initial-global-scheme.png "Initial global scheme")

## 3- Initial technical scheme
![Semantic description of image](/images/initial-wiring-scheme.png "Initial wiring scheme")
<<<<<<< HEAD

=======
>>>>>>> belkacem

## 4- Firmware
### Hardware components
- **Board**: [STM32 Nucleo-G071rb ](https://www.st.com/en/evaluation-tools/nucleo-g071rb.html)
- [**Breadboard & jumper wires**](https://www.amazon.fr/wire-jumper/s?k=wire+jumper)
- [**RGB sensor TCS34725**](https://www.adafruit.com/product/1334)
- [**DHT11**](https://www.amazon.fr/Module-Capteur-dhumidit%C3%A9-Temp%C3%A9rature-Arduino/dp/B00HCAC95K)
- [**Water Level Sensor**](https://fr.aliexpress.com/item/32705326396.html)
- [**MH sensor series (moisture)**](https://www.amazon.fr/Moisture-contr%C3%B4leur-d%C3%A9tection-darrosage-automatiquement/dp/B072VDXH4K)
- [**RGB Led**](https://www.amazon.fr/Gratuit-KY-016-couleur-Module-Arduino/dp/B00VAPB47M)
- [**12V water pump**](https://www.amazon.fr/gp/product/B07XJ4L81H/ref=ppx_yo_dt_b_asin_title_o00_s00?ie=UTF8&psc=1)
- [**9V Duracell Battery**](https://www.amazon.fr/pile-9v-duracell/s?k=pile+9v+duracell)
- [**Power Supply Module (3.3V & 5V) MB102**](https://www.amazon.fr/s?k=mb102&i=electronics&__mk_fr_FR=%C3%85M%C3%85%C5%BD%C3%95%C3%91&ref=nb_sb_noss_1)

### Software tools
### CubeMX configuration
### Implementation 

## 5- Gateway

## 6- AWS
For this section, we'll need the following steps:
######
### 1. **Sign to the AWS IoT Console**

Go to: [Aws IoT Console ](https://aws.amazon.com/fr/console/)

### 2. **Create thing** 

> AWS IoT is used to communicate with the your board (**Nucleo-G071rb** in my case) as well as process data coming from the device. Data is automatically processed by AWS IoT Rules Engine.
> To connect our device to the AWS IoT (in my case, i used **MQTT** Protocole), at least, we need 4 elements (use case in **GateWay** section):

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

### 3. **Configure rules** 

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

### 4. **Create dynamonDB table(s)**

> I need two tables for this project:

1. **weathermap**: to store weather data fetched from **openweathermap.com**
2. **final_project**: to store sensors data of device the device 
> Go to amozon **Services** and select **Dynamodb**, then **Create a new Table**.
> Give a name table, a **primary key** and **sort key** (optinal). Once created, go to the table presentation and save somewhere the table **ARN**
### 5. **Build lambda functions and API Gateway(s)**

> Three lambda functions were built for this project:

##### 1. **fetchweather-lambda function**: To fetch weather data from openweathermap.com and store the data in dynamoDB (triggered with a periodic AWS **CloudWatch** alarm)
#### Step 1 -Create lambda function-
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

#### Step 2 -Add an inline policy for lambda-
* Select **Authorizations** and press **the created lambda role**
* Press add a **new inline policy**
* Select the **DynamoDB** service, open **write** section and check **PutItem**
* For **add ARN**, go to the dynamodb table presentation and copy/paste the ARN, the press **Add**
* In the next window, give a **name** policy and press **Create policy**
#### Step 3 -CloudWatch to Schedule AWS Lambda
* In the created lambda function, go to the **Designer** section and press ** add trigger**
* 
![Semantic description of image](/images/weather-lambda.png "cloudWatch")

* Select **CloudWatch Events/EventBridge**. For **rule** select **Create a new rule**
* Give a **name** rule and for **Expression of planification**, put the period you want (for me: 1 day)
> In my case, this means that lambda function will be triggered to fetch data from **openweathermap.com** station every **day** 

##### 2. **get-data-lambda-function**: To publish a message action (get data from device) in a desired topic where device is subcribing 
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

##### 3. **irrigate-lambda-function**: The same as the second function, the difference is in the action (here: water pump action). The both are triggered by an API REST (**AWS API GateWay**)
> In lamnbda python code, change **{'command': 'get data\r'}** by **{'command': 'irrigate\r'}** 
> To this function, you can use the same **API Gateway** previously created, ad**a new ressource**, **a new method** and match it with this lambda function
> Do the same as previously in deploying the **API REST**: **API Url** (*https://xxxxxxxxxx.execute-api.eu-west-2.amazonaws.com/irrigate/irrigate*)
> To test your lambda function, go to **aws Services**/**IoT Core**/**Test**/**Subscribe to a topic** ((ex: irrigateTopic)/Invoke the trigger **API Url** and see the results
### 6. **Host dashboard in Aws S3**


## 7- Dashboard
![Semantic description of image](/images/dashboard.png "Initial wiring scheme")

## 8- Android

## 9- Conclusion


