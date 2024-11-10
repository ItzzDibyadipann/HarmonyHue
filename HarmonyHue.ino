#include <LiquidCrystal_I2C.h>
#include <NewPing.h>    // Library for Ultrasonic sensor

// Define pins for components
#define TRIGGER_PIN 3    // Ultrasonic sensor trigger pin
#define ECHO_PIN 4       // Ultrasonic sensor echo pin
#define MAX_DISTANCE 200 // Maximum distance for ultrasonic sensor (in cm)
#define SPEAKER_PIN 9    // Speaker pin
#define RED_LED 7        // Red LED for 'sad' mood
#define GREEN_LED 6      // Green LED for 'happy' mood
#define RED_BUTTON 5     // Red button for 'sad' mood
#define GREEN_BUTTON 2   // Green button for 'happy' mood

// Initialize LCD and Ultrasonic sensor
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust I2C address if needed
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Motivational quotes
const char *quotes[] = {
  "Keep going!",
  "You got this!",
  "Stay positive!",
  "Believe in yourself!"
};
int numQuotes = sizeof(quotes) / sizeof(quotes[0]);

void setup() {
  lcd.init();
  lcd.backlight();
  
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  
  Serial.begin(9600); // For debugging
}

void loop() {
  // Check for user nearby
  int distance = sonar.ping_cm();
  if (distance > 0 && distance < 50) {  // User detected within 50 cm
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello!");  // Greeting message
    delay(1000);
  }

  // Check for mood button press
  if (digitalRead(RED_BUTTON) == LOW) {
    displayMood("Sad", RED_LED);
    playTone(262); // Play C note for 'sad' mood
  } else if (digitalRead(GREEN_BUTTON) == LOW) {
    displayMood("Happy", GREEN_LED);
    playTone(523); // Play C note one octave higher for 'happy' mood
  }
}

// Function to display mood on LCD and light up corresponding LED
void displayMood(const char* mood, int ledPin) {
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(ledPin, HIGH);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mood: ");
  lcd.print(mood);
  
  // Display random motivational quote
  int quoteIndex = random(0, numQuotes);
  lcd.setCursor(0, 1);
  lcd.print(quotes[quoteIndex]);
  
  delay(3000);  // Display mood and quote for 3 seconds
}

// Function to play a tone for the speaker based on mood
void playTone(int frequency) {
  unsigned long period = 1000000L / frequency; // Period of one cycle in microseconds
  unsigned long halfPeriod = period / 2;
  unsigned long duration = 500000; // Duration in microseconds (500 ms)

  for (unsigned long i = 0; i < duration; i += period) {
    digitalWrite(SPEAKER_PIN, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(SPEAKER_PIN, LOW);
    delayMicroseconds(halfPeriod);
  }
}

