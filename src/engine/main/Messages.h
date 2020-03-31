#ifndef MESSAGES_H
#define MESSAGES_H

enum class BackEndMessages : int {
    NULL_MESSAGE = -1,
    LUA_COMPILED,
    REFRESH_CAMERA,
    MESSAGES_END
};

#endif // !MESSAGES_H
