//
// Created by adria on 26/09/2018.
//

#ifndef SPACEINVADERS_FACTORY_HPP
#define SPACEINVADERS_FACTORY_HPP

#include <game-engine/api.hpp>
#include <unordered_map>
#include <memory>
#include <game-engine/internal/Data.hpp>
#include <game-engine/components/ComponentData.hpp>

namespace GameEngine {
namespace Internal {
    template<typename ReturnType, typename BuilderType, typename IdType>
    class PUBLICAPI Factory {
        std::unordered_map<IdType, BuilderType *> builders_;
    public:
        void AddBuilder(const IdType &type, BuilderType *builder) {
            auto it = builders_.find(type);

            if (it != builders_.end())
                throw std::runtime_error("builder already exist");

            builders_.insert(std::pair<IdType, BuilderType *>(type, builder));

        };

        void RemoveBuilder(const IdType &type) {
            auto it = builders_.find(type);

            if (it == builders_.end())
                throw std::runtime_error("builder doesn't exist");

            delete it->second;
            builders_.erase(type);
        };

        std::shared_ptr<ReturnType> Create(const IdType &type) {
            auto it = builders_.find(type);

            if (it == builders_.end())
                throw std::runtime_error("builder doesn't exist");

            return it->second->Create();
        };

        std::shared_ptr<ReturnType> Create(const IdType &type, const Data &data) {
            auto it = builders_.find(type);

            if (it == builders_.end())
                throw std::runtime_error("builder doesn't exist");

            return it->second->Create(data);
        };

        std::vector<IdType> getIds() const
        {
            std::vector<IdType> ids;
            for(const auto & property : builders_) {
                ids.push_back(property.first);
            }
            return ids;
        }

        ComponentDataRef createProperties(const IdType &type)
        {
            auto it = builders_.find(type);

            if (it == builders_.end())
                throw std::runtime_error("builder doesn't exist");

            return it->second->createProperties();
        }
    };
}
}
#endif //SPACEINVADERS_FACTORY_HPP
