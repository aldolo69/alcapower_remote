#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "ssid_wifi"
#define STAPSK  "pass_wifi"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

//gpio0(d3) 2(d4) 15(d8) with pullup. do not use
const int switchPin = D1; //drives button. low at startup
const int redLedInPin = D2;//high on red led on
const int greenLedInPin = D5;//high on red led on
const int blinkingLedPin = D0;//high=led off

//be sure NOT to use pins with an external pullup!!!!


void handleToggle() {
  digitalWrite(switchPin, 1);
  delay(4000);
  digitalWrite(switchPin, 0);

  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

void handleRoot() {


  String strColR;
  String strColG;

  strColG = digitalRead(greenLedInPin) == 0 ? "003000" : "00FF00";
  strColR = digitalRead(redLedInPin) == 0 ? "300000" : "FF0000";

  server.send(200, "text/html",
              "\
<html>\
<head>\
<meta http-equiv=\"Content-Type\"\
content=\"text/html; charset=utf-8\">\
<title>INVERTER REMOTE</title>\
<style>\
.button {\
  background-color: #202020;\
  border:  5px solid white;\
  color: white;\
  padding: 30px;\
  text-align: center;\
  text-decoration: none;\
  display: inline-block;\
  font-size: 3em;\
  border-radius: 50%;\
  margin: 8px 8px;\
}\
.table {\
 font-size: 3em;\
 font-family: sans-serif;\
 text-align: center;\
 color: white\
}\
.p {\
 font-size: 6em;\
}\
</style>\
<script language=\"JavaScript\">\
function toggle()\
  {\
if(true==confirm(\"Press button?\")) window.location.href = '/toggle';\
  }\
</script>\
</head>\
<body bgcolor=\"#000000\">\
<table class =\"table\" border=\"0\" width=\"100%\">\
    <tr>\
        <td colspan=\"2\"><p>LED(Green) - INVERTER ON</p></td>\
    </tr>\
    <tr>\
        <td colspan=\"2\"><p>LED(Red) - INVERTER FAULT</p></td>\
    </tr>\
    <tr>\
        <td width=\"50%\"><p class =\"p\"><font color=\"#" + strColG + "\" size=\"9em\">&#x2B24;</font></p></td>\
        <td width=\"50%\"><p class =\"p\"><font color=\"#" + strColR + "\" size=\"9em\">&#x2B24;</font></p></td>\
    </tr>\
    <tr>\
        <td  width=\"50%\"><p>POWER</font></p></td>\
        <td  width=\"50%\"><p>FAULT</font></p></td>\
    </tr>\
    <tr>\
        <td colspan=\"2\"><p><button class=\"button\" onclick=\"toggle()\">0/1</button> </p></td>\
    </tr>\
    <tr>\
        <td colspan=\"2\">\
        <p align=\"center\">INVERTER REMOTE</p>\
        <p>CONTROL ON/OFF SWITCH</p>\
        </td>\
     </tr>\
</table>\
</body>\
</html>\
"
             );
}

void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}


void blinkLed(int stat)
{
  static int ledStat = 1;
  if (stat == 0)
  {
    ledStat = 1;
    digitalWrite(blinkingLedPin, 1);
  }
  else {
    ledStat = ledStat == 0 ? 1 : 0;
    digitalWrite(blinkingLedPin, ledStat);
  }
}


void waitWatchdog(void)
{
  while (1) {};
}



void setup(void) {
  pinMode(blinkingLedPin, OUTPUT);
  pinMode(switchPin, OUTPUT);
  pinMode(redLedInPin, INPUT);
  pinMode(greenLedInPin, INPUT);

  digitalWrite(switchPin, 0);
  digitalWrite(blinkingLedPin, 1);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  int iCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    blinkLed(1);
    iCount++;
    if (iCount == 50) waitWatchdog();

  }
  blinkLed(0);

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.on("/toggle", handleToggle);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();

  if (WiFi.status() != WL_CONNECTED) waitWatchdog(); //wait for the watchdog


}
