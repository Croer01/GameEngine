//
// Created by adria on 26/01/2020.
//

#ifndef GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H
#define GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H


#include "BaseDialog.h"

class CreateDataFileDialog : public BaseDialog<boost::filesystem::path>
{
    std::vector<const char*> fileTypes_;
    int selectedFileType_;

    std::string name_;
    boost::filesystem::path dataPath_;
    boost::filesystem::path basePath_;
    void updateData();
public:
    explicit CreateDataFileDialog(const ConfirmCallback &callback);
    void open(const boost::filesystem::path &basePath, const std::string &defaultName);
    virtual ~CreateDataFileDialog(){};
    void setDataPath(const boost::filesystem::path &dataPath);
protected:
    void renderContent() override;
    bool canConfirm() override;
};


#endif //GAMEDEVWARS_CREATEPROTOTYPEDIALOG_H
