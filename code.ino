#define MODE_MANUAL 0
#define MODE_AUTO   1

#include "WiFi.h"

#define MAX_SSID_LENGTH 32

int ledPin = 23;
int ledChannel = 0;
int ledValue = 0;
int ledMode = MODE_MANUAL;

int ldrPin = 36;
int ldrMax = 4000;

int thresholdValue = 50;

int enabled = 0;
int pool_delay = 200;

typedef struct {
  char ssid[MAX_SSID_LENGTH];
  int rssi;
} Scan;

Scan *scan = NULL;

void setup() {
    Serial.begin(9600);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    pinMode(LED_BUILTIN, OUTPUT);
    for (int i=9; i>=0; i--) {
        digitalWrite(LED_BUILTIN, i % 2 ? HIGH : LOW);
        delay(50);
    }

    ledcSetup(1, 5000, 8);
    ledcAttachPin(LED_BUILTIN, 1); // Led Builtin aceita PWM no ESP32
    
    pinMode(ledPin, OUTPUT);
    ledcSetup(ledChannel, 5000, 8);
    ledcAttachPin(ledPin, ledChannel);
    
    Serial.printf("DBG SmartLamp Initialized.\n");
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

    ledUpdate();

    delay(pool_delay);
}

void changeLedInt(int ledTmp) {
  ledValue = ledTmp;
  ledMode  = MODE_MANUAL;
  ledUpdate();

  // Liberar o ponteiro scan somente se estiver alocado
//  if (scan != NULL) {
//    free(scan);
//    scan = NULL; // Define o ponteiro como NULL para evitar desalocação dupla
//  }
}

void processCommand(String command) {
    command.trim();
    command.toUpperCase();

    // Serial.println("DBG Received command: " + command);

    if (command.startsWith("SET_LED ")) {
        int ledTmp = command.substring(8).toInt();
        if (ledTmp >= 0 && ledTmp <= 100) {
            changeLedInt(ledTmp);
            Serial.printf("RES SET_LED 1\n");
        }
        else {
            Serial.printf("RES SET_LED -1\n");
        }
    }

    else if (command == "GET_LDR") {
      int n = WiFi.scanNetworks();
//      Scan *scan;
//      scan = (Scan *) malloc(n*sizeof(Scan));
//      if (n > 0) {
//        for (int i = 0; i < n; ++i) {
//          scan[i].rssi = WiFi.RSSI(i);
//          strncpy(scan[i].ssid, WiFi.SSID(i).c_str(), MAX_SSID_LENGTH - 1);
//          scan[i].ssid[MAX_SSID_LENGTH - 1] = '\0';
//          delay(10);
//        }
//      }
        Serial.printf("[");  
        for (int i = 0; i < n; ++i) {
          Serial.printf("%s", WiFi.SSID(i).c_str());
          Serial.printf(",");  
          Serial.printf("%d", WiFi.RSSI(i));
          if (i != n - 1) {
            Serial.printf(",");
          }
        }
        Serial.printf("]\n");
//        free(scan);
        WiFi.scanDelete(); 
    }

    else if (command == "GET_LED")
      Serial.printf("RES GET_LED %d\n", ledValue);
      
    else if (command.startsWith("SET_THRESHOLD ")) {
        int thresholdTmp = command.substring(14).toInt();
        if (thresholdTmp >= 0 && thresholdTmp <= 100) {
            thresholdValue = thresholdTmp;
            ledMode = MODE_AUTO;
            ledUpdate();
            Serial.printf("RES SET_THRESHOLD 1\n");
        }
        else {
            Serial.printf("RES SET_THRESHOLD -1\n");
        }
    }

    else if (command.startsWith("ENABLED ")) {
        int enableValue = command.substring(8).toInt();

        if (enableValue == 0) {
            enabled = enableValue;
            changeLedInt(0);
            Serial.printf("RES ENABLED 0\n");
        }
        else if(enableValue == 1){
            enabled = enableValue;
            changeLedInt(10);
            Serial.printf("RES ENABLED 1\n");
        }
        else {
            Serial.printf("RES ENABLED -1\n");
        }
    }

    else if (command.startsWith("POLL_DELAY ")) {
        int poll_value = command.substring(11).toInt();

        if (poll_value >= 100) {
            pool_delay = poll_value;
            Serial.printf("RES POLL_DELAY %d\n", poll_value);
        }
        else {
            Serial.printf("RES POLL_DELAY -1\n");
        }
    }
    
    else if (command == "GET_THRESHOLD")
      Serial.printf("RES GET_THRESHOLD %d\n", thresholdValue);
    
    else
      Serial.println("ERR Unknown command.");
      
}


void ledUpdate() {
    if (ledMode == MODE_MANUAL || (ledMode == MODE_AUTO && ldrGetValue() < thresholdValue)) {
        ledcWrite(ledChannel, 255.0*(ledValue/100.0));
        ledcWrite(1, 255.0*(ledValue/100.0));
    }
    else {
        ledcWrite(ledChannel, 0);
        ledcWrite(1, 0);
    }
}

int ldrGetValue() {
    int ldrAnalog = analogRead(ldrPin);
    int ldrValue = 100*ldrAnalog/ldrMax;

    // Serial.printf("DBG LDR_MAX=%d, LDR_ANALOG=%d, LDR_VALUE=%d\n", ldrMax, ldrAnalog, ldrValue);

    return ldrValue > 100 ? 100 : ldrValue;
}
