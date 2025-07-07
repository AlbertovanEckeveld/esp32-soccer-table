#ifndef IR_CONTROLLER_H
#define IR_CONTROLLER_H

#include <Arduino.h>

// IR Sensor Configuration
#define IR_SENSOR_GOAL_1_PIN 18    // IR sensor for goal 1 (Team A)
#define IR_SENSOR_GOAL_2_PIN 19    // IR sensor for goal 2 (Team B)

// IR sensor settings
#define IR_DEBOUNCE_TIME 500       // Debounce time in milliseconds
#define IR_BLOCKED_THRESHOLD 10    // Number of consecutive readings to confirm goal

// Goal detection states
enum GoalState {
  NO_GOAL,
  GOAL_TEAM_A,
  GOAL_TEAM_B
};

// Team definitions
enum Team {
  TEAM_A = 1,
  TEAM_B = 2
};

// Goal detection structure
struct GoalEvent {
  Team team;
  unsigned long timestamp;
  bool isValid;
};

// Function declarations
void initIRSensors();
void updateIRSensors();
GoalEvent checkForGoal();
bool isGoalDetected(int sensorPin);
void resetGoalDetection();

// Goal scoring functions
void onGoalScored(Team team);
void celebrateGoal(Team team);

// Utility functions
bool readIRSensor(int pin);
void printGoalEvent(GoalEvent event);

// Score tracking
extern int scoreTeamA;
extern int scoreTeamB;
void incrementScore(Team team);
void resetScore();
void printScore();

#endif // IR_CONTROLLER_H
