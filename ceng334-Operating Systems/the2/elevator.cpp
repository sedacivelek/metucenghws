#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include "monitor.h"
#include <iostream>
#include <string>
#include<list>
#include<fstream>
#include<algorithm>
using namespace std;

struct Person {
    int id;
    int wp;
    int initf;
    int destf;
    int priority;
    string direction;
};

struct Elevator{
    int weight_cap;
    int person_cap;
    int travelt;
    int idlet;
    int inoutt;
    string state;
    int cfloor;
    int currentp;
    int currentw;
    list<Person> inelev;
    list<int> destq;

};

class Controller:public Monitor {
    int nfloors;
    int npeople;
    int capacity;
    int pcap;
    int travelt;
    int idlet,inout;
    Condition request;
    Condition served;
    list<Person> reqs;
    Elevator elevator;
    Person* people;
    list<Person>* floors; 
    Condition infloor;
    int* finished;
    int* inelev;
    Condition request2;
    public:
    Controller(int nd,int np,int wc,int pc,int tt,int it, int iot):request(this),served(this),infloor(this),request2(this){
        nfloors = nd;
        npeople = np;
        capacity = wc;
        pcap = pc;
        travelt=tt;
        idlet=it;
        inout=iot;
        elevator.weight_cap= wc;
        elevator.person_cap= pc;
        elevator.travelt= tt;
        elevator.idlet= it;
        elevator.inoutt= iot;
        elevator.state="idle";
        elevator.currentp=0;
        elevator.currentw= 0;
        elevator.cfloor=0;
        inelev = new int[np];
        finished = new int[np];
        floors = new list<Person>[nfloors];
        people = new Person[np];

    }
    
    void printPerson(Person &p){
        string pri = p.priority == 1 ? "hp" :"lp";
        cout<< "Person ("<< p.id <<", " << pri << ", "<< p.initf <<" -> " << p.destf << ", " << p.wp << ") made a request" << endl; 
    }
    
    void addPerson(int id, int wp, int initf, int destf,int pri){
        people[id].id = id;
        people[id].wp=wp;
        people[id].initf=initf;
        people[id].destf=destf;
        people[id].priority=pri;
        people[id].direction= (initf<destf) ? "up" : "down";
    }
    Person* allperson(){
        return people;
    }
    int finito(){
        __synchronized__;
        int flag=1;
        for(int i=0;i<npeople;i++){
            if(finished[i]==0) flag=0;
        }
        return flag;
    }
    void elev(){
        
        int flag=0;
        while(!flag){
            while(elevator.destq.empty()&&flag){
                request2.notifyAll();
                usleep(idlet);
                flag=finito();
                

            }
            
            usleep(inout);
            
            request2.notifyAll();
            
            usleep(travelt);
            
            move();
            
            request2.notifyAll();
            
            
            flag=finito();
        }
    }
    void move(){
        __synchronized__;
        
        if(elevator.destq.front()<elevator.cfloor){
            
            elevator.destq.sort(greater<>());
            elevator.cfloor--;
            elevator.state="down";
        }
        if(elevator.destq.front()>elevator.cfloor){
            elevator.destq.sort();
            elevator.cfloor++;
            elevator.state="up";
        }
        list<int>::iterator erased= find(elevator.destq.begin(),elevator.destq.end(),elevator.cfloor);
        if(erased!=elevator.destq.end()){
            
            elevator.destq.erase(erased);
            

        }
        
        
        bool flag = false;
        for(int i=0;i<npeople;i++){
            if(finished[i]==0) flag=true;
        }
        
        
        if(elevator.destq.empty()){
            
            elevator.state="idle";
        }
        if(flag) printElev();
        infloor.notifyAll();
        request2.notifyAll();
    }

    void printElev(){
        
        if(elevator.state=="idle"){
            cout<<"Elevator (Idle, "<<elevator.currentw<<", "<<elevator.currentp<<", "
            <<elevator.cfloor<<" ->)" << endl;
        }
        else if(elevator.state=="up"){
            
            cout<<"Elevator (Moving-up, "<<elevator.currentw<< ", "<<elevator.currentp<< ", "
            <<elevator.cfloor<< " -> ";
            list<int>::iterator it;
            list<int>::iterator befend=elevator.destq.end();
            advance(befend,-1);
            for(it=elevator.destq.begin();it!=befend;++it){
                cout<< *it << ",";
            }
            cout<<*it<<")"<<endl;
        }
        else if(elevator.state=="down"){
            cout<<"Elevator (Moving-down, "<<elevator.currentw<<", "<<elevator.currentp<<", "
            <<elevator.cfloor<<" -> ";
            list<int>::iterator it;
            list<int>::iterator befend=elevator.destq.end();
            advance(befend,-1);
            for(it=elevator.destq.begin();it!=befend;++it){
                cout<< *it << ",";
            }
            cout<<*it<<")"<<endl;
        }
        
    }

    bool perso(Person p){

        __synchronized__;
        string req=requestat(p);
        
        if(req!=""){
            while(p.initf!=elevator.cfloor){
                infloor.wait();
            }
            while(isThere()){
                served.wait();
            }
            list<int>::iterator it=find(elevator.destq.begin(),elevator.destq.end(),elevator.cfloor);
            if(it != elevator.destq.end()){
                elevator.destq.erase(it);
            }
            if(!floors[elevator.cfloor].empty()){
                while(p.id!=floors[elevator.cfloor].front().id){
                    request.wait();
                }
                if(elevator.person_cap!=elevator.currentp && elevator.weight_cap>=(elevator.currentw+p.wp)){
                    if(elevator.state=="idle"){
                        elevator.inelev.push_back(p);
                        inelev[p.id]=1;
                        cout<<"Person ("<<p.id<<", "<<((p.priority == 1) ? "hp" :"lp")<<", "
                        <<p.initf<<" -> "<<p.destf<<", "<<p.wp<< ") entered the elevator"<<endl;
                        elevator.currentw+=p.wp;
                        elevator.currentp++;
                        std::list<int>::iterator it=find(elevator.destq.begin(),elevator.destq.end(),p.destf);
                        if(it==elevator.destq.end())
                            elevator.destq.push_back(p.destf);

                        elevator.state= (p.destf>elevator.cfloor) ? "up":"down";    
                        if(elevator.state=="down") 
                        elevator.destq.sort(greater<>());
                        else if(elevator.state=="up") 
                        elevator.destq.sort();
                        printElev();
                    }
                    else if(elevator.state==p.direction){
                        elevator.inelev.push_back(p);
                        inelev[p.id]=1;
                        cout<<"Person ("<<p.id<<", "<<((p.priority == 1) ? "hp" :"lp")<<", "
                        <<p.initf<<" -> "<<p.destf<<", "<<p.wp<< ") entered the elevator"<<endl;
                        elevator.currentw+=p.wp;
                        elevator.currentp++;
                        list<int>::iterator it=find(elevator.destq.begin(),elevator.destq.end(),p.destf);
                        if(it==elevator.destq.end())
                            elevator.destq.push_back(p.destf);

                
                        if(elevator.state=="down") 
                        elevator.destq.sort(greater<>());
                        else if(elevator.state=="up") 
                        elevator.destq.sort();
                        printElev();

                    }

                }
                list<Person>::iterator i=floors[elevator.cfloor].begin();
                if(i!=floors[elevator.cfloor].end())
                floors[elevator.cfloor].erase(i);
                

                
            }
            request.notifyAll();
            if(inelev[p.id]==1){
                while(elevator.cfloor!=p.destf){
                    infloor.wait();
                }
                list<Person>::iterator posit;
                for(posit=elevator.inelev.begin();posit!=elevator.inelev.end();++posit){
                    if(posit->id==p.id) 
                        break;
                }
                if(posit!=elevator.inelev.end());
                elevator.inelev.erase(posit);
                elevator.currentp--;
                elevator.currentw-=p.wp;
                cout<<"Person ("<<p.id<<", "<<((p.priority == 1) ? "hp" :"lp")<<", "
                <<p.initf<<" -> "<<p.destf<<", "<<p.wp<< ") has left the elevator"<<endl;
                finished[p.id]=1;
                printElev();
                served.notifyAll();
                return true;

            }
        }
        request2.wait();
        return false;

    }

    bool isThere(){
        bool flag=false;
        list<Person>::iterator it;
        for(it=elevator.inelev.begin();it!=elevator.inelev.end();++it){
            if(elevator.cfloor==it->destf)
                flag=true;
        }
        return flag;
    }
    string requestat(Person p){
        if(p.initf>=elevator.cfloor && p.direction=="up" && elevator.state=="up"){
            printPerson(p);
            floors[p.initf].push_back(p);
            list<int>::iterator it=find(elevator.destq.begin(),elevator.destq.end(),p.initf);
            if(it == elevator.destq.end()&& elevator.cfloor!=p.initf){
                elevator.destq.push_back(p.initf);
            }
            floors[p.initf].sort(prior);
            elevator.destq.sort();
            printElev();
            return "up";

        }
        else if(p.initf<=elevator.cfloor && p.direction=="down"&&elevator.state=="down"){
            printPerson(p);
            floors[p.initf].push_back(p);
            list<int>::iterator it=find(elevator.destq.begin(),elevator.destq.end(),p.initf);
            if(it == elevator.destq.end()&& elevator.cfloor!=p.initf){
                elevator.destq.push_back(p.initf);
            }
            floors[p.initf].sort(prior);
            elevator.destq.sort(greater<>());
            printElev();
            return "down";
        }
        else if(elevator.state=="idle"){
            if(floors[elevator.cfloor].empty()){
                printPerson(p);
                list<int>::iterator it=find (elevator.destq.begin(),elevator.destq.end(),p.initf);
                if(it==elevator.destq.end()){
                    elevator.destq.push_back(p.initf);
                }
                floors[p.initf].push_back(p);
                floors[p.initf].sort(prior);
                if(p.initf>elevator.cfloor)
                    elevator.state="up";
                else if(p.initf<elevator.cfloor)
                    elevator.state="down";
                printElev();
                return "idle";
            }
        }
        return "";
    }
    static bool prior(Person a,Person b){
        if(a.priority<b.priority){
            return true;
        }
        return false;
    }
};

struct Pparam{
    Controller *cs;
    int i;
};
void *person_routine(void *p){
    int i;
    Pparam* params = (Pparam*) p;
    Controller* controller = params->cs;
    i=params->i;
    Person person = controller->allperson()[i];
    while(1){
        if(controller->perso(person)) break;
    }
}

void *elev_routine(void *p){
    Controller* con = (Controller*) p;
    con->elev();

}


int main(int argc,char *argv[]){
    int a,b,c,d, nd,np,wc,pc,tt,it,iot;
    pthread_t *people = new pthread_t[np];
    pthread_t elevo;
    Pparam* params = new Pparam[np];
    ifstream ifile;
    ifile.open(argv[1]);
    ifile >> nd >> np >> wc >> pc >> tt >> it >> iot;
    Controller con(nd,np,wc,pc,tt,it,iot);
    int wp,initf,destf,pri;
    for(int i=0;i<np;i++){
        ifile >>wp >> initf >> destf >> pri;
        con.addPerson(i,wp,initf,destf,pri);
    }
    ifile.close();
    for(int i=0;i<np;i++){
        params[i].cs = &con;
        params[i].i=i;
        pthread_create(&people[i],NULL,person_routine,(void *)(params+i));
    }
    pthread_create(&elevo,NULL,elev_routine,(void *) &con);
    
    
    for(int i=0;i<np;i++){
        pthread_join(people[i],NULL);
    }
    pthread_join(elevo,NULL);

    
    delete []people;
    delete []params;
    return 0;
}