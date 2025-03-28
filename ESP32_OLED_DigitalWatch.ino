#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <EasyButton.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SSD1306_I2C_ADDRESS 0x3C  // User-defined constant for I2C address

RTC_DS1307 rtc;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int modeChangeButtonPin = 12;  // Pin for mode change button
const int pauseResetButtonPin = 14;  // Pin for pause/reset button

EasyButton modeChangeButton(modeChangeButtonPin);
EasyButton pauseResetButton(pauseResetButtonPin);

bool stopwatchMode = false;
bool isPaused = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup() {
  Serial.begin(115200);

  pinMode(modeChangeButtonPin, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(pauseResetButtonPin, INPUT_PULLUP); // Enable internal pull-up resistor

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  // Clear the display before setting up "ESD Project"
  display.clearDisplay();

  // Set cursor position and display "ESD Project"
  display.setCursor(0, 20);
  display.print("ES Project");
  display.setTextSize(1);
  display.setCursor(40, 40);
  display.print("by Atharva");
  display.display();
  delay(2000);

  display.clearDisplay();

  // Configure buttons
  modeChangeButton.begin();
  modeChangeButton.onPressed(switchModes);

  pauseResetButton.begin();
  pauseResetButton.onPressed(pauseReset);
  pauseResetButton.onPressedFor(1000, resetStopwatch);
}

void loop() {
  DateTime now = rtc.now();
  unsigned long currentTime = millis();

  modeChangeButton.read();
  pauseResetButton.read();

  if (stopwatchMode && !isPaused) {
    elapsedTime = currentTime - startTime;
  }

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 20);

  if (stopwatchMode) {
    display.print("Stopwatch:");
    display.setTextSize(2);
    display.setCursor(0, 40);
    display.print(elapsedTime / 1000);  // Display elapsed time in seconds
  } else {
    // Display regular time
    display.print("India Time:");
    display.setTextSize(2);
    display.setCursor(0, 40);
    display.print(now.hour(), DEC);
    display.print(':');
    display.print(now.minute(), DEC);
    display.print(':');
    display.print(now.second(), DEC);
  }

  display.display();
  delay(100);
  display.clearDisplay();
}

void switchModes() {
  stopwatchMode = !stopwatchMode;
  startTime = millis();
  isPaused = false;
  elapsedTime = 0;
}

void pauseReset() {
  if (stopwatchMode) {
    isPaused = !isPaused;
    if (!isPaused) {
      startTime = millis() - elapsedTime; // Adjust start time for the paused duration
    }
  }
}

void resetStopwatch() {
  if (stopwatchMode) {
    elapsedTime = 0;
    startTime = millis(); // Reset start time to current time
    isPaused = true;
  }
}
