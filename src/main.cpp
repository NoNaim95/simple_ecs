#include <iostream>
#include "Ecs.h"

using health = int;
using speed = float;
using weight = float;


int main() {
    Ecs ecs;


    ecs.makeEntity(100, 3.14f, 'c');


    return 0;
}
