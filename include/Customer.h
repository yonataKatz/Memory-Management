#ifndef CUSTOMER_H_
#define CUSTOMER_H_

#include <vector>
#include <string>
#include "Workout.h"

using namespace std;

class Customer{
public:
    Customer(std::string c_name, int c_id);
    //default distructor
    virtual ~Customer();
    virtual std::vector<int> order(const std::vector<Workout> &workout_options)=0;
    virtual std::string toString() const = 0;
    //cloning of customers later to use in copy constructor and assignment operator of Trainer
    virtual Customer* clone()=0;
    std::string getName() const;
    int getId() const;
    // setter and getter to field ordered
    bool hasOrdered();
    void makeOrder();
private:
    const std::string name;
    const int id;
    bool ordered;
};


class SweatyCustomer : public Customer {
public:
	SweatyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    SweatyCustomer* clone();
private:
};


class CheapCustomer : public Customer {
public:
	CheapCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    CheapCustomer* clone ();

private:
};


class HeavyMuscleCustomer : public Customer {
public:
	HeavyMuscleCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    HeavyMuscleCustomer* clone();
private:
};


class FullBodyCustomer : public Customer {
public:
	FullBodyCustomer(std::string name, int id);
    std::vector<int> order(const std::vector<Workout> &workout_options);
    std::string toString() const;
    FullBodyCustomer* clone ();
private:
};


#endif