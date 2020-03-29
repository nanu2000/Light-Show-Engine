#ifndef LS_LUA_H
#define LS_LUA_H
#include "debug.h"

extern "C" {
#include "lauxlib.h"
#include "lua.h"
#include "lualib.h"
}

#include "sol/forward.hpp"
#include "sol/sol.hpp"

namespace LUA_HANDLER {
    static const char* REGISTERED_FUNC_AR_NAME = "registered_functions";
}

class LuaHandler {

public:
    sol::state* getState() { return &state; }

    //!To be used after checking if function is callable with canUseFunction.
    sol::function getFunction(const std::string& functionName) {
#ifdef DEBUG

        if (canUseFunction(functionName) == false) {
            assert("Function is not registered in lua script! If this was on purpose please check isFunctionRegistered before calling function.");
        }

#endif // DEBUG

        return state[functionName];
    }

    //!Checks if a function name was found in the lua scripts registered_functions table
    bool canUseFunction(const std::string& functionName) {
        return isCompiled && std::find(registeredFunctionNames.begin(), registeredFunctionNames.end(), functionName) != registeredFunctionNames.end();
    }

    void initialize() {
        recompile();
    }

    void recompile() {
        registeredFunctionNames.clear();

        DBG_LOG("Compiling Lua...\n");
        //Before script_file is when we would add c++ objects/functions to the script.

        state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);

        try {
            state.script_file("assets/scripts/main.lua");

        } catch (const std::exception& e) {

            DBG_LOG("Lua failed to compile.\n");
            DBG_LOG("Using default C++ values.\n");

            return;
        }

        getRegisteredFunctions();

        DBG_LOG("Lua Compiled.\n");

        isCompiled = true;
    }

private:
    void getRegisteredFunctions() {

        sol::table registeredFunctionList = state[LUA_HANDLER::REGISTERED_FUNC_AR_NAME];

        for (unsigned int i = 1; true; i++) {
            std::string str = registeredFunctionList[i];

            if (std::empty(str)) {
                break;
            }

            registeredFunctionNames.push_back(str);
        }
    }

    std::vector<std::string> registeredFunctionNames;
    sol::state state;
    bool isCompiled = false;
};

#endif
