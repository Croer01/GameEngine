//
// Created by adria on 08/02/2020.
//

#ifndef GAMEDEVWARS_ERRORDIALOG_H
#define GAMEDEVWARS_ERRORDIALOG_H


#include <string>
#include "BaseDialog.h"

class ErrorDialog : public BaseDialog<std::string>
{
public:
    ErrorDialog();

protected:
    void renderContent() override;
};


#endif //GAMEDEVWARS_ERRORDIALOG_H
