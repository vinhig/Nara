# Nara

Nara was a failed attempt to great a little rendering engine. Why *attempt*? Simply because it uses many things I had always wanted to try. Why *failed*? Failed isn't the good word actually, it's just that I need to learn 300% more things before creating something big and good.

## Characteristics

* Multi-threading (working)
* C++ 17 (std::thread, std::optional, ...)
* FBX Models (half working)
* ECS (working but no hierarchy)
* OpenGL 4.6 (DSA)
* Support for switching backend
    * Notion of `Loader`
    * Double level of API abstraction (half proud of it)
* Custom unit testing (not enough )

It doesn't feature any advanced rendering technics but a simple deferred rendering pipeline that just works.

## Building

### Windows

You can use `vcpkg` to include dependencies.

```
vpckg install glew:x64-windows
vpckg install glm:x64-windows
vpckg install glfw:x64-windows
```

Then you can generate and compile the project:

```
git clone https://github.com/vinhig/Nara
cd Nara
git submodule init
git submodule update
mkdir build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE="C:\path\to\your\vcpkg\scripts\buildsystems\vcpkg.cmake"
MSBuild.exe Nara.sln
cd ../
./build/Debug/Attempt.exe
```

### GNU/Linux

You'll need to fetch those dependencies:

* GLFW
* GLEW
* GLM

Then:

```
git clone https://github.com/vinhig/Nara
cd Nara
git submodule init
git submodule update
sh source/tools/build.sh
./build/Nara
```

## Extern Libraries

* https://github.com/nothings/stb (Load textures)
* https://github.com/nem0/OpenFBX (Load FBX)

## Purposes

As saied before, this attempt allowed me to understand maannnnyy concepts that I didn't know until then. To say that I became an expert would be a lie, but I'm more confident now. Can you C++? I *can*. Can you multi-thread? I *can*. Can you OpenGL DSA? I *can*.

## Why stop?

I don't have a lot of experience about game engine architecture and this project is a proof. I have to learn many things before calling a thousand of LoC a **usable** *game engine*. I noticed it when implementing SSAO requires too many tweaks and hacks.

## And now?

Virtually improving the `double level of API abstraction` will be the priority in the next weeks.
