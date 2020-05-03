

#define THINGNAME "dht11" //the name of the your iot thing
//Topics names
const char MQTT_SUB_TOPIC[] = "iot/topic";//"$aws/things/dht11/shadow/update";
const char MQTT_PUB_TOPIC[] = "iot/topic";//"$aws/things/dht11/shadow/update";
// Local wireless network
const char* WIFI_SSID     = "Livebox-985F";
const char* WIFI_PASSWORD = "CCE3307F9AEE19E35E6E78825E";
//aws mqqt brocker address
const char AWS_IOT_ENDPOINT[] = "a2can1njaaznqt-ats.iot.eu-west-2.amazonaws.com";

// Amazon Root CA 1
const String AWS_CERT_CA = \
//-----BEGIN CERTIFICATE-----
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF"\
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6"\
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL"\
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv"\
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj"\
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM"\
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw"\
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6"\
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L"\
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm"\
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC"\
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA"\
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI"\
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs"\
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv"\
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU"\
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy"\
"rqXRfboQnoZsG4q5WTP468SQvvG5";
//-----END CERTIFICATE-----

// Device Certificate
const String AWS_CERT_CRT = \
//-----BEGIN CERTIFICATE-----
"MIIDWjCCAkKgAwIBAgIVALDM+LOHwOJeaCbo+Qwx+iR0EuZbMA0GCSqGSIb3DQEB"\
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t"\
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDA0MDYxOTE5"\
"MThaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh"\
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDpmaVxrfXJYQVqyE4R"\
"0Z+l1faFLGbK3UoSgPDDzdrXnae0XF0MaVydW7iOgcvzxNoHvXiXnzRRNYWFPSGR"\
"+HqfpBd8KMEKio3cWeywj0il6X52g9wcy65ZCm3md1H2Qp7dLJP0tmftyCWohbd2"\
"SUAfRyM/sZ1T2c2YSJrzVxsF65x6f7lWFTsGJVryrtXHTQIZDNmfeilVPeZDz+dt"\
"ZQkh0xtM5yOigIGFsaewZWKGbxMFPN77lLHy88EYiuJPNV3NeCQpBEwBRI1A97Zn"\
"sfRizkDnk7I+IPiEJhHZfh39CpopU/Un0RLkD9RuAMnbMV8NNoVtbRLWc7DkqPgf"\
"t4iNAgMBAAGjYDBeMB8GA1UdIwQYMBaAFFiFrerpG6/gr3Sp3Lxvy1li/2AiMB0G"\
"A1UdDgQWBBTfLm6zHJyL+785EcpBmpBLQ88MMzAMBgNVHRMBAf8EAjAAMA4GA1Ud"\
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAX/PEgLuCbkWuJQXJQed653Sl"\
"Io7viW3wfrucKq8P2hHUurfOOJ1NBbJSdyaKIuCg9TWTPaJHyPmwQg5T+iRP2tGt"\
"EFD2EmRn6CxyFWkrfpnaIUcEgRA0KAFtKdMWpbybmyKHljKNtG2RxNA2mIxXcUpQ"\
"pvhjqDRES+/vl1CMM1spzd1H350PraGFPhHP7CWveBkHFUV4hbhDlXDGUaSu4c6k"\
"CmAbwZo+gE4ftE0fbCmw9KpPHJRqXnlPb1xL02IDkKrwMZI+gYHCw2S+SlBU4XVA"\
"ZZlkAulFdXw8RNHnr7f2CIG62/gPCS1nQlzbTYQjWe3uaskE67eZxz4gYaBdGw==";
//-----END CERTIFICATE-----


// Device Private Key
const String AWS_CERT_PRIVATE = \
//-----BEGIN RSA PRIVATE KEY-----
"MIIEpQIBAAKCAQEA6Zmlca31yWEFashOEdGfpdX2hSxmyt1KEoDww83a152ntFxd"\
"DGlcnVu4joHL88TaB714l580UTWFhT0hkfh6n6QXfCjBCoqN3FnssI9Ipel+doPc"\
"HMuuWQpt5ndR9kKe3SyT9LZn7cglqIW3dklAH0cjP7GdU9nNmEia81cbBeucen+5"\
"VhU7BiVa8q7Vx00CGQzZn3opVT3mQ8/nbWUJIdMbTOcjooCBhbGnsGVihm8TBTze"\
"+5Sx8vPBGIriTzVdzXgkKQRMAUSNQPe2Z7H0Ys5A55OyPiD4hCYR2X4d/QqaKVP1"\
"J9ES5A/UbgDJ2zFfDTaFbW0S1nOw5Kj4H7eIjQIDAQABAoIBAHi/ZvmABlB1yisM"\
"Ju6nAHLkSm1EKk4qXKN76vgLbFePsqRUKfTEcY8VXiyrgxUaZ+kgUzlsncpsc/wP"\
"QU8GtbvXQhx3pHEidXm1MdeB+Pdp3mi8eAaaciCsd2ptuvwkK6H/7OrNbqNGldCq"\
"bc7CxlZkmx/NW5mAN/nDbfTizaEcz//AGGVlYkDseWibW0nPyBNVyPVll1NtEmfa"\
"iEA5IqLgnuirAvbImvpveD3fbW8Fz1ROLMmJltG/X5sYcoOJ3utw/kMFqbHtFhNh"\
"xhsQT6VkN3JZ395AgkGEI4JHNTpCnvgKqkZU5fOHZfD7c98cnfMZrtvkVvDFzbYn"\
"qlQ/tYECgYEA+l2KDH3lCHXE24ucSlUA/LiSNEf3eOXlPm2gu/RP3wh+THURuVW1"\
"o/ldnUp9/KmHKv3RJ+zBWjk/fJrlSfM0NGzPalh8f719Hc1rm8DodtsvORw635Qo"\
"kCG0EgiLDLMBRZizQIeeOOx60MdzzAzbJ2iU0KUcqMKjUFQOVgr4iRUCgYEA7tuD"\
"/KpCN0rVCe3K2E6ilFTMDB9y3+KSb/+uGI9KKZ1+RGO+eZyoZG6n/lO1jq6ptba7"\
"LQofTXuwsL+ajMCCZcNS1eVrIu5k+6zMz48hbts7i0vlbSCkZo46OJidSNZDyCLj"\
"XuwTrzEBP80capOpJ5gevoEDOJ28+3utCcsn75kCgYEAyZe76qcMXi2F30mhBPFB"\
"JDzx7IruJzjlx8i6Hd1w2qOaHrI9HUB4S5ok2Nv8ag+POPy7ByzJ234xMoH+kfb0"\
"WCotMPIi/TB0qCSZuwYvqFSAx7mFZjWEG9onhkW+wPD52zJDU3hf3HH9ylJvWSQY"\
"6rh46y+nzfM7URj9mianTKkCgYEA4vypCUujcXgjzDO2OKlBx0RuRHb+a9Sc5OJN"\
"R/mjuLh6bfN754YAYLDn5MEYIGfLz2u/pKWh6bhJyS3MaUXopVxhM/O5RJDYVnN4"\
"jyat1vNwOof9lx8WHH+8xFwD3FwCPgcpm+MtMaeYUFFTV9TLQ11EmjVwIjWEiliV"\
"sGDnqsECgYEAnd0IYAJ71YzhxeSHo5NSuxnNnzAIDB3/BSDEAga7zgp/zgoDE9GC"\
"JLfez314SrKVVb9RAtAyfuL0M0Ou7fkhqQmMSWASrX76u+fvoIfdrD21ow2cyftM"\
"21ueb+wwe5HNW7XY/pXBDoKjbZmI/y3G8i5Agl6AzLUXHKlO0/RxAb0=";
//-----END RSA PRIVATE KEY-----