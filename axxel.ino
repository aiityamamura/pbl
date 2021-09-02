#define M5STACK_MPU6886
#include <M5Stack.h>
#include <math.h>

float accX = 0;
float accY = 0;
float accZ = 0;

const int numOfSample = 20;
float sample[numOfSample];
float threshold = 0;
int countSample = 0;
float range = 50.0;

uint8_t countStep = 0;giy 
float getFilterdAccelData() {
    static float y[2] = {0};
    M5.IMU.getAccelData(&accX,&accY,&accZ);
    y[1] = 0.8 * y[0] + 0.2 * (abs(accX) + abs(accY) + abs(accZ)) * 1000.0;
    y[0] = y[1];
    return y[1];
}

void setup() {
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.IMU.Init();
  sample[countSample] = getFilterdAccelData();
}

void loop() {
  delay(100);
  countSample++;
  sample[countSample] = getFilterdAccelData();
  if (abs(sample[countSample] - sample[countSample-1]) < range) {
    sample[countSample] = sample[countSample-1];
    countSample--;
  }
  if (sample[countSample] < threshold && sample[countSample-1] > threshold) {
    countStep++;
  }
  if (countSample == numOfSample) {
    threshold = getDynamicThreshold(&sample[0]);
    countSample = 0;
    sample[countSample] = getFilterdAccelData(); }
  
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.println("Walked " + String(countStep) + " steps");  }
