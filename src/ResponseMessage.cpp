#include "ResponseMessage.h"

std::string Response::ToString(ResponseMessage message) {
    switch (message) {
        case ResponseMessage::OK:
            return "OK";
        case ResponseMessage::NIL:
            return "NIL";
        case ResponseMessage::WRONGTYPE:
            return "WRONG TYPE";
        default:
            return "Unknown response.";
    }
}