//
// Created by adria on 26/09/2018.
//

#ifndef SPACEINVADERS_FACTORY_HPP
#define SPACEINVADERS_FACTORY_HPP

#include <unordered_map>
#include <memory>

template <typename ReturnType, typename BuilderType, typename IdType>
class Factory {
    std::unordered_map<IdType,BuilderType*> builders_;
public:
    void AddBuilder(IdType type, BuilderType* builder){
        auto it = builders_.find(type);

        if(it != builders_.end())
            throw new std::exception("builder already exist");

        builders_.insert(std::pair<IdType, BuilderType*>(type, builder));

    };
    void RemoveBuilder(IdType type){
        auto it = builders_.find(type);

        if(it == builders_.end())
            throw new std::exception("builder doesn't exist");

        delete it->second;
        builders_.erase(type);
    };
    std::shared_ptr<ReturnType> Create(IdType type){
        auto it = builders_.find(type);

        if(it == builders_.end())
            throw new std::exception("builder doesn't exist");

        return it->second->Create();
    };
};

#endif //SPACEINVADERS_FACTORY_HPP
