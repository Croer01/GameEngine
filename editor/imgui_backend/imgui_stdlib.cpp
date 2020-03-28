// imgui_stdlib.cpp
// Wrappers for C++ standard library (STL) types (std::string, etc.)
// This is also an example of how you may wrap your own similar types.

// Compatibility:
// - std::string support is only guaranteed to work from C++11. 
//   If you try to use it pre-C++11, please share your findings (w/ info about compiler/architecture)

// Changelog:
// - v0.10: Initial version. Added InputText() / InputTextMultiline() calls with std::string

#include <cmath>
#include "imgui.h"
#include "imgui_stdlib.h"

struct InputTextCallback_UserData
{
    std::string*            Str;
    ImGuiInputTextCallback  ChainCallback;
    void*                   ChainCallbackUserData;
};

static int InputTextCallback(ImGuiInputTextCallbackData* data)
{
    InputTextCallback_UserData* user_data = (InputTextCallback_UserData*)data->UserData;
    if (data->EventFlag == ImGuiInputTextFlags_CallbackResize)
    {
        // Resize string callback
        // If for some reason we refuse the new length (BufTextLen) and/or capacity (BufSize) we need to set them back to what we want.
        std::string* str = user_data->Str;
        IM_ASSERT(data->Buf == str->c_str());
        str->resize(data->BufTextLen);
        data->Buf = (char*)str->c_str();
    }
    else if (user_data->ChainCallback)
    {
        // Forward to user callback, if any
        data->UserData = user_data->ChainCallbackUserData;
        return user_data->ChainCallback(data);
    }
    return 0;
}

bool ImGui::InputText(const char* label, std::string* str, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputText(label, (char*)str->c_str(), str->capacity() + 1, flags, InputTextCallback, &cb_user_data);
}

bool ImGui::InputTextMultiline(const char* label, std::string* str, const ImVec2& size, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback, void* user_data)
{
    IM_ASSERT((flags & ImGuiInputTextFlags_CallbackResize) == 0);
    flags |= ImGuiInputTextFlags_CallbackResize;

    InputTextCallback_UserData cb_user_data;
    cb_user_data.Str = str;
    cb_user_data.ChainCallback = callback;
    cb_user_data.ChainCallbackUserData = user_data;
    return InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1, size, flags, InputTextCallback, &cb_user_data);
}

// based on code example of this github's thread https://github.com/ocornut/imgui/issues/705#issuecomment-247959437
void ImGui::VerticalText(const char *text)
{
    ImFont *font = GetFont();
    const ImFont::Glyph *glyph;
    char c;
    ImGuiContext& g = *GetCurrentContext();
    const ImGuiStyle& style = GetStyle();
    float pad = style.FramePadding.x;
    ImVec2 text_size = CalcTextSize(text);
    ImVec2 pos = ImGui::GetCursorScreenPos();
    pos.x += pad;
    pos.y += text_size.x + pad;

    const  ImU32 text_color = ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_Text]);
    ImGui::PushID(text);
    while ((c = *text++)) {
        glyph = font->FindGlyph(c);
        if (!glyph) continue;

        ImGui::GetWindowDrawList()->PrimReserve(6, 4);
        ImGui::GetWindowDrawList()->PrimQuadUV(
            ImVec2(pos.x + glyph->Y0, pos.y -glyph->X0),
            ImVec2(pos.x + glyph->Y0, pos.y -glyph->X1),
            ImVec2(pos.x + glyph->Y1, pos.y -glyph->X1),
            ImVec2(pos.x + glyph->Y1, pos.y -glyph->X0),

            ImVec2(glyph->U0, glyph->V0),
            ImVec2(glyph->U1, glyph->V0),
            ImVec2(glyph->U1, glyph->V1),
            ImVec2(glyph->U0, glyph->V1),
            text_color);
        pos.y -= glyph->AdvanceX;
    }
    ImGui::PopID();

    // set the size of vertical text to avoid clipping over other content
    ImGui::Dummy(ImVec2(text_size.y + pad * 2,
                             text_size.x + pad * 2));
}

