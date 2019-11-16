#ifndef MESSENGER_H
#define MESSENGER_H
#include "GameInfo.h"
#include "HelpingHand.h"
#include "Messages.h"
#include <vector>

template <class T>
class Messenger {

public:
    void AddMessage(const T& message) {
        if (numberOfCurrentMessages < GameInfo::DEFAULT_MESSAGE_CAPACITY) {
            DBG_LOG("Current messages at capacity (Messenger.h)");
            messageList.at(numberOfCurrentMessages) = message;
        } else {
            messageList.push_back(message);
        }
        numberOfCurrentMessages++;
    }

    bool getMessagesThenRemove(T& msg) {
        if (numberOfCurrentMessages - 1 >= 0) {
            msg = messageList.at(numberOfCurrentMessages - 1);
            numberOfCurrentMessages--;
            return true;
        } else {
            msg = static_cast<T>(-1);
            return false;
        }
    }

    bool findThenRemoveMessage(T msg) {

        for (int i = 0; i < numberOfCurrentMessages; i++) {
            if (messageList[i] == msg) {
                hh::moveItemToBack(messageList, i);
                numberOfCurrentMessages--;
                return true;
            }
        }
        return false;
    }

    void clearAll() {
        numberOfCurrentMessages = 0;
    }

private:
    int numberOfCurrentMessages = 0;
    std::vector<T> messageList  = std::vector<T>(GameInfo::DEFAULT_MESSAGE_CAPACITY);
};

#endif // !MESSENGER_H
