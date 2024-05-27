#include <SoftwareWire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// SDA = pin 2, SCL = pin 3
SoftwareWire myWire(2, 3); 

// CE ve CSN pinleri
RF24 radio(5, 6);

// Adres
const byte address[6] = "00001";

struct Data_Package {
  int joy1X;
  int joy1Y;
  int joy2X;
  int joy2Y;
};
Data_Package data;

int rightMov, leftMov, forward, backward;

void setup() {
  Serial.begin(9600);
  myWire.begin(); // I2C başlat

  bool init = radio.begin();
  if(init)
  Serial.println("SPI Succes");
  else
  Serial.println("SPI Fail");

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setChannel(5);
  radio.startListening();

}

void loop() {

  if (radio.available() > 0) {
    radio.read(&data, sizeof(data));

    leftMov = map(data.joy1Y, 1023, 492, 255, 0);
    rightMov = map(data.joy1Y, 491, 0, 0, 255);
    forward = map(data.joy2Y, 1023, 522, 255, 0);
    backward = map(data.joy2Y, 521, 0, 0, 255);

    if((leftMov <= 0) || (rightMov <= 0)){
      if (leftMov <= 0){
        leftMov = 0;
      } else if (rightMov <= 0){
        rightMov = 0;
      }
    }

    if((forward <= 0) || (backward <= 0)){
      if (forward <= 0){
        forward = 0;
      } else if (backward <= 0){
        backward = 0;
      }
    }

    Serial.println(leftMov);
    Serial.println(rightMov);
    Serial.println(forward);
    Serial.println(backward);

    myWire.beginTransmission(8); // 8 adresindeki cihaza veri gönder

    if (forward > 0) {
      myWire.write('F'); // 'F' for forward
      myWire.write(forward);
    } else if (backward > 0) {
      myWire.write('B'); // 'B' for backward
      myWire.write(backward);
    } else if (leftMov > 0) {
      myWire.write('L'); // 'L' for left
      myWire.write(leftMov);
    } else if (rightMov > 0) {
      myWire.write('R'); // 'R' for right
      myWire.write(rightMov);
    } else {
      myWire.write('S'); // 'S' for stop
      myWire.write((byte)0);
    }

    myWire.endTransmission(); // Veri gönderimini sonlandır
    //delay(1000); // 1 saniye bekle

  }
}
