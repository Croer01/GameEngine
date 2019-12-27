//
// Created by adria on 16/10/2019.
//

#ifndef GAMEENGINEEDITOR_CREATEPROJECTEDITOR_HPP
#define GAMEENGINEEDITOR_CREATEPROJECTEDITOR_HPP


#include <functional>
#include "ViewModels.hpp"

class CreateProjectEditor
{
public:
    typedef std::function<void(const std::shared_ptr<ProjectData>&)> ConfirmCallback;

private:
    bool opened_;
    bool canCreateProject_;
    std::shared_ptr<ProjectData> newProject_;
    ConfirmCallback confirmCallback_;

    void close();
    void confirm();
    void computeCanCreateProject();
public:
    explicit CreateProjectEditor(const ConfirmCallback &callback);
    void Render();
    void open();
    bool isOpen() const;
};


#endif //GAMEENGINEEDITOR_CREATEPROJECTEDITOR_HPP
