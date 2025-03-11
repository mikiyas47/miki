#include <TimerOne.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Initialize LCD and Bluetooth Serial
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
SoftwareSerial BTSerial(2, 3); // RX, TX for HC-06 Bluetooth module

// Define pins and variables
int val;
int tempPin = A0; // Temperature sensor pin
int HBSensor = 7;  // Heartbeat sensor pin
int HBCount = 0;   // Heartbeat count
int HBCheck = 0;   // Heartbeat check flag
int TimeinSec = 0; // Time in seconds
int HBperMin = 0;  // Heartbeats per minute
int HBStart = 2;   // Start button pin
int HBStartCheck = 0; // Start check flag
float mv;          // Millivolt variable
int cel;           // Temperature in Celsius
unsigned long prevMillis = 0;
const unsigned long interval = 100; // Interval for temperature reading

void setup() {
  Serial.begin(9600); // Start Serial Monitor for debugging
  lcd.begin(20, 4); // Initialize the LCD
  pinMode(HBSensor, INPUT); // Set heartbeat sensor pin as input
  pinMode(HBStart, INPUT_PULLUP); // Set start button pin as input with pull-up
  Timer1.initialize(1000000); // Initialize Timer1 to trigger every second
  Timer1.attachInterrupt(timerIsr); // Attach the interrupt service routine
  BTSerial.begin(9600);  // Start Bluetooth communication
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("HEALTH MONITORING");
  lcd.setCursor(0, 1);
  lcd.print("TIME IN SEC : ");
  lcd.setCursor(0, 3);
  lcd.print("BODY TEMP   : ");
  lcd.setCursor(0, 2);
  lcd.print("HB PER MIN  : ");
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Read temperature every 100 ms
  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis;
    val = analogRead(tempPin);
    mv = (val / 1023.0) * 5000; // Convert to millivolts
    cel = mv / 10; // Convert to Celsius
    lcd.setCursor(14, 3);
    lcd.print(cel);
    lcd.print("   "); // Clear previous data
  }

  // Check if the start button is pressed
  if (digitalRead(HBStart) == LOW && HBStartCheck == 0) { 
    HBStartCheck = 1;  // Start monitoring only if it's not already started
    HBCount = 0;       // Reset heartbeat count
    TimeinSec = 0;     // Reset timer
    Serial.println("Start button pressed. Monitoring started."); // Debugging output
}


  // Heartbeat detection logic
  if (HBStartCheck == 1) {
    if ((digitalRead(HBSensor) == HIGH) && (HBCheck == 0)) {
      HBCount++; // Increment heartbeat count
      HBCheck = 1; // Set the check flag
      Serial.println("Heartbeat detected! Count: " + String(HBCount)); // Debugging output
    }

    if ((digitalRead(HBSensor) == LOW) && (HBCheck == 1)) {
      HBCheck = 0; // Reset the check flag
    }

    // Every 10 seconds, calculate and send data
    if (TimeinSec == 1) {
      HBperMin = HBCount * 6; // Calculate heartbeats per minute
      HBStartCheck = 0; // Reset start check
      lcd.setCursor(14, 2);
      lcd.print(HBperMin);
      lcd.print(" "); // Clear previous data
      HBCount = 0; // Reset heartbeat count
      TimeinSec = 0; // Reset timer

      // Send Data via Bluetooth
      BTSerial.print("HB:"); // Send heartbeats
      BTSerial.print(HBperMin);
      BTSerial.print(","); // Separator
      BTSerial.print("Temp:"); // Send temperature
      BTSerial.println(cel); // Send temperature value

      Serial.println("Data sent via Bluetooth: HB:" + String(HBperMin) + ", Temp:" + String(cel)); // Debugging output
    }
  }
}

// Timer interrupt service routine
void timerIsr() {
  if (HBStartCheck == 1) {
    TimeinSec++; // Increment time in seconds
    lcd.setCursor(14, 1);
    lcd.print(TimeinSec);
    lcd.print(" "); // Clear previous data
  }
}