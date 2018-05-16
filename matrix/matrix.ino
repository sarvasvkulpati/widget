#include <SPI.h>
#include "LedMatrix.h"


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "dlink-E9AC";
const char* password = "npayi40545";
 
#define NUMBER_OF_DEVICES 1
#define CS_PIN D4
LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);

ESP8266WebServer server(80);
void setup() {
  Serial.begin(9600);
  

WiFi.begin(ssid, password);  



  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

    
WiFi.localIP();
server.on("/", handleRoot);
server.on ("/save", handleSave);
server.begin();         


  
  ledMatrix.init();
  
  ledMatrix.setIntensity(4); // range is 0-15
  ledMatrix.setText("The quick brown fox jumps over the lazy dog");
  
}
char htmlResponse[3000];

void handleRoot() {
  snprintf( htmlResponse, 3000,
"<!DOCTYPE html>"
"<html lang=\"en\">"
  "<head>"
    "<meta charset=\"utf-8\">"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
 "</head>"
 "<body>"
          "<h1 >Set Text</h1>"
          "<input type='text' name='text' id='text' size=2 autofocus style=\"width:100px;\"> text "
          "<div>"
          "<br><button id=\"save_button\">Save</button>"
          "</div>"
    "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js\"></script>"   
    "<script>"
      "var text;"
    
      "$('#save_button').click(function(e){"
        "e.preventDefault();"
        "text = $('#text').val();"
       
        "$.get('/save?text=' + text, function(data){"
          "console.log(data);"
        "});"
      "});"    
    "</script>"
  "</body>"
"</html>");
server.send ( 200, "text/html", htmlResponse ); 
}

 
void loop() 
{


  
  server.handleClient(); 




  
  ledMatrix.clear();
    ledMatrix.setRotation(true);
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit();
  delay(50);
}

void handleSave() {
  if (server.arg("text")!= ""){
    Serial.println("text: " + server.arg("text"));
    ledMatrix.setText(server.arg("text"));
  }
}
