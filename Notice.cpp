#include "Notice.h"

Notice::Notice(NoticeType type, const std::string& message) 
    : type(type), message(message) {}

NoticeType Notice::getType() const {
    return type;
}

std::string Notice::getMessage() const {
    return message;
}