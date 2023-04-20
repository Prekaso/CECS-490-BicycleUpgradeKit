#include <SoftwareSerial.h>   // header file of software serial port

SoftwareSerial Serial1(2, 3); // define software serial port name as Serial1 and define pin2 as RX & pin3 as TX
const int ledPin = 9;         // define pin9 as the LED's RX
int dist;                     // actual distance measurements of LiDAR
String tailgate;              // indicator if rider is being tailgated
int check;                    // save check value
int i;
int uart[9];                  // save data measured by LiDAR
const int HEADER = 0x59;      // frame header of data package
 
 
void setup()
{
  Serial.begin(9600);         // set bit rate of serial port connecting Arduino with computer
  Serial1.begin(115200);      // set bit rate of serial port connecting LiDAR with Arduino
  pinMode(ledPin, OUTPUT);
}
 
 
void loop() {
  if (Serial1.available())                // check if serial port has data input
  {
    if (Serial1.read() == HEADER)        // assess data package frame header 0x59
    {
      uart[0] = HEADER;
      if (Serial1.read() == HEADER)      // assess data package frame header 0x59
      {
        uart[1] = HEADER;
        for (i = 2; i < 9; i++)         // save data in array
        {
          uart[i] = Serial1.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff))        //verify the received data as per protocol
        {
          dist = uart[2] + uart[3] * 256;     //calculate distance value
          tailgate = "NO";
          if (dist <= 10)
          {
            tailgate = "YES";
            digitalWrite(ledPin, HIGH);
            delay(50);
            digitalWrite(ledPin, LOW);
            delay(50);
          }
          digitalWrite(ledPin, LOW);
          Serial.print("dist = ");
          Serial.print(dist);                 //output measure distance value of LiDAR
          Serial.print('\t');
          Serial.print("Being Tailgated? ");
          Serial.print(tailgate);             //output if the rider is being tailgated
          Serial.print('\n');
        }
      }
    }
  }
}