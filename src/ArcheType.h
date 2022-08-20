//
// Created by deni on 05.08.22.
//

#pragma once
#include <vector>
#include <map>
#include <optional>
#include <queue>
#include <memory>

#include "ComponentArray.h"
#include "types.h"




class ArcheType {
public:
    explicit ArcheType(Signature sig): sig(sig){}
    ArcheType() = default;

    template<typename... T>
    static ArcheType create();

    template<typename... T>
    [[nodiscard]] std::tuple<T&...> getComponents(entKey entity);

    template<typename... T>
    [[nodiscard]] std::tuple<const T&...> getComponents(entKey entity) const;

    void transferEntity(entKey entity, ArcheType& other);

    template<typename... T>
    void addEntity(entKey entity, T&&... components);
    void addEntity(entKey entity);

    void removeEntity(entKey entity);

    [[nodiscard]] const Signature& getSig() const;
private:
    Signature sig;
    std::map<componentId, std::unique_ptr<IComponentArray>> arrays;
    std::vector<entKey> entities;
    std::map<entKey, size_t> indices;
};


template<typename... T>
ArcheType ArcheType::create() {
    auto archetype = ArcheType(Sig::createSig<T...>());
    ((archetype.arrays[TypeId<T>::id] = std::make_unique<ComponentArray<T>>()), ...);
    return archetype;
}


void ArcheType::addEntity(entKey entity) {
    entities.push_back(entity);
    indices[entities.size()] = entity;
}

template<typename... T>
void ArcheType::addEntity(entKey entity, T&&... components) {
    addEntity(entity);
    (static_cast<ComponentArray<T>*>(arrays.at(TypeId<T>::id).get())
    ->getArray().push_back(std::forward<T>(components)),
    ...);
}

void ArcheType::removeEntity(entKey entity) {
    for(const auto & [id, array] : arrays)
        array->removeComponent(indices.at(entity));
    indices[entities.back()] = indices.at(entity);
    std::swap(entities[indices.at(entity)] , entities.back());
    entities.pop_back();
    indices.erase(entity);
}

const Signature& ArcheType::getSig() const {
    return sig;
}

void ArcheType::transferEntity(entKey entity, ArcheType& other) {
    std::swap(entities[indices.at(entity)], entities.back());
    entities.pop_back();
    other.addEntity(entity);

    for(const auto & [id, array] : arrays)
        // create missing arrays and transfer the component to it
        if(other.sig.test(id)){
            if(!other.arrays.contains(id)) // array nonexistent in other array
                other.arrays[id] = array->emptyClone();
            array->transerComponent(*other.arrays.at(id), indices.at(entity));
        }

    indices.erase(entity);
}

template<typename... T>
std::tuple<T& ...> ArcheType::getComponents(entKey entity) {
    return std::tie((static_cast<ComponentArray<T>>(arrays.at(TypeId<T>::id))[indices.at(entity)], ...));
}

template<typename... T>
std::tuple<const T&...> ArcheType::getComponents(entKey entity) const {
    return std::tie((static_cast<ComponentArray<T>>(arrays.at(TypeId<T>::id))[indices.at(entity)], ...));
}