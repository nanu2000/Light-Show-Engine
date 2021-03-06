#Note:
#I am not the best at using cmake, it is definitely a skill I could brush up on.
#Please excuse me, this file does not look very good, but it works very well for me.

# Projects Settings
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project (lightshow_bp_project)
add_library(lightshow_bp-lib "")

if(UNIX AND NOT APPLE)
    add_compile_definitions(LINUX)
endif()

#64 bits
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
   set(ARCHITECTURE "64")
#32 bits
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
   set(ARCHITECTURE "86")
endif()

set(USE_DEBUG_TOOLS true)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set("BUILD_TYPE" "Debug")
elseif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
    set("BUILD_TYPE" "Release")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    set("BUILD_TYPE" "Release")
    set(USE_DEBUG_TOOLS false)
elseif(CMAKE_BUILD_TYPE MATCHES MinSizeRel)
    set("BUILD_TYPE" "Release")
    set(USE_DEBUG_TOOLS false)
endif()


file(GLOB_RECURSE TESTS_FILES "tests/*.cpp")

file(GLOB_RECURSE SOURCE_FILES_H "src/*.h")

file(GLOB_RECURSE SOURCE_FILES_CPP "src/*.cpp")

file(GLOB_RECURSE SOURCE_FILES 
    "src/*.h" 
    "src/*.cpp")
    
target_sources(lightshow_bp-lib PRIVATE ${SOURCE_FILES_CPP} PUBLIC ${SOURCE_FILES_H})

include_directories(lightshow_bp 
    "include/sol2/single/include"
    "src" 
        "src/engine" 
            "src/engine/main" 
            "src/engine/locators" 
            "src/engine/main"
            "src/engine/physics"
        "src/game"
        "src/game/"
            "src/game/3dm"
                "src/game/3dm/shapes"
            "src/game/collision"
            "src/game/components"
            "src/game/entities"
            "src/game/global"
            "src/game/gui"
            "src/game/io"
            "src/game/rendering"
            "src/game/systems"
                "src/game/systems/subsystems"
)

target_include_directories(lightshow_bp-lib PUBLIC src)
target_include_directories(lightshow_bp-lib PUBLIC include/sol2/single/include)

if(USE_DEBUG_TOOLS)
    add_executable(lightshow_bp ${SOURCE_FILES})
else()
    if(WIN32)
        add_executable(lightshow_bp WIN32 ${SOURCE_FILES})
    else()
        add_executable(lightshow_bp ${SOURCE_FILES})
    endif()
endif()

# Download and unpack googletest at configure time
configure_file(GTestConfig.cmake lib/googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-src
                    ${CMAKE_CURRENT_BINARY_DIR}/lib/googletest-build
                    EXCLUDE_FROM_ALL)
add_executable(
    unit_tests
    ${TESTS_FILES}
)
target_link_libraries(
    unit_tests
    gtest_main
    lightshow_bp-lib
)

# for using ctest
# add_test(NAME unit_tests COMMAND unit_tests)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})
target_include_directories(lightshow_bp-lib PUBLIC ${SDL2_INCLUDE_DIRS})
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
target_link_libraries(lightshow_bp PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(lightshow_bp-lib PRIVATE ${SDL2_LIBRARIES})
message("${SDL2_INCLUDE_DIRS}")
# SDL_Mixer
find_package(SDL2_mixer REQUIRED)
include_directories(SYSTEM ${SDL_MIXER_INCLUDE_DIR})
target_include_directories(lightshow_bp-lib PUBLIC ${SDL_MIXER_INCLUDE_DIR})
target_link_libraries(lightshow_bp PRIVATE ${SDL_MIXER_LIBRARY})
target_link_libraries(lightshow_bp-lib PRIVATE ${SDL_MIXER_LIBRARY})

# SDL_Image
find_package(SDL2_image REQUIRED)
include_directories(SYSTEM ${SDL2IMAGE_INCLUDE_DIRS})
target_include_directories(lightshow_bp-lib PUBLIC ${SDL2IMAGE_INCLUDE_DIRS})
target_link_libraries(lightshow_bp PRIVATE ${SDL2IMAGE_LIBRARIES})
target_link_libraries(lightshow_bp-lib PRIVATE ${SDL2IMAGE_LIBRARIES})

# Lua
find_package(Lua53 REQUIRED)
include_directories(SYSTEM ${LUA_INCLUDE_DIR})
target_include_directories(lightshow_bp-lib PUBLIC ${LUA_INCLUDE_DIR})
target_link_libraries(lightshow_bp PRIVATE ${LUA_LIBRARIES})
target_link_libraries(lightshow_bp-lib PRIVATE ${LUA_LIBRARIES})

# GLEW
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(lightshow_bp PRIVATE libglew_static)
target_include_directories(lightshow_bp-lib PUBLIC lib/glew/include)
target_link_libraries(lightshow_bp-lib PRIVATE libglew_static)

# GLM
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(lightshow_bp PRIVATE glm)
target_include_directories(lightshow_bp-lib PUBLIC lib/glm)

# GameNetworkingSockets
find_package(GNS REQUIRED)
include_directories(SYSTEM ${GNS_INCLUDE_DIRS})
target_include_directories(lightshow_bp-lib PUBLIC ${GNS_INCLUDE_DIRS})
target_link_libraries(lightshow_bp PRIVATE ${GNS_LIBRARIES})
target_link_libraries(lightshow_bp-lib PRIVATE ${GNS_LIBRARIES})

# Bullet
include_directories(SYSTEM lib/bullet/src)
target_include_directories(lightshow_bp-lib PUBLIC lib/bullet/src)
target_link_libraries(lightshow_bp PRIVATE BulletDynamics)
target_link_libraries(lightshow_bp PRIVATE BulletCollision)
target_link_libraries(lightshow_bp PRIVATE LinearMath)
target_link_libraries(lightshow_bp-lib PRIVATE BulletDynamics)
target_link_libraries(lightshow_bp-lib PRIVATE BulletCollision)
target_link_libraries(lightshow_bp-lib PRIVATE LinearMath)
# BulletDynamics.lib;BulletCollision.lib;

# Some Bullet specific definitions need to be used, to be compatible
if (USE_DOUBLE_PRECISION)
    add_definitions( -DBT_USE_DOUBLE_PRECISION)
    set( BULLET_DOUBLE_DEF "-DBT_USE_DOUBLE_PRECISION")
endif (USE_DOUBLE_PRECISION)

add_subdirectory(lib/bullet EXCLUDE_FROM_ALL)


# Asset copying
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if(WIN32)

    #copy all assets using robocopy cmd script
    #do this so visual studio will copy after a file change. cmake's copy wont copy unless the cmake script is rebuilt.
    add_custom_command(
        TARGET lightshow_bp PRE_BUILD 
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/RoboCopyAssets.bat"
        )

    add_custom_command(
        TARGET lightshow_bp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            "${SDL2_LIBRARIES_PATH}/SDL2.dll"
            $<TARGET_FILE_DIR:lightshow_bp>
    )
        
    add_custom_command(
        TARGET lightshow_bp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${SDL2IMAGE_LIBRARIES_PATH}"
            $<TARGET_FILE_DIR:lightshow_bp>
    )

    add_custom_command(
        TARGET lightshow_bp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${SDL_MIXER_LIBRARY_PATH}"
            $<TARGET_FILE_DIR:lightshow_bp>
    )
    
    add_custom_command(
        TARGET lightshow_bp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${LUA_LIBRARIES_PATH}"
            $<TARGET_FILE_DIR:lightshow_bp>
    )

    add_custom_command(
        TARGET lightshow_bp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            "${GNS_LIBRARIES_PATH}/GameNetworkingSockets.dll"
            $<TARGET_FILE_DIR:lightshow_bp>
    )

    if(BUILD_TYPE MATCHES "Release")

        add_custom_command(
            TARGET lightshow_bp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/sdk/protobuf/Release/protobuf-amd${ARCHITECTURE}/bin/libprotobuf.dll"
                $<TARGET_FILE_DIR:lightshow_bp>
        )

    elseif(BUILD_TYPE MATCHES "Debug")

        add_custom_command(
            TARGET lightshow_bp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/sdk/protobuf/Debug/protobuf-amd${ARCHITECTURE}/bin/libprotobufd.dll"
                $<TARGET_FILE_DIR:lightshow_bp>
        )
    
    endif()

    if(ARCHITECTURE EQUAL 86)

        add_custom_command(
            TARGET lightshow_bp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/Program Files (x86)/OpenSSL-Win32/bin/libcrypto-1_1.dll"
                $<TARGET_FILE_DIR:lightshow_bp>
        )

    elseif(ARCHITECTURE EQUAL 64)

        add_custom_command(
            TARGET lightshow_bp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/Program Files/OpenSSL-Win64/bin/libcrypto-1_1-x64.dll"
                $<TARGET_FILE_DIR:lightshow_bp>
        )

    endif()
endif()