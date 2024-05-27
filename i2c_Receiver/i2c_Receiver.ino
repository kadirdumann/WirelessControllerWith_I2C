#include <Wire.h>

const int right_LPWM = 8;   //BACKWARD
const int right_RPWM = 7;   //FORWARD
const int left_LPWM = 9;    //BACKWARD
const int left_RPWM = 10;    //FORWARD

char command;
int value;

void setup() {
  Wire.begin(8); // I2C adresi 8 olarak başlat
  Wire.onReceive(receiveEvent); // Veri alındığında çalışacak fonksiyonu tanımla
  Serial.begin(9600); // Seri haberleşmeyi başlat

  pinMode(right_LPWM, OUTPUT);
  pinMode(right_RPWM, OUTPUT);
  pinMode(left_LPWM, OUTPUT);
  pinMode(left_RPWM, OUTPUT);

  analogWrite(right_LPWM, 0);    //backward 
  analogWrite(right_RPWM, 0);
  analogWrite(left_LPWM, 0);     //backward
  analogWrite(left_RPWM, 0);
}

void loop() {
  // Ana döngüde bir şey yapılmıyor, veri alımı kesme ile yapılacak
  delay(1000); // Her saniye alınan değeri yazdır
  Serial.print("Command: ");
  Serial.println(command);
  Serial.print("Value: ");
  Serial.println(value);

  if (command == 'F') {
    analogWrite(right_LPWM, 0);    //backward 
    analogWrite(right_RPWM, value);
    analogWrite(left_LPWM, 0);     //backward
    analogWrite(left_RPWM, value);
  } else if (command == 'B') {
    analogWrite(right_LPWM, value);    //backward 
    analogWrite(right_RPWM, 0);
    analogWrite(left_LPWM, value);     //backward
    analogWrite(left_RPWM, 0);
  } else if (command == 'L') {
    analogWrite(right_LPWM, 0);    //backward 
    analogWrite(right_RPWM, value);
    analogWrite(left_LPWM, value);     //backward
    analogWrite(left_RPWM, 0);
  } else if (command == 'R') {
    analogWrite(right_LPWM, value);    //backward 
    analogWrite(right_RPWM, 0);
    analogWrite(left_LPWM, 0);     //backward
    analogWrite(left_RPWM, value);
  } else if (command == 'S') {
    analogWrite(right_LPWM, 0);    //backward 
    analogWrite(right_RPWM, 0);
    analogWrite(left_LPWM, 0);     //backward
    analogWrite(left_RPWM, 0);
  }
}

void receiveEvent(int howMany) {
  if (howMany == 2) { // 1 byte komut + 1 byte değer
    command = Wire.read(); // İlk byte komut
    value = Wire.read(); // İkinci byte değer
  }
}
