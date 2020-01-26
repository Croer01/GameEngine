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

public:
    explicit CreateProjectEditor(const ConfirmCallback &callback);
    virtual ~CreateProjectEditor(){};
private:
    bool canCreateProject_;

    void computeCanCreateProject();

protected:
    virtual void onConfirm();
    virtual void onOpen();
    virtual void renderContent();
    bool canConfirm() override;
};


#endif //GAMEENGINEEDITOR_CREATEPROJECTEDITOR_HPP
