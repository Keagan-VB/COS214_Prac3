
#include "FoodVendor.h"



FoodVendor::FoodVendor(const std::string& name, int capacity, int stockLevel) : EventUnit(name, capacity) {

    this->stockLevel = stockLevel;
}

void FoodVendor::reportStatus() const{

    printCommonStatus();
    std::cout << ", Stock Level" << stockLevel << "\n";
}

void FoodVendor::update(const Notice& notice){

    EventUnit::update(notice);

    if(notice.getType() == NoticeType::CAPACITY_ALERT){

        if(stockLevel < 10){

            stockLevel = 0;
        }else{

            stockLevel -= 10;
        }
    }

}
