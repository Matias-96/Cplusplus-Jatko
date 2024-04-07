//
// Created by Matias on 7.4.2024.
// miniprojekti 2

#ifndef UNTITLED_LOG_PTR_H
#define UNTITLED_LOG_PTR_H

#include <iostream>
#include <memory>
#include <ctime>
#include <mutex>
#include <cstring>

template<typename T>
class Log_Ptr {
private:
    T* ptr;
    int* ref_count;
    std::mutex mutex_;

public:
    explicit Log_Ptr(T* p);

    ~Log_Ptr();

    Log_Ptr(const Log_Ptr& other);

    Log_Ptr& operator=(const Log_Ptr& other);

    T* operator->();

    T& operator*();

private:
    std::string getCurrentTime();
};

// konstruktori, ota osoite viitatusta oliosta, lokiin aikaleima
template<typename T>
Log_Ptr<T>::Log_Ptr(T* p) : ptr(p), ref_count(new int(1)) {
    std::cout << getCurrentTime() << " omistajuus siirretty " << ptr << ", viitelaskuri " << *ref_count << std::endl;
}

// destruktori lokaa myös muistiosoitteen
template<typename T>
Log_Ptr<T>::~Log_Ptr() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (ptr) {
        (*ref_count)--;
        std::cout << getCurrentTime() << " olio tuhottu " << ptr << ", viitelaskuri " << *ref_count << std::endl;
        if (*ref_count == 0) {
            delete ptr;
            delete ref_count;
        }
    }
}

// Kopiointikonstruktori
template<typename T>
Log_Ptr<T>::Log_Ptr(const Log_Ptr& other) : ptr(other.ptr), ref_count(other.ref_count) {
    std::lock_guard<std::mutex> lock(mutex_);
    (*ref_count)++;
    std::cout << getCurrentTime() << " kopioitu, uusi viitelaskuri " << *ref_count << std::endl;
}

// Sijoitusoperaattori
template<typename T>
Log_Ptr<T>& Log_Ptr<T>::operator=(const Log_Ptr& other) {
    if (this != &other) {
        std::lock(mutex_, other.mutex_);
        std::lock_guard<std::mutex> self_lock(mutex_, std::adopt_lock);
        std::lock_guard<std::mutex> other_lock(other.mutex_, std::adopt_lock);

        if (ptr && --(*ref_count) == 0) {
            delete ptr;
            delete ref_count;
        }
        ptr = other.ptr;
        ref_count = other.ref_count;
        (*ref_count)++;
        std::cout << getCurrentTime() << " sijoitettu, uusi viitelaskuri " << *ref_count << std::endl;
    }
    return *this;
}

// overloadattu ->
template<typename T>
T* Log_Ptr<T>::operator->() {
    std::cout << getCurrentTime() << " operator-> " << ptr << std::endl;
    return ptr;
}

// overloadattu *
template<typename T>
T& Log_Ptr<T>::operator*() {
    std::cout << getCurrentTime() << " operator* " << ptr << std::endl;
    return *ptr;
}

// ota aikaleima
template<typename T>
std::string Log_Ptr<T>::getCurrentTime() {
    std::time_t now = std::time(nullptr);
    char* timeStr = std::ctime(&now);
    timeStr[std::strlen(timeStr) - 1] = '\0';
    return std::string(timeStr);
}

#endif //UNTITLED_LOG_PTR_H
