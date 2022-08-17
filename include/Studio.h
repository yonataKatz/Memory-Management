#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string.h>
#include "Workout.h"
#include "Trainer.h"
#include "Action.h"
#include <sstream>
extern Studio* backup;

class Studio{		
public:
	Studio();

    //rule of 5
    virtual ~Studio();
    Studio (const Studio &other);
    Studio& operator=(const Studio &other);
    Studio (Studio &&other);
    Studio& operator=(Studio &&other);

    Studio(const std::string &configFilePath);
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
};

#endif