cmake_minimum_required(VERSION 3.21)
project(simple_ecs)

set(CMAKE_CXX_STANDARD 20)

file(GLOB src
        "src/*.h"
        "src/*.cpp"
        )

add_executable(simple_ecs ${src} src/Ecs.cpp src/Ecs.h src/ArcheType.cpp src/ArcheType.h src/ComponentArray.cpp src/ComponentArray.h src/types.h src/IComponentArray.cpp src/IComponentArray.h)
