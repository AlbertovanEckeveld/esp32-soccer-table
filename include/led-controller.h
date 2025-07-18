#ifndef LED_CONTROLLER_H
#define LED_CONTROLLER_H

#include <Arduino.h>
#include <FastLED.h>

#define LED_PIN 2
#define NUM_LEDS 300  // 5m * 60 LEDs/m = 300 LEDs
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define BRIGHTNESS 150  

#define SECTION_1_START 0    // 120cm = 72 LEDs (0-71)
#define SECTION_1_END 71
#define SECTION_2_START 72   // 70cm = 42 LEDs (72-113)
#define SECTION_2_END 113
#define SECTION_3_START 114  // 120cm = 72 LEDs (114-185)
#define SECTION_3_END 185
#define SECTION_4_START 186  // 70cm = 42 LEDs (186-227)
#define SECTION_4_END 227
#define UNUSED_START 228     // Remaining LEDs (228-299)
#define UNUSED_END 299

// Celebration settings
#define GOAL_CELEBRATION_DURATION 3000    // 3 seconds for goal celebration
#define GAME_WIN_CELEBRATION_DURATION 10000  // 10 seconds for game win celebration
#define CELEBRATION_WAVE_SPEED 30          // Faster wave for celebration
#define GAME_WIN_WAVE_SPEED 20             // Even faster for game win

// Team colors
#define TEAM_A_COLOR CRGB::Yellow       // Team A = Yellow
#define TEAM_B_COLOR CRGB::Orange       // Team B = Orange

enum LEDEffect {
  LED_OFF,
  LED_FULL_WHITE,
  LED_COLOR_WAVE,
  LED_RAINBOW_WAVE,
  LED_BREATHING,
  LED_GOAL_CELEBRATION_A,            // Goal celebration for Team A
  LED_GOAL_CELEBRATION_B,            // Goal celebration for Team B
  LED_GAME_WIN_CELEBRATION_A,        // Game win celebration for Team A
  LED_GAME_WIN_CELEBRATION_B         // Game win celebration for Team B
};

void initLEDs();
void updateLEDs();
void setLEDEffect(LEDEffect effect);
void setWaveColor(CRGB color);
void setBrightness(uint8_t brightness);

void triggerGoalCelebration(int team); // team: 1 = Team A, 2 = Team B
void triggerGameWinCelebration(int team); // team: 1 = Team A, 2 = Team B
void showGoalCelebration();
void showGameWinCelebration();
bool isCelebrationActive();
void endCelebration();

void showFullWhite();
void showColorWave();
void showRainbowWave();
void showBreathing();
void turnOffLEDs();

void fillSection(int startLED, int endLED, CRGB color);
void fadeSection(int startLED, int endLED, uint8_t fadeAmount);
int getSectionStart(int section);
int getSectionEnd(int section);
int getSectionLength(int section);

uint8_t getWaveIntensity(int position, int waveCenter, int waveWidth);
CRGB blendColors(CRGB color1, CRGB color2, uint8_t blend);

#endif // LED_CONTROLLER_H
