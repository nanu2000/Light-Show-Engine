# Light-Show-Boilerplate
LightShow is a 3D boilerplate created with the purpose of flexibility, scalability, and simplicity.

Currently this project only works with Microsoft Windows 10

This application uses two other projects for 3D model and font atlas file creation that are currently private.

## Features
* 3D skeletal animation
* Directional shadow mapping
* Runtime GLSL modifications
* Physics, Collision, Callbacks and Triggers
* Audio and music support
* Text rendering
* Phong shading
* Point lights
* Diffuse shading
* Directional lights
* Omnidirectional shadow mapping
* Particle systems with instanced rendering
* Bone Colliders (updates scale, position and rotation of a collision object according to a bone)

## Getting Started

### Prerequisites
Required:
A linux or windows system that supports OpenGL 3.3+

### Windows 10 install for Visual Studio Cross Platform with Linux
* Git clone --recursive
* open root dir in Visual Studio
* Supply CMake settings
* install https://slproweb.com/products/Win32OpenSSL.html - both 86 and 64. These are used to build Google protobuf
* navigate to /lib/protobuf_build
* run populate_all.bat and supply your version of VsDevCmd.bat as an arg. 

ex:
```populate_all.bat "C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\Common7\Tools\VsDevCmd.bat"```

Protobuf should then be installed in C:\sdk\protobuf
This WILL take awhile. 

* navigate to /lib/gamenetworksockets_build
* run populate_all.bat and supply your version of VsDevCmd.bat as an arg. 

ex: 
```populate_all.bat "C:\Program Files (x86)\Microsoft Visual Studio\2019\Preview\Common7\Tools\VsDevCmd.bat"```

GameNetworkSockets should then be installed in /lib/gamenetworksockets_build This shouldn't take as long as it took protobuf to build.

* Create a vm for linux. I used Ubuntu 16.04. This could also be a real machine you can ssh into.
* Configure ssh login for linux machine (not going to go into this, plenty of tutorials)
* Add machine ssh credentials to cmakesettings
* in launch.vs.json, your pipeArgs should look something like this:
```
"pipeArgs": [
          "/s",
          "${debugInfo.remoteMachineId}",
          "/p",
          "${debugInfo.parentProcessId}",
          "/c",
          "export DISPLAY=:0;${debuggerCommand}",
          "--tty=${debugInfo.tty}"
],
```
The export DISPLAY=:0; is a MUST or else you will get a segfault..
#### Configure linux machine for development
Install SDL2:
``` 
apt install libsdl2-dev  
apt install libsdl2-mixer-dev
apt install libsdl2-image-dev
```
Install libssl:
```
apt install libssl-dev
```
Now if you're unlucky like me, you probably need to update libssl and apt-get wont do the job. This awesome q/a will show you how to update libssl from it's source: https://askubuntu.com/questions/1102803/how-to-upgrade-openssl-1-1-0-to-1-1-1-in-ubuntu-18-04

Install protobuf:
```
apt install libprotobuf-dev protobuf-compiler
```
Install cmake:
```
apt install cmake
```
Install ninja:
```
apt install ninja-build
```
Install gamenetworkingsockets:
```
git clone 
cd GameNetworkingSockets
mkdir build
cd build
cmake -G Ninja ..
ninja
```

Optional but I move the output files into lib where sdl is
```
cd src
mv libGameNetworkingSockets_s.a /usr/lib/x86_64-linux-gnu
mv libGameNetworkingSockets.so /usr/lib/x86_64-linux-gnu
```

#### With that, you should have a dev environment running on Microsoft Visual Studio capable of compiling and debugging for Windows, and Linux. 

### Building on Linux (standalone)
I haven't tested building only on linux, however it should be easier to setup. Install everything mentioned [above](#configure-linux-machine-for-development)
* Git clone --recursive
* run cmake
* run make

Should be something like that. You will probably have to provide certain variables in the CMake config to make this work.

## Debuging
* Double check CMakeSettings like SDL2_DIR and GNS_LIB_DIR, they should both point to their library directories.
* The populate_all scripts need to be supplied with VsDevCmd.bat. The path for VsDevCmd.bat differs on each version of visual studio.
* Lots of trial and error. Building this first try with no errors seems unlikely.

## Built With
* Bullet3
* SDL2
* SDL_Mixer
* SDL_Image
* OpenGL
* GLM
* GLew
* GameNetworkingSockets
