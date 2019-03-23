//
// Created by adria on 17/03/2019.
//

#ifndef SPACEINVADERS_YAMLCONVERTERS_HPP
#define SPACEINVADERS_YAMLCONVERTERS_HPP

namespace YAML {
    template<>
    struct convert<GameEngine::Vec2D> {
        static Node encode(const GameEngine::Vec2D &rhs) {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            return node;
        }

        static bool decode(const Node &node, GameEngine::Vec2D &rhs) {
            if (!node.IsSequence() || node.size() != 2) {
                return false;
            }

            rhs.x = node[0].as<float>(0);
            rhs.y = node[1].as<float>(0);
            return true;
        }
    };
}

#endif //SPACEINVADERS_YAMLCONVERTERS_HPP
