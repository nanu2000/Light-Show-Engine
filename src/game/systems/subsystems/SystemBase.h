#ifndef SYSTEM_BASE_H
#define SYSTEM_BASE_H

enum class SystemMessages : int {
    NULL_MESSAGE = -1,
    LUA_COMPILED,
    MESSAGES_END
};

class SystemBase {
public:
    virtual void recieveMessage(const SystemMessages& msg) {}
};

#endif
