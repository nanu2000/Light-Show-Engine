set(LUA_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include/lua")

if(UNIX AND NOT APPLE)


else()

    #64 bits
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(LUA_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/lua/x64/lua53.lib")
        set(LUA_LIBRARIES_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/lua/x64")
   
    #32 bits
    elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(LUA_LIBRARIES "${CMAKE_CURRENT_LIST_DIR}/lib/lua/x86/lua53.lib")
        set(LUA_LIBRARIES_PATH "${CMAKE_CURRENT_LIST_DIR}/lib/lua/x86")
    endif()

endif()
