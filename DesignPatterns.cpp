#include<iostream>
#include<cstdlib>
#include <vector>
#include <algorithm>
#include <string>
#include <exception>
using namespace std;


//Design Patterns are reusable, general and typical solutions to recurring problems in object-oriented system design (design problems)

//More specifically, design patterns are pre-made abstract blueprints of general solution to a particular design problem (no implementation details, just conceptual description for how to solve a design problem)
//We present deisgn patterns in terms of their intent (describes the design problem and solution briefly), motivation, class structure (class diagram: parts of the pattern and how they relate), code example, applicability (where it is best used and what you can do with it), and relation to other design patterns

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

//Facade: the use of a simplified, limited but straightforward interface to a 3rd-party or secondary complex subsystem (library, framework or set of classes) without having to worry about their objects, execution order, dependency, etc => facade is a class-level encapsulation and abstraction (similar to an API endpoint)

class Client{
    string name;
    Facade* f;
    public:
        Client(string n): name(n){}
        void makeRequest(string request){
            cout << "Client made a request " << request; 
            string type = f->interpretRequest(request);
            if (type == "Warehouse"){
                f = new Warehouse(11,"Warehouse1", 20); 
                f->handleRequest(request);
            } else if (type == "Delivery"){
                f = new Delivery(11,"Delivery1", 20); 
                f->handleRequest(request);
            } else {
                cout << "invalid request"; //this part can be handled by an additional facade that handles unrelated requests, functionalities or features
            }
        }
}; 
class Facade{
    public:
        //interpretation of request
        string interpretRequest(string request){
            if (request == "I want to deliver something"){
                return "Delivery";
            } else if (request == "I want to store something"){
                return "Warehouse"; 
            } else{
                return ""; 
            }
        }
        //delegation of request
        virtual void handleRequest(string request) = 0;
        virtual ~Facade() = default;

}; 

//you can create a subsystem layer facade that serves as a communication interface between the subsystems themselves
class Warehouse: public Facade{
    int code;
    string name;
    int capacity;
    public:
        Warehouse(int c, string n, int cap): code(c), name(n), capacity(cap){}

        void handleRequest(string request) override{
            cout << "Warehouse request handled!";
        }
}; 

class Delivery: public Facade{
    int code;
    string name;
    int speed;
    public:
        Delivery(int c, string n, int s): code(c), name(n), speed(s){}

        void handleRequest(string request) override{
            cout << "Delivery request handled!";            
        }
    
};

int main(){
    Client cl("Ayoub");
    cl.makeRequest("I want to deliver something"); 
    return 0;
}

//Flyweight (Cache): caching common parts (immutable intrinsic states) of a massive number of similar objects in memory that heavily consume RAM for computational efficiency.
///my analogy: prepared statements (compile template once, change/bind param values for each run)
///my intuition: store or cache memory-heavy common parts (runtime-invariant members: attributes/states or methods/actions, not to say static attributes/methods) in a template object (flyweight), and only change what needs to be changed (mutatis mutandis)

//another implmentation of the flyweight in this example: extract extrinsic states (or intrinsic states) into a separate class -> apply Singleton pattern to the intrinsic states class (one instance) -> Link the extrinsic classes (Contexts) and intrinsic class (Flyweight: which contains extrinsic states as parametters to its methods) by association or use Flyweight as a data object in one or more Contexts for full original object completion
class Particle{ //the Flyweight class is the class that stores immutable intrinsic states
    // let's assume that size, speed and color are common parts between objects/instances (runtime-invariant: immutable and intrinsic state as opposed to mutable and extrinsic states)
    inline const static int size = 20;
    inline const static int speed = 10;
    string color; 
    string type;
    string shape;
    public:
        Particle(string t, string c, string sh): type(t), color(c), shape(sh){} //extrinsic mutable states

        string getShape(){
            return this->shape;
        }
    
};

// the RAM cost problem comes from aggregation/composition of the Particle class in a Game class

class Game{ //Flyweight factory
    string name;
    vector<Particle> particles;
    public:
        Game(string n, vector<Particle> pas): name(n), particles(pas){}

        void addParticle(Particle p){
            particles.push_back(p);
        }
        void drawParticle(Particle p){
            cout << p.getShape() << "drawn"; 
        }
};

int main(){
    Particle redBullet("red", "bullet", "bulletShape");
    Particle blueBullet("blue", "bullet", "bulletShape");

    Particle blueMissile("blue", "missile", "missileShape"); 
    return 0;
}


//Proxy: it lets you substitute or placehold an original object that consumes system resources but it's not always needed by controlling access to it (protection proxy) as it performs actions before or after the request is made to the original object -> create the heavyweight object only when needed (virtual proxy: lazy initialization)
///Proxy also does caching of results to client/user requests so that in case of multiple similar requests, the proxy merely returns the cached result without delegation to the original service
///Proxy implements the Open-Closed principle: you can extend the service beahvior just by adding more proxies
///Proxy knows about the User and Service but they aren't aware of it
class User{
    string name;
    string credentials; 
    ServiceInterface* SI;
    public:
        User(string n, string cred){
            this->name = n;
            this->credentials = cred;
        }

        void chooseService(string name){
            SI = new Proxy(name, 20);
        }
        void makeRequest(string request){
            SI->processRequest(request, *this); 
        }

        string getName() const {
            return name;
        }

        string getCredentials() const {
            return credentials; 
        }
}; 

class ServiceInterface{
    public:
        virtual void processRequest(string request, const User& user) = 0;
        virtual ~ServiceInterface() = default;
};
class Proxy: public ServiceInterface{ //only create a service object when needed -> it needs to implement the Service Interface to be able to disguise as a service to the User
    string name;
    int size;
    vector<string> cachedResults; //caching proxy
    vector<string> requestHistory; //logging proxy (logging requests: keeping a track of history of requests to the service object before processing)
    public:
        Proxy(string n, int s): name(n), size(s){}
        bool verifyAccess(Service* s, string userCredentials){ //protection proxy (user credentials verification)
            if (userCredentials == s->serviceCredentials) return true;
            else return false;
        }

        void processRequest(string request, const User& user) override{
           Service* ser = new Service(name, size);// virtual proxy (lazy initialization and lifecycle control: only create the service object when needed for task delegation: make-to-order on the fly) -> virtual proxy introduces concurrency (non-blocking I/O asynchronous execution: we don't wait for the service object to be available or ready e.g., cached results)
           requestHistory.push_back(request);//logging proxy (logging requests: keeping a track of history of requests to the service object before processing)
           vector<string>::iterator it = find(cachedResults.begin(), cachedResults.end(), ser->outputResult(request)); //iterator type
            if ( it != cachedResults.end()){ //cached results
                int index = it - cachedResults.begin(); 
                cout << cachedResults[index];
            } else if (verifyAccess(ser,user.getCredentials())){
                ser->processRequest(request, user); //remote proxy (service object located in a remote server -> local execution of remote service because the remote proxy handles all nasty details of working with an network)
                string result = ser->outputResult(request);
                cachedResults.push_back(result); //caching proxy (caching resource-consuming request results)
            } else{
                cout << "Service access invalid";
            }
        }
}; 


class Service: public ServiceInterface{
    string name; 
    string serviceCredentials;
    int size;
    public:
        Service(string n, int s): name(n), size(s){}

        void processRequest(string request, const User& user) override{
            cout << user.getName() << "Request processed!";
        }

        string outputResult(string request){
            return request + "processed!"; 
        }

        friend class Proxy; 
};


int main(){
    User us("Ayoub", "abc");
    us.chooseService("Delivery"); 
    us.makeRequest("I want to deliver my product!"); 
    return 0; 
}


//Behavioral Design Patterns: how objects communicate (information exchange mechanisms) and assign responsbilities to each others (delegation mechanisms)


//Chain of Responsibility (Chain of Command): a sequence of handlers 



class Handler{
    public:
        virtual void processRequest(string request) = 0;
        virtual Handler* setNext(Handler* h) = 0;
        virtual ~Handler() = default; 

}; 

//you can have a BaseHandler class that contains all the boilerplate code of all concrete handlers, their attriutes and order of execution
class AuthenticateException: public exception{
    public:
        AuthenticateException(const char* msg): exception(msg){}

        const char* what() const noexcept override{
            return exception::what();
        }
};
class AuthorizeException: public exception{
    public:
        AuthorizeException(const char* msg): exception(msg){}

        const char* what() const noexcept override{
            return exception::what();
        }
};
class ValidateException: public exception{
    public:
        ValidateException(const char* msg): exception(msg){}

        const char* what() const noexcept override{
            return exception::what();
        }
};

class Authenticate: public BaseHandler{
    int capacity;
    public:
        inline static int usedAuthObjects = 0;
        Authenticate(int c): capacity(c){
            Authenticate::usedAuthObjects++;
        }
        void processRequest(string request){
            if (capacity >= 20){ // in case of sufficient capacity: we can either automatically setNext or make it optional
                cout << request << ": authenticated!"; 
                return;
            } else { //in case of insufficient capacity, we can't: 1- reproces the request with the same object 2- do nothing (and manually/optionally setNext) 3- automatically setNext and pass request to next handler anyway (unless we have no sequential dependence or order of execution or that the request is universal and can be validly processed by any type of handler or that if all handlers are same type)
                //before throwing error, you can give it another chance by passing the request to another object of the same class type until the request is processed or a maximum iteration is exceeded (static attribute) -> throw error)
                while (usedAuthObjects < 10){
                    Authenticate* auth = new Authenticate(2*capacity); //aggregation: double the capacity
                    auth->processRequest(request);
                }
                throw AuthenticateException("Insufficient Authentication Capacity"); //throw error to indicate sequential dependence (no need for error throwing in case of no sequential order)
            }
        }
 
        ~Authenticate() noexcept {
            cout << "Authentication destroyed";
        }

}; 
class Authorize: public BaseHandler{
    int capacity;
    public:
        inline static int usedAuthoObjects = 0;
        Authorize(int c): capacity(c){
            Authorize::usedAuthoObjects++; 
        }

        void processRequest(string request){
            if (capacity >= 20){
                cout << request << ": auhtorized!"; 
            } else {
                while (usedAuthoObjects < 10){
                    Authorize* autho = new Authorize(2*capacity);
                    autho->processRequest(request);
                    if (2*capacity >= 20){
                        break;
                    }
                }
                throw AuthorizeException("Insufficient Authorization capacity"); 
            }
        }

        ~Authorize() noexcept {
            cout << "Authorization destroyed";
        }

}; 

class Validate:public BaseHandler{
    int capacity;
    public:
        inline static int usedValObjects = 0;
        Validate(int c): capacity(c){
            Validate::usedValObjects++;
        }

        void processRequest(string request){
            if (capacity >= 20){
                cout << request << ": validated!"; 
                return;
            } else {
                while (usedValObjects < 10){
                    Validate* val = new Validate(2*capacity);
                    val->processRequest(request);
                }
                throw ValidateException("Insufficient Validation capacity");
            }
        }

        ~Validate() noexcept {
            cout << "Validation destroyed";
        }
}; 


class BaseHandler:public Handler{
    Handler* currentHandler; //this attribute will solve the decoupling problem that forces setNext automatic call by bridging the handling logic chronologically through memorizing the current handler
    public:
        void processRequest(string request) override{
            currentHandler->processRequest(request);
        }  

        Handler* setNext(Handler* h){
            currentHandler = h;
            return currentHandler;
        };
};

int main(){
    try {        
        //concrete handlers
        Authenticate* authen = new Authenticate(10);
        Authorize* autho = new Authorize(4);
        Validate* val = new Validate(11);

        // unique_ptr<Authenticate> authenp = make_unique<Authenticate>(Authenticate(10));
        // unique_ptr<Authorize> authop = make_unique<Authorize>(Authorize(4));
        // unique_ptr<Validate> valp = make_unique<Validate>(Validate(11));

        BaseHandler bh;
        bh.setNext(authen);
        bh.processRequest("Authenticate this please");

        bh.setNext(autho);
        bh.processRequest("Authorize this please");

        bh.setNext(val);
        bh.processRequest("Validate this please");

        //hint queston: do we always guarantee memory release of these dynamically allocated objects? -> allocate using smart/unique ptrs instead!
        delete authen;
        delete autho; 
        delete val;

    } catch (const AuthenticateException& authen){
        cerr << "Authentication Exception caught" << authen.what(); 
    } catch (const AuthorizeException& autho){
        cerr << "Authorize Exception caught" << autho.what(); 
    } catch(const ValidateException& valerr){
        cerr << "Authentication Exception caught" << valerr.what(); 
    } catch (const exception& e){
        cerr << "Standard Exception caught" << e.what(); 
    } catch(...){
        cerr << "Generic Exception caught"; 
    }


    return 0;
}


//Command: Decoupling the GUI from the business logic (Separation of Concerns principle) by allowing the GUI to delegate user requests to a Command class that contains request metadata
///Prior to using Command pattern, the GUI was the one responsible for handling request info like its name, the business logic object it would invoke, its list of arguments and how business logic objects will process it
///The point of Command Pattern is to get rid of subclasses redundancy (that implement the same functionality) and tight coupling -> separation of concerns between a GUI layer and a business logic layer
class Command{ //request class containing request type (business logic object to be invoked), name, list of arguments
    //we can create a Command Interface that delegates work to its concrete Command classes which has 1-1 association with each Receiver/BL subclass (or depedency relation for code simplicity)
    BusinessLogic* BL; 
    string content;
    vector<string> arguments; //list of constraints to be respected for each command type
    const char name;
    public:
        Command(const char n, string content, vector<string> args, BusinessLogic* bl): name(n), content(content), arguments(args){
            BL = bl;
        }
        Command(const char n): name(n){
            switch(name){
                case 'Save':
                    BL = new Save;
                    content = "Save this item"; //pdf format + HD quality
                    arguments = {"pdfFormat", "HD"}; //save logic metadata
                case 'Copy':
                    BL = new Copy;
                    content = "Copy this item"; // have 2 copies for 30 days
                    arguments = {"2", "30"}; //copy logic metadata
                case 'Cnl':
                    BL = new Cancel;
                    content = "Cancel this operation"; 
                    arguments = {""};
                default:
                    BL = new Cancel;
                    content = "Cancel this operation"; 
                    arguments = {""};
            }
        }

        void sendRequest(){
            BL->receiveRequest(*this); 
        }

        void executeRequests(){
            BL->processRequests();
        }

        friend class BusinessLogic;
        friend class Copy;
        friend class Save;
        friend class Cancel;
};

class CL{ //Client: responsible for creating the command/request and passing it to the GUI/Sender/Invoker class
    public:
        Command makeRequest(const char name, string content, vector<string> arguments){
            if (name == 'Save'){
                Command c(name, content, arguments, new Save);
                return c; 
            } else if (name == 'Copy'){
                Command c(name, content, arguments, new Copy);
                return c; 
            } else {
                Command c(name, content, arguments, new Cancel);
                return c; 
            }

        }
}; 
class GUI{ //Sender or Invoker class: its role is to trigger the command, it does not create the command (up to Client class) or send the request directly to the Business Logic (up to Command Class)
    public:
        //trigger command (with client)
        void triggerCommand(Command c){
            c.sendRequest();
        }
        //trigger command (no client)
        void clickSaveButton(){
            Command c('Save');
            c.sendRequest(); 
        }

        void clickSaveMenuItemButton(){
            Command c('Save');
            c.sendRequest(); 
        }

        void clickSaveShortcutButton(){
            Command c('Save');
            c.sendRequest(); 
        }

        void clickCopyButton(){
            Command c('Copy');
            c.sendRequest(); 

        }

        void clickCopyMenuButton(){
            Command c('Copy');
            c.sendRequest(); 

        }

        void clickCancelButton(){
            Command c('Cnl');
            c.sendRequest(); 
        }
}; 

class BusinessLogic{ //Receiver class
    protected:
        vector<Command> CommandQueue; 
    public:
        void receiveRequest(Command& request){
            CommandQueue.push_back(request); 
            cout << request.name << "request received!"; 
        }
        virtual void processRequests() = 0;
        virtual ~BusinessLogic() = default;
};

class Copy: public BusinessLogic{
    public:
        void processRequest(string content, vector<string> args){
            cout << "Processing the following request: " << content << endl;
            for (int k = 0; k < args.size(); k++){
                cout << "Validating the following constraint: " << args[k] << "" << "Please wait..." << endl;
            }

            cout << "Your request" << content << "has been processed!";
        }

        void processRequests(){
            for (Command c: CommandQueue){
                if (c.name == 'Copy'){
                    processRequest(c.content, c.arguments);
                }
            }
        }
}; 
class Save: public BusinessLogic{
    public:
        void processRequest(string content, vector<string> args){
            cout << "Processing the following request: " << content << endl;
            for (int k = 0; k < args.size(); k++){
                cout << "Validating the following constraint: " << args[k] << "" << "Please wait..." << endl;
            }
            cout << "Your request" << content << "has been processed!";

        }

        void processRequests(){
            for (Command c: CommandQueue){
                if (c.name == 'Save'){
                    processRequest(c.content, c.arguments);
                }
            }
        }

};
class Cancel: public BusinessLogic{
    public:
        void processRequest(string content, vector<string> args){
            cout << "Processing the following request: " << content << endl;
            for (int k = 0; k < args.size(); k++){
                cout << "Validating the following constraint: " << args[k] << "" << "Please wait..." << endl;
            }
            cout << "Your request" << content << "has been processed!";
            
        }

        void processRequests(){
            for (Command c: CommandQueue){
                if (c.name == 'Cnl'){
                    processRequest(c.content, c.arguments);
                }
            }
        }

};


int main(){
    //loose coupoling (separation of concerns into GUI layer and Business Logic layer): 
    /// the GUI doesn't care about the specificities of the business logic, it simply allows the user to click a button and delegate all the work to the Command class which handles all the request's metadata
    GUI g; 
    //all the following save buttons delegate work to the same command type -> eliminate redundancy
    g.clickSaveButton();
    g.clickSaveMenuItemButton(); 
    g.clickSaveShortcutButton(); 

    //copying in different ways but same funcitonality -> both creates a command of type copy
    g.clickCopyButton();
    g.clickCopyMenuButton(); 
    //cancelling
    g.clickCancelButton();

    //With client class
    CL cl;
    Command saveCommand = cl.makeRequest('Save', "Save this file", {"pdfFormat", "HD"}); //implicit copy
    Command saveCommand2 = cl.makeRequest('Save', "Save this file", {"pdfFormat", "SD"}); //implicit copy
    Command copyCommand = cl.makeRequest('Copy', "Copy this file", {"2", "30"}); //implicit copy
    g.triggerCommand(saveCommand);
    g.triggerCommand(saveCommand2);
    g.triggerCommand(copyCommand);
    saveCommand.executeRequests(); //runs both save commands in a queue 
    copyCommand.executeRequests(); 
    return 0; 
}

//Observer
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

