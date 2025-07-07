#include "led-controller.h"

#define WAVE_SPEED 50        
#define WAVE_WIDTH 20     


CRGB leds[NUM_LEDS];
LEDEffect currentEffect = LED_OFF;
LEDEffect previousEffect = LED_FULL_WHITE; // Store previous effect for returning after celebration
CRGB waveColor = CRGB::Red;
unsigned long lastUpdate = 0;
unsigned long wavePosition = 0;
uint8_t breathingBrightness = 0;
bool breathingDirection = true;

// Goal celebration variables
bool celebrationActive = false;
unsigned long celebrationStartTime = 0;
LEDEffect celebrationEffect = LED_OFF;
CRGB celebrationColor = CRGB::White;

const int sectionStarts[] = {SECTION_1_START, SECTION_2_START, SECTION_3_START, SECTION_4_START};
const int sectionEnds[] = {SECTION_1_END, SECTION_2_END, SECTION_3_END, SECTION_4_END};
const int sectionLengths[] = {72, 42, 72, 42}; // LEDs per section

void initLEDs() {
  Serial.println("Initializing LED strip...");
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  
  Serial.print("LED strip initialized with ");
  Serial.print(NUM_LEDS);
  Serial.println(" LEDs");
  Serial.println("Sections: 72 + 42 + 72 + 42 + 72 unused LEDs");
}

void updateLEDs() {
  unsigned long currentTime = millis();
  
  // Check if celebration should end
  if (celebrationActive && currentTime - celebrationStartTime > CELEBRATION_DURATION) {
    endCelebration();
  }
  
  // Handle celebration effects with higher priority
  if (celebrationActive) {
    if (currentTime - lastUpdate >= CELEBRATION_WAVE_SPEED) {
      showGoalCelebration();
      lastUpdate = currentTime;
    }
    return; // Skip normal effects during celebration
  }
  
  // Normal effect updates
  switch (currentEffect) {
    case LED_OFF:
      // No update needed
      break;
      
    case LED_FULL_WHITE:
      // Static effect, no animation needed
      break;
      
    case LED_COLOR_WAVE:
      if (currentTime - lastUpdate >= WAVE_SPEED) {
        showColorWave();
        lastUpdate = currentTime;
      }
      break;
      
    case LED_RAINBOW_WAVE:
      if (currentTime - lastUpdate >= WAVE_SPEED) {
        showRainbowWave();
        lastUpdate = currentTime;
      }
      break;
      
    case LED_BREATHING:
      if (currentTime - lastUpdate >= 20) { 
        showBreathing();
        lastUpdate = currentTime;
      }
      break;
      
    case LED_GOAL_CELEBRATION_A:
    case LED_GOAL_CELEBRATION_B:
      // These are handled above in celebration logic
      break;
  }
}

void setLEDEffect(LEDEffect effect) {
  if (currentEffect != effect && !celebrationActive) {
    previousEffect = currentEffect; // Store previous effect
    currentEffect = effect;
    FastLED.clear();
    
    Serial.print("LED Effect changed to: ");
    switch (effect) {
      case LED_OFF:
        Serial.println("OFF");
        turnOffLEDs();
        break;
      case LED_FULL_WHITE:
        Serial.println("FULL WHITE");
        showFullWhite();
        break;
      case LED_COLOR_WAVE:
        Serial.println("COLOR WAVE");
        wavePosition = 0;
        break;
      case LED_RAINBOW_WAVE:
        Serial.println("RAINBOW WAVE");
        wavePosition = 0;
        break;
      case LED_BREATHING:
        Serial.println("BREATHING");
        breathingBrightness = 0;
        breathingDirection = true;
        break;
      case LED_GOAL_CELEBRATION_A:
        Serial.println("GOAL CELEBRATION TEAM A");
        break;
      case LED_GOAL_CELEBRATION_B:
        Serial.println("GOAL CELEBRATION TEAM B");
        break;
    }
  }
}

void setWaveColor(CRGB color) {
  waveColor = color;
  Serial.print("Wave color set to RGB(");
  Serial.print(color.r);
  Serial.print(", ");
  Serial.print(color.g);
  Serial.print(", ");
  Serial.print(color.b);
  Serial.println(")");
}

void setBrightness(uint8_t brightness) {
  FastLED.setBrightness(brightness);
  FastLED.show();
  Serial.print("Brightness set to: ");
  Serial.println(brightness);
}

void showFullWhite() {
  for (int section = 0; section < 4; section++) {
    fillSection(sectionStarts[section], sectionEnds[section], CRGB::White);
  }
  FastLED.show();
}

void showColorWave() {
  FastLED.clear();
  
  for (int section = 0; section < 4; section++) {
    int sectionStart = sectionStarts[section];
    int sectionEnd = sectionEnds[section];
    int sectionLength = sectionLengths[section];
    
    int localWavePos = (wavePosition + section * 30) % (sectionLength + WAVE_WIDTH);
    
    for (int i = sectionStart; i <= sectionEnd; i++) {
      int localIndex = i - sectionStart;
      uint8_t intensity = getWaveIntensity(localIndex, localWavePos, WAVE_WIDTH);
      
      if (intensity > 0) {
        leds[i] = waveColor;
        leds[i].fadeToBlackBy(255 - intensity);
      }
    }
  }
  
  wavePosition++;
  if (wavePosition > 300) wavePosition = 0; 
  
  FastLED.show();
}

void showRainbowWave() {
  FastLED.clear();
  
  for (int section = 0; section < 4; section++) {
    int sectionStart = sectionStarts[section];
    int sectionEnd = sectionEnds[section];
    int sectionLength = sectionLengths[section];
    
    int localWavePos = (wavePosition + section * 20) % (sectionLength + WAVE_WIDTH);
    
    for (int i = sectionStart; i <= sectionEnd; i++) {
      int localIndex = i - sectionStart;
      uint8_t intensity = getWaveIntensity(localIndex, localWavePos, WAVE_WIDTH);
      
      if (intensity > 0) {
        uint8_t hue = (i * 255 / NUM_LEDS + wavePosition * 2) % 255;
        leds[i] = CHSV(hue, 255, intensity);
      }
    }
  }
  
  wavePosition++;
  if (wavePosition > 300) wavePosition = 0;
  
  FastLED.show();
}

void showBreathing() {
  for (int section = 0; section < 4; section++) {
    fillSection(sectionStarts[section], sectionEnds[section], CRGB::White);
  }
  
  if (breathingDirection) {
    breathingBrightness += 2;
    if (breathingBrightness >= 255) {
      breathingBrightness = 255;
      breathingDirection = false;
    }
  } else {
    breathingBrightness -= 2;
    if (breathingBrightness <= 20) {
      breathingBrightness = 20;
      breathingDirection = true;
    }
  }
  
  FastLED.setBrightness(breathingBrightness);
  FastLED.show();
}

void turnOffLEDs() {
  FastLED.clear();
  FastLED.show();
}

void fillSection(int startLED, int endLED, CRGB color) {
  for (int i = startLED; i <= endLED; i++) {
    leds[i] = color;
  }
}

void fadeSection(int startLED, int endLED, uint8_t fadeAmount) {
  for (int i = startLED; i <= endLED; i++) {
    leds[i].fadeToBlackBy(fadeAmount);
  }
}

int getSectionStart(int section) {
  if (section >= 0 && section < 4) {
    return sectionStarts[section];
  }
  return 0;
}

int getSectionEnd(int section) {
  if (section >= 0 && section < 4) {
    return sectionEnds[section];
  }
  return 0;
}

int getSectionLength(int section) {
  if (section >= 0 && section < 4) {
    return sectionLengths[section];
  }
  return 0;
}

uint8_t getWaveIntensity(int position, int waveCenter, int waveWidth) {
  int distance = abs(position - waveCenter);
  
  if (distance <= waveWidth / 2) {
    // Calculate intensity based on distance from wave center
    float intensity = 1.0 - (float)distance / (waveWidth / 2);
    return (uint8_t)(intensity * 255);
  }
  
  return 0;
}

CRGB blendColors(CRGB color1, CRGB color2, uint8_t blend) {
  return CRGB(
    ((uint16_t)color1.r * (255 - blend) + (uint16_t)color2.r * blend) / 255,
    ((uint16_t)color1.g * (255 - blend) + (uint16_t)color2.g * blend) / 255,
    ((uint16_t)color1.b * (255 - blend) + (uint16_t)color2.b * blend) / 255
  );
}

// Goal celebration functions
void triggerGoalCelebration(int team) {
  if (celebrationActive) return; // Don't interrupt ongoing celebration
  
  Serial.print("🎉 Starting goal celebration for Team ");
  Serial.println((team == 1) ? "A (RED)" : "B (BLUE)");
  
  celebrationActive = true;
  celebrationStartTime = millis();
  previousEffect = currentEffect; // Store current effect to restore later
  
  if (team == 1) {
    celebrationEffect = LED_GOAL_CELEBRATION_A;
    celebrationColor = TEAM_A_COLOR;
    currentEffect = LED_GOAL_CELEBRATION_A;
  } else {
    celebrationEffect = LED_GOAL_CELEBRATION_B;
    celebrationColor = TEAM_B_COLOR;
    currentEffect = LED_GOAL_CELEBRATION_B;
  }
  
  wavePosition = 0; // Reset wave position for celebration
  FastLED.setBrightness(255); // Full brightness for celebration
}

void showGoalCelebration() {
  // Create intense team-colored wave effect
  FastLED.clear();
  
  // Multiple waves for more dramatic effect
  for (int waveOffset = 0; waveOffset < 3; waveOffset++) {
    int currentWavePos = (wavePosition + waveOffset * 50) % 300;
    
    for (int section = 0; section < 4; section++) {
      int sectionStart = sectionStarts[section];
      int sectionEnd = sectionEnds[section];
      int sectionLength = sectionLengths[section];
      
      // Map wave position to section
      int localWavePos = (currentWavePos + section * 30) % (sectionLength + WAVE_WIDTH);
      
      for (int i = sectionStart; i <= sectionEnd; i++) {
        int localIndex = i - sectionStart;
        uint8_t intensity = getWaveIntensity(localIndex, localWavePos, WAVE_WIDTH);
        
        if (intensity > 0) {
          // Blend with existing color for multiple wave effect
          CRGB newColor = celebrationColor;
          newColor.fadeToBlackBy(255 - intensity);
          
          if (leds[i]) {
            leds[i] = blendColors(leds[i], newColor, 128);
          } else {
            leds[i] = newColor;
          }
        }
      }
    }
  }
  
  // Add sparkle effect for extra celebration
  if (random(100) < 30) { // 30% chance per update
    int sparklePos = random(NUM_LEDS);
    if (sparklePos <= SECTION_4_END) { // Only in used sections
      leds[sparklePos] = CRGB::White;
    }
  }
  
  wavePosition += 3; // Faster wave for celebration
  FastLED.show();
}

bool isCelebrationActive() {
  return celebrationActive;
}

void endCelebration() {
  if (!celebrationActive) return;
  
  Serial.println("🏁 Goal celebration ended");
  celebrationActive = false;
  
  // Restore previous effect and brightness
  FastLED.setBrightness(BRIGHTNESS);
  setLEDEffect(previousEffect);
}
