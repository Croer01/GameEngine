//
// Created by adria on 26/01/2020.
//

#ifndef GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H
#define GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H


#include "BaseDialog.h"

class CreatePrototypeDialog : public BaseDialog<boost::filesystem::path>
{
    std::string name_;
    boost::filesystem::path basePath_;
    void updateData();
public:
    explicit CreatePrototypeDialog(const ConfirmCallback &callback);
    void open(const boost::filesystem::path &basePath, const std::string &defaultName);
    virtual ~CreatePrototypeDialog(){};
protected:
    void renderContent() override;
    bool canConfirm() override;
};


#endif //GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H
