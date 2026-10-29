#include <Arduino.h>
#include <Keypad.h>
#include <BleKeyboard.h>

#define LED_PIN 2

BleKeyboard bleKeyboard("ESP32 Shortcut Pad", "Maker", 100);

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Hardware mapped Row & Column pins from diagnostic test
byte rowPins[ROWS] = {33, 25, 26, 27};
byte colPins[COLS] = {14, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Flags for one-shot long presses
bool longPressed1 = false;
bool longPressed2 = false;
bool longPressed3 = false;
bool longPressed0 = false;
bool longPressedStar = false;
bool longPressed5 = false;
bool longPressedHash = false;

// Variables for continuous repeat while holding (7, 9, 4, 6)
char activeRepeatKey = 0;
unsigned long lastRepeatTime = 0;
unsigned long repeatInterval = 90;

void flashLED(int times = 1, int duration = 50) {
  for (int i = 0; i < times; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(duration);
    digitalWrite(LED_PIN, LOW);
    if (i < times - 1) delay(duration);
  }
}

// Instant app launcher via direct GNOME keybinding (Super + Alt + <num>)
void triggerDirectLauncher(char keyNum) {
  flashLED(2, 60);
  bleKeyboard.press(KEY_LEFT_GUI);
  bleKeyboard.press(KEY_LEFT_ALT);
  bleKeyboard.press(keyNum);
  delay(80);
  bleKeyboard.releaseAll();
}

void keypadEvent(KeypadEvent key) {
  KeyState state = keypad.getState();

  // ================= 1. KEY PRESSED =================
  if (state == PRESSED) {
    if (!bleKeyboard.isConnected()) {
      Serial.printf("[Keypad] %c pressed, but Bluetooth not connected\n", key);
      return;
    }

    if (key == '1') longPressed1 = false;
    else if (key == '2') longPressed2 = false;
    else if (key == '3') longPressed3 = false;
    else if (key == '0') longPressed0 = false;
    else if (key == '*') longPressedStar = false;
    else if (key == '5') longPressed5 = false;
    else if (key == '#') longPressedHash = false;
    else if (key == '7' || key == '9' || key == '4' || key == '6') {
      activeRepeatKey = 0;
    }
    // Instant keys
    else if (key == '8') {
      Serial.println("[Keypad 8] Mute / Unmute");
      flashLED(1, 50);
      bleKeyboard.write(KEY_MEDIA_MUTE);
    }
  }

  // ================= 2. KEY HOLD (Long Press Detected) =================
  else if (state == HOLD) {
    if (!bleKeyboard.isConnected()) return;

    // --- Continuous Repeat Keys (7: Vol-, 9: Vol+, 4: Bright-, 6: Bright+) ---
    if (key == '7' || key == '9' || key == '4' || key == '6') {
      activeRepeatKey = key;
      lastRepeatTime = millis();
      repeatInterval = (key == '7' || key == '9') ? 80 : 120;
      digitalWrite(LED_PIN, HIGH);
    }
    // --- Direct Instant App Launchers (Super + Alt + [1, 2, 3, 5]) ---
    else if (key == '1') {
      longPressed1 = true;
      Serial.println("[Keypad 1 LONG HOLD] Instant Launch Browser");
      triggerDirectLauncher('1');
    }
    else if (key == '2') {
      longPressed2 = true;
      Serial.println("[Keypad 2 LONG HOLD] Instant Launch Telegram");
      triggerDirectLauncher('2');
    }
    else if (key == '3') {
      longPressed3 = true;
      Serial.println("[Keypad 3 LONG HOLD] Instant Launch Antigravity");
      triggerDirectLauncher('3');
    }
    else if (key == '5') {
      longPressed5 = true;
      Serial.println("[Keypad 5 LONG HOLD] Instant Launch Spotify");
      triggerDirectLauncher('5');
    }
    // --- Other Long Presses ---
    else if (key == '0') {
      longPressed0 = true;
      Serial.println("[Keypad 0 LONG HOLD] Escape (ESC)");
      flashLED(2, 60);
      bleKeyboard.write(KEY_ESC);
    }
    else if (key == '*') {
      longPressedStar = true;
      Serial.println("[Keypad * LONG HOLD] Close Active Window (Alt + F4)");
      flashLED(2, 80);
      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press(KEY_F4);
      delay(100);
      bleKeyboard.releaseAll();
    }
    else if (key == '#') {
      longPressedHash = true;
      Serial.println("[Keypad # LONG HOLD] Shutdown PC (Power Off)");
      flashLED(3, 100);
      bleKeyboard.press(KEY_LEFT_CTRL);
      bleKeyboard.press(KEY_LEFT_ALT);
      bleKeyboard.press('p');
      delay(120);
      bleKeyboard.releaseAll();
    }
  }

  // ================= 3. KEY RELEASED =================
  else if (state == RELEASED) {
    if (!bleKeyboard.isConnected()) return;

    // Stop continuous repeating if active
    if (activeRepeatKey == key) {
      activeRepeatKey = 0;
      digitalWrite(LED_PIN, LOW);
      return;
    }

    // --- Short Tap Actions ---
    switch (key) {
      case '1':
        if (!longPressed1) {
          Serial.println("[Keypad 1 SHORT TAP] Select All (Ctrl + A)");
          flashLED(1, 50);
          bleKeyboard.press(KEY_LEFT_CTRL);
          bleKeyboard.press('a');
          delay(70);
          bleKeyboard.releaseAll();
        }
        longPressed1 = false;
        break;

      case '2':
        if (!longPressed2) {
          Serial.println("[Keypad 2 SHORT TAP] Copy (Ctrl + C)");
          flashLED(1, 50);
          bleKeyboard.press(KEY_LEFT_CTRL);
          bleKeyboard.press('c');
          delay(70);
          bleKeyboard.releaseAll();
        }
        longPressed2 = false;
        break;

      case '3':
        if (!longPressed3) {
          Serial.println("[Keypad 3 SHORT TAP] Paste (Ctrl + V)");
          flashLED(1, 50);
          bleKeyboard.press(KEY_LEFT_CTRL);
          bleKeyboard.press('v');
          delay(70);
          bleKeyboard.releaseAll();
        }
        longPressed3 = false;
        break;

      case '4':
        Serial.println("[Keypad 4 SHORT TAP] Previous Track");
        flashLED(1, 50);
        bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
        break;

      case '5':
        if (!longPressed5) {
          Serial.println("[Keypad 5 SHORT TAP] Play / Pause");
          flashLED(1, 50);
          bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
        }
        longPressed5 = false;
        break;

      case '6':
        Serial.println("[Keypad 6 SHORT TAP] Next Track");
        flashLED(1, 50);
        bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
        break;

      case '7':
        Serial.println("[Keypad 7 SHORT TAP] Volume Down");
        flashLED(1, 50);
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
        break;

      case '9':
        Serial.println("[Keypad 9 SHORT TAP] Volume Up");
        flashLED(1, 50);
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
        break;

      case '0':
        if (!longPressed0) {
          Serial.println("[Keypad 0 SHORT TAP] Screenshot (PrintScreen)");
          flashLED(1, 50);
          bleKeyboard.write(KEY_PRTSC);
        }
        longPressed0 = false;
        break;

      case '*':
        if (!longPressedStar) {
          Serial.println("[Keypad * SHORT TAP] Minimize Window (Super + H)");
          flashLED(1, 50);
          bleKeyboard.press(KEY_LEFT_GUI);
          bleKeyboard.press('h');
          delay(80);
          bleKeyboard.releaseAll();
        }
        longPressedStar = false;
        break;

      case '#':
        if (!longPressedHash) {
          Serial.println("[Keypad # SHORT TAP] Sleep PC (Suspend)");
          flashLED(1, 50);
          bleKeyboard.press(KEY_LEFT_CTRL);
          bleKeyboard.press(KEY_LEFT_ALT);
          bleKeyboard.press('u');
          delay(120);
          bleKeyboard.releaseAll();
        }
        longPressedHash = false;
        break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Responsive 450ms threshold for snappy long press
  keypad.setHoldTime(450);
  keypad.addEventListener(keypadEvent);

  Serial.println("\n========================================================");
  Serial.println("  ESP32 Direct Instant Launchers Ready!                 ");
  Serial.println("========================================================\n");

  bleKeyboard.begin();
}

void loop() {
  keypad.getKeys();

  // Continuous hold handling (Volume & Brightness repeat)
  if (activeRepeatKey != 0 && bleKeyboard.isConnected()) {
    unsigned long now = millis();
    if (now - lastRepeatTime >= repeatInterval) {
      lastRepeatTime = now;

      if (activeRepeatKey == '7') {
        bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
      }
      else if (activeRepeatKey == '9') {
        bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
      }
      else if (activeRepeatKey == '4') {
        // Safe Brightness Down (Super + F5)
        bleKeyboard.press(KEY_LEFT_GUI);
        bleKeyboard.press(KEY_F5);
        delay(30);
        bleKeyboard.releaseAll();
      }
      else if (activeRepeatKey == '6') {
        // Safe Brightness Up (Super + F6)
        bleKeyboard.press(KEY_LEFT_GUI);
        bleKeyboard.press(KEY_F6);
        delay(30);
        bleKeyboard.releaseAll();
      }
    }
  }

  delay(5);
}
