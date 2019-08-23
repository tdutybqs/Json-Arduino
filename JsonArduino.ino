#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "";
const char* password = "";
const String server  = "http://tools.aimylogic.com";
const String url     = server + "/api/googlesheet2json?sheet=1&id=1jgBb8KttBiJ5vyj_j3cFTMlr-TZcYTBanCdFAWPMigw";

String color = "";
String text = "";
String httpData;

void setup() {
   Serial.begin(115200);
   WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
    httpRequest();
    parseData();
    delay(10000);
}

bool httpRequest() {
  HTTPClient client;
  bool find = false;
  client.begin(url);
  int httpCode = client.GET();

  if (httpCode > 0) {
    if (httpCode == HTTP_CODE_OK) {
      httpData = client.getString();
    }
  }
  else Serial.printf("failed, error: %s\n", client.errorToString(httpCode).c_str());
  client.end();
  return find;
}

bool parseData() {
  Serial.println(httpData);

  DynamicJsonBuffer jsonBuffer;
  JsonArray& root = jsonBuffer.parseArray(httpData);		
  // arduinojson.org/v5/faq/parsing-succeeds-but-i-cant-read-the-values/  

  if (!root.success()) {
    Serial.println("Json parsing failed!");
    return false;
  }
  color = root[0]["color"].as<String>();
  text = root[0]["text"].as<String>();

  httpData = "";
  Serial.println("Color is: " + color);
  Serial.println("Text is: " + text);
  return true;
}
