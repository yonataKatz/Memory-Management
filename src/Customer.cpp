#include <bits/stdc++.h>
#include "Customer.h"



    Customer::Customer(std::string c_name, int c_id) : name(c_name), id(c_id), ordered(false)
    {}
    //default destructor
    Customer::~Customer(){
    }
    std::string Customer::getName() const {
        return name;
    }
    int Customer::getId() const {
        return id;
    }
    bool Customer::hasOrdered()
    {
        return ordered;
    }
    void Customer:: makeOrder()
    {
        ordered=true;
    }


    SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name,id){}

    std::vector<int> SweatyCustomer::order(const std::vector<Workout> &workout_options)
    {
        vector<int> v;
        for (int i=0; i<static_cast<int>(workout_options.size()); i++)
        {
            if (workout_options[i].getType()==2)
                v.push_back(workout_options[i].getId());
        }
        return v;
    }

    std::string SweatyCustomer::toString() const
    {
        return ("SWT_TOSTRING "+ getName()+" "+ to_string(getId()));
    }
    SweatyCustomer* SweatyCustomer::clone()
    {
        string name = getName();
        int id = getId();
        return new SweatyCustomer(name,id);
    }



    CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id){}

    std::vector<int> CheapCustomer::order(const std::vector<Workout> &workout_options)
    {
        vector<int> v ;
        int price =workout_options[0].getPrice();
        int j=0;
        for (int i=0; i<static_cast<int>(workout_options.size()); i++)
        {
            if (workout_options[i].getPrice()<price) {
                price = workout_options[i].getPrice();
                j = i;
            }
        }
        v.push_back (workout_options[j].getId());
        return v;
    }
    std::string CheapCustomer::toString() const
    {
        return ("CHP_TOSTRING "+ getName()+" "+ to_string(getId()));

    }
    CheapCustomer* CheapCustomer::clone ()
    {
        string name =getName();
        int id = getId();
        CheapCustomer* sc = new CheapCustomer(name,id);
        return sc;
    }



    bool compareByPrice(const Workout w1, const Workout w2)
    {
        return (w1.getPrice()>=w2.getPrice());
    }

    HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id):Customer(name,id){}

    std::vector<int> HeavyMuscleCustomer::order(const std::vector<Workout> &workout_options)
    {
        vector<Workout> v;
        for (int i=0; i<static_cast<int>(workout_options.size()); i++)
        {
            if (workout_options[i].getType()==0)
                v.push_back(workout_options[i]);
        }
        //stable_sort(v.begin(), v.end(),compareByPrice);
        //sort(v.begin(), v.end(),compareByPrice);
        vector<int> v_price;
        for (int i=0; i<static_cast<int>(v.size()); i++)
            v_price.push_back(v[i].getPrice());

        //sorts the vector
        vector<Workout> v_sor;
        sort(v_price.begin(),v_price.end());
        for (int i=0; i<static_cast<int>(v_price.size()); i++)
            for (int j=0; j<static_cast<int>(v_price.size()); j++)
                if (v[j].getPrice()==v_price[v_price.size()-1-i])
                    v_sor.push_back(v[j]);
        vector<int> v2;
        for (int i=0; i<static_cast<int>(v_sor.size()); i++)
            v2.push_back(v_sor[i].getId());
        return v2;
    }

    std::string HeavyMuscleCustomer::toString() const
    {
        return ("MCL_TOSTRING "+ getName()+" "+ to_string(getId()));
    }
    HeavyMuscleCustomer* HeavyMuscleCustomer::clone ()
    {
        string name = getName();
        int id = getId();
        HeavyMuscleCustomer* sc = new HeavyMuscleCustomer(name,id);
        return sc;
    }





    FullBodyCustomer::FullBodyCustomer(std::string name, int id):Customer(name,id){}

    std::vector<int> FullBodyCustomer::order(const std::vector<Workout> &workout_options)
    {
        int pricemincardio =2147483647;
        int priceminuneroby =2147483647;
        int pricemaxmixed=-1;
        int j_0=-1;
        int j_1=-1;
        int j_2=-1;
        vector<int> v;
        for (int i=0; i<static_cast<int>(workout_options.size()); i++)
        {
            if (workout_options[i].getType()==0) {
                if (priceminuneroby>workout_options[i].getPrice()) {
                    priceminuneroby = workout_options[i].getPrice();
                    j_0 = i;
                }
            }

            if (workout_options[i].getType()==1)
            {
                if (pricemaxmixed<workout_options[i].getPrice())
                {
                    pricemaxmixed=workout_options[i].getPrice();
                    j_1=i;
                }
            }
            if (workout_options[i].getType()==2)
            {
                if (pricemincardio >workout_options[i].getPrice())
                {
                    pricemincardio =workout_options[i].getPrice();
                    j_2=i;
                }
            }


        }
        if (j_2!=-1) {
            v.push_back(workout_options[j_2].getId());
        }
        if (j_1!=-1) {
            v.push_back(workout_options[j_1].getId());
        }
        if (j_0!=-1) {
            v.push_back(workout_options[j_0].getId());
        }
        return v;
    }
    std::string FullBodyCustomer::toString() const
    {
        return ("FBD_TOSTRING "+ getName()+" "+ to_string(getId()));
    }
    FullBodyCustomer* FullBodyCustomer::clone ()
    {
        string name = getName();
        int id = getId();
        FullBodyCustomer* sc = new FullBodyCustomer(name,id);
        return sc;
    }



