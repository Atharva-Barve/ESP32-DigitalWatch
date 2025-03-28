#include <Wire.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include <EasyButton.h>
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS     5
#define TFT_RST    4
#define TFT_DC     1

RTC_DS1307 rtc;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

const int modeChangeButtonPin = 12;  // Pin for mode change button
const int pauseResetButtonPin = 14;  // Pin for pause/reset button

EasyButton modeChangeButton(modeChangeButtonPin);
EasyButton pauseResetButton(pauseResetButtonPin);

bool stopwatchMode = false;
bool isPaused = false;
unsigned long startTime = 0;
unsigned long elapsedTime = 0;

void setup() 
{
  Serial.begin(115200);
  tft.initR(INITR_BLACKTAB); // Initialize ST7735 with black tab
  tft.setRotation(0); // Adjust rotation if needed (0, 1, 2, or 3)

  pinMode(modeChangeButtonPin, INPUT_PULLUP); // Enable internal pull-up resistor
  pinMode(pauseResetButtonPin, INPUT_PULLUP); // Enable internal pull-up resistor

  if (!rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (!rtc.isrunning()) 
  {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }


  tft.fillScreen(ST7735_BLACK); // Fill screen with black color
  tft.setCursor(20, 70); // Set cursor position
  tft.setTextColor(ST7735_WHITE); // Set text color
  tft.setTextSize(2); // Set text size
  tft.println("ATHARVA."); // Print text
  delay(4000); // Wait for 4 seconds
  tft.fillScreen(ST7735_BLACK); // Fill screen with black color
  
  modeChangeButton.begin();
  modeChangeButton.onPressed(switchModes);

  pauseResetButton.begin();
  pauseResetButton.onPressed(pauseReset);
  pauseResetButton.onPressedFor(1000, resetStopwatch);


}

void loop() 
{
  DateTime now = rtc.now();
  unsigned long currentTime = millis();

  modeChangeButton.read();
  pauseResetButton.read();

  if (stopwatchMode && !isPaused) 
  {
    elapsedTime = currentTime - startTime;
  }

  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(0, 20);

  if (stopwatchMode) 
  {
    tft.setCursor(5, 10);
    tft.println("Stopwatch:");
    tft.setTextSize(2);
    tft.setCursor(5, 40);
    tft.println(elapsedTime / 1000);  // Display elapsed time in seconds
  } 
  else 
  {
    // Display regular time
    tft.println("India Time:");
    tft.setTextSize(2);
    tft.setCursor(0, 40);
    tft.print(now.hour(), DEC);
    tft.print(':');
    tft.print(now.minute(), DEC);
    tft.print(':');
    tft.print(now.second(), DEC);
  }

//  tft.initR(INITR_BLACKTAB); // Initialize ST7735 with black tab
  delay(100);
  tft.fillRect(0, 0, tft.width(), tft.height(), ST7735_BLACK); // Clear the screen
//  tft.fillScreen(ST7735_BLACK);

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

