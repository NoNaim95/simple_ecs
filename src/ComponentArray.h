//
// Created by deni on 05.08.22.
//

#pragma once

#include "IComponentArray.h"
#include <cstddef>
#include <memory>
#include <vector>

template<typename T>
class ComponentArray : public IComponentArray{
public:
    std::vector<T>& getArray(){return array;}
    const std::vector<T>& getArray() const{return array;}

    void transerComponent(IComponentArray& other, size_t idx) override;

    T& operator[](size_t idx);
    const T& operator[](size_t idx)const;

    void removeComponent(size_t index) override;
private:
    std::vector<T> array;
};

template<typename T>
void ComponentArray<T>::removeComponent(size_t index) {
    std::swap(array[index], array.back());
    array.pop_back();
}

template<typename T>
T& ComponentArray<T>::operator[](size_t idx) {
    return array[idx];
}

template<typename T>
const T& ComponentArray<T>::operator[](size_t idx) const {
    return array[idx];
}

template<typename T>
void ComponentArray<T>::transerComponent(IComponentArray& other, size_t idx) {
    static_cast<decltype(*this)>(other).array.push_back(this->array[idx]);
    removeComponent(idx);
}