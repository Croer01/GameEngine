//
// Created by adria on 16/10/2019.
//

#ifndef GAMEENGINEEDITOR_CREATEPROJECTEDITOR_HPP
#define GAMEENGINEEDITOR_CREATEPROJECTEDITOR_HPP


#include <functional>
#include "ViewModels.hpp"
#include "BaseDialog.h"

class CreateProjectEditor : public BaseDialog<ProjectDataRef>
{

private:
    bool canCreateProject_;

    void computeCanCreateProject();

protected:
    virtual void onConfirm();
    virtual void onOpen();
    virtual void renderContent();
    bool canConfirm() override;

public:
    explicit CreateProjectEditor(const ConfirmCallback &callback);
};


#endif //GAMEENGINEEDITOR_CREATEPROJECTEDITOR_HPP
