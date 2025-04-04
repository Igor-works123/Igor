#define WIFI_SSID "_" //здесь необходимо ввести пароль от интернет сети и токен бота
#define WIFI_PASS "_"
#define BOT_TOKEN "_"
#define SENSOR_PIN A0
#include <FastBot.h>
#include <DHT.h>   
#include <String.h>
#include <Servo.h> 
Servo servo1;
DHT dht(D5, DHT11);  
FastBot bot(BOT_TOKEN);
float temperature = 0.0;
float hv = 0;
const int DRY = 600;   
const int WET = 200; 
int S = 0;
int W = 0;

const unsigned long recordInterval = 7200000;
unsigned long lastRecord = 0;
const int Siz = 12;
float t[12];
float h[12];
int Index = 0;

void setup() {
  connectWiFi();
  dht.begin();
  bot.attach(newMsg);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D8, OUTPUT);
  servo1.attach(D7);
  delay(2000);
  servo1.write(90);
}

void newMsg(FB_msg& msg) {
  float he = dht.readHumidity();
  float te = dht.readTemperature();
  int i = Index;
  if (millis() - lastRecord >= recordInterval) {
    t[Index] = te;
    h[Index] = he;
    Index = (Index + 1) % Siz;
    lastRecord = millis();
  }


  int sensorValue = analogRead(SENSOR_PIN);
  int humidityP = map(sensorValue, DRY, WET, 0, 100);
  humidityP = constrain(humidityP, 0, 100);
  String(tempMessage) = "Температура: " + String(te) +  " C";
  String(hMessage) = "Влажность: " + String(he) +  "%"; 
  String(hpochva) = "Влажность почвы: " + String(humidityP) + "%";
  Serial.println("New message received:");
  Serial.println(msg.text);
  if (msg.text == "/info") {
  bot.replyMessage( tempMessage, msg.messageID, String(msg.chatID));
  bot.replyMessage( hMessage, msg.messageID, String(msg.chatID));
  bot.replyMessage( hpochva, msg.messageID, String(msg.chatID));
  } else if (msg.text == "/nopp") {
    digitalWrite(D4, LOW);
    S = 0;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
  } else if (msg.text == "/pp") {
    S = 3;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
    } else if (msg.text == "/wind") {
    W = 3;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
    } else if (msg.text == "/wind50") {
    W = 1;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
    } else if (msg.text == "/wind85") {
    W = 2;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
    } else if (msg.text == "/nowind") {
    W = 0;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
     } else if (msg.text == "/pp50") {
    S = 1;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
     } else if (msg.text == "/pp25") {
    S = 2;
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
    } else if (msg.text == "/sv") {
    digitalWrite(D8, HIGH);
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
    } else if (msg.text == "/nosv") {
    digitalWrite(D8, LOW);
    bot.replyMessage( "команда принята", msg.messageID, String(msg.chatID));
    } else if (msg.text == "/stat") {
    for (int i = 0; i < Siz; i++)
    bot.replyMessage( "температура: " + String(t[i]) + "C" + "   Влажность: " + String(h[i]) + "%",msg.messageID, String(msg.chatID));
     }else {
    bot.sendMessage(msg.text);
  

    
  }
}
void loop() {
  float h = dht.readHumidity();
  bot.tick();
  int sensorValue = analogRead(SENSOR_PIN);
  int humidityP = map(sensorValue, DRY, WET, 0, 100);
  humidityP = constrain(humidityP, 0, 100);
  if ((humidityP < 50 && S == 1) || (humidityP < 25 && S == 2) || (S == 3)) {
  digitalWrite(D4, HIGH);
  }else{  
  digitalWrite(D4, LOW);
  }
  if ((h > 50 && W == 1) || (h > 85 && W == 2) || (W == 3)) {
  digitalWrite(D6, HIGH);
  servo1.write(0);  
  }else{  
  digitalWrite(D6, LOW);
  servo1.write(180);
  }
}
void connectWiFi() {
  delay(2000);
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (millis() > 15000) ESP.restart();
  }
  Serial.println("Connected");
}
