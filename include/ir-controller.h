#ifndef IR_CONTROLLER_H
#define IR_CONTROLLER_H

#include <Arduino.h>

#define IR_SENSOR_GOAL_1_PIN 18    // IR sensor for goal 1 (Team A)
#define IR_SENSOR_GOAL_2_PIN 19    // IR sensor for goal 2 (Team B)

#define IR_DEBOUNCE_TIME 500       // Debounce time in milliseconds
#define IR_BLOCKED_THRESHOLD 10    // Number of consecutive readings to confirm goal

enum GoalState {
  NO_GOAL,
  GOAL_TEAM_A,
  GOAL_TEAM_B
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

void onGoalScored(Team team);
void celebrateGoal(Team team);

bool readIRSensor(int pin);
void printGoalEvent(GoalEvent event);

extern int scoreTeamA;
extern int scoreTeamB;
void incrementScore(Team team);
void resetScore();
void printScore();

#endif // IR_CONTROLLER_H
