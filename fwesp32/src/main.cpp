#include "WiFi.h"
#include <sstream>

#define MAX_SSID_LENGTH 32

int pool_delay = 200;

void setup() {
    Serial.begin(9600);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    Serial.printf("\nGET WIFI Initialized.\n");
}

char * listNetworks() {
  // scan for nearby networks:
  //Serial.println("** Scan Networks **");
  int numSsid = WiFi.scanNetworks();
  static char buf[MAX_SSID_LENGTH];
  char * retval = (char*)malloc(numSsid*MAX_SSID_LENGTH);
  memset(retval, '\0', sizeof(retval)); 
  if(numSsid > 0){
    for (int i = 0; i < numSsid; ++i) 
    {
        strcat(retval, WiFi.SSID(i).c_str());
        strcat(retval, ",");
        std::ostringstream oss;
        oss << WiFi.RSSI(i);
        std::string str = oss.str();
        strcat(retval,str.c_str());
        if (i != numSsid - 1) {
            strcat(retval, ",");
          }else {
            strcat(retval, "]");
          }
    }
  }
  WiFi.scanDelete(); 
  return retval;
}
void processCommand(String command) {
    command.trim();
    command.toUpperCase();

    //Serial.println("DBG Received command: " + command);

    if (command == "GET_WIFI") {

        Serial.printf("\nRES GET_WIFI: [%s\n", listNetworks());
    }
    else
    {
      Serial.println("ERR Unknown command.");
    }
      
}

void loop() {
    String serialCommand;

    while (Serial.available() > 0) {
        char serialChar = Serial.read();
        serialCommand += serialChar; 

        if (serialChar == '\n') {
            processCommand(serialCommand);
            serialCommand = "";
        }
    }
    delay(pool_delay);
}