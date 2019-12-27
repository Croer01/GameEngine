//
// Created by adria on 24/08/2019.
//

#ifndef GAMEENGINEEDITOR_COMMAND_HPP
#define GAMEENGINEEDITOR_COMMAND_HPP

#include <game-engine/api.hpp>

namespace GameEngine {
class Command;
typedef std::shared_ptr<Command> CommandRef;

class PUBLICAPI Command
{
public:
    virtual ~Command() {};
    virtual void execute() = 0;
};
}
#endif //GAMEENGINEEDITOR_COMMAND_HPP
