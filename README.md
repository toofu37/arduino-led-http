# arduino-led-http
Arduino M0 Proに接続したLEDの色をインターネットから変える

http://toofu.net/arduino/ledsample/

![sample](sample.gif)


* colorLED
arduinoに書き込むためのスケッチファイル。
Serial5がArduino M0 Pro用のクラスなので他のArduinoでは動かないかも。

使ってるWi-FiモジュールはスイッチサイエンスのESP-WROOM-02ピッチ変換済みモジュール《シンプル版》
https://www.switch-science.com/catalog/2341/

使ってるLEDはNeoPixel RGB 8mm。ライブラリが配布されるので落としておく。
https://github.com/adafruit/Adafruit_NeoPixel
