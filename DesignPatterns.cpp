#include<iostream>
#include<cstdlib>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;


//Design Patterns are reusable, general and typical solutions to recurring problems in object-oriented system design (design problems)

//More specifically, design patterns are pre-made abstract blueprints of general solution to a particular design problem (no implementation details, just conceptual description for how to solve a design problem)
//We present deisgn patterns in terms of their intent (describes the design problem and solution briefly), motivation, class structure (class diagram: parts of the pattern and how they relate), code example

///Creational Design Patterns: how objects are created (object creation mechanisms) to ensure flexibility and reusability of existing code

// Singleton: ensure that only one instance of the class is created (1-1 correspondence between the class and the instance -> the use of static attributes and method)

class Singleton{
    private:
        static Singleton* instance; 
        Singleton(){}; //private constructor to prevent creating an object in main()
    public:
        static Singleton& getInstance(){
            if (instance == nullptr){
                instance = new Singleton;
                return *instance;     
            }
            return *instance; 
        }

        static Singleton* getPtrInstance(){
            if (instance == nullptr){
                instance = new Singleton;
                return instance;     
            }
            return instance; 
        }

        void show(){
            cout << "Singleton instance create!"; 
        }

};


Singleton* Singleton::instance = nullptr;


int main(){
    Singleton& a = Singleton::getInstance();
    Singleton& b = Singleton::getInstance();
    // a == b is true

    Singleton* ap = Singleton::getPtrInstance();
    Singleton* bp = Singleton::getPtrInstance();

};

//Factory Method: Object creation depend on conditions like a factory of products of different types


class Animal {
public:
    virtual void speak() = 0;
};

class Dog : public Animal {
public:
    void speak() override {
        cout << "Woof\n";
    }
};

class Cat : public Animal {
public:
    void speak() override {
        cout << "Meow\n";
    }
};

class AnimalFactory{
    public:
        static Animal* createAnimal(string type){
            if (type == "Dog") return new Dog; 
            if (type == "Cat") return new Cat;
            return nullptr; 
        }
}; 

int main(){
    Animal* ad = AnimalFactory::createAnimal("Dog");   //dynamic type is Dog 
    Animal* ac = AnimalFactory::createAnimal("Cat");   //dynamic type is Cat
    //both have the same static type

    ad->speak(); //woof
    ac->speak(); //meow
}

class Transport{ //interface
    public:
        virtual void drive() = 0;
        virtual ~Transport() = default;
};

class Truck: public Transport{
    int capacity; 
    public:
        Truck(int c): capacity(c){}

        void drive() override{
            cout << "Truck transport!";
        }
        
};


class Ship: public Transport{
    int capacity; 
    public:
        Ship(int c): capacity(c){}

        void drive() override{
            cout << "Ship transport!";
        }
        
};

class TransportFactoryInterface{
    public:
        virtual Transport* createTransport(int capacity) = 0; 
        virtual ~TransportFactoryInterface() = default;
};

class TruckFactory: public TransportFactoryInterface{
    public:
        Transport* createTransport(int capacity) override{
            return new Truck(capacity);
        }
}; 


class ShipFactory: public TransportFactoryInterface{
    public:
        Transport* createTransport(int capacity) override{
            return new Ship(capacity);
        }

}; 

class TransportFactory{
    public:
        Transport* createTransport(string type, int capacity) noexcept {
            if (type == "Truck"){
                return new Truck(capacity);
            }

            if (type == "Ship"){
                return new Ship(capacity);
            } else{
                return nullptr;
            }
        }

        static Transport* createStaticTransport(string type, int capacity) noexcept {
            if (type == "Truck"){
                return new Truck(capacity);
            }

            if (type == "Ship"){
                return new Ship(capacity);
            } else{
                return nullptr;
            }
        }

};

int main(){

    //Simple Factory
    TransportFactory TF;
    Transport* t = TF.createTransport("Truck", 20); //static type: Transport*
    t->drive(); //prints Truck transport (dynamic type: Truck*)
    Transport* s = TF.createTransport("Ship", 20); //static type: Transport*
    s->drive(); //prints Ship transport (dynamic type: Ship*)
    //Simple Factory but using a static method
    Transport* st = TransportFactory::createStaticTransport("Truck", 20); 
    st->drive(); 

    Transport* ss = TransportFactory::createStaticTransport("Ship", 20); 
    ss->drive(); 
    
    delete t, s;

    //Factory Method
    TruckFactory tr;
    Transport* tt = tr.createTransport(20); 

    ShipFactory sh; 
    Transport* ss = sh.createTransport(20);
    
    return 0;
}



//Abstract Factory: nested factory methods

class Human {
    string name;
    public:
        Human(string n): name(n){}
        virtual void reproduce() = 0; 
}; 

class Female: public Human{
    string name;
    public:
        Female(string n): Human(n){}
        // void reproduce() override {cout <<"Female reproduction";}
};

class Male: public Human{
    string name;
    public:
        Male(string n): Human(n){}
        // void reproduce() override{cout <<"Male reproduction";}
};

class HumanFactory{
    public:
        static Human* createHuman(string name, string type){
            if (type == "Male") return new Male(name);
            else return new Female(name);
        } 
};

class AbstractFactory{

};

class Entity{
    int id;
    public:
        Entity(int idd): id(idd){}

        virtual void update() = 0; 
};


//Builder: Object has many optional parts constructed step-by-step (constructor overloading) -> separating construction of a complex object from its representation (one construction can work with different representations)

class Builder{
    int x,y,z;
    public: 
        Builder(){
            this->x = 2; 
        }
        // Builder(int xx){ //100
        //     this->x = xx;
        // }
        // Builder(int yy){ // 010
        //     this->y = yy;
        // }
        // Builder(int zz){ //001
        //     this->z = zz;
        // }

        Builder(int p, bool isY, bool isZ){
            if (!isY && !isZ) this->x = p; //00 
            else if (!isZ) this->y = p; //10 
            else this->z = p;    //01
        }
        
        // Builder(int xx, int yy){
        //     this->x = xx;
        //     this->y = yy;
        // }
        // Builder(int xx, int zz){
        //     this->x = xx;
        //     this->z = zz;
        // }
        // Builder(int yy, int zz){
        //     this->y = yy;
        //     this->z = zz;
        // }

        Builder(int v, int u, bool isY, bool isZ){
            if (isZ && isY){ //11
                this->y = v;
                this->z = u;
            } else if (isY){ //10
                this->x = v;
                this->y = u;
            } else { //00
                this->x = v;
                this->z = u;
            }
        }
        Builder(int xx, int yy, int zz){
            this->x = xx;
            this->y = yy;
            this->z = zz;
        }
};


int main(){
    Builder b(2,0,0);
};


//Prototype: Create an object by cloning an existing one (expensive object creation) -> copy constructor

class Book{
    int code;
    string title; 
    Book(){
        this->code = 1;
        this->title = "Book";}
    public:
        static Book* b; 
        
        static Book& getOrigin(){
            if (b == nullptr){
                b = new Book;
                return *b; 
            }
            return *b;
        }

        Book(const Book& b){
            this->code = b.code;
            this->title = b.title;
        }

};

Book* Book::b = nullptr;

int main(){
    //expensive object created once

    Book x = Book::getOrigin(); //implicit copy (no need for copy constructor declaration)

    Book xx(Book::getOrigin()); //explicit copy (no shallow-deep distinction)

    return 0; 
}

///Structural Design Patterns: how objects and classes are composed and assembled into flexible and efficient large structures

//Adapter: Converts one interface into another expected by the client

class Think{ //an interface representing the capability of thinking
    public:
        virtual void think() = 0; 
        virtual ~Think() = default; 
};

class Philosophize{ //an interface representing the capability of philosophizing
    public:
        virtual void philosophize() = 0; 
        virtual ~Philosophize() = default; 
};

class Human: public Think, public Philosophize{
    public:
        string name; 
        Human(){name = "Ayoub";}
        Human(string s): name(s){};
        void think() override {
            cout << "Human thinks!";
        }

        void philosophize() override{
            cout << "Human philosophizes!"; 
        }
};

class Adapter{
    Human h; //using object composition (value attribute)
    public:
        void actHuman(string type){
            if (type == "think"){
                h.think(); 
            }
            if (type == "philosophize"){
                h.philosophize(); 
            }
        }
};


int main(){
    string t;
    cin >> "give a capability" >> t; 
    
    Adapter ap;

    ap.actHuman(t); 
    
}; 


//Bridge: decouples abstraction (high-level concept) from implementation (low-level details) to ensure flexibility by using composition instead of inheritance to prevent class explosion due to abstraction-implementation intricacy

/// Terminological remark: 
//// Abstraction (or Interface or Frontend) is what the user interacts with (GUI: modeled as a normal OOP/UML class), ie, the high-level control layer -> it's not the same thing as abstraction or abstract class or interface that we know in UML or OOP
//// Implementation (or Platform or Backend) is how the work is done in the background, ie, low-level control layer (modeled as an OOP/UML interface) that we delegate implementation to -> don't confuse it with the OOP/UML interface implmentation or realization through concrete child classes.
/// the idea is to separate the what (abstraction/interface: what the user sees or interact with) from the how (implementation/platform: how it works) so that we can add a lot of whats without having to worry about the how for each what as we simply delegate implementation via composition (also true for adding a new how)
/// What-How Orthogonality: all combinations between what the user interacts with and how it works underneath are possible by design -> no need to add as many subclasses of a given refinement/extension as exisitng implememtations (because implementatons are universal and independent of what the user interacts with, ie, interface)
//// Single Responsibility Principle: focus on high-level logic in the abstraction and platform details in implementation one at a time.

class UserInterface{ //Abstraction/Interface: what the user sees or interacts with
    protected:
        int quality; 
        string version;
        int date;
        string design;
        Kernel* ker;
    public:
        UserInterface(int q, string v, int d, string des): quality(q), version(v), date(d), design(des){
            if (this->design == "Mono") ker = new MonoKernel(20); 
            if (design == "Micro") ker = new MicroKernel(20); 
        }

        UserInterface& showInterface(){
            return *this;
        }

        void displayResult(){
            cout << "Result!"; 
        }
}; 


////Abstraction/Interface Refinement: extending the what

class GraphicalUserInterface: public UserInterface{
    string cursor;
    public:
        GraphicalUserInterface(int q, string v, int d, string des, string c): UserInterface::UserInterface(q,v,d,des), cursor(c){}

        string showCursor(){
            ker->sendRequest("show Cursor!"); 
            return this->cursor;
        }

        void clickCursor(){
            ker->sendRequest("click Cursor!"); 
            cout << "Cursor clicked!";
        }
};


class Terminal: public UserInterface{
    vector<string> commands; 
    public:
        Terminal(int q, string v, int d, string des, vector<string> c): UserInterface(q,v,d,des), commands(c){}
        
        vector<string> showAllCommands(){
            ker->sendRequest("show commands!"); 
            return commands;
        }

        string showCommand(int k){
            ker->sendRequest("show command!"); 
            return commands[k]; 
        }
        
        void executeCommands(int k){
            ker->sendRequest("execute command!"); 
            cout << "Command" << commands[k] << "executed!"; 
        }
};


class Kernel{ //Implementation/Platform: how a user interface work 
    public:
        virtual ~Kernel() = default;
        virtual void sendRequest(string hardware) = 0; 
}; 


class MicroKernel: public Kernel{
    int memory;
    public:
        MicroKernel(int m): memory(m){}

        void sendRequest(string hardware) override{
            cout << "Multiple requests sent to" << hardware; 
        }
}; 


class MonoKernel: public Kernel{
    int memory;
    public:
        MonoKernel(int m): memory(m){}

        void sendRequest(string hardware) override{
            cout << "One request sent to" << hardware; 
        }

};


//Decorator: add behavior to an object dynamically without modifying the class (dependency)

class Student {
    string name;
    double grade;
    public:
        Student(string s, double g): name(s){
            this->grade = g;
        }

        void passExam(){
            cout << "passed exam!"; 
        }

        friend class Decorator; 
};  

class Decorator{
    public:
        //new methods added dynamically to the object without modifying the class
        void prepareExam(Student& s){
            cout << "prepare exam!"; 
        }

        double getGrade(Student& s){
            return s.grade; 
        }

        void setGrade(Student& s, double newGrade){
            s.grade = newGrade;
        }
};  

int main(){
    Student stu("Ayoub", 12.0); 

    Decorator dec;

    dec.prepareExam(stu); 
    double gr = dec.getGrade(stu);
    dec.setGrade(stu, 18.12);

    return 0;
}


//Composite (Object Tree): treats individual objects and groups of objects uniformly by creating a common interface for both (flatten categorical hierarchy on a tree)

class ObjectInterface{
    public:
        virtual ~ObjectInterface() = default;
        virtual void getPrice() = 0;
        virtual void getInfo() = 0;
}; 


class Object: public ObjectInterface{
    string name;
    double price; 
    public:
        Object(string n, double p): name(n){
            this->price = p; 
        }

        void getInfo() override{
            cout << price << name;
        }

        void getPrice() override {
            cout << price;
        }
}; 

class ObjectGroup: public ObjectInterface{
    vector<Object*> objects; //aggregation or composition
    vector<ObjectGroup*> groupObjects; //aggregation or composition
    public:
        ObjectGroup(vector<Object*> objList, vector<ObjectGroup*> goList): objects(objList), groupObjects(goList){}
        
        void getInfo() override{//base case of recursion is determined in runtime by the list attributes
            for (Object* obj: objects){
                obj->getInfo();
            }
            for (ObjectGroup* objG: groupObjects){
                objG->getInfo();
            }
        }
        void getPrice() override { //base case of recursion is determined in runtime by the list attributes
            for (Object* obj: objects){
                obj->getPrice();
            }
            for (ObjectGroup* objG: groupObjects){
                objG->getPrice();
            }
        }
};

class Tree{
    int level;
    int depth; 
    ObjectInterface* OI; 
    public:
        Tree(int l, int d): level(l){
            this->depth = d;
        }; 

        void readElement(){
            OI->getInfo(); 
        }

        void getPrices(){
            OI->getPrice(); 
        }
};



int main(){


    return 0; 
}

//Facade: the use of a simplified interface to a 3rd-party or secondary library, framework or set of classes without having to worry about their objects, execution order, dependency, etc => facade is a class-level encapsulation (similar to an API endpoint)

class Client{
    string name;
    Facade* f;
    public:
        Client(string n): name(n){}

        void makeRequest(string request){
            cout << "Client demands " << request; 
            f->handleRequest(request);
        }
}; 

class Facade{
    public:
        virtual void handleRequest(string request) = 0;
        virtual ~Facade() = default;
}; 

class Warehouse: public Facade{
    int code;
    string name;
    int capacity;
    public:
        Warehouse(int c, string n, int cap): code(c), name(n), capacity(cap){}

        void handleRequest(string request) override{

        }
}; 

class Delivery: public Facade{
    int code;
    string name;
    int speed;
    public:
        Delivery(int c, string n, int s): code(c), name(n), speed(s){}

        void handleRequest(string request) override{
            
        }
    
};


//Proxy


//Fly-Weight



//Behavioral Design Patterns: how objects communicate (information exchange mechanisms) and assign responsbilities to each others (delegation mechanisms)




class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& message) = 0;
};

class Subject {
public:
    virtual ~Subject() = default;
    virtual void attach(Observer* observer) = 0;
    virtual void detach(Observer* observer) = 0;
    virtual void notify() = 0;
};

class Model : public Subject {
    private:
        std::vector<Observer*> observers;
        std::string state;

    public:
        Model(string st){
            this->state = st;
        }
        void attach(Observer* observer) override {
            observers.push_back(observer);
        }

        void detach(Observer* observer) override {
            observers.erase(
                std::remove(observers.begin(), observers.end(), observer),
                observers.end()
            );
        }

        void setState(const std::string& newState) {
            state = newState;
            notify();
        }

        std::string getState() const {
            return state;
        }

        void notify() override {
            for (Observer* observer : observers) {
                observer->update(state);
            }
        }
};

class View : public Observer {
    private:
        std::string name;

    public:
        explicit View(const std::string& observerName)
            : name(observerName) {}

        void update(const std::string& message) override {
            std::cout << "Observer " << name 
                    << " received update: " << message << std::endl;
        }
};

class Controller{
    std::string name;
    public:
        explicit Controller(string n): name(n){}

        void effectChange(Model& m, const string& newState){
            m.setState(newState);
        }
};
int main(){
    Model m("initialState"); //User Profile

    View v1("GUI1"); //User Profile View 
    View v2("GUI2"); //User Profile View
    View v3("GUI3"); //User Profile View

    View* vp1 = &v1;
    View* vp2 = &v2;
    View* vp3 = &v3; 

    m.attach(vp1);
    m.attach(vp2);
    m.attach(vp3); 

    Controller c("x"); //User Profile Controller
    c.effectChange(m, "profileChange"); //Model/Subject set change -> Views notified and updated automatically

    m.detach(vp3); //unregister an observer 
    return 0;
}

