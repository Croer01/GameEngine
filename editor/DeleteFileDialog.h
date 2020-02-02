//
// Created by adria on 27/01/2020.
//

#ifndef GAMEDEVWARS_DELETEFILEDIALOG_H
#define GAMEDEVWARS_DELETEFILEDIALOG_H

#include <boost/filesystem/path.hpp>
#include "BaseDialog.h"

class DeleteFileDialog : public BaseDialog<boost::filesystem::path>
{
public:
    explicit DeleteFileDialog(const ConfirmCallback &callback);

    virtual ~DeleteFileDialog() {};

protected:
    void renderContent() override;
};


#endif //GAMEDEVWARS_DELETEFILEDIALOG_H
