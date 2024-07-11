#include <WiFi.h>

const char* ssid           = "CobaBang";     // Change this to the desired Access Point name
const char* password       = "mekatronik22"; // Change this to the desired Access Point password
const int   channel        = 10;             // WiFi Channel number between 1 and 13
const bool  hide_SSID      = false;          // To disable SSID broadcast -> SSID will not appear in a basic WiFi scan
const int   max_connection = 1;              // Maximum simultaneous connected clients on the AP

// Motor 1
int R_EN1 = 27;  // Roda Kiri
int L_EN1 = 26;  
int R_PWM1 = 25;  
int L_PWM1 = 33; 

// Motor 2
int R_EN2 = 16;  // Roda Kanan
int L_EN2 = 17;  
int R_PWM2 = 18; 
int L_PWM2 = 19;  
  
int pwm_low = 40;
int pwm = 0;

WiFiServer ESPserver(80);

void setup() {
  Serial.begin(115200);
  pinMode(R_EN1, OUTPUT);
  pinMode(L_EN1, OUTPUT);
  pinMode(R_PWM1, OUTPUT);
  pinMode(L_PWM1, OUTPUT);
  pinMode(R_EN2, OUTPUT);
  pinMode(L_EN2, OUTPUT);
  pinMode(R_PWM2, OUTPUT);
  pinMode(L_PWM2, OUTPUT);

  Serial.println();
  Serial.println();
  Serial.print("Setting up Access Point: ");
  Serial.println(ssid);

  // Set ESP32 as an Access Point
  WiFi.softAP(ssid, password, channel, hide_SSID, max_connection);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  ESPserver.begin();
  Serial.println("Server started");
}

void loop() {
  WiFiClient client = ESPserver.available();
  if (!client) {
    return;
  }

  Serial.print("New Client");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  int value = 0;
  if (request.indexOf("/Pwm%2090") != -1) {
    Serial.println("Pwm 90");
    pwm = 90;
    value = 1;
  }
  if (request.indexOf("/Pwm%20180") != -1) {
    Serial.println("Pwm 180");
    pwm = 180;
    value = 2;
  }
  if (request.indexOf("/Pwm%20255") != -1) {
    Serial.println("Pwm 255");
    pwm = 255;
    value = 3;
  }
  if (request.indexOf("/Maju") != -1) {
    Serial.println("Maju | PWM : " + String(pwm));
    maju(pwm);
    value = 4;
  }
  if (request.indexOf("/Atas%20Kanan") != -1) {
    Serial.println("Atas Kanan | PWM : " + String(pwm));
    ataskanan(pwm);
    value = 5;
  }
  if (request.indexOf("/Kanan") != -1) {
    Serial.println("Kanan | PWM : " + String(pwm));
    kanan(pwm);
    value = 6;
  }
  if (request.indexOf("/Bawah%20Kanan") != -1) {
    Serial.println("Bawah Kanan | PWM : " + String(pwm));
    bawahkanan(pwm);
    value = 7;
  }
  if (request.indexOf("/Mundur") != -1) {
    Serial.println("Mundur | PWM : " + String(pwm));
    mundur(pwm);
    value = 8;
  }
  if (request.indexOf("/Bawah%20Kiri") != -1) {
    Serial.println("Bawah Kiri | PWM : " + String(pwm));
    bawahkiri(pwm);
    value = 9;
  }
  if (request.indexOf("/Kiri") != -1) {
    Serial.println("Kiri | PWM : " + String(pwm));
    kiri(pwm);
    value = 10;
  }
  if (request.indexOf("/Atas%20Kiri") != -1) {
    Serial.println("Atas Kiri | PWM : " + String(pwm));
    ataskiri(pwm);
    value = 11;
  }
  if (request.indexOf("/Berhenti") != -1) {
    Serial.println("Berhenti");
    berhenti();
    value = 12;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("Status: ");

  switch(value) {
    case 1:
      client.print("Pwm 90");
      break;
    case 2:
      client.print("Pwm 180");
      break;
    case 3:
      client.print("Pwm 255");
      break;
    case 4:
      client.print("Maju");
      break;
    case 5:
      client.print("Atas Kanan");
      break;
    case 6:
      client.print("Kanan");
      break;
    case 7:
      client.print("Bawah Kanan");
      break;
    case 8:
      client.print("Mundur");
      break;
    case 9:
      client.print("Bawah Kiri");
      break;
    case 10:
      client.print("Kiri");
      break;
    case 11:
      client.print("Atas Kiri");
      break;
    case 12:
      client.print("Berhenti");
      break;
  }

  client.println("</html>");

  delay(1);
  Serial.println("Client Disconnected");
  Serial.println("");
}

void maju(int pwm) { 
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, pwm);
  analogWrite(R_PWM2, pwm);
  analogWrite(L_PWM2, 0);
}

void ataskanan(int pwm) { 
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, pwm);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, pwm-pwm_low);
}

void kanan(int pwm) { 
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, pwm);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, pwm);
}

void bawahkanan(int pwm) {
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, pwm);
  analogWrite(L_PWM1, 0);
  analogWrite(R_PWM2, pwm-pwm_low);
  analogWrite(L_PWM2, 0);
}

void mundur(int pwm) {
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, pwm);
  analogWrite(L_PWM1, 0);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, pwm);
}

void bawahkiri(int pwm) {
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, pwm - pwm_low);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, pwm);
}

void kiri(int pwm) { 
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, pwm);
  analogWrite(L_PWM1, 0);
  analogWrite(R_PWM2, pwm);
  analogWrite(L_PWM2, 0);
}

void ataskiri(int pwm) { 
  digitalWrite(R_EN1, HIGH);
  digitalWrite(L_EN1, HIGH);
  digitalWrite(R_EN2, HIGH);
  digitalWrite(L_EN2, HIGH);
  analogWrite(R_PWM1, pwm-pwm_low);
  analogWrite(L_PWM1, 0);
  analogWrite(R_PWM2, pwm);
  analogWrite(L_PWM2, 0);
}

void berhenti() {  
  digitalWrite(R_EN1, LOW);
  digitalWrite(L_EN1, LOW);
  digitalWrite(R_EN2, LOW);
  digitalWrite(L_EN2, LOW);
  analogWrite(R_PWM1, 0);
  analogWrite(L_PWM1, 0);
  analogWrite(R_PWM2, 0);
  analogWrite(L_PWM2, 0);
}
