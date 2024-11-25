/*
  -> Author: Owen Hartson
  -> Description: Simple Website Server for ESP32
  -> Notes: reading from an html file
  -> Pin Connections for SD Card:
      +---------------------+------------+
      | SD Card Reader Pins | ESP32 Pins |
      +---------------------+------------+
      | VCC                 | 5V         |
      | GND                 | GND        |
      | CS                  | G5         |
      | SCK                 | G18        |
      | MISO                | G19        |
      | MOSI                | G23        |
      +---------------------+------------+
*/

/*============================== LIBRARIES ==============================*/
//  -> WiFi.h         : for connecting to local wifi network
//  -> WebServer.h    : for creating a web server
//  -> SPI.h          : for providing access to the Serial Peripheral Interface
//  -> SD.h           : for using the sd card module (sd card must be <= 32Gb)
/*=======================================================================*/
#include <WiFi.h>
#include <WebServer.h>
#include <SPI.h>
#include <SD.h>


/*==================== FORWARD FUNCTION DECLERATIONS ====================*/
/*=======================================================================*/
void ErrorHandler(String error);
void HandleOnConnect();
String SendHTML();
String ReadFile(const char * path);
// void handle_led1on();
// void handle_led1off();


/*============================ CONST VARIABLES ==========================*/
/*=======================================================================*/
const char* ssid = "";  // name of local wifi network
const char* password = ""; // wifi password
const int CS = 5; // cs pin for sd card reader


/*================================ OBJECTS ==============================*/
/*=======================================================================*/
WebServer server(80); // establish a web server object on port 80
File myFile;  // create a file object for sd card reading


/*=========================== GLOBAL VARIABLES ==========================*/
/*=======================================================================*/
// uint8_t LED1pin = 4;
// bool LED1status = LOW;


/*================================= SETUP ===============================*/
/*=======================================================================*/
void setup(){
    Serial.begin(115200);
    delay(1000);

    // pinMode(LED1pin, OUTPUT);


    // #################### WIFI COMPONENTS ####################
    WiFi.mode(WIFI_STA); // optional
    WiFi.begin(ssid, password);
    Serial.print("\nConnecting");

    uint8_t i = 0;
    while(WiFi.status() != WL_CONNECTED){
        if(i <= 25){
          Serial.print(".");
          i++;
        }else{
          Serial.println();
          i = 0;
        }
        delay(100);
    }

    Serial.println("\n========= WIFI STATUS ==========");
    Serial.println("Connected to the WiFi network");
    Serial.println("================================\n");
    // #########################################################


    // ################# IP ADDRESS COMPONENTS #################
    Serial.println("========== IP ADDRESS ==========");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("================================\n");
    // #########################################################


    // ################ HTTP ROUTING COMPONENTS ################
    server.on("/", HandleOnConnect);
    // server.on("/led1on", handle_led1on);
    // server.on("/led1off", handle_led1off);
    // #########################################################


    // ################### SERVER COMPONENTS ###################
    server.begin();
    Serial.println("========= SERVER STATUS ========");
    Serial.println("HTTP Server Started");
    Serial.println("================================\n");
    // #########################################################


    // ################## SD CARD COMPONENTS ###################
    while(!Serial){;}
    Serial.println("Initializing SD card...");
    if(!SD.begin(CS)){
      ErrorHandler("SD card initilization failed...");
      return;
    }
    Serial.println("initialization done.");
    // #########################################################
}


/*================================ LOOP =================================*/
/*=======================================================================*/
void loop(){
  server.handleClient();

  // if(LED1status){
  //   digitalWrite(LED1pin, HIGH);
  // }else{
  //   digitalWrite(LED1pin, LOW);
  // }
}


/*========================== HANDLE ON CONNECT ==========================*/
/*=======================================================================*/
void HandleOnConnect(){
  server.send(200, "text/html", SendHTML(false));

  return;
}

// void handle_led1on() {
//   LED1status = HIGH;
//   Serial.println("GPIO4 Status: ON");
//   server.send(200, "text/html", SendHTML(true)); 
// }

// void handle_led1off() {
//   LED1status = LOW;
//   Serial.println("GPIO4 Status: OFF");
//   server.send(200, "text/html", SendHTML(false)); 
// }


/*============================== SEND HTML ==============================*/
/*=======================================================================*/
String SendHTML(uint8_t led1stat){
  String websiteCode = ReadFile("/website.html");

  return websiteCode;
}


/*============================== READ FILE ==============================*/
/*=======================================================================*/
String ReadFile(const char * path){
  String fileStuff = "";

  myFile = SD.open(path); // open the file for reading
  if(myFile){
    Serial.printf("Reading file from %s\n", path);
    while (myFile.available()) {  // read from the file until there's nothing else in it:
      // Serial.write(myFile.read());
      char c = myFile.read();
      fileStuff.concat(c);
    }
    myFile.close(); // close the file:
  }else{
    ErrorHandler("Not able to open file..."); // if the file didn't open, print an error:
  }

  return fileStuff;
}


/*============================ ERROR HANDLER ============================*/
//  -> Handles internal error messages as strings and prints them to
//      the serial monitor.
/*=======================================================================*/
void ErrorHandler(String error){
  String beginningOfError = "/////////////ERROR//////////////";
  String endOfError       = "////////////////////////////////";
  Serial.println();
  Serial.println(beginningOfError);
  Serial.println(error);
  Serial.println(endOfError);

  return;
}