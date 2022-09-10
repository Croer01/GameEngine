//
// Created by adria on 09/02/2021.
//

#include <imgui.h>
#include "imgui_backend/imgui_stdlib.h"
#include "GameObjectEditor.hpp"
#include "DataFile.hpp"

void GameObjectEditor::update()
{
    if(!target_)
        return;

    ImGui::Begin("GameObject Editor");

    std::vector<std::weak_ptr<GameEngine::geComponent>> components = target_->getAllComponents();
    
    for( auto weakComp : components)
    {
        if(auto comp = weakComp.lock())
        {
            GameEngine::ComponentData *compData = comp->getData();
            for(auto it = compData->begin(); it != compData->end(); it++)
            {
                renderPropertyByType(*it);
            }
        }
    }
    ImGui::End();
}

bool GameObjectEditor::renderPropertyByType(const std::shared_ptr<GameEngine::PropertyBase> &property)
{
    assert(target_);
    bool edited = false;

    ImGui::PushID(property->name().c_str());
    switch (property->type())
    {
        case GameEngine::PropertyTypes::INT:
        {
            auto propertyInt = std::dynamic_pointer_cast<GameEngine::Property<int>>(property);
            int value = propertyInt->get();
            if(ImGui::DragInt(property->name().c_str(), &value, .1f))
            {
                propertyInt->set(value);
                edited = true;
            }
        }
            break;
        case GameEngine::PropertyTypes::FLOAT:
        {
            auto propertyFloat = std::dynamic_pointer_cast<GameEngine::Property<float>>(property);
            float value = propertyFloat->get();
            if(ImGui::DragFloat(property->name().c_str(), &value, .1f))
            {
                propertyFloat->set(value);
                edited = true;
            }
        }
            break;
        case GameEngine::PropertyTypes::STRING:
        {
            auto propertyString = std::dynamic_pointer_cast<GameEngine::Property<std::string>>(property);
            std::string value = propertyString->get();

            // Always include the first line
            float numLines = 1 + std::count(value.begin(), value.end(), '\n');
            // Arbitrary limit to avoid text block grows indefinitely
            if(numLines > 10)
                numLines = 10;
            if(ImGui::InputTextMultiline(propertyString->name().c_str(), &value, ImVec2(0, ImGui::GetFrameHeight() + ImGui::GetTextLineHeight() * numLines)))
            {
                propertyString->set(value);
                edited = true;
            }
        }
            break;
        case GameEngine::PropertyTypes::BOOL:
        {
            auto propertyBool = std::dynamic_pointer_cast<GameEngine::Property<bool>>(property);
            bool value = propertyBool->get();
            if(ImGui::Checkbox(property->name().c_str(), &value))
            {
                propertyBool->set(value);
                edited = true;
            }
        }
            break;
        case GameEngine::PropertyTypes::VEC2D:
        {
            auto propertyVec2D = std::dynamic_pointer_cast<GameEngine::Property<GameEngine::Vec2D>>(property);

            std::array<float,2> value = {propertyVec2D->get().x, propertyVec2D->get().y};

            if(property->name() == "screenPos" || property->name() == "screenSize")
                ImGui::DragFloat2(property->name().c_str(), value.data(),0.001f, .0f, 1.f);
            else
                ImGui::DragFloat2(property->name().c_str(), value.data());

            if(ImGui::IsItemEdited())
            {
                propertyVec2D->get().x = value[0];
                propertyVec2D->get().y = value[1];
                edited = true;
            }
        }
            break;
        case GameEngine::PropertyTypes::COLOR:
        {
            auto propertyColor = std::dynamic_pointer_cast<GameEngine::Property<GameEngine::geColor>>(property);

            std::array<float,3> value = {
                propertyColor->get().r,
                propertyColor->get().g,
                propertyColor->get().b
            };

            if(ImGui::ColorEdit3(property->name().c_str(), value.data()))
            {
                propertyColor->get().r = value[0];
                propertyColor->get().g = value[1];
                propertyColor->get().b = value[2];
                edited = true;
            }
        }
            break;
        case GameEngine::PropertyTypes::ARRAY_STRING:
        {
            auto propertyStringArray = std::dynamic_pointer_cast<GameEngine::Property<std::vector<std::string>>>(property);

            ImGui::Text("%s", propertyStringArray->name().c_str());
            ImGui::SameLine();
            if(ImGui::Button("+"))
            {
                propertyStringArray->get().emplace_back("");
                edited = true;
            }

            auto &values = propertyStringArray->get();
            for (int i = 0; i < values.size(); ++i)
            {
                ImGui::PushID(i);
                ImGui::InputText("", &values[i]);
                if(ImGui::IsItemEdited())
                    edited = true;
                ImGui::PopID();
            }
        }
            break;
        case GameEngine::PropertyTypes::ARRAY_VEC2D:
        {
            auto propertyVec2DArray = std::dynamic_pointer_cast<GameEngine::Property<std::vector<GameEngine::Vec2D>>>(property);

            ImGui::Text("%s", propertyVec2DArray->name().c_str());
            ImGui::SameLine();
            if(ImGui::Button("+"))
            {
                propertyVec2DArray->get().emplace_back();
                edited = true;
            }

            auto &values = propertyVec2DArray->get();
            for (int i = 0; i < values.size(); ++i)
            {
                ImGui::PushID(i);
                std::array<float,2> value = {values[i].x, values[i].y};
                if(ImGui::DragFloat2("", value.data()))
                {
                    values[i].x = value[0];
                    values[i].y = value[1];
                    edited = true;
                }
                ImGui::PopID();
            }
        }
            break;
        case GameEngine::PropertyTypes::FILEPATH:
        {
            auto propertyFilePath = std::dynamic_pointer_cast<GameEngine::PropertyFilePath>(property);
            DataFileRef fileSelected;
            DataFileType fileType = DataFileType::Other;
            switch (propertyFilePath->getFileType())
            {
                case GameEngine::FileType::OTHER:
                    fileType = DataFileType::Other;
                    break;
                case GameEngine::FileType::PROTOTYPE:
                    fileType = DataFileType::Prototype;
                    break;
                case GameEngine::FileType::SCENE:
                    fileType = DataFileType::Scene;
                    break;
                case GameEngine::FileType::IMAGE:
                    fileType = DataFileType::Image;
                    break;
            }
            if(renderFileSelector(propertyFilePath->name().c_str(),
                                  propertyFilePath->get(),
                                  fileType,
                                  fileSelected))
            {
                propertyFilePath->set(fileSelected->getFullPath().string());
                edited = true;
            }
        }
            break;
        case GameEngine::PropertyTypes::ENUM:
        {
            auto propertyEnum = std::dynamic_pointer_cast<GameEngine::PropertyEnum>(property);

            std::string newValue;
            if (ImGui::BeginCombo(propertyEnum->name().c_str(), propertyEnum->get().c_str()))
            {
                for(const auto &item : propertyEnum->getAllowedValues())
                {
                    bool is_selected = (propertyEnum->get() == item);
                    if (ImGui::Selectable(item.c_str(), is_selected))
                    {
                        newValue = item;
                    }
                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            if(!newValue.empty() && propertyEnum->get() != newValue)
            {
                propertyEnum->set(newValue);
                edited = true;
            }
        }
            break;
    }
    ImGui::PopID();

    return edited;
}

bool GameObjectEditor::renderFileSelector(const std::string &label, const std::string &currentFile, DataFileType typeSupported, DataFileRef &result)
{
    bool selected = false;
    if (ImGui::BeginCombo(label.c_str(), currentFile.c_str()))
    {
        std::vector<DataFileRef> files = root_->filterFilesByType(typeSupported);
        for(auto item : files)
        {
            const std::string &itemFilepath = item->getFullPath().string();
            bool is_selected = (currentFile == itemFilepath);
            if (ImGui::Selectable(itemFilepath.c_str(), is_selected))
            {
                result = item;
                selected = true;
            }
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    return selected;
}

void GameObjectEditor::setRoot(const boost::filesystem::path &directory)
{
    root_ = std::make_shared<DataDirectory>(directory);
}

void GameObjectEditor::setTarget(const GameEngine::geGameObjectRef &target)
{
    target_ = std::dynamic_pointer_cast<GameEngine::Internal::GameObject>(target);
}

void
GameObjectEditor::onEvent(const GameEngine::Subject<FileExplorerEvent, const GameEngine::geGameObjectRef &> &target,
                          FileExplorerEvent, const GameEngine::geGameObjectRef &goTarget)
{
    setTarget(goTarget);
}
