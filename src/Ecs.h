//
// Created by deni on 05.08.22.
//

#pragma once
#include <vector>
#include <map>
#include <queue>
#include <algorithm>

#include "ArcheType.h"
#include "types.h"


class Ecs {
public:
    template<typename... T>
    entKey makeEntity(T... components);


    void removeEntity(entKey entity);

    template<typename... T>
    void addComponents(entKey entity);

    template<typename... T>
    std::optional<std::tuple<T&&...>> getComponents(entKey entity);

    template<typename... T>
    void removeComponents(entKey entity);

private:
    template<typename... T>
    std::optional<std::reference_wrapper<ArcheType>> getArcheType();
    std::optional<std::reference_wrapper<ArcheType>> getArcheType(Signature sig);

private:
    std::vector<ArcheType> archetypes;

    std::queue<entKey>freeKeys;
    entKey currKey{};

    std::map<entKey, ArcheType&> homes;
};


template<typename... T>
entKey Ecs::makeEntity(T... components) {
    // determining if entkey will be taken from queue or currKey
    entKey entKey;
    if(!freeKeys.empty()){
        entKey = freeKeys.front();
        freeKeys.pop();
    }
    else{
        entKey = currKey++;
    }

    auto sig = createSig<T...>();
    auto archeTypeIt = std::find_if(archetypes.begin(), archetypes.end(),[sig](auto it){return it->getSig() == sig;});
    // determine if we can put this entity in an existing ArcheType or if we have to create a new one
    if(archeTypeIt != archetypes.end()){
        archeTypeIt->addEntity(entKey, components...);
    }else{
        archetypes.push_back(ArcheType::create<T...>());
        archetypes.back().addEntity(entKey, components...);
    }

    return entKey;
}

void Ecs::removeEntity(entKey entity) {
    homes.at(entity).removeEntity(entity);
}

template<typename... T>
void Ecs::addComponents(entKey entity) {
    auto oldSig = homes.at(entity).getSig();
    Signature newSig = Sig::createSig<T...>() | oldSig;
    if(auto arch = getArcheType(newSig))
        homes.at(entity).transferEntity(entity, *arch);
    else{
        ArcheType::create<>()
    }
}

template<typename... T>
std::optional<std::tuple<T&&...>> Ecs::getComponents(entKey entity) {
    if(auto archeType = getArcheType<T...>())
        return archeType->getComponents();
    return std::nullopt;
}

template<typename... T>
std::optional<std::reference_wrapper<ArcheType>> Ecs::getArcheType() {
    return getArcheType(Sig::createSig<T...>());
}

std::optional<std::reference_wrapper<ArcheType>> Ecs::getArcheType(Signature sig) {
    auto it = std::find_if(archetypes.begin(), archetypes.end(),[sig](auto arch){return arch.getSig() == sig;});
    if(it == archetypes.end())
        return std::nullopt;
    return *it;
}

template<typename... T>
void Ecs::removeComponents(entKey entity) {
}
