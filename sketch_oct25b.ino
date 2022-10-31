#include <Servo.h>
#include <EEPROM.h>

Servo Servos[4]; //0 = scan1; 1 = scan2; 2 = pos1; 3 = pos2;

Servo ScanX;
Servo ScanY;
Servo MoveX;
Servo MoveY;

float Max[2][2] = {{0, 1}, {0, 1}};
const float ex[2][2] = {{0, 1}, {0, 1}};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  MoveX.attach(2);
  MoveY.attach(3);
  ScanX.attach(5);
  ScanY.attach(4);
  ScanX.write(0);
  ScanY.write(0);
  MoveX.write(0);
  MoveY.write(0);
  /*EEPROM.write(1, 0.);
  EEPROM.write(0, 0.);
  EEPROM.write(2, 0.);*/
  // 0 = indexes, 1 = moveavg, 2 = staticavg
}

int Read (int pin, int pos, int max) {
  float read = analogRead(pin);
  read = read * 5/1023;
  read = floor(read*1000)/1000;
  if (read > Max[max][0]) {
    Max[max][0] = read;
    Max[max][1] = pos;
  }
}

int Store (int pin, int address) {
  float read = analogRead(pin);
  read = read * 5/1023;
  read = floor(read*100);
  float avg = EEPROM.read(address);
  Serial.println(avg);
  float index = EEPROM.read(0);
  avg = avg*index;
  avg = avg + read;
  avg = avg/(index + 1);
  Serial.print(avg);
  Serial.print(" ");
  Serial.println(index);
  EEPROM.write(address, avg);
  float avg2 = EEPROM.read(address);
  Serial.println(avg2);
}

void loop() {
  // put your main code here, to run repeatedly:
  ScanX.write(0);
  ScanY.write(0);
  delay(60000);
  for (int i = 45; i < 135; i++) {
    ScanX.write(i);
    ScanY.write(i);
    delay(15);
    Read(A0, i, 0);
    Read(A1, i, 1);
  }
  ScanX.write(Max[0][1]);
  ScanY.write(Max[1][1]);
  MoveX.write(Max[0][1]);
  MoveY.write(Max[1][1]);
  Max[0][0] = 0;
  Max[0][1] = 1;
  Max[1][0] = 0;
  Max[1][1] = 1;
  Store(A3, 2);
  Store(A2, 1);
  EEPROM.write(0, EEPROM.read(0) + 1);
  delay(2000);
}
