#Note:
#I am not the best at using cmake, it is definitely a skill I could brush up on.
#Please excuse me, this file does not look very good, but it works very well for me.

# Projects Settings
cmake_minimum_required (VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# target_compile_options(main PRIVATE -Wall)

project (lightshow-bp)
add_library(lightshow-lib "")

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


file(GLOB TESTS_FILES "tests/*.cpp")

file(GLOB SOURCE_FILES_H "src/*.h")

file(GLOB SOURCE_FILES_CPP "src/*.cpp")

file(GLOB SOURCE_FILES "src/*.h" "src/*.cpp")

target_sources(lightshow-lib PRIVATE ${SOURCE_FILES_CPP} PUBLIC ${SOURCE_FILES_H})

target_include_directories(lightshow-lib PUBLIC src)             

if(USE_DEBUG_TOOLS)
    add_executable(lightshow ${SOURCE_FILES})
else()
    if(WIN32)
        add_executable(lightshow WIN32 ${SOURCE_FILES})
    else()
        add_executable(lightshow ${SOURCE_FILES})
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
    lightshow-lib
)

# for using ctest
# add_test(NAME unit_tests COMMAND unit_tests)

# SDL2
find_package(SDL2 REQUIRED)
include_directories(SYSTEM ${SDL2_INCLUDE_DIRS})
target_include_directories(lightshow-lib PUBLIC ${SDL2_INCLUDE_DIRS})
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)
target_link_libraries(lightshow PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(lightshow-lib PRIVATE ${SDL2_LIBRARIES})
message("${SDL2_INCLUDE_DIRS}")
# SDL_Mixer
find_package(SDL2_mixer REQUIRED)
include_directories(SYSTEM ${SDL_MIXER_INCLUDE_DIR})
target_include_directories(lightshow-lib PUBLIC ${SDL_MIXER_INCLUDE_DIR})
# string(STRIP "${SDL_MIXER_LIBRARY}" SDL_MIXER_LIBRARY)
target_link_libraries(lightshow PRIVATE ${SDL_MIXER_LIBRARY})
target_link_libraries(lightshow-lib PRIVATE ${SDL_MIXER_LIBRARY})

# SDL_Image
find_package(SDL2_image REQUIRED)
include_directories(SYSTEM ${SDL2IMAGE_INCLUDE_DIRS})
target_include_directories(lightshow-lib PUBLIC ${SDL2IMAGE_INCLUDE_DIRS})
# string(STRIP "${SDL_MIXER_LIBRARY}" SDL_MIXER_LIBRARY)
target_link_libraries(lightshow PRIVATE ${SDL2IMAGE_LIBRARIES})
target_link_libraries(lightshow-lib PRIVATE ${SDL2IMAGE_LIBRARIES})

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew EXCLUDE_FROM_ALL)
target_link_libraries(lightshow PRIVATE libglew_static)
target_include_directories(lightshow-lib PUBLIC lib/glew/include)
target_link_libraries(lightshow-lib PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(lightshow PRIVATE glm)
target_include_directories(lightshow-lib PUBLIC lib/glm)

# GameNetworkingSockets
find_package(GNS REQUIRED)
include_directories(SYSTEM ${GNS_INCLUDE_DIRS})
target_include_directories(lightshow-lib PUBLIC ${GNS_INCLUDE_DIRS})
target_link_libraries(lightshow PRIVATE ${GNS_LIBRARIES})
target_link_libraries(lightshow-lib PRIVATE ${GNS_LIBRARIES})

# bullet
include_directories(SYSTEM lib/bullet/src)
target_include_directories(lightshow-lib PUBLIC lib/bullet/src)
target_link_libraries(lightshow PRIVATE BulletDynamics)
target_link_libraries(lightshow PRIVATE BulletCollision)
target_link_libraries(lightshow PRIVATE LinearMath)
target_link_libraries(lightshow-lib PRIVATE BulletDynamics)
target_link_libraries(lightshow-lib PRIVATE BulletCollision)
target_link_libraries(lightshow-lib PRIVATE LinearMath)
# BulletDynamics.lib;BulletCollision.lib;

#some Bullet specific definitions need to be used, to be compatible
if (USE_DOUBLE_PRECISION)
add_definitions( -DBT_USE_DOUBLE_PRECISION)
set( BULLET_DOUBLE_DEF "-DBT_USE_DOUBLE_PRECISION")
endif (USE_DOUBLE_PRECISION)

add_subdirectory(lib/bullet EXCLUDE_FROM_ALL)

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

if(WIN32)

    #copy all assets using robocopy cmd script
    #do this so visual studio will copy after a file change. cmake's copy wont copy unless the cmake script is rebuilt.
    add_custom_command(
        TARGET lightshow PRE_BUILD 
        COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/RoboCopyAssets.bat"
        )

    add_custom_command(
        TARGET lightshow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            "${SDL2_LIBRARIES_PATH}/SDL2.dll"
            $<TARGET_FILE_DIR:lightshow>
    )
        
    add_custom_command(
        TARGET lightshow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${SDL2IMAGE_LIBRARIES_PATH}"
            $<TARGET_FILE_DIR:lightshow>
    )

    add_custom_command(
        TARGET lightshow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${SDL_MIXER_LIBRARY_PATH}"
            $<TARGET_FILE_DIR:lightshow>
    )

    add_custom_command(
        TARGET lightshow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different 
            "${GNS_LIBRARIES_PATH}/GameNetworkingSockets.dll"
            $<TARGET_FILE_DIR:lightshow>
    )

    if(BUILD_TYPE MATCHES "Release")

        add_custom_command(
            TARGET lightshow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/sdk/protobuf/Release/protobuf-amd${ARCHITECTURE}/bin/libprotobuf.dll"
                $<TARGET_FILE_DIR:lightshow>
        )

    elseif(BUILD_TYPE MATCHES "Debug")

        add_custom_command(
            TARGET lightshow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/sdk/protobuf/Debug/protobuf-amd${ARCHITECTURE}/bin/libprotobufd.dll"
                $<TARGET_FILE_DIR:lightshow>
        )
    
    endif()

    if(ARCHITECTURE EQUAL 86)

        add_custom_command(
            TARGET lightshow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/Program Files (x86)/OpenSSL-Win32/bin/libcrypto-1_1.dll"
                $<TARGET_FILE_DIR:lightshow>
        )

    elseif(ARCHITECTURE EQUAL 64)

        add_custom_command(
            TARGET lightshow POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different 
                "C:/Program Files/OpenSSL-Win64/bin/libcrypto-1_1-x64.dll"
                $<TARGET_FILE_DIR:lightshow>
        )

    endif()
endif()