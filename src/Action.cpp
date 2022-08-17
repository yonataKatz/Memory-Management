#include "Action.h"
#include "Studio.h"
#include "Trainer.h"


BaseAction::~BaseAction()
{}

BaseAction::BaseAction(): errorMsg("General ERROR"), status(ERROR)
{
}

ActionStatus BaseAction::getStatus() const
{
    return status;
}

void BaseAction::complete()
{
    status=COMPLETED;
}
void BaseAction::error(std::string errorMsg)
{
    status=ERROR;
    this->errorMsg=errorMsg;
}
std::string BaseAction::getErrorMsg() const
{
    return errorMsg;
}



//destructor for OpenTrainer
OpenTrainer::~OpenTrainer()
{
    if (customers.size()!=0) {
        for (int i = 0; i < static_cast<int>(customers.size()); i++) {
            if (customers[i]) {
                delete customers[i];
            }
        }
    }
    customers.clear();
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):BaseAction(), trainerId(id),customers(customersList), arguments(""){
    error("Trainer does not exist is not open");
}


void OpenTrainer::act(Studio &studio)
{
    //we want to save the arguments before editing them - later we might not have al the arguments because of deletion
    //therefore we have created the arguments field
    Trainer* t1 =studio.getTrainer(trainerId);
    arguments= "open "+ to_string(trainerId)+" ";
    for (int i=0; i<static_cast<int>(customers.size()); i++)
        arguments=arguments+customers[i]->getName()+" ";

    if(t1==nullptr || t1->isOpen())
    {
        error(getErrorMsg());
        cout<<getErrorMsg()<<endl;
        //vector we wish to destroy
        if (t1!=nullptr)
        {
            for (int i=0; i<static_cast<int>(customers.size()); i++)
                delete customers[i];
            customers.clear();
        }
    }
    else {
        //the case we can try and place an order
        t1->openTrainer();
        int cap = t1->getCapacity();
        for (int i = 0; (i < static_cast<int>(customers.size())) & (i<cap); i++)
            t1->addCustomer(customers[i]->clone());
        complete();
        //if we have stopped because we have reached the limits
    //    if (cap == static_cast<int>(t1->getCustomers().size()))
      //  {
        //    for (; cap <static_cast<int>(customers.size()) ;cap ++)
          //      delete customers[cap];
            //customers.clear();
       //     for (int i=0; i<static_cast<int>(t1->getCustomers().size()); i++)
        //        customers.push_back(t1->getCustomers()[i]);
      //  }
    }
    if (getStatus()==COMPLETED)
        arguments = arguments+"COMPLETED";
    else
        arguments = arguments+"ERROR:"+getErrorMsg();
}


std::string OpenTrainer::toString() const
{
    return arguments;
}
void OpenTrainer::setArguments() {
  //  Trainer* t1 =studio.getTrainer(trainerId);
    arguments= "open "+ to_string(trainerId)+" ";
    for (int i=0; i<static_cast<int>(customers.size()); i++)
        arguments=arguments+customers[i]->getName()+" ";
    if (getStatus()==COMPLETED)
        arguments = arguments+"COMPLETED";
    else
        arguments = arguments+"ERROR:"+getErrorMsg();
}

OpenTrainer* OpenTrainer::clone()
{
    int id = trainerId;

    vector <Customer*> v;
    for (int i=0; i<static_cast<int>(customers.size()); i++)
    {
        Customer* c1=customers[i]->clone();
        v.push_back(c1);
    }
    OpenTrainer* op = new OpenTrainer(id,v);
    if (getStatus()==COMPLETED) {
        op->complete();
        op->setArguments();
    }
    else {
        op->error(getErrorMsg());
        op->setArguments();
    }
    return op;
}



Order::Order(int id): BaseAction(), trainerId(id)
{
    error("Trainer does not exist or is not open");
}

void Order::act(Studio &studio)
{
    Trainer* t1 =studio.getTrainer(trainerId);
    if (t1!= nullptr && t1->isOpen())
        //the case we can try and place an order
    {
        vector<Customer*> cus= t1->getCustomers();
        for (int i=0; i<static_cast<int>(cus.size()); i++)
        {
            if (!cus[i]->hasOrdered()) {
                cus[i]->makeOrder();
                vector<int> v = cus[i]->order(studio.getWorkoutOptions());
                if (v.size() > 0)
                    t1->order(cus[i]->getId(), v, studio.getWorkoutOptions());
                else
                    t1->removeCustomer(cus[i]->getId());
            }
        }
        complete();
        string s ;
        vector<OrderPair> ord = t1->getOrders();
        for (int i=0; i<static_cast<int>(ord.size()); i++)
        {
            //need to get the name
            if (t1->getCustomer(ord[i].first)) {
                string name = t1->getCustomer(ord[i].first)->getName();
                s = s +name+ " is doing " + ord[i].second.getName()+'\n';
            }
        }
        cout<<s << endl;
    }
    else
    {
        error(getErrorMsg());
        cout << "ERROR:" +getErrorMsg();
    }
}
std::string Order::toString() const
{
    string s = "order"+ to_string(trainerId);
    if (getStatus()==COMPLETED)
        s=s+" COMPLETED";
    else
        s=s+" ERROR: " +getErrorMsg();
    return s;


}

Order* Order::clone()
{
    Order* o=new Order(trainerId);
    if (getStatus()==COMPLETED)
        o->complete();
    else
        o->error(getErrorMsg());
    return o;
}






MoveCustomer::MoveCustomer(int src, int dst, int customerId): BaseAction(), srcTrainer(src), dstTrainer(dst), id(customerId){
    error("Cannot move customer");
}

void MoveCustomer::act(Studio &studio)
{
    Trainer* tsrc = studio.getTrainer(srcTrainer);
    Trainer* tdst = studio.getTrainer(dstTrainer);
    if ((tsrc!=nullptr) &&
    (tdst!=nullptr )&&
    (tdst->getCapacity()>static_cast<int>(tdst->getCustomers().size()))&
    (tsrc->isOpen())&
    (tdst->isOpen()))
    {
        Customer* c = tsrc->getCustomer(id);
        if (!c) {
            error(getErrorMsg());
            cout << "ERROR:" + getErrorMsg();
        }
        else
        {
            tdst->addCustomer(c);
            vector<OrderPair> dstl = tdst->getOrders();
            vector<OrderPair> l = tsrc->getOrders();
            for (int i=0; i<static_cast<int>(l.size()); i++)
                if (l[i].first==id)
                    tdst->addOrder(OrderPair(l[i].first, l[i].second));
            //removes customer and orders using Trainer::removeCustomer
            tsrc->removeCustomer(c->getId());
            if (static_cast<int>(tsrc->getCustomers().size())==0)
                tsrc->closeTrainer();
            complete();
        }
    }
    else
        cout << "ERROR:"+ getErrorMsg();
}
std::string MoveCustomer::toString() const
{
    string s = "move " + to_string(srcTrainer)+" "+ to_string(dstTrainer)+" "+ to_string(id)+" ";
    if (getStatus()==COMPLETED)
        s=s+"COMPLETED";
    else
        s=s+"ERROR: "+getErrorMsg();
    return s;
}

MoveCustomer* MoveCustomer::clone()
{
    MoveCustomer* m = new MoveCustomer(srcTrainer,dstTrainer,id);
    if(getStatus()==COMPLETED)
        m->complete();
    else
        m->error(getErrorMsg());
    return m;
}






Close::Close(int id): BaseAction(), trainerId(id)
{
    error ("Trainer does not exist or is not open");
}


void Close::act(Studio &studio)
{
    Trainer* t1 = studio.getTrainer(trainerId);
    if (t1!=nullptr && t1->isOpen() )
    {
        int sal = t1->getSalary();
        t1->addToSal(t1->currentSessionSal());
        t1->closeTrainer();
        cout<<"Trainer "+ to_string(trainerId)+" closed. Salary " + to_string(sal)+"NIS"<<endl;
        complete();
    }
    else
        cout<< "ERROR:"+getErrorMsg();


}
std::string Close::toString() const
{
    string s = "close "+ to_string(trainerId);
    if (getStatus()==COMPLETED)
        return s+" COMPLETED";
    return s+" ERROR: "+getErrorMsg();

}

Close* Close::clone()
{
    Close* c= new Close(trainerId);
    if(getStatus()==COMPLETED)
        c->complete();
    else
        c->error(getErrorMsg());
    return c;
}







CloseAll::CloseAll():BaseAction()
{}

void CloseAll::act(Studio &studio)
{
    if (backup) {
        delete backup;
        backup=nullptr;
    }
    int numT = studio.getNumOfTrainers();
    for (int i=0; i<static_cast<int>(numT); i++)
    {
        Trainer* t1 = studio.getTrainer(i);
        if (t1->isOpen())
        {
            Close c1 (i);
            c1.act(studio);
        }
        complete();
    }
}
std::string CloseAll::toString() const
{
    return "close all COMPLETED";
}

CloseAll* CloseAll::clone()
{
    CloseAll* ca=new CloseAll();
    if (getStatus()==COMPLETED)
        ca->complete();
    else
        ca->error(getErrorMsg());
    return ca;
}






PrintWorkoutOptions::PrintWorkoutOptions():BaseAction()
{}

void PrintWorkoutOptions::act(Studio &studio)
{
    for (int i=0; i<static_cast<int>(studio.getWorkoutOptions().size()); i++) {
        string name = studio.getWorkoutOptions().at(i).getName();
        string price = to_string(studio.getWorkoutOptions().at(i).getPrice());
        int n= studio.getWorkoutOptions().at(i).getType();
        string type="";
        if (n==0)
            type="ANEROBIC";
        if (n==1)
            type = "MIXED";
        if (n==2)
            type="CARDIO";

        cout<< name+", "+ type +", "+price << endl;
        complete();
    }


}
std::string PrintWorkoutOptions::toString() const
{
    return "workout_options COMPLETED";
}
PrintWorkoutOptions* PrintWorkoutOptions::clone()
{
    PrintWorkoutOptions* p=new PrintWorkoutOptions();
    if (getStatus()==COMPLETED)
        p->complete();
    else
        p->error(getErrorMsg());
    return p;
}







PrintTrainerStatus::PrintTrainerStatus(int id):trainerId(id)
{}

void PrintTrainerStatus::act(Studio &studio)
{
    Trainer* t1 = studio.getTrainer(trainerId);
    if (!t1->isOpen())
        cout << "Trainer "+ to_string(trainerId)+" status : Closed"<<endl;
    else
    {
        cout << "Trainer "+ to_string(trainerId)+" status : Open"<<endl;
        cout << "Customers:"<<endl;
        for (int i=0; i<static_cast<int>(t1->getCustomers().size()); i++)
        {
            string c = to_string(t1->getCustomers()[i]->getId())+" "+t1->getCustomers()[i]->getName();
            cout << c << endl;
        }
        cout << "Orders:"<<endl;
        for (int i=0; i<static_cast<int>(t1->getOrders().size()); i++)
        {
                string s=t1->getOrders()[i].second.getName() +" "+
                         to_string(t1->getOrders()[i].second.getPrice())+" NIS "+
                         to_string(t1->getOrders()[i].first);

                cout<<s<<endl;
        }
        cout<< "Current trainer salary is:" + to_string(t1->getSalary())<<endl;
    }
    complete();
}
std::string PrintTrainerStatus::toString() const
{
    string s = "status "+ to_string(trainerId);
    if (getStatus()==COMPLETED)
        return s+" COMPLETED";
    return s+" ERROR: "+getErrorMsg();
}
PrintTrainerStatus* PrintTrainerStatus::clone()
{
    PrintTrainerStatus* p=new PrintTrainerStatus(trainerId);
    if (getStatus()==COMPLETED)
        p->complete();
    else
        p->error(getErrorMsg());
    return p;
}





PrintActionsLog::PrintActionsLog():BaseAction()
{}

void PrintActionsLog::act(Studio &studio)
{
    vector<BaseAction*> l = studio.getActionsLog();
    for (int i=0; i<static_cast<int>(l.size()); i++) {
        cout << l[i]->toString() << endl;
    }
}

// is string representation is ok ??????????/--------------- not clear
std::string PrintActionsLog::toString() const
{
    return "log COMPLETED";
}

PrintActionsLog* PrintActionsLog::clone()
{
    PrintActionsLog* p = new PrintActionsLog();
    if (getStatus()==COMPLETED)
        p->complete();
    else
        p->error(getErrorMsg());
    return p;
}

BackupStudio::BackupStudio():BaseAction()
{}



void BackupStudio::act(Studio &studio)
{
    if(backup != nullptr)
        delete backup;
    // if backup -> delete backup and backup =nullptr  ------- for backup backup
    backup = new Studio(studio);
    complete();
}
std::string BackupStudio::toString() const
{
    return "BackUp was done";
}
BackupStudio* BackupStudio::clone()
{
    BackupStudio* b=new BackupStudio();
    if (getStatus()==COMPLETED)
        b->complete();
    else
        b->error(getErrorMsg());
    return b;
}



RestoreStudio::RestoreStudio():BaseAction()
{
    error("No backup available");
}

void RestoreStudio::act(Studio &studio)
{
    if (!backup) {
        error(getErrorMsg());
        cout << getErrorMsg() << endl;
    }
    else
    {
        studio= *backup;
        complete();
    }
}
std::string RestoreStudio::toString() const
{
    return "RESTORE was done";
}
RestoreStudio* RestoreStudio::clone()
{
    RestoreStudio* b=new RestoreStudio();
    if (getStatus()==COMPLETED)
        b->complete();
    else
        b->error(getErrorMsg());
    return b;
}
