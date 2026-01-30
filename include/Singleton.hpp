#pragma once

#include "Local/Game/SingletonHolder.hpp"

template<typename T>
class TKSingletonHolder {
public:
    static void init() {
        if (sInstance == nullptr) {
            sInstance = new T();
        }
    }

    inline static T* get() {
        return sInstance;
    }

private:
    static T* sInstance;
};

template<typename T>
T* TKSingletonHolder<T>::sInstance;

namespace TK {
    template<typename T>
    inline void initSingleton() {
        TKSingletonHolder<T>::init();
    };
    
    template<typename T>
    inline T* getSingleton() {
        return TKSingletonHolder<T>::get();
    };
}