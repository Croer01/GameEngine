//
// Created by adria on 26/01/2020.
//

#ifndef GAMEDEVWARS_BASEDIALOG_H
#define GAMEDEVWARS_BASEDIALOG_H

#include <functional>
#include <memory>

template <typename ReturnType>
class BaseDialog
{
public:
    typedef std::function<void(const ReturnType &)> ConfirmCallback;

private:
    bool opened_;
    ConfirmCallback confirmCallback_;
protected:
    ReturnType data_;
    std::string dialogName_;
    std::string confirmButtonName_;
    std::string cancelButtonName_;

    virtual void renderContent() = 0;
    virtual void onOpen() {};
    virtual void onConfirm() {};
    virtual bool canConfirm();

    void close();
    void confirm();
public:
    explicit BaseDialog(const ConfirmCallback &callback);
    virtual ~BaseDialog(){}
    void Render();
    void open();
    bool isOpen() const;

};

template<typename ReturnType>
BaseDialog<ReturnType>::BaseDialog(const BaseDialog::ConfirmCallback &callback) :
        opened_(false),
        confirmCallback_(callback),
        dialogName_("Dialog"),
        confirmButtonName_("Confirm"),
        cancelButtonName_("Cancel")
{
}

template<typename ReturnType>
void BaseDialog<ReturnType>::Render()
{
    if(!opened_)
        return;

    if(!ImGui::IsPopupOpen(dialogName_.c_str()))
        ImGui::OpenPopup(dialogName_.c_str());

    if (ImGui::BeginPopupModal(dialogName_.c_str()))
    {
        renderContent();

        if(!canConfirm())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.6f);
            ImGui::Button(confirmButtonName_.c_str());
            ImGui::PopStyleVar();
        }
        else
        {
            if (ImGui::Button(confirmButtonName_.c_str()))
                confirm();
        }
        ImGui::SameLine();
        if (ImGui::Button(cancelButtonName_.c_str()))
            close();

        ImGui::EndPopup();
    }
}

template<typename ReturnType>
void BaseDialog<ReturnType>::open()
{
    opened_ = true;
    onOpen();
}

template<typename ReturnType>
void BaseDialog<ReturnType>::close()
{
    opened_ = false;
    ImGui::CloseCurrentPopup();
}

template<typename ReturnType>
void BaseDialog<ReturnType>::confirm()
{
    onConfirm();
    if(confirmCallback_)
        confirmCallback_(data_);

    close();
}

template<typename ReturnType>
bool BaseDialog<ReturnType>::isOpen() const
{
    return opened_;
}

template<typename ReturnType>
bool BaseDialog<ReturnType>::canConfirm()
{
    return true;
}

#endif //GAMEDEVWARS_BASEDIALOG_H
