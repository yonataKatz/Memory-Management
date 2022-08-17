#include <Studio.h>
#include <iostream>
#include "string.h"
#include <fstream>

using namespace std;


    Studio::Studio() {}

    // Distructor - we have BaseActions and Trainers(which contain costumers)
    Studio::~Studio()
    {
        if (trainers.size()>0)
        {
            for(int i=0; i< static_cast<int>(trainers.size()); i++)
                delete trainers[i];
            }
        if (actionsLog.size()>0)
        {
            for(int i=0; i<static_cast<int>(actionsLog.size()); i++)
                delete actionsLog[i];
        }
        actionsLog.clear();
        trainers.clear();
        workout_options.clear();
    }

    //copy constructor
    Studio::Studio (const Studio &other)
    {
        open = other.open;
        //using copy constructor of Trainer
        for (int i=0; i < static_cast<int>(other.trainers.size()); i++)
        {
            Trainer* t1 = new Trainer(*other.trainers[i]);
            trainers.push_back(t1);
        }
        //using clone of base actions
        for (int i=0; i < static_cast<int>(other.actionsLog.size()); i++)
        {
            BaseAction* ba =other.actionsLog[i]->clone();
            actionsLog.push_back(ba);
        }
        for (int i=0; i<static_cast<int>(workout_options.size()); i++)
        {
            WorkoutType wt;
            if (other.workout_options[i].getType()==MIXED)
                wt=MIXED;
            if (other.workout_options[i].getType()==CARDIO)
                wt=CARDIO;
            if (other.workout_options[i].getType()==ANAEROBIC)
                wt=ANAEROBIC;
            Workout w (other.workout_options[i].getId(),
                       other.workout_options[i].getName(),
                       other.workout_options[i].getPrice(),
                       wt);
            workout_options.push_back(w);
        }
    }

    // assignment operator
    Studio& Studio::operator=(const Studio &other)
    {
        //checking if the current studio is te other - safe assignment
        if (this!=&other)
        {
            //cleaning the heap of the current studio before we copy the new info
            open=other.open;
            for(int i=0; i<static_cast<int>(trainers.size()); i++)
                delete trainers[i];
            trainers.clear();
            for(int i=0; i<static_cast<int>(actionsLog.size()); i++)
                delete actionsLog[i];
            actionsLog.clear();

            //deep copy of trainers;
            for (int i=0; i<static_cast<int>(other.trainers.size()); i++)
            {
                Trainer* t1 = new Trainer(*other.trainers[i]);
                trainers.push_back(t1);
            }
            //deep copy of options
            for (int i=0; i<static_cast<int>(other.workout_options.size()); i++) {
                WorkoutType wt;
                if (other.workout_options[i].getType() == MIXED)
                    wt = MIXED;
                if (other.workout_options[i].getType() == CARDIO)
                    wt = CARDIO;
                if (other.workout_options[i].getType() == ANAEROBIC)
                    wt = ANAEROBIC;
                Workout w(other.workout_options[i].getId(),
                          other.workout_options[i].getName(),
                          other.workout_options[i].getPrice(),
                          wt);
                workout_options.push_back(w);
            }

            //deep copy of actions log
            for (int i=0; i<static_cast<int>(other.actionsLog.size()); i++)
            {
                //using clone of base actions
                BaseAction* ba = other.actionsLog[i]->clone();
                actionsLog.push_back(ba);
            }
        }
        return *this;
    }

    // move constructor
    Studio::Studio (Studio &&other)
    {
        open =other.open;
        //now we steal the info from the rvalue
        for (int i=0; i<static_cast<int>(other.trainers.size()); i++) {
            trainers.push_back(other.trainers[i]);
            other.trainers[i]= nullptr;
        }
        for (int i=0; i<static_cast<int>(other.workout_options.size()); i++)
        {
            WorkoutType wt;
            if (other.workout_options[i].getType() == MIXED)
                wt = MIXED;
            if (other.workout_options[i].getType() == CARDIO)
                wt = CARDIO;
            if (other.workout_options[i].getType() == ANAEROBIC)
                wt = ANAEROBIC;
            Workout w(other.workout_options[i].getId(),
                      other.workout_options[i].getName(),
                      other.workout_options[i].getPrice(),
                      wt);
            workout_options.push_back(w);
        }
        for (int i=0; i<static_cast<int>(other.actionsLog.size()); i++)
        {
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i]=nullptr;
        }
        other.trainers.clear();
        other.actionsLog.clear();
        other.workout_options.clear();
    }

    //move assignment operator
    Studio& Studio::operator=(Studio &&other)
    {
        for(int i=0; i<static_cast<int>(trainers.size()); i++)
            delete trainers[i];
        trainers.clear();
        for(int i=0; i<static_cast<int>(actionsLog.size()); i++)
            delete actionsLog[i];
        actionsLog.clear();
        workout_options.clear();

        //now we wish to steal the resources
        open=other.open;
        for (int i=0; i<static_cast<int>(other.trainers.size()); i++) {
            trainers.push_back(other.trainers[i]);
            other.trainers[i]= nullptr;
        }
        for (int i=0; i<static_cast<int>(other.workout_options.size()); i++)
        {
            WorkoutType wt;
            if (other.workout_options[i].getType() == MIXED)
                wt = MIXED;
            if (other.workout_options[i].getType() == CARDIO)
                wt = CARDIO;
            if (other.workout_options[i].getType() == ANAEROBIC)
                wt = ANAEROBIC;
            Workout w(other.workout_options[i].getId(),
                      other.workout_options[i].getName(),
                      other.workout_options[i].getPrice(),
                      wt);
            workout_options.push_back(w);
        }
        for (int i=0; i<static_cast<int>(other.actionsLog.size()); i++)
        {
            actionsLog.push_back(other.actionsLog[i]);
            other.actionsLog[i]=nullptr;
        }
        other.trainers.clear();
        other.actionsLog.clear();
        other.workout_options.clear();
        return *this;
    }

    Studio::Studio(const std::string &configFilePath): open(false), trainers(), workout_options(), actionsLog() {

        int counter = 0;
        std::ifstream file(configFilePath);
        char line[256];
        //we use contin boolean to safely use the counter and execute the specific phase of the inputFile
        bool contin = true;
        while (file) {
            file.getline(line, 256);
            if ((line[0] == '#') | (line[0] == '\0')) {
                contin = false;
            }
            else
                contin = true;
            if (contin) {
                //not we have a line with actual information
                if (counter == 0) {
                    //number of trainers - dont need it
                    counter++;
                } else if (counter == 1) {
                    //capacities !
                    string st = "";
                    int cap = 0;
                    for (int i = 0; (i < 256) & (line[i] != '\0'); i++) {
                        if (line[i] != ',') {
                            st = st+line[i];
                        } else {
                            stringstream ss;
                            ss << st;
                            ss >> cap;
                            Trainer *t1 = new Trainer(cap);
                            trainers.push_back(t1);
                            st = "";
                        }
                    }
                    stringstream ss;
                    ss << st;
                    ss >> cap;
                    Trainer *t1 = new Trainer(cap);
                    trainers.push_back(t1);
                    counter++;
                }
                else {
                    //workouts are being inserted by lines
                    int counter2 = 0;
                    string name, type, cost;
                    stringstream ss;
                    string st = "";
                    for (int i = 0; (i < 256) & (line[i] != '\0'); i++) {
                        if (line[i] != ',') {
                            st = st+line[i];
                        } else {
                            if (counter2 == 0) {
                                name = st;
                                st = "";
                                counter2++;
                            }
                            else if (counter2 == 1) {
                                     type = st;
                                     st = "";
                                     counter2++;
                            }

                        }
                    }

                    WorkoutType wt(ANAEROBIC);
                    if (type.at(1) == 'M')
                        wt = WorkoutType::MIXED;
                    if (type.at(1) == 'C')
                        wt = WorkoutType::CARDIO;
                    int price;
                    stringstream s1;
                    s1 << st.substr(1);
                    s1 >> price;
                    Workout w(counter - 2, name, price, wt);
                    workout_options.push_back(w);
                    counter++;
                }
            }
        }
    }


    void Studio::start()
    {
        cout << "The studio is now open ! \n" ;
        int ids=0;
        // now we need to get instructions from the user
        string str;
        getline(cin,str);
        stringstream ss;
        while (str.compare("closeall")!=0)
        {
            if (str.at(0)=='o')
            {
                //order
                if (str.at(1)=='r')
                {
                    string tid = str.substr(6);
                    stringstream  ss ;
                    ss<<tid;
                    int id;
                    ss>>id;
                    Order* o1 = new Order(id);
                    o1->act(*this);
                    actionsLog.push_back(o1);
                }
                //open
                else
                {
                    int num =str.find(' ',5);
                    int tid=0;
                    string id = str.substr(5,num-5);
                    stringstream ss;
                    ss<<id;
                    ss>>tid;
                    str=str+" ";
                    str=str.substr(num+1);
                    string c=str.substr(0, str.find(' ',0));
                    vector<Customer*> v ;
                    while (str.compare("")!=0)
                    {
                        string name = c.substr(0,c.find(','));
                        string type = c.substr(c.find(',')+1);
                        //SWEATY
                        if (type.at(0)=='s')
                        {
                            Customer* c1 = new SweatyCustomer(name,ids);
                            v.push_back(c1);
                        }
                        //CHeaP
                        if (type.at(0)=='c')
                        {
                            Customer* c1 = new CheapCustomer(name,ids);
                            v.push_back(c1);
                        }
                        //MUSCLE
                        if (type.at(0)=='m')
                        {
                            Customer* c1 = new HeavyMuscleCustomer(name,ids);
                            v.push_back(c1);
                        }
                        //FULL BODY
                        if (type.at(0)=='f')
                        {
                            Customer* c1 = new FullBodyCustomer(name,ids);
                            v.push_back(c1);
                        }

                        ids++;
                        str = str.substr(str.find(' ') );
                        if (str.length()<3)
                            str="";
                        else {
                            str=str.substr(1);
                            c = str.substr(0, str.find(' '));
                        }
                    }

                    OpenTrainer* ot = new OpenTrainer(tid, v  );
                    ot->act(*this);
                    actionsLog.push_back(ot);
                    str="~";
                }
            }


            //close and not close all !!!
            if (str.at(0)=='c')
            {
                string tid = str.substr(6);
                stringstream  ss ;
                ss<<tid;
                int id;
                ss>>id;
                Close *c1 = new Close(id);
                c1->act(*this);
                actionsLog.push_back(c1);
            }


            // move costumer
            if (str.at(0)=='m')
            {
                str=str+" ";
                str=str.substr(str.find(' ')+1);
                string srcid = str.substr(0,str.find(' '));
                str=str.substr(str.find(' ')+1);
                string desid= str.substr(0,str.find(' '));
                str=str.substr(str.find(' ')+1);
                string cusid=str.substr(0,str.find(' '));
                stringstream  s1,s2,s3 ;
                s1<<srcid;
                int sid;
                s1>>sid;
                s2<<desid;
                int did;
                s2>>did;
                s3<<cusid;
                int cid;
                s3>>cid;
                MoveCustomer* m1 = new MoveCustomer(sid,did,cid);
                m1->act(*this);
                actionsLog.push_back(m1);
            }

            //workout options
            if (str.at(0)=='w')
            {
                PrintWorkoutOptions *p1= new PrintWorkoutOptions();
                p1->act(*this);
                actionsLog.push_back(p1);

            }

            //status
            if (str.at(0)=='s')
            {
                string tid = str.substr(7);
                stringstream  ss ;
                ss<<tid;
                int id;
                ss>>id;
                PrintTrainerStatus *p1 = new PrintTrainerStatus(id);
                p1->act(*this);
                actionsLog.push_back(p1);
            }

            //log
            if (str.at(0)=='l')
            {
                PrintActionsLog *p1=new PrintActionsLog();
                p1->act(*this);
                actionsLog.push_back(p1);
            }


            // backup
            if (str.at(0)=='b')
            {
                BackupStudio *b = new BackupStudio();
                b->act(*this);
                actionsLog.push_back(b);
            }


            //restore
            if (str.at(0)=='r')
            {
                RestoreStudio *r=new RestoreStudio();
                r->act(*this);
                actionsLog.push_back(r);
            }
            getline(cin,str);
        }
        //now is close all
        CloseAll* co=new CloseAll();
        co->act(*this);
        actionsLog.push_back(co);


    }
    int Studio::getNumOfTrainers() const
    {
        return trainers.size();
    }
    Trainer* Studio::getTrainer(int tid)
    {
        if (tid<static_cast<int>(trainers.size()))
            return trainers[tid];
        return nullptr;
    }
    const std::vector<BaseAction*>& Studio::getActionsLog() const
    {
        return actionsLog;
    }                                                           // Return a reference to the history of actions
    std::vector<Workout>& Studio::getWorkoutOptions()
    {
        return workout_options;
    }








