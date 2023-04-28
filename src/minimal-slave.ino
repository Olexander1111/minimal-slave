#define MY_HOSTNAME "remote"
#define MY_VERSION "20230428:204101"
#define MY_DEBUG true

#include <myArduino.cpp>
#include <myEspNow.cpp>
#include <myTimestamp.cpp>


extern "C" {
  #include "eboot_command.h"
  #include "user_interface.h"
  #include "ets_sys.h"
  #include "os_type.h"
  #include "osapi.h"
  #include "mem.h"
  #include "cont.h"
  extern uint32_t _SPIFFS_start;
}

//===================================POWER=LEVEL=SETTINGS===============================
#define WIFI_TX_POWER 15 // TX power of ESP module (0 -> 0.25dBm) (0...85)
#define WIFI_TX_MAX_POWER 55
//========================================================================================



bool ledState;
void myMessageHandler(myMessage *msg) {
    myDebugger::print(msg->topic);
    myDebugger::print(msg->data);

     if (myEspNow::isControlMatch("light/set", msg)) {
        int reset=atoi(msg->data);
        //if(reset==69){
        Serial.print("got TOPIC is: "); 
        ledState = !ledState;
        digitalWrite(2,ledState);
        char buffer[8];
        snprintf(buffer, sizeof buffer, "%d",ledState);
        //myEspNow::send("master", "control/master/report/remote", buffer);
        //myDebugger::print("reset THIS NODE");
        //ESP.restart();
        //}
    }
      if (myEspNow::isControlMatch("power/set", msg)) {
        int power=atoi(msg->data);
        system_phy_set_max_tpw(power);
        char buffer[8];
        snprintf(buffer, sizeof buffer, "%d",power);
        //myEspNow::send("master", "control/master/power/value", buffer);
        }
    
 
    //Serial.println(msg->data);
    const char* topic=msg->topic;
      Serial.print("TOPIC is: ");Serial.println(topic);
    const char* value=msg->data;
      Serial.print("VALUE is: ");Serial.println(value);
    int state=atoi(value);
    if (state==1)
    {
     //   digitalWrite(D4,1);
        Serial.println("SEND TO MASTER");
        //myEspNow::send("master", "test/topic", "good job");
    }
     if (state==2)
    {
     //   digitalWrite(D4,0);
        Serial.println("SEND TO DOMOTICZ");
        //myEspNow::send("master", "domoticz/in", "test");
    }

}

void setup() {

    ESP.eraseConfig();
    system_phy_set_max_tpw(WIFI_TX_POWER);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("SMART_NODE", "12345678", 1);

 sprintf(CONFIG_ESPNOW_PASSWORD, "%s","ju5tAp@$$w0rd");    

    pinMode(2,OUTPUT);
    myArduino::init();
    myEspNow::setHostname(MY_HOSTNAME);
    //myEspNow::setSSID(CONFIG_ESPNOW_SSID);
    myEspNow::setSSID("easySMARThome");
    myEspNow::setMasterName(CONFIG_ESPNOW_MASTER_NAME);
    //myEspNow::setChannel(CONFIG_ESPNOW_CHANNEL);
    myEspNow::setChannel(1);
    myEspNow::init();
    myEspNow::setMessageHandler(myMessageHandler);


    
}

myTimer every5sec(5);

void loop() {
    myEspNow::loop();
 
    if(every5sec.check()) {
     //myEspNow::send("master", "control/master/light/set", "69");
     //Serial.println("SEND TO MASTER 69");
    }

}   
