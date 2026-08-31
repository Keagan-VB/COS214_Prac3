#include "EventGroup.h"

#include "Notice.h"
#include "NoticeType.h"

EventGroup::EventGroup(const std::string& name, int capacityThreshold) : EventComponent(name, capacityThreshold) {
    currentNotice = nullptr;
}

void EventGroup::add(EventComponent* c){

    if(c != nullptr){

        children.push_back(c);
    }
}

void EventGroup::remove(EventComponent* c){

    for(auto it = children.begin(); it != children.end();){

        if(*it == c){
            children.erase(it);
            break;
        }else{
            ++it;
        }
    }

}

void EventGroup::open(){

    for(auto& a : children){
    
        a->open();
    }
}

void EventGroup::close(){

    for(auto& a : children){
    
        a->close();
    }
}

void EventGroup::reportStatus() const{

    std::cout << name << "[";

    for(auto& a : children){
       a->reportStatus();
    }

    std::cout << "]"; 

}

int EventGroup::getCapacity() const{

    int total = 0;

    for(const auto& a : children){

        total += a->getCapacity();
    }

    return total;

}

void EventGroup::attach(Observer* o){

    bool found = false;
    for(auto& a : observerList){

        if(a == o){
            found = true;
            break;
        }
    }

    if(!found){
        observerList.push_back(o);
    }
}

void EventGroup::detach(Observer* o){

    for(auto it = observerList.begin(); it != observerList.end();){

        if(*it == o){
            observerList.erase(it);
            break;
        }else{
            ++it;
        }
    }

}

void EventGroup::notify(){

    if(currentNotice == nullptr){
        return;
    }else{

        for(auto& a : observerList){

            a->update(*currentNotice);
        }
    }
}

void EventGroup::update(const Notice& n){

    currentNotice = &n;

    if(n.getType() == NoticeType::CAPACITY_ALERT){

        if(getCapacity() >= capacity){

            notify();
        }
    }else{

        notify();
    }
    
}

EventGroup::~EventGroup(){

    for(auto& a : children){

        delete a;
    }
}


bool EventGroup::transferChild(EventComponent* child, EventGroup* destination){

    if(child == nullptr || destination == nullptr){
        return false;
    }

    bool found = false;
    for(auto& a : this->children){

        if(a == child){
            found = true;
            break;
        }
    }

    if(!found){
        return false;
    }

    remove(child);

    Observer* asObserver = dynamic_cast<Observer*>(child);

    if(asObserver != nullptr){
        detach(asObserver);
    }

    destination->add(child);
    if(asObserver != nullptr){
        destination->attach(asObserver);
    }

    return true;
}


