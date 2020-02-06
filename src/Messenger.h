#ifndef MESSENGER_H
#define MESSENGER_H
#include "GameInfo.h"
#include "HelpingHand.h"
#include "Messages.h"
#include <vector>

template <class T>

/*!The messenger- a pretty basic queue object used to send messages to one service from another. 

ex: from the Game class to the FixedUpdatingSystem class.
*/
class Messenger {

public:
    //!Add a message to the queue
    void addMessage(const T& message) {
        if (numberOfCurrentMessages >= GameInfo::DEFAULT_MESSAGE_CAPACITY) {
            DBG_LOG("Current messages at capacity (Messenger.h)");
            messageList.at(numberOfCurrentMessages) = message;
        } else {
            messageList.push_back(message);
        }
        numberOfCurrentMessages++;
    }

    //!Retrieves a message and stored it in the param `msg` then pops it from the list.
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

    //!Returns true if the `msg` value is in the message queue and removes the message from the queue.
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

    //!Clears all messages from the queue.
    void clearAll() {
        numberOfCurrentMessages = 0;
    }

private:
    int numberOfCurrentMessages = 0;
    std::vector<T> messageList  = std::vector<T>(GameInfo::DEFAULT_MESSAGE_CAPACITY);
};

#endif // !MESSENGER_H
