//
// Created by user on 08.02.23.
//

#include "Array.h"


template<typename T>
Array<T>::Array(uint64_t len, T number) {
    length = len;
    data = new T[length];
    for (int idx = 0; idx < length; idx++) {
        data[idx] = number;
    }
}

/*
template<typename T>
Array<T>::Array(T Array[]) {
    T *a = Array;
    length = sizeof(*a) / sizeof(a[0]);
}
*/

template<typename T>
Array<T>::~Array() {
    delete[] data;
}


template<typename T>
Array<T>::Array(const Array<T> &other) {
    length = other.length;
    data = new T[length];
    memcpy(data, other.data, sizeof(T) * length);
}


template<typename T>
Array<T>::Array(Array<T> &&other) noexcept {
    length = other.length;
    data = other.data;
    other.data = nullptr;
}


template<typename T>
T Array<T>::operator[](uint64_t index) {
    return data[index];
}


template<typename T>
Array<T>& Array<T>::operator=(const Array<T> &other) {
    if (this == &other) {
        return *this;
    }
    length = other.length;
    delete[] data;
    data = new T[length];
    std::memcpy(data, other.data, sizeof(T) * length);
    return *this;
}


template<typename T>
Array<T>& Array<T>::operator=(Array<T> &&other) noexcept {
    if (this == &other) {
        return *this;
    }
    length = other.length;
    delete[] data;
    data = other.data;
    other.data = nullptr;
    return *this;
}


template<typename T>
Array<T> Array<T>::operator+(const Array<T> &other) {
    if (length == 0) {
        if (other.length == 0) {
            return Array<T>(0);
        }
        else {
            return other;
        }
    }
    else {
        if (other.length == 0) {
            return *this;
        }
    }
    Array<T> result(length + other.length);
    memcpy(result.data, data, sizeof(T) * length);
    memcpy(result.data + length, other.data, sizeof(T) * other.length);
    return result;
}


template<typename T>
uint64_t Array<T>::len() {
    return length;
}


template<typename T>
void Array<T>::print() {
    if (length == 0) {
        return;
    }
    std::cout << "[";
    for (int idx = 0; idx < length - 1; idx++) {
        std::cout << data[idx] << ", ";
    }
    std::cout << data[length-1] << "]" << std::endl;
}


template<typename T>
void Array<T>::setData(uint64_t index, T object) {
    if (index >= length) {
        throw ArrayException("error: bad index");
    }
    data[index] = object;
}


template<typename T>
void Array<T>::append(T object) {
    length++;
    T* buffer = data;
    data = new T[length];
    std::memcpy(data, buffer, sizeof(T) * (length - 1));
    data[length-1] = object;
    delete[] buffer;
}


template<typename T>
void Array<T>::pop() {
    length--;
    T* buffer = data;
    data = new T[length];
    std::memcpy(data, buffer, sizeof(T) * length);
    delete[] buffer;
}


template<typename T>
void Array<T>::pop(uint64_t index) {
    if(index >= length) {
        throw ArrayException("error: bad index");
    }
    length--;
    T* buffer = data;
    data = new T[length];
    std::memcpy(data, buffer, sizeof(T) * index);
    for (int idx = length; idx > index; idx--) {
        data[idx-1] = buffer[idx];
    }
    delete[] buffer;
}


template<typename T>
void Array<T>::clear() {
    length = 0;
    delete[] data;
    data = nullptr;
}


template<typename T>
void Array<T>::quickSort() {
    if (length < 2) {
        return;
    }
    T barrier = data[length / 2];
    uint64_t equal_cnt = 0;
    Array<T> left(0);
    uint64_t middle_cnt = 0;
    Array<T> right(0);
    for (int idx = 0; idx < length; idx++) {
        if (data[idx] < barrier) {
            left.append(data[idx]);
        }
        if (data[idx] == barrier) {
            middle_cnt++;
        }
        if (data[idx] > barrier) {
            right.append(data[idx]);
        }
    }
    left.quickSort();
    right.quickSort();
    *this = left + Array<T>(middle_cnt, barrier) + right;
}


template
class Array<double>;

template
class Array<int>;
