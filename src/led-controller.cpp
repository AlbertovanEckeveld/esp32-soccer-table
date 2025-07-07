#include "led-controller.h"

CRGB leds[NUM_LEDS];
LEDEffect currentEffect = LED_OFF;
CRGB waveColor = CRGB::Red;
unsigned long lastUpdate = 0;
unsigned long wavePosition = 0;
uint8_t breathingBrightness = 0;
bool breathingDirection = true;

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
  }
}

void setLEDEffect(LEDEffect effect) {
  if (currentEffect != effect) {
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
