using namespace std;
#include "Trainer.h"
#include "vector"
#include "Customer.h"
#include "Studio.h"
    // check the syntax
    // is it supposed to be on the heap ? (vectors)
    Trainer::Trainer(int t_capacity): capacity(t_capacity), open(false),salary(0), customersList(), orderList()
    {}

    Trainer::~Trainer()
    {
        if (customersList.size()!=0)
        {
            for (int i=0; i<static_cast<int>(customersList.size()); i++)
                delete customersList[i];
            customersList.clear();
            orderList.clear();
        }
    }

    //copy constructor
    Trainer::Trainer (const Trainer &other)
    {
        capacity=other.capacity;
        open = other.open;
        salary = other.salary;
        for (int i=0; i<static_cast<int>(other.customersList.size()); i++)
        {
            customersList.push_back(other.customersList[i]->clone());
        }
        for (int i=0; i<static_cast<int>(other.orderList.size()); i++)
            orderList.push_back(make_pair(other.orderList[i].first,other.orderList[i].second));
    }

    //assignment operator
    Trainer& Trainer::operator=(const Trainer &other)
    {
        capacity=other.getCapacity();
        open=other.open;
        salary=other.salary;
        // we need to clear thr vectors without the arguments which are resources
        if (this!=&other) {
            customersList.clear();
            orderList.clear();
            for (int i=0; i<static_cast<int>(other.customersList.size()); i++)
            {
                // deep copy of customer - how to use CLONE---------------------------------------- is it supposed to be static
                Customer* copy = (*other.customersList[i]).clone();
                customersList.push_back(copy);
            }
            for (int i=0; i<static_cast<int>(other.orderList.size()); i++)
                orderList.push_back(make_pair(other.orderList[i].first,other.orderList[i].second));
        }
        return *this;
    }

    //move constructor
    Trainer::Trainer ( Trainer &&other)
    {
        capacity=other.capacity;
        open=other.open;
        salary=other.salary;
        for (int i=0; i<static_cast<int>(other.customersList.size()); i++) {
            customersList.push_back(other.customersList[i]);
            other.customersList[i]=nullptr;
        }
        other.customersList.clear();
        for (int i=0; i<static_cast<int>(other.orderList.size()); i++) {
            orderList.push_back(make_pair(other.orderList[i].first,other.orderList[i].second));
        }
        other.orderList.clear();

    }

    //move assignment operator
    Trainer& Trainer::operator=(Trainer &&other)
    {
        capacity=other.capacity;
        open=other.open;
        salary=other.salary;
        for (int i=0; i<static_cast<int>(customersList.size()); i++)
        for (int i=0; i<static_cast<int>(customersList.size()); i++)
            delete customersList[i];
        customersList.clear();
        orderList.clear();
        for (int i=0; i<static_cast<int>(other.customersList.size()); i++) {
            customersList.push_back(other.customersList[i]);
            other.customersList[i]=nullptr;
        }
        other.customersList.clear();
        for (int i=0; i<static_cast<int>(other.orderList.size()); i++) {
            orderList.push_back(make_pair(other.orderList[i].first,other.orderList[i].second));
        }
        other.orderList.clear();

        return *this;
    }



    int Trainer::getCapacity() const
    {
        return capacity;
    }
    void Trainer::addOrder(OrderPair op)
    {
        orderList.push_back(op);
    }
    void Trainer:: addCustomer(Customer* customer)
    {
        customersList.push_back(customer);
    }
    void Trainer::removeCustomer(int id)
    {
        for (int i=0; i<static_cast<int>(customersList.size()); i++)
        {
            if (customersList[i]->getId()==id)
                customersList.erase(customersList.begin()+i);
        }
        // instead of deleting the pairs - we put -1 ????????????
        vector<OrderPair> v ;
        for (int i=0; i<static_cast<int>(orderList.size()); i++)
        {
            if (orderList[i].first!=id)
                v.push_back(OrderPair(orderList[i].first,orderList[i].second));
        }
        orderList.clear();
        for (int i=0; i<static_cast<int>(v.size()); i++)
            orderList.push_back(OrderPair (v[i].first,v[i].second));

    }

    //returns null if no such id exists
    Customer* Trainer:: getCustomer(int id)
    {
        for (int i=0; i<static_cast<int>(customersList.size()); i++)
        {
            if (customersList[i]->getId()==id)
                return customersList[i];
        }
        return nullptr;
    }

    std::vector<Customer*>& Trainer:: getCustomers()
    {
        return customersList;
    }

    std::vector<OrderPair>& Trainer::getOrders()
    {
        return orderList;
    }

    //still need to update the customersList
    void Trainer:: order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options)
    {
        //assumes that this specific customer is not assigned to this trainer
        if (open)
        {
            capacity++;
            for (int i=0; i<static_cast<int>(workout_ids.size()); i++)
                for (int j=0; j<static_cast<int>(workout_options.size()); j++)
                {
                    if (workout_options[j].getId()==workout_ids[i])
                    {
                        orderList.push_back(OrderPair(customer_id,workout_options[j]));
                    }
                }
        }
    }

    void Trainer:: openTrainer()
    {
        open=true;
    }

    void Trainer:: closeTrainer()
    {
        open=false;
        for (int i=0; i<static_cast<int>(getCustomers().size()); i++)
            if (customersList[i])
                delete getCustomers()[i];
        customersList.clear();
        orderList.clear();
    }

    int Trainer:: getSalary()
    {
        return salary + currentSessionSal();
    }

    bool Trainer:: isOpen()
    {
        return open;
    }

    int Trainer::currentSessionSal()
    {
        int sum=0;
        for (int i=0; i<static_cast<int>(orderList.size()); i++)
        {
            if (orderList[i].first>=0)
                sum=sum+orderList[i].second.getPrice();
        }
        return sum;
    }

    void Trainer::addToSal(int sal)
    {
        salary=salary+sal;
    }


;
