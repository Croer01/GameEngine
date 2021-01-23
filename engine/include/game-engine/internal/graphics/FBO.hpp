//
// Created by adria on 12/04/2020.
//

#ifndef GAMEENGINE_FBO_HPP
#define GAMEENGINE_FBO_HPP

namespace GameEngine{
namespace Internal{
class FBO
{
    unsigned int fbo_;
    unsigned int rbo_;
    unsigned int textureAttatchment_;

public:
    FBO(int width, int height);
    ~FBO();

    void bind() const;
    void unBind() const;
    unsigned int getTextId() const;
};

}
}
#endif //GAMEENGINE_FBO_HPP
