#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#define i2c_Address 0x3c
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
#define OLED_RESET -1   
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <SoftwareSerial.h>
SoftwareSerial mySerial(12, 14); // RX, TX :: [Rx GPIO12-D6] , [Tx GPIO14-D5]

#include "PMS.h"
PMS pms(mySerial);
PMS::DATA data;

#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
Adafruit_BME680 bme;

void setup() {
  display.begin(i2c_Address, true);

  mySerial.begin(9600);
  pms.passiveMode();
  delay(500);
  pms.wakeUp();
  delay(1000);

  bme.begin();
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);

  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(2, 0);
  display.println("Weather");
  display.println("Station");
  display.setTextSize(1);
  display.setCursor(25, 54);
  display.print("By ");
  display.drawLine(42, 54, 42, 62, SH110X_WHITE); //left 
  display.drawLine(42, 62, 120, 62, SH110X_WHITE); //down
  display.drawLine(42, 53, 120, 53, SH110X_WHITE); //up
  display.setTextColor(SH110X_BLACK, SH110X_WHITE); // 'inverted' text
  display.println("Atreya Biswas");
  display.display();
  delay (2000);
   

}

void loop() {
  

 if (bme.performReading()) {
  display.clearDisplay();
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  display.setTextSize(1);
  display.drawLine(0, 0, 125, 0, SH110X_WHITE); //up
  display.setCursor(0, 1);
  display.println("    W E A T H E R    ");
  display.println("");
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);
  display.print(bme.temperature);
  display.drawCircle(63,18, 2, SH110X_WHITE);
  display.setCursor(72, 17);
  display.println("C");
  display.print(bme.humidity);
  display.println(" %");
  display.print(bme.pressure / 100.0);
  display.setTextSize(1);
  display.print(" ");
  display.setTextSize(2);
  display.print("hPa"); 
  display.display();
  delay (7000);
}
else {
  display.clearDisplay();
  display.display();
  display.println("BME NOT FOUND");
  display.display();
  delay (2000);
  
}

pms.requestRead();
  if (pms.readUntil(data))
{
  display.clearDisplay();
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  display.drawLine(0, 0, 125, 0, SH110X_WHITE); //up
  display.setTextSize(1);
  display.setCursor(0, 1);
  display.println("  AIR QUALITY INDEX  ");
  display.println("");
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);
  display.print("PM 1.0:");
  display.println(data.PM_AE_UG_1_0);
  display.print("PM 2.5:");
  display.println(data.PM_AE_UG_2_5);
  display.print("PM 10:");
  display.setTextSize(1);
  display.print("  ");
  display.setTextSize(2);
  display.println(data.PM_AE_UG_10_0);
  display.display();
  delay(7000);
}
else {
  display.clearDisplay();
  display.display();
  display.println("PMS NOT FOUND");
  display.display();
  delay (2000);
}

}
