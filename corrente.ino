// based on https://youtu.be/okNECYf2xlY

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <Wire.h>
#include <INA219.h>

INA219 monitor;

// Fill ssid and password with your network credentials
#include "./secrets/secrets.h"

#define R_SHUNT 0.1
#define V_SHUNT_MAX 0.05
#define V_BUS_MAX 16.0
#define I_MAX_EXPECTED 0.5

const char* host = "script.google.com";
const int httpsPort = 443;

WiFiClientSecure client; //create a WiFiClientSecure object

//const char *GScriptId = ""; //ID 

void sendData(float _v_sh, float _i_sh, float _v_bus, float _p_bus);

void setup() {
    Serial.begin(115200);
    delay(500);

    monitor.begin();
    monitor.configure(INA219::RANGE_16V, INA219::GAIN_2_80MV, INA219::ADC_12BIT, INA219::ADC_12BIT, INA219::CONT_SH_BUS);
    monitor.calibrate(R_SHUNT, V_SHUNT_MAX, V_BUS_MAX, I_MAX_EXPECTED);
    delay(500);

    WiFi.begin(ssid,password);
    Serial.println("");

    pinMode(5,OUTPUT);
    digitalWrite(5,LOW);

    Serial.print("Connecting");
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        digitalWrite(5,HIGH);
        delay(250);
        digitalWrite(5,LOW);
        delay(250);
    }
    digitalWrite(5,LOW); //turnoff led after connection stabilished
    Serial.println("");
    Serial.print("Successfully connected to: ");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    client.setInsecure();

}

void loop() {
    float v_bus = monitor.busVoltage();
    float v_sh = monitor.shuntVoltage() * 1000;
    float i_sh = monitor.shuntCurrent() * 1000;    
    float p_bus = monitor.busPower() * 1000;

    // int h = 30;
    // float t = 90.0;

    if(isnan(v_sh)||isnan(i_sh)||isnan(v_bus)||isnan(p_bus)){
        Serial.println("Failed to read");
        delay(500);
        return;
    }

    Serial.println("Ready(?): " + String(monitor.ready()));
    Serial.println("Shunt Voltage: " + String(v_sh, 6) + " mV");
    Serial.println("Shunt Current: " + String(i_sh, 6) + " mA");
    Serial.println("Bus Voltage: " + String(v_bus, 6) + " V");
    Serial.println("Bus Power: " + String(p_bus, 6) + " mW");

    sendData(v_sh, i_sh, v_bus, p_bus);  

    monitor.recalibrate();
    monitor.reconfig();
    delay(10);                          
}

void sendData(float _v_sh, float _i_sh, float _v_bus, float _p_bus){
    Serial.println("=====");
    Serial.print("Connecting to ");
    Serial.println(host);

    if(!client.connect(host,httpsPort)){
        Serial.println("Connection failed.");
        return;
    }

    // String s_Temp = String(tem);
    // String s_Humi = String(hum,DEC);
    // Write to Google Spreadsheet
    String url = String("/macros/s/") + GScriptId + "/exec?v_sh=" + String(_v_sh, 6) + "&i_sh=" + String(_i_sh, 6) + "&v_bus=" + String(_v_bus, 6) + "&p_bus=" + String(_p_bus, 6);
    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");
    Serial.println("Request sent:");
    Serial.println(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "User-Agent: BuildFailureDetectorESP8266\r\n" + "Connection: close\r\n\r\n");

    //Checking wether data was sent successfully or not
    while (client.connected()){
        String line = client.readStringUntil('\n');
        if (line == "\r"){
            Serial.println("headers received");
            break;
        }
    }
    String line = client.readStringUntil('\n');
    if(line.startsWith("{\"state\":\"success\"")){
        Serial.println("esp8266/Arduino CI successfull!");
    } else{
        Serial.println("esp8266/Arduino CI has failed :(");
    }
    Serial.print("Reply was: ");
    Serial.println(line);
    Serial.println("Closing connection");
    Serial.println("=====");
    Serial.println();    

}