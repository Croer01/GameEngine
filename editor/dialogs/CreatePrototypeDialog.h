//
// Created by adria on 26/01/2020.
//

#ifndef GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H
#define GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H


#include "BaseDialog.h"

class CreatePrototypeDialog : public BaseDialog<std::string>
{
public:
    explicit CreatePrototypeDialog(const ConfirmCallback &callback);
    virtual ~CreatePrototypeDialog(){};
protected:
    void renderContent() override;
    bool canConfirm() override;
};


#endif //GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H
