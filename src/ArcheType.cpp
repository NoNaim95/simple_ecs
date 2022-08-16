//
// Created by deni on 05.08.22.
//

#include "ArcheType.h"
#include "ComponentArray.h"

template<typename... T>
ArcheType ArcheType::create() {
    auto archetype = ArcheType();
    archetype.setSig(Sig::createSig<T...>());
    ((archetype.arrays[TypeId<T>::id] = ComponentArray<T>()), ...);
    return archetype;
}

ArcheType ArcheType::create(Signature sig) {
    auto archetype = ArcheType();
    archetype.setSig(sig);

    ((archetype.arrays[TypeId<T>::id] = ComponentArray<T>()), ...);
    return archetype;
}

template<typename... T>
void ArcheType::addEntity(entKey entity, T&&... components) {
    entities.push_back(entity);
    indices[entities.size()] = entity;
    (static_cast<ComponentArray<T>>(arrays.at(TypeId<T>::id)).getArray().pushback(std::forward<T>(components)), ...);
}

void ArcheType::removeEntity(entKey entity) {
    for(auto & [id, array] : arrays)
        array.removeComponent(indices.at(entity));
    indices[entities.back()] = indices.at(entity);
    std::swap(entities[indices.at(entity)] , entities.back());
    entities.pop_back();
    indices.erase(entity);
}

void ArcheType::setSig(const Signature& sig) {
    this->sig = sig;
}

size_t ArcheType::getSize() {
    return entities.size();
}

const Signature& ArcheType::getSig() const {
    return sig;
}

template<typename... T>
std::tuple<T&& ...> ArcheType::getComponents(entKey entity) {
    return std::make_tuple((static_cast<ComponentArray<T>>(arrays.at(TypeId<T>::id))[indices.at(entity)], ...));
}

void ArcheType::transferEntity(entKey entity, ArcheType& other) {
    std::swap(entities[indices.at(entity)], entities.back());
    entities.pop_back();
    for(auto & [id, array] : arrays)
        array.transerComponent(other.arrays.at(id), indices.at(entity));
}

