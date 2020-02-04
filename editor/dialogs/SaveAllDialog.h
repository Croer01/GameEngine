//
// Created by adria on 03/02/2020.
//

#ifndef GAMEDEVWARS_SAVEALLDIALOG_H
#define GAMEDEVWARS_SAVEALLDIALOG_H


#include "../ProjectDirectory.h"
#include "BaseDialog.h"

class SaveAllDialog : public BaseDialog<int>
{
    std::vector<DataFile> filesToSave_;

public:
    explicit SaveAllDialog(const ConfirmCallback &callback);

    void setFilesToSave(const std::vector<DataFile> &files);
protected:
    void renderContent() override;
};


#endif //GAMEDEVWARS_SAVEALLDIALOG_H
