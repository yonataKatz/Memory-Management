#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "Customer.h"
#include "Workout.h"

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    Trainer(int t_capacity);

    //rule of 5
    virtual ~Trainer();
    Trainer (const Trainer &other);
    Trainer&  operator=(const Trainer &other);
    Trainer ( Trainer &&other);
    Trainer& operator=( Trainer &&other);

    int getCapacity() const;
    void addOrder(OrderPair op);
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    //aded method to calculate current opened session salary + the salary we got so far
    int getSalary();
    bool isOpen();
    // TO MAINTAIN SALARY WE NEED MORE METHODS
    int currentSessionSal();
    void addToSal(int sal);

private:
    int capacity;
    bool open;
    int salary;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
};


#endif