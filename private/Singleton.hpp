//
// Created by adria on 22/09/2018.
//

#ifndef SPACEINVADERS_SINGLETON_HPP
#define SPACEINVADERS_SINGLETON_HPP

namespace GameEngine {
namespace Internal {
    template<typename T>
    class Singleton {
    public:
        static T &GetInstance() {
            static T *instance_;
            if (!instance_) {
                instance_ = new T();
                static_cast<Singleton<T>*>(instance_)->onCreateInstance();
            }
            return *instance_;
        }

        Singleton(const Singleton &) = delete;

        Singleton &operator=(const Singleton) = delete;

    protected:
        Singleton() {};

        virtual void onCreateInstance(){};

        virtual ~Singleton() {}
    };
}
}

#endif //SPACEINVADERS_SINGLETON_HPP
