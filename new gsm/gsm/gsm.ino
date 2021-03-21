#include <Ftp.h>
#include <Geo.h>
#include <GPRS.h>
#include <Http.h>
#include <Parser.h>
#include <Result.h>
#include <Sim800.h>
#include <ArduinoJson.h>

const char BEARER[] PROGMEM = "airtelgprs.com";
DynamicJsonDocument doc(1024);
void setup() {
  Serial.begin(9600);
  while(!Serial)
  Serial.println("Starting!");

}

void loop() {
  HTTP http(9600, 10, 11, 2);
    char response[32];
  char body[90];
  Result result;
result = http.connect(BEARER);
  Serial.print(F("HTTP connect: "));
  Serial.println(result);

  sprintf(body, "{\"bpm\": \"75\", \"steps\": \"60\", \"deviceToken\": \"cee28af31726\"}", "Arduino", "Test", 1);
  result = http.post("https://https://lit-refuge-49573.herokuapp.com/device/data", body, response);
  Serial.print(F("HTTP POST: "));
  Serial.println(result);

  if (result == SUCCESS)
  {
    
    Serial.println(result);
//    DeserializationError error = deserializeJson(doc, result);

//  // Test if parsing succeeds.
//  if (error) {
//    Serial.print(F("deserializeJson() failed: "));
//    Serial.println(error.f_str());
//    return;
//  }
//     const char* datas = doc["message"];
  }

}
