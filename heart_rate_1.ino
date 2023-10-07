#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET     -1 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
unsigned long previousMillisGetHR = 0; 
unsigned long previousMillisResultHR = 0;
const long intervalGetHR = 20;
const long intervalResultHR = 10000; 
int PulseSensorSignal;
const int PulseSensorHRWire = 0;
const int LED_A1 = A1; 
int UpperThreshold = 550; 
int LowerThreshold = 500; 
int cntHB = 0; 
boolean ThresholdStat = true; 

int BPMval = 0; 
int x=0; 
int y=0; 
int lastx=0; 
int lasty=0;
const unsigned char Heart_Icon [] PROGMEM = {
  0x00, 0x00, 0x18, 0x30, 0x3c, 0x78, 0x7e, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xee, 0xee, 0xd5, 0x56, 
  0x7b, 0xbc, 0x3f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x07, 0xc0, 0x03, 0x80, 0x01, 0x00, 0x00, 0x00
};
void setup() {
  pinMode(LED_A1,OUTPUT); 
  Serial.begin(9600); 
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); \
  }
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 12); 
  display.print("     Please wait");
  display.setCursor(0, 22);
  display.print("     10  seconds");
  display.setCursor(0, 32); 
  display.print("       to get");
  display.setCursor(0, 42); 
  display.print(" the Heart Rate value");
  display.display(); 
  delay(3000);
  display.clearDisplay();
  display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE);
  display.drawLine(0, 43, 127, 43, WHITE); 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(20, 48);
  display.print(": 0 BPM");
  display.display(); 
  Serial.println();
  Serial.println("Please wait 10 seconds to get the BPM Value");
}
void loop() {
  GetHeartRate(); 
}
void GetHeartRate() {
  unsigned long currentMillisGetHR = millis();
  if (currentMillisGetHR - previousMillisGetHR >= intervalGetHR) {
    previousMillisGetHR = currentMillisGetHR;
    PulseSensorSignal = analogRead(PulseSensorHRWire); 
    if (PulseSensorSignal > UpperThreshold && ThresholdStat == true) {
      cntHB++;
      ThresholdStat = false;
      digitalWrite(LED_A1,HIGH);
    }
    if (PulseSensorSignal < LowerThreshold) {
      ThresholdStat = true;
      digitalWrite(LED_A1,LOW);
    }
    DrawGraph(); 
  }
  unsigned long currentMillisResultHR = millis();
  if (currentMillisResultHR - previousMillisResultHR >= intervalResultHR) {
    previousMillisResultHR = currentMillisResultHR;
    BPMval = cntHB * 6; 
    Serial.print("BPM : ");
    Serial.println(BPMval);
    display.fillRect(20, 48, 108, 18, BLACK);
    display.drawBitmap(0, 47, Heart_Icon, 16, 16, WHITE); 
    display.drawLine(0, 43, 127, 43, WHITE);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(20, 48);
    display.print(": ");
    display.print(BPMval);
    display.print(" BPM");
    display.display(); 
    cntHB = 0;
  }
}
void DrawGraph() {
  if (x > 127) {
    display.fillRect(0, 0, 128, 42, BLACK);
    x = 0;
    lastx = 0;
  }
  int ySignal = PulseSensorSignal; 
  if (ySignal > 850) ySignal = 850;
  if (ySignal < 350) ySignal = 350;
  int ySignalMap = map(ySignal, 350, 850, 0, 40);
  y = 40 - ySignalMap;
  display.writeLine(lastx,lasty,x,y,WHITE);
  display.display();
  lastx = x;
  lasty = y;
   x++;
}