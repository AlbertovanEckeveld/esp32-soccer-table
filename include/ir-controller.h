#ifndef IR_CONTROLLER_H
#define IR_CONTROLLER_H

#include <Arduino.h>

#define IR_SENSOR_GOAL_1_PIN 18    // IR sensor for goal 1 (Team A)
#define IR_SENSOR_GOAL_2_PIN 19    // IR sensor for goal 2 (Team B)

#define IR_DEBOUNCE_TIME 500       // Debounce time in milliseconds
#define IR_BLOCKED_THRESHOLD 10    // Number of consecutive readings to confirm goal

// Game configuration
#define POINTS_TO_WIN 10           // Points needed to win a game

// Game states
enum GameState {
  GAME_ACTIVE,
  GAME_WON_TEAM_A,
  GAME_WON_TEAM_B,
  GAME_CELEBRATION
};

enum Team {
  TEAM_A = 1,
  TEAM_B = 2
};

struct GoalEvent {
  Team team;
  unsigned long timestamp;
  bool isValid;
};

void initIRSensors();
void updateIRSensors();
GoalEvent checkForGoal();
bool isGoalDetected(int sensorPin);
void resetGoalDetection();

// Game management functions
void startNewGame();
void checkGameEnd();
bool isGameActive();
GameState getGameState();
void onGameWon(Team winningTeam);
void celebrateGameWin(Team winningTeam);
void onGameWinCelebrationEnd();

void onGoalScored(Team team);
void celebrateGoal(Team team);

bool readIRSensor(int pin);
void printGoalEvent(GoalEvent event);

// Score and game state variables
extern int scoreTeamA;
extern int scoreTeamB;
extern GameState currentGameState;

void incrementScore(Team team);
void resetScore();
void printScore();
void printGameStatus();

#endif // IR_CONTROLLER_H
