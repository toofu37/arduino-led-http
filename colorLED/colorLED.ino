#include <SoftwareSerial.h>

#include <doxygen.h>
#include <ESP8266.h>


#include <Adafruit_NeoPixel.h>

// 転送レート
#define SERIAL_SPEED  115200

// Wi-Fi
#define WLAN_SSID     "YOUR_WIFI_SSID"
#define WLAN_PASS     "YOUR_WIFI_PASS"

// サーバーのホスト名
#define HOST_NAME     "toofu.net"
#define FILE_PATH      "/arduino/ledsample/rgb1.txt"
// ポート番号
#define PORT_NUMBER   80

// Wi-Fiモジュール
ESP8266 wifi;

// LED
#define PIN 6
#define NUMLED 1
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMLED, PIN, NEO_RGB + NEO_KHZ400);

SoftwareSerial softSerial(0,1);

void setup() {
  pixels.begin();
  Serial.begin(SERIAL_SPEED);
  softSerial.begin(SERIAL_SPEED);
  setupWiFi();
}

/*
  Wi-Fiの設定
*/
void setupWiFi() {
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  

  // シリアルポートの指定
  wifi.begin(softSerial);

  // Wi-Fiへ接続(成功するまで処理を繰り返す)
  while (!wifi.joinAP(WLAN_SSID, WLAN_PASS)) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Wi-Fi connected");
  Serial.println("IP address: ");
  // Wi-FiのローカルIPアドレスをシリアルモニターへ表示
  Serial.println(wifi.getLocalIP().c_str());
}

void loop() {
  if(wifi.createTCP(HOST_NAME, PORT_NUMBER)) {
    char sendData[] = "";
    sprintf(sendData, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", FILE_PATH, HOST_NAME);
    Serial.println(sendData);
    
    wifi.send((const uint8_t*)sendData, strlen(sendData));
    uint8_t buffer[512] = {0};
    uint32_t len = wifi.recv(buffer, sizeof(buffer), 10000);
    
    if (len > 0) {
      String resultStringR = parse(len, buffer, "R");
      String resultStringG = parse(len, buffer, "G");
      String resultStringB = parse(len, buffer, "B");
      int pixelColor[] = {resultStringR.toInt(), resultStringG.toInt(), resultStringB.toInt()};
      
      for (int i=0; i<NUMLED; i++) {
        pixels.setPixelColor(i, pixels.Color(pixelColor[0], pixelColor[1], pixelColor[2]));
        Serial.print("R:");
        Serial.println(pixelColor[0]);
        Serial.print("G:");
        Serial.println(pixelColor[1]);
        Serial.print("B:");
        Serial.println(pixelColor[2]);
        pixels.show();
        delay(100);
      }
    }
  }
}

/*
  サーバーから取得した文字列を解析します
*/
String parse(int len, uint8_t buffer[512], String RGB) {
  // サーバーから取得した文字列
  String resultCode = "";

  // サーバーから取得した文字列を1つずつ格納
  for (uint32_t i = 0; i < len; i++) {
    resultCode += (char)buffer[i];
  }
  
  if (RGB == "R") {
    String resultString = getColorR(resultCode);
    return resultString;
  } else if (RGB == "G") {
    String resultString = getColorG(resultCode);
    return resultString;
  } else if (RGB == "B") {
    String resultString = getColorB(resultCode);
    return resultString;
  } else {
    Serial.println("parse error");
    return "0";
  }
}

String getColorR(String resultCode) {
  int indexR = resultCode.indexOf("colorR=");
  int indexG = resultCode.indexOf("colorG=");
  if (indexR > 0) {
        String resultString = resultCode.substring(indexR+7, indexG);
        return resultString;
  } else {
      Serial.println("color get error");
      return "0";
  }
}

String getColorG(String resultCode) {
  int indexG = resultCode.indexOf("colorG=");
  int indexB = resultCode.indexOf("colorB=");
  if (indexG > 0) {
        String resultString = resultCode.substring(indexG+7, indexB);
        return resultString;
  } else {
      Serial.println("color get error");
      return "0";
  }
}

String getColorB(String resultCode) {
  int indexB = resultCode.indexOf("colorB=");
  if (indexB > 0) {
        String resultString = resultCode.substring(indexB+7, indexB+10);
        return resultString;
  } else {
      Serial.println("color get error");
      return "0";
  }
}
