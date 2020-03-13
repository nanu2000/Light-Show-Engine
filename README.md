# Light-Show-Boilerplate
LightShow is a 3D boilerplate created for my own projects with the purpose of flexibility, scalability, and simplicity.

This application uses two other projects for 3D model and font atlas file creation that are currently private.

## Features
* 3D skeletal Animation
* Directional and Omnidirectional Shadow Mapping
* Physics, Collision, Callbacks and Triggers via Bullet3
* Audio and Music Support
* Text rendering
* Point and Directional lights
* Particle Systems via Instanced Rendering
* Bone Colliders (updates scale, position and rotation of a collision object according to a bone)
* Entity Component System structure

![LSBP-1](https://raw.githubusercontent.com/nanu2000/Light-Show-Boilerplate/master/docs/images/lsbp-1.png)
![LSBP-2](https://raw.githubusercontent.com/nanu2000/Light-Show-Boilerplate/master/docs/images/lsbp-2.png)

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
This WILL take a while. 

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
The export DISPLAY=:0; is a MUST or else you will get a segfault. Additionally, if you are using a linux VM you may have to prepend
```export MESA_GL_VERSION_OVERRIDE=3.3;``` to your pipeArgs after ```export DISPLAY=:0;```.

Verify the configuration name in your launch.vs.json file matches your cmake executable name.

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
You will most likely need to update libssl because apt-get wont do the job. This awesome q/a will show you how to update libssl from it's source: https://askubuntu.com/questions/1102803/how-to-upgrade-openssl-1-1-0-to-1-1-1-in-ubuntu-18-04

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

Optional but I move the output files into lib
```
cd src
mv libGameNetworkingSockets_s.a /usr/lib
mv libGameNetworkingSockets.so /usr/lib
```

#### With that, you should have a dev environment running on Microsoft Visual Studio capable of compiling and debugging for Windows, and Linux. 

### Building on Linux (standalone)
I haven't tested building only on linux, however it should be easier to setup. Install everything mentioned [above](#configure-linux-machine-for-development)
then run:
* git clone --recursive
* run cmake
* run make

You will need to provide certain variables in the CMake config to make this work.

## Debuging Build System
* Double check CMakeSettings like SDL2_DIR and GNS_LIB_DIR, they should both point to their library directories.
* In Windows, the populate_all scripts need to be supplied with VsDevCmd.bat. The path for VsDevCmd.bat differs on each version of visual studio.

## Built With
* Bullet3
* SDL2
* SDL_Mixer
* SDL_Image
* OpenGL
* GLM
* GLew
* GameNetworkingSockets
