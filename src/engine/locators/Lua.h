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

#include <glm/gtc/quaternion.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace LUA_HANDLER {
    static const char* REGISTERED_FUNC_AR_NAME = "registered_functions";
    static const char* LUA_MAIN_ENTRY_FILE     = "assets/scripts/main.lua";
}

//!Handles the lifecycle of lua.
class LuaHandler {

public:
    //!Retrieves the current sol state.
    sol::state* getState() { return &state; }

    //!To be used after checking if function is callable with canUseFunction. Wrap in try/catch for error checking.
    sol::function getFunction(const std::string& functionName) {
#ifdef DEBUG

        if (canUseFunction(functionName) == false) {
            assert("Function is not registered in lua script! If this was on purpose please check isFunctionRegistered before calling function.");
        }

#endif // DEBUG

        return state[functionName];
    }

    //!Checks if a function name was found in the lua scripts registered_functions table.
    bool canUseFunction(const std::string& functionName) {
        return isCompiled && std::find(registeredFunctionNames.begin(), registeredFunctionNames.end(), functionName) != registeredFunctionNames.end();
    }

    //!Compiles and initializes sol/lua
    void initialize() {
        recompile();
    }

    void recompile() {
        registeredFunctionNames.clear();

        DBG_LOG("Compiling Lua...\n");

        //Before script_file is when we would add c++ objects/functions to the script.
        bindGLM();

        state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::io);

        try {
            state.script_file(LUA_HANDLER::LUA_MAIN_ENTRY_FILE);

        } catch (const std::exception&) {

            DBG_LOG("Lua failed to compile.\n");
            DBG_LOG("Using default C++ values.\n");

            return;
        }

        getRegisteredFunctions();

        DBG_LOG("Lua Compiled.\n");

        isCompiled = true;
    }

private:
    //!Retrieves functions defined in lua file under table name LUA_HANDLER::REGISTERED_FUNC_AR_NAME.
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

    //!Binds glm objects to lua for usage.
    void bindGLM() {

        //Add glm::vec3<float> to lua.
        DBG_LOG("Binding GLM vec3<float> as vec3f.\n");
        sol::usertype<glm::vec3> gv3 = state.new_usertype<glm::vec3>("vec3f", sol::constructors<glm::vec3(float, float, float)>());

        gv3["x"] = &glm::vec3::x;
        gv3["y"] = &glm::vec3::y;
        gv3["z"] = &glm::vec3::z;

        DBG_LOG("Binding GLM vec2<float> as vec2f.\n");
        sol::usertype<glm::vec2> gv2 = state.new_usertype<glm::vec2>("vec2f", sol::constructors<glm::vec2(float, float)>());

        gv2["x"] = &glm::vec2::x;
        gv2["y"] = &glm::vec2::y;

        DBG_LOG("Binding GLM quat<float> as quatf.\n");
        sol::usertype<glm::quat> gq = state.new_usertype<glm::quat>("quatf", sol::constructors<glm::quat(float, float, float, float)>());

        gq["w"] = &glm::quat::w;
        gq["x"] = &glm::quat::x;
        gq["y"] = &glm::quat::y;
        gq["z"] = &glm::quat::z;
    }

    //!List of functions registered in lua.
    std::vector<std::string> registeredFunctionNames;
    sol::state state;
    bool isCompiled = false;
};
#endif
