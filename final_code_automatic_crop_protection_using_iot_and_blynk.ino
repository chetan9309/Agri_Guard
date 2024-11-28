#define BLYNK_TEMPLATE_ID "***********"
#define BLYNK_TEMPLATE_NAME "***********"
#define BLYNK_AUTH_TOKEN "******************************8"

#include <WiFiManager.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

int IN1 = 0;
int IN2 = 1;
int IN3 = 2;
int IN4 = 3;
int IR1 =4;
int IR2 = 5;

BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}
void setup()
{
  Serial.begin(115200);

  pinMode( Rain_S, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode( IR1, INPUT );
  pinMode( IR2, INPUT );
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();
  automatic(input0);
  send_R_Sensor();
  Serial.println("In Loop");
  delay(200);
}

BLYNK_WRITE(V0) {
  input0 = param.asInt();
  Serial.println("blynk change V0");
  automatic(input0);
}

BLYNK_WRITE(V1) {
  input1 = param.asInt();
  Serial.println("blynk change V1");
  automatic(input1); 
}
  
void automatic(int input0){
  if (input0 == 1) {
    Serial.println(" Automatic input = 1");
    int RainValue = send_R_Sensor();
    if (RainValue > 60) {
      RUN_A(); 
    } else if (RainValue < 60 && RainValue > 10) {
      RUN_B(); 
    } else {
      RUN_C();
    } 
    } else if (input0 == 0){
      Serial.println("through blynk input0 = 0");
      throughBlynk(input1);
  }
    delay(200);
}
void throughBlynk(int input1) {
  if (input1 == 1) {
    Serial.println("through blynk input1 = 1");
    RUN_A();
  } else if (input1 == 0){
    Serial.println("through blynk input1 = 0");
    RUN_B();
  }
    delay(200);
}

void RUN_A(){
  Serial.println("Forward");
  if ( digitalRead(IR2) == 1 ){
    digitalWrite(IN1,0);digitalWrite(IN2,1); 
    digitalWrite(IN3,0);digitalWrite(IN4,1);
    Serial.println("IR2   0");
  }else{
  digitalWrite(IN1,0);digitalWrite(IN2,0); 
  digitalWrite(IN3,0);digitalWrite(IN4,0);
  Serial.println("IR2   1");
  }
  delay(200);
} 
  
void RUN_B(){
  Serial.println("Backward");
  if ( digitalRead(IR1) == 1 ){
    digitalWrite(IN1,1);digitalWrite(IN2,0); 
    digitalWrite(IN3,1);digitalWrite(IN4,0);
    Serial.println("IR1   1");
  }else{
    digitalWrite(IN1,0);digitalWrite(IN2,0); 
    digitalWrite(IN3,0);digitalWrite(IN4,0);
    Serial.println("IR1   0");
  }
  delay(200);
}

void RUN_C(){
    Serial.println("Stop");
    digitalWrite(IN1,0);digitalWrite(IN2,0); 
    digitalWrite(IN3,0);digitalWrite(IN4,0);
    delay(200);
}

int send_R_Sensor() {
  int sensorVal = analogRead(Rain_S);
  Blynk.virtualWrite(V3, percentageRain);
  Serial.println(percentageRain);
  delay(200);
  
}
