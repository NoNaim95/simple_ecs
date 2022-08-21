#include "Ecs.h"

using health = int;
using speed = float;
using weight = double;


int main() {
    Ecs ecs;

    std::vector<entKey> entities;
    entities.reserve(10);
    for(int i = 0; i < 10; i++){
        entities.push_back(ecs.makeEntity(health(i), speed(13.37)));
    }
    ecs.addComponents(1,weight(82.4));
    ecs.removeComponents<weight>(1);
    return 0;
}
