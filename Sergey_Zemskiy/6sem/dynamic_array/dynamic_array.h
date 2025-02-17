
#ifndef DYNAMIC_ARRAY_DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_DYNAMIC_ARRAY_H

#include <iostream>

class Exception : std::domain_error {
public:
   Exception (const char* const message) : std::domain_error(message)  {};
};
Exception MEMORY ("Problems with allocating memory");
Exception NON_EXISTENT ("You try apply to non-existent element ");


template<typename T>
class Dynamic_array {
private:
    T* val;
    size_t size;
    size_t buffer_size;

public:
    Dynamic_array();
    Dynamic_array(size_t size);
    Dynamic_array(const Dynamic_array &other);
    ~Dynamic_array();

public:
    Dynamic_array<T> operator=(const Dynamic_array &other);
    T& operator[](size_t index);

public:
    size_t get_size();
    bool resize(size_t new_size);
};

template<typename T>
Dynamic_array<T>::Dynamic_array() {
    val = nullptr;
    size = 0;
    buffer_size = 0;
}

template<typename T>
Dynamic_array<T>::Dynamic_array(size_t size) {
    this->size = size;
    buffer_size = size / 2;
    val = new T[size + buffer_size];
    if (val == nullptr) throw MEMORY;
}

template<typename T>
Dynamic_array<T>::Dynamic_array(const Dynamic_array &other) {
    size = other.size;
    buffer_size = other.buffer_size;
    val = new T[size + buffer_size];
    if (val == nullptr) throw MEMORY;
    memcpy(val,other.val,sizeof(T) * (buffer_size + size));
}

template<typename T>
Dynamic_array<T>::~Dynamic_array() {
size = 0;
buffer_size = 0;
if (val != nullptr) delete[] val;
}

template<typename T>
Dynamic_array<T> Dynamic_array<T>::operator=(const Dynamic_array &other) {
    if(*this == other) return *this;
    size = other.size;
    buffer_size = other.buffer_size;
    delete[] val;
    val = new T[size + buffer_size];
    memcpy(val, other.val, (buffer_size + size) * sizeof (T) );
    return *this;
}

template<typename T>
T& Dynamic_array<T>::operator[](size_t index) {
    if (index > size) throw NON_EXISTENT;
    return val[index];
}

template<typename T>
size_t Dynamic_array<T>::get_size() {
    return size;
}

template<typename T>
bool Dynamic_array<T>::resize(size_t new_size) {
    if (new_size <= size + buffer_size) {
        buffer_size += (size - new_size);
        size = new_size;
        return true;
    }
    T* new_val = new T[new_size];
    if (new_val == nullptr) return false;
    memcpy(new_val, val, (buffer_size + size) * sizeof (T));
    delete[] val;
    val = new_val;
    size = new_size;
    buffer_size = new_size / 2;
    return true;
}


#endif //DYNAMIC_ARRAY_DYNAMIC_ARRAY_H
