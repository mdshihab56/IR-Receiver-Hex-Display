#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the IR receiver pin and buzzer pin
const int RECV_PIN = 2;
const int BUZZER_PIN = 8; // Connect the buzzer to pin 8

// Initialize IR receiver and LCD
IRrecv irrecv(RECV_PIN);
decode_results results;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust the address (0x27) if needed

// Variables for tracking delay between signals
unsigned long lastDetectionTime = 0;
unsigned long currentDetectionTime = 0;

void setup() {
  // Start serial communication
  Serial.begin(9600);

  // Initialize the IR receiver
  irrecv.enableIRIn();

  // Initialize the LCD
  lcd.begin();
  lcd.backlight();

  // Initialize the buzzer pin
  pinMode(BUZZER_PIN, OUTPUT);

  // Welcome message
  lcd.setCursor(0, 0);
  lcd.print("IR Receiver Ready");
  delay(2000);
  lcd.clear();
}

void loop() {
  if (irrecv.decode(&results)) {
    // Record the current detection time
    currentDetectionTime = millis();
    unsigned long delayTime = currentDetectionTime - lastDetectionTime; // Calculate delay

    // Beep the buzzer for detection
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100); // Buzzer ON for 100 ms
    digitalWrite(BUZZER_PIN, LOW);

    // Display the signal code on the LCD
    lcd.setCursor(0, 0);
    lcd.print("Code: ");
    lcd.print(results.value, HEX); // Display the code in HEX format

    // Display the delay time on the second line
    lcd.setCursor(0, 1);
    lcd.print("Delay: ");
    lcd.print(delayTime);
    lcd.print(" ms   "); // Ensure trailing spaces to clear old data

    // Print the code and delay to the Serial Monitor (for debugging)
    Serial.print("Code: ");
    Serial.println(results.value, HEX);
    Serial.print("Delay: ");
    Serial.println(delayTime);

    // Update the last detection time
    lastDetectionTime = currentDetectionTime;

    // Resume the IR receiver
    irrecv.resume();
  }
}
