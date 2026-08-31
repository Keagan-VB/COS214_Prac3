#include "EventComponent.h"

EventComponent::EventComponent(const std::string& name, int capacity){

    this->name = name;
    this->capacity = capacity;
}

std::string EventComponent::getName() const{

    return this->name;
}