#include <Keypad.h>

// Relay pins
#define RELAY1_PIN 23   // Solenoid lock relay
#define RELAY2_PIN 22   // Extra relay (optional)
#define BUZZER_PIN 19   // Buzzer

// Keypad setup 4x4
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Keypad pin connections to ESP32
byte rowPins[ROWS] = {32, 33, 25, 26};
byte colPins[COLS] = {27, 14, 12, 13};

// Create keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Password
String password = "1234";
String input = "";

void setup() {
  Serial.begin(115200);

  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Both relays OFF initially (HIGH = off for active-low relays)
  digitalWrite(RELAY1_PIN, HIGH);  
  digitalWrite(RELAY2_PIN, HIGH);

  Serial.println("System Ready...");
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    // When user presses ENTER (#)
    if (key == '#') {
      if (input == password) {
        unlockDoor();
      } else {
        wrongPass();
      }
      input = "";  // Clear after enter
    }

    // CLEAR password (*)
    else if (key == '*') {
      input = "";
      tone(BUZZER_PIN, 1000, 100);
    }

    // Add character to input
    else {
      input += key;
      tone(BUZZER_PIN, 2000, 30);
    }
  }
}

// =========================
// UNLOCK FUNCTION
// =========================
void unlockDoor() {
  Serial.println("ACCESS GRANTED");
  tone(BUZZER_PIN, 1500, 200);

  // Unlock solenoid (relay ON → LOW)
  digitalWrite(RELAY1_PIN, LOW);
  delay(5000);  // Keep unlocked 5 seconds

  // Lock again
  digitalWrite(RELAY1_PIN, HIGH);
  Serial.println("DOOR LOCKED");
}

// =========================
// WRONG PASSWORD
// =========================
void wrongPass() {
  Serial.println("ACCESS DENIED");
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, 400, 200);
    delay(200);
  }
}
