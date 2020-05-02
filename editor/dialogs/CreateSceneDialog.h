//
// Created by adria on 02/05/2020.
//

#ifndef GAMEENGINE_CREATESCENEDIALOG_H
#define GAMEENGINE_CREATESCENEDIALOG_H


#include "BaseDialog.h"

class CreateSceneDialog : public BaseDialog<std::string>
{
public:
    explicit CreateSceneDialog(const ConfirmCallback &callback);
    virtual ~CreateSceneDialog(){};
protected:
    void renderContent() override;
    bool canConfirm() override;
};


#endif //GAMEENGINE_CREATESCENEDIALOG_H
