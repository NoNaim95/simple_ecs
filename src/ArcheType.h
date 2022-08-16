//
// Created by deni on 05.08.22.
//

#pragma once
#include <vector>
#include <map>
#include <optional>
#include <queue>
#include "IComponentArray.h"
#include "types.h"

class ArcheType;

struct Record{
    ArcheType* archeType;
    size_t row;
};


class ArcheType {
public:
    template<typename... T>
    static ArcheType create();
    static ArcheType create(Signature sig);

    template<typename... T>
    std::tuple<T&&...> getComponents(entKey entity);

    void transferEntity(entKey entity, ArcheType& other);

    template<typename... T>
    void addEntity(entKey entity, T&&... components);

    void removeEntity(entKey entity);
    size_t getSize();

    [[nodiscard]] const Signature& getSig() const;
    void setSig(const Signature& sig);
private:
    Signature sig;
    std::map<componentId, IComponentArray&> arrays;
    std::vector<entKey> entities;
    std::map<entKey, size_t> indices;
};