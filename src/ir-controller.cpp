#include "ir-controller.h"
#include "led-controller.h" 

// Score and game state variables
int scoreTeamA = 0;
int scoreTeamB = 0;
GameState currentGameState = GAME_ACTIVE;

unsigned long lastGoalTime = 0;
unsigned long lastSensorCheck = 0;
bool goal1Blocked = false;
bool goal2Blocked = false;
int goal1BlockedCount = 0;
int goal2BlockedCount = 0;

unsigned long lastGoal1TriggerTime = 0;
unsigned long lastGoal2TriggerTime = 0;

void initIRSensors() {
  Serial.println("Initializing IR sensors for goal detection...");
  
  pinMode(IR_SENSOR_GOAL_1_PIN, INPUT_PULLUP);
  pinMode(IR_SENSOR_GOAL_2_PIN, INPUT_PULLUP);
  
  startNewGame(); // Initialize new game
  
  Serial.println("IR sensors initialized:");
  Serial.print("- Goal 1 (Team A) sensor on pin ");
  Serial.println(IR_SENSOR_GOAL_1_PIN);
  Serial.print("- Goal 2 (Team B) sensor on pin ");
  Serial.println(IR_SENSOR_GOAL_2_PIN);
  Serial.println("⚽ Soccer table ready for 10-point games! ⚽");
  printGameStatus();
}

void updateIRSensors() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastSensorCheck < 10) {
    return;
  }
  lastSensorCheck = currentTime;
  
  bool sensor1Triggered = !readIRSensor(IR_SENSOR_GOAL_1_PIN); // Inverted because of pull-up
  bool sensor2Triggered = !readIRSensor(IR_SENSOR_GOAL_2_PIN); // Inverted because of pull-up
  
  // Goal 1 (Team A) detection with debouncing
  if (sensor1Triggered) {
    goal1BlockedCount++;
    if (goal1BlockedCount >= IR_BLOCKED_THRESHOLD && !goal1Blocked) {
      // Check debounce time
      if (currentTime - lastGoal1TriggerTime > IR_DEBOUNCE_TIME) {
        goal1Blocked = true;
        lastGoal1TriggerTime = currentTime;
        onGoalScored(TEAM_A);
      }
    }
  } else {
    goal1BlockedCount = 0;
    if (goal1Blocked && currentTime - lastGoal1TriggerTime > 1000) {
      goal1Blocked = false; 
    }
  }
  
  // Goal 2 (Team B) detection with debouncing
  if (sensor2Triggered) {
    goal2BlockedCount++;
    if (goal2BlockedCount >= IR_BLOCKED_THRESHOLD && !goal2Blocked) {
      // Check debounce time
      if (currentTime - lastGoal2TriggerTime > IR_DEBOUNCE_TIME) {
        goal2Blocked = true;
        lastGoal2TriggerTime = currentTime;
        onGoalScored(TEAM_B);
      }
    }
  } else {
    goal2BlockedCount = 0;
    if (goal2Blocked && currentTime - lastGoal2TriggerTime > 1000) {
      goal2Blocked = false; 
    }
  }
}

GoalEvent checkForGoal() {
  GoalEvent event;
  event.isValid = false;
  
  unsigned long currentTime = millis();
  
  // Check if enough time has passed since last goal
  if (currentTime - lastGoalTime < IR_DEBOUNCE_TIME) {
    return event;
  }
  
  // Check Goal 1 (Team A)
  if (isGoalDetected(IR_SENSOR_GOAL_1_PIN)) {
    event.team = TEAM_A;
    event.timestamp = currentTime;
    event.isValid = true;
    lastGoalTime = currentTime;
    return event;
  }
  
  // Check Goal 2 (Team B)
  if (isGoalDetected(IR_SENSOR_GOAL_2_PIN)) {
    event.team = TEAM_B;
    event.timestamp = currentTime;
    event.isValid = true;
    lastGoalTime = currentTime;
    return event;
  }
  
  return event;
}

bool isGoalDetected(int sensorPin) {
  // Read sensor (LOW means object detected with pull-up resistor)
  return !digitalRead(sensorPin);
}

void resetGoalDetection() {
  lastGoalTime = 0;
  goal1Blocked = false;
  goal2Blocked = false;
  goal1BlockedCount = 0;
  goal2BlockedCount = 0;
  lastGoal1TriggerTime = 0;
  lastGoal2TriggerTime = 0;
}

void onGoalScored(Team team) {
  // Only count goals during active game
  if (!isGameActive()) {
    Serial.println("⚠️ Goal detected but game is not active!");
    return;
  }
  
  Serial.println("🥅 GOAL SCORED! 🥅");
  
  // Increment score (this will also check for game end)
  incrementScore(team);
  
  // Print goal information
  Serial.print("Team ");
  Serial.print((team == TEAM_A) ? "A (YELLOW)" : "B (ORANGE)");
  Serial.println(" scored!");
  printScore();
  
  // Only trigger goal celebration if game is still active
  // (if game ended, the game win celebration will be triggered instead)
  if (isGameActive()) {
    celebrateGoal(team);
  }
}

void celebrateGoal(Team team) {
  // Trigger LED celebration wave with team colors
  Serial.print("🎉 Celebrating goal for Team ");
  Serial.println((team == TEAM_A) ? "A" : "B");
  
  // Trigger LED celebration (1 = Team A, 2 = Team B)
  triggerGoalCelebration((team == TEAM_A) ? 1 : 2);
}

bool readIRSensor(int pin) {
  return digitalRead(pin);
}

void printGoalEvent(GoalEvent event) {
  if (event.isValid) {
    Serial.print("Goal Event - Team: ");
    Serial.print((event.team == TEAM_A) ? "A" : "B");
    Serial.print(", Time: ");
    Serial.println(event.timestamp);
  }
}

void incrementScore(Team team) {
  if (team == TEAM_A) {
    scoreTeamA++;
  } else if (team == TEAM_B) {
    scoreTeamB++;
  }
  
  // Check if game is won after scoring
  checkGameEnd();
}

void resetScore() {
  scoreTeamA = 0;
  scoreTeamB = 0;
  Serial.println("Score reset to 0-0");
}

void printScore() {
  Serial.print("Current Score - Team A: ");
  Serial.print(scoreTeamA);
  Serial.print(" | Team B: ");
  Serial.println(scoreTeamB);
}

// ===========================================
// GAME MANAGEMENT FUNCTIONS
// ===========================================

void startNewGame() {
  Serial.println("🏁 Starting new game! First to 10 points wins! 🏁");
  resetScore();
  resetGoalDetection();
  currentGameState = GAME_ACTIVE;
  printGameStatus();
}

void checkGameEnd() {
  if (currentGameState != GAME_ACTIVE) {
    return; // Game already ended
  }
  
  if (scoreTeamA >= POINTS_TO_WIN) {
    currentGameState = GAME_WON_TEAM_A;
    onGameWon(TEAM_A);
  } else if (scoreTeamB >= POINTS_TO_WIN) {
    currentGameState = GAME_WON_TEAM_B;
    onGameWon(TEAM_B);
  }
}

bool isGameActive() {
  return currentGameState == GAME_ACTIVE;
}

GameState getGameState() {
  return currentGameState;
}

void onGameWon(Team winningTeam) {
  Serial.println();
  Serial.println("🏆🏆🏆 GAME WON! 🏆🏆🏆");
  Serial.print("Team ");
  Serial.print((winningTeam == TEAM_A) ? "A (YELLOW)" : "B (ORANGE)");
  Serial.println(" wins the game!");
  printScore();
  Serial.println("🎉 Game celebration starting! 🎉");
  
  // Trigger game win celebration
  celebrateGameWin(winningTeam);
  
  // Note: New game will start automatically after celebration ends
  Serial.println("New game will start automatically after celebration!");
}

void celebrateGameWin(Team winningTeam) {
  // Trigger extended LED celebration for game win
  Serial.print("🎆 Celebrating GAME WIN for Team ");
  Serial.println((winningTeam == TEAM_A) ? "A" : "B");
  
  // Trigger game win celebration (1 = Team A, 2 = Team B)
  triggerGameWinCelebration((winningTeam == TEAM_A) ? 1 : 2);
}

void onGameWinCelebrationEnd() {
  Serial.println("🏁 Game win celebration ended - Starting new game!");
  startNewGame();
}

void printGameStatus() {
  Serial.println("📊 Game Status:");
  Serial.print("🎯 Target: ");
  Serial.print(POINTS_TO_WIN);
  Serial.println(" points to win");
  printScore();
  
  switch (currentGameState) {
    case GAME_ACTIVE:
      Serial.println("⚽ Game is ACTIVE - Play on!");
      break;
    case GAME_WON_TEAM_A:
      Serial.println("🏆 Team A has WON the game!");
      break;
    case GAME_WON_TEAM_B:
      Serial.println("🏆 Team B has WON the game!");
      break;
    case GAME_CELEBRATION:
      Serial.println("🎉 Game celebration in progress...");
      break;
  }
  Serial.println("-------------------");
}
