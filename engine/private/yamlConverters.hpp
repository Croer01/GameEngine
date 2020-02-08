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

            rhs.x = node[0].as<float>(0.f);
            rhs.y = node[1].as<float>(0.f);
            return true;
        }
    };

template<>
struct convert<GameEngine::geColor> {
    static Node encode(const GameEngine::geColor &rhs) {
        Node node;
        node.push_back(rhs.r);
        node.push_back(rhs.g);
        node.push_back(rhs.b);
        return node;
    }

    static bool decode(const Node &node, GameEngine::geColor &rhs) {
        if (!node.IsSequence() || node.size() != 3) {
            return false;
        }

        rhs.r = node[0].as<float>(0.f);
        rhs.g = node[1].as<float>(0.f);
        rhs.b = node[1].as<float>(0.f);
        return true;
    }
};

template<>
struct convert<GameEngine::FilePath> {
    static Node encode(const GameEngine::FilePath &rhs) {
        Node node;
        node["path"] = rhs.path;
        if(rhs.fileType == GameEngine::FileType::IMAGE)
            node["type"] = "IMAGE";
        else
            node["type"] = "OTHER";
        return node;
    }

    static bool decode(const Node &node, GameEngine::FilePath &rhs) {
        if (!node.IsMap() || node.size() != 2) {
            return false;
        }

        rhs.path = node["path"].as<std::string>("");

        std::string fileType = node["type"].as<std::string>("");
        if(fileType == "IMAGE")
            rhs.fileType = GameEngine::FileType::IMAGE;
        else
            rhs.fileType = GameEngine::FileType::OTHER;

        return true;
    }
};
}

#endif //SPACEINVADERS_YAMLCONVERTERS_HPP
