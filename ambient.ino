#define M5STACK_MPU6886
#include <M5Stack.h>
#include <Ambient.h>
#define SAMPLE_PERIOD 12    // サンプリング間隔(ミリ秒)
#define SAMPLE_SIZE 150     // 20ms x 150 = 3秒

WiFiClient client;
Ambient ambient;

const char* ssid = "aterm-929f78-g";
const char* password = "067a7d333a528";
unsigned int channelId = 40626; // AmbientのチャネルID
const char* writeKey = "2fc8372e189ce187"; // ライトキー

float accX = 0.0F;
float accY = 0.0F;
float accZ = 0.0F;


void setup(){
    M5.begin();
    M5.Lcd.setRotation(3);
    M5.IMU.Init();  //Init IMU sensor. 
    WiFi.begin(ssid, password);  //  Wi-Fi APに接続
    while (WiFi.status() != WL_CONNECTED) {  //  Wi-Fi AP接続待ち
        delay(100);
    }
    
    ambient.begin(channelId, writeKey, &client); // チャネルIDとライトキーを指定してAmbientの初期化
}

void loop() {  M5.IMU.getAccelData(&accX, &accY, &accZ);
  M5.Lcd.setCursor(0, 32); M5.Lcd.printf("x: %7.2f mg",1000*accX);  
  M5.Lcd.setCursor(0, 64); M5.Lcd.printf("y: %7.2f mg",1000*accY);  
  M5.Lcd.setCursor(0, 96); M5.Lcd.printf("z: %7.2f mg",1000*accZ);  
           
        // Ambientに送信する
        ambient.set(1, 1000*accX);
        ambient.set(2, 1000*accY);
        ambient.set(3, 1000*accZ);

        ambient.send();
 delay(200); 
    }
