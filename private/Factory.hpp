//
// Created by adria on 26/09/2018.
//

#ifndef SPACEINVADERS_FACTORY_HPP
#define SPACEINVADERS_FACTORY_HPP

#include <unordered_map>
#include <memory>
#include <yaml-cpp/node/node.h>
#include "Data.hpp"

namespace GameEngine {
namespace Internal {
    template<typename ReturnType, typename BuilderType, typename IdType>
    class Factory {
        std::unordered_map<IdType, BuilderType *> builders_;
    public:
        void AddBuilder(IdType type, BuilderType *builder) {
            auto it = builders_.find(type);

            if (it != builders_.end())
                throw std::runtime_error("builder already exist");

            builders_.insert(std::pair<IdType, BuilderType *>(type, builder));

        };

        void RemoveBuilder(IdType type) {
            auto it = builders_.find(type);

            if (it == builders_.end())
                throw std::runtime_error("builder doesn't exist");

            delete it->second;
            builders_.erase(type);
        };

        std::shared_ptr<ReturnType> Create(IdType type, const YAML::Node &data) {
            auto it = builders_.find(type);

            if (it == builders_.end())
                throw std::runtime_error("builder doesn't exist");

            return it->second->Create(Data(data));
        };
    };
}
}
#endif //SPACEINVADERS_FACTORY_HPP
