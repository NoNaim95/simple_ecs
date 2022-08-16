#include <iostream>

int main() {

    auto entity1 = ecs::makeEntity<FooComp, BooComp, BarComp>();
    ecs::removeComponent<BooComp>(entity1);
    ecs::removeComponent<BazComp>(entity1);
    ecs::hasComponent<BooComp>(entity1);
    ecs::getComponent<BooComp>(entity1);
    ecs::removeEntity(entity1);

    ecs::getComponents<BooComp>(Signature::from<FooComp, BooComp, BarComp>());

    {
        //ecs::getComponents<BooComp>(Signature::from<FooComp, BooComp, BarComp>());
        //ecs::getComponents<BarComp>(Signature::from<FooComp, BooComp, BarComp>());


        auto [barIt, fooIt] = ecs::getComponent<BarComp, FooComp>();
    }

    {
        ecs::getEntity()


    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
