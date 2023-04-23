#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Software serial pins
#define SIM800_TX_PIN 3
#define SIM800_RX_PIN 2

// GPS pins
#define GPS_TX_PIN 4
#define GPS_RX_PIN 5

const int buttonPin = 6;  // the number of the pushbutton pin
const int ledPin = 7;     // the number of the LED pin

// Initialize software serial for SIM800L and GPS
SoftwareSerial sim800lSerial(SIM800_TX_PIN, SIM800_RX_PIN);
SoftwareSerial gpsSerial(GPS_TX_PIN, GPS_RX_PIN);

// Initialize TinyGPS++ object
TinyGPSPlus gps;

// Array to store phone numbers
String phoneNumbers[] = { "+13107546003" };

// Array to store flags for each phone number to indicate whether a message has been sent
bool smsSentFlags[sizeof(phoneNumbers) / sizeof(phoneNumbers[0])] = { false };
bool callFlag = false;
bool callMade = false;

void setup() {
  // Start serial communication with computer
  Serial.begin(9600);
  while (!Serial)
    ;

  // Start software serial communication with SIM800L
  sim800lSerial.begin(9600);

  // Start software serial communication with GPS
  gpsSerial.begin(9600);

  // Configure SIM800L module
  sim800lSerial.println("AT+CMGF=1");  // Set SMS mode to text
  delay(1000);
  sim800lSerial.println("AT+CNMI=2,2,0,0,0");  // Set new message indication to ON
  delay(1000);

  // Initialize button and LED pins
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  static bool buttonPressed = false;
  static unsigned long buttonPressedTime = 0;
  bool sendSMS = false;
  bool makeCall = false;

  // Check if button is pressed
  if (digitalRead(buttonPin) == HIGH) {
    buttonPressed = true;
    buttonPressedTime = millis();
    digitalWrite(ledPin, HIGH);  // turn on LED connected to pin 7
  } else {
    digitalWrite(ledPin, LOW);  // turn off LED connected to pin 7
  }

  // Read GPS data and send SMS messages
  if (!buttonPressed || millis() - buttonPressedTime > 5000) {
    while (gpsSerial.available() > 0) {
      if (gps.encode(gpsSerial.read())) {
        if (gps.location.isValid()) {
          // Extract latitude and longitude from GPS data
          float lat = gps.location.lat();
          float lon = gps.location.lng();

          // Get current time and date from GPS data
          int hour = gps.time.hour();
          int minute = gps.time.minute();
          int second = gps.time.second();
          int year = gps.date.year();
          int month = gps.date.month();
          int day = gps.date.day();

          // Send SMS message with GPS coordinates and time to each phone number in the array
          if (!buttonPressed) {
            for (int i = 0; i < sizeof(phoneNumbers) / sizeof(phoneNumbers[0]); i++) {
              if (!smsSentFlags[i]) {  // Check if message has not been sent to this contact yet
                // Send the message twice with a delay of 1 minute
                for (int j = 0; j < 2; j++) {
                  String smsMsg = "Project Wheelz\n\nHello, customer was reported in an accident near this location at " + String(hour) + ":" + String(minute) + ":" + String(second) + " (UTC) on " + String(month) + "/" + String(day) + "/" + String(year) + ":\n\nGPS coordinates: " + String(lat, 6) + ", " + String(lon, 6);
                  sim800lSerial.println("AT+CMGS=\"" + phoneNumbers[i] + "\"");
                  delay(1000);
                  sim800lSerial.println(smsMsg);
                  delay(1000);
                  sim800lSerial.write(26);
                  delay(1000);
                  for (int x = 0; x < 5; j++)
                      digitalWrite(ledPin, HIGH);
                      digitalWrite(ledPin, LOW);
                  // Wait for 10 sec between messages
                  if (j == 0) {
                    delay(5000);
                  }
                }

                smsSentFlags[i] = true;  // Set the flag to true after messages have been sent to this contact
              }
            }

            sendSMS = true;  // Set the flag to true after messages are sent
          }
        }
      }
    }
  }

  // Check if button is pressed and reset buttonPressed and buttonPressedTime variables
  if (buttonPressed && millis() - buttonPressedTime <= 5000) {
    buttonPressed = false;
    buttonPressedTime = millis();
  }

  // If messages have been sent and button was not pressed before 5 seconds, make a call
  if (sendSMS && !buttonPressed && !callFlag) {
    delay(10000); //time delayed for text to be delivered first
    sim800lSerial.println("ATD" + phoneNumbers[0] + ";");
    delay(10000);                   // Wait for the call to connect
    sim800lSerial.println("ATH");  // Hang up the call
    callFlag = true;
  }


}
