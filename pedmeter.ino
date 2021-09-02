#define M5STACK_MPU6886
#include <M5Stack.h>
#include <Ambient.h>
#define SAMPLE_PERIOD 12    // サンプリング間隔(ミリ秒)
#define SAMPLE_SIZE 150     // 20ms x 150 = 3秒

WiFiClient client;
Ambient ambient;

const char* ssid = "aterm-929f78-g";
const char* password = "067a7d333a528";
unsigned int channelId = 40627; // AmbientのチャネルID
const char* writeKey = "81264799f8660136"; // ライトキー

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;

const int numOfSample = 50;
float sample[numOfSample];
float threshold = 0;
int countSample = 0;
float a


void setup(){
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.IMU.Init();  //Init IMU sensor. 
    WiFi.begin(ssid, password);  //  Wi-Fi APに接続
    while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
        delay(100);
    }
    
    ambient.begin(channelId, writeKey, &client); // チャネルIDとライトキーを指定してAmbientの初期化
    
  M5.Lcd.fillScreen(BLACK); //Set the screen background color to black. 设置屏幕背景色为黑色
  M5.Lcd.setTextColor(GREEN , BLACK); //Sets the foreground color and background color of the displayed text.  设置显示文本的前景颜色和背景颜色
  M5.Lcd.setTextSize(2);  //Set the font size. 
}

void loop() {  M5.IMU.getAccelData(&accX, &accY, &accZ);

// XYZ 軸の合成
float accX,accY,accZ;
float accel = sqrt(accX * accX + accY * accY + accZ * accZ);

//// 100回ごとに加速度の平均値を求め、閾値を決める
  if (countSample < 100) {
    total += accel;
    countSample += 1;
  } else {
    avg = total / count;
    width = avg / 10;
    total = avg;
    countSample = 1;
  }

  // 閾値を超えた場合に歩数を加算する
   if (accel > avg + width) {
    state = true;
  } else if (accel < avg - width) {
     state = false;
  }
  if (!old_state && state) {
    step += 1; // 歩数を加算
  }
  old_state = state;
}
// 歩数の出力
M5.Lcd.print(step);

// 簡易カロリー計算
int calorie;
calorie = step * 0.1;
M5.Lcd.print(calorie);

// Ambientに送信する
        ambient.set(1, steps);
        ambient.set(2, calorie);
        ambient.set(3, 1000*accX);
        ambient.set(4, 1000*accY);
        ambient.set(5, 1000*accZ);
        ambient.send();
 delay(200); 
}
