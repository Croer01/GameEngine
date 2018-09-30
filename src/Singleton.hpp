//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_SINGLETON_HPP
#define SPACEINVADERS_SINGLETON_HPP

template <typename T>
class Singleton {
public:
    static T& GetInstance() {
        static T * instance_;
        if (!instance_) {
            instance_ = new T();
        }
        return *instance_;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() { };
    virtual ~Singleton() { }
};


#endif //SPACEINVADERS_SINGLETON_HPP
