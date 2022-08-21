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
    entKey makeEntity(T&&... components);


    void removeEntity(entKey entity);

    template<typename... T>
    void addComponents(entKey entity, T&& ... components);

    template<typename... T>
    std::optional<std::tuple<T&...>> getComponents(entKey entity);

    template<typename... T>
    void removeComponents(entKey entity);

private:
    template<typename... T>
    std::optional<std::reference_wrapper<ArcheType>> getArcheType();
    std::optional<std::reference_wrapper<ArcheType>> getArcheType(const Signature& sig);

private:
    std::map<Signature, ArcheType, Sig::Comparer> archetypes;

    std::queue<entKey>freeKeys;
    entKey currKey{};

    std::map<entKey, std::reference_wrapper<ArcheType>> homes;
};



template<typename... T>
entKey Ecs::makeEntity(T&&... components) {
    // determining if entKey will be taken from queue or currKey
    entKey entKey;
    if(!freeKeys.empty()){
        entKey = freeKeys.front();
        freeKeys.pop();
    }
    else{
        entKey = currKey++;
    }

    auto sig = Sig::createSig<T...>();

    if(auto it = archetypes.find(sig); it != archetypes.end())
        it->second.addEntity(entKey, std::forward<T>(components)...);
    else{
        archetypes[sig] = ArcheType::create<T...>();
        archetypes.at(sig).addEntity(entKey, std::forward<T>(components)...);
    }

    return entKey;
}

void Ecs::removeEntity(entKey entity) {
    homes.at(entity).get().removeEntity(entity);
    homes.erase(entity);
}

template<typename... T>
void Ecs::addComponents(entKey entity, T&& ... components) {

    Signature newSig = Sig::createSig<T...>() | homes.at(entity).get().getSig();

    if(auto arch = getArcheType(newSig))
        // Archetype found, use it
        homes.at(entity).get().transferEntity(entity, *arch);
    else{
        // Archetype not found, create one and use it

        ArcheType arche = ArcheType::create<T...>();
        arche.addEntity(components...);

        archetypes[newSig] = arche;
        arche.transferEntity(entity, *arch);
        homes[entity] = arche;
    }
}

template<typename... T>
std::optional<std::tuple<T&...>> Ecs::getComponents(entKey entity) {
    if(auto archeType = getArcheType<T...>())
        return archeType->get().getComponents(entity);
    return std::nullopt;
}

template<typename... T>
std::optional<std::reference_wrapper<ArcheType>> Ecs::getArcheType() {
    return getArcheType(Sig::createSig<T...>());
}

std::optional<std::reference_wrapper<ArcheType>> Ecs::getArcheType(const Signature& sig) {
    if(auto it = archetypes.find(sig); it != archetypes.end())
        return std::ref(it->second);
    return std::nullopt;
}

template<typename... T>
void Ecs::removeComponents(entKey entity) {
    Signature newSig = homes.at(entity).get().getSig();
    (newSig.reset(TypeId<T>::id), ...);
    if(auto arch = getArcheType(newSig))
        // Archetype found, use it
        homes.at(entity).get().transferEntity(entity, *arch);
    else{
        // Archetype not found, create one and use it

        ArcheType arche = ArcheType(newSig);
        archetypes[newSig] = arche;
        arche.transferEntity(entity, *arch);
        homes[entity] = arche;
    }
}