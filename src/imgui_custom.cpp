#include "imgui_custom.h"
#include "imgui/imgui.h"
#include "nexus/Nexus.h"
#include "shared.h"
#include <format>

void CustomSelectable(std::function<void()> on_click, std::string label, bool selected , std::string subtitle, std::string imageEndpoint)
{
	if (ImGui::Selectable(("##" + label).c_str(), selected, 0, ImVec2(0, 32)))
	{
		on_click();
	}

    ImTextureID image = nullptr;
    if (!imageEndpoint.empty())
    {
        Texture* tex_from_url = APIDefs->Textures.GetOrCreateFromURL(std::format("TEX_{}", imageEndpoint).c_str(), "https://assets.gw2dat.com", imageEndpoint.c_str());

        if (tex_from_url != nullptr)
        {
            image = tex_from_url->Resource;
        }
        ImGui::SameLine();
        ImGui::Image(image, ImVec2(32, 32));
    }

	ImGui::SameLine();
	float pos_y = ImGui::GetCursorPosY();
	float label_y = subtitle.empty() ? 16.0f : 8.0f;
	ImGui::SetCursorPosY(pos_y + label_y - (ImGui::GetTextLineHeight() / 2));
	ImGui::Text("%s", label.c_str());
	if (!subtitle.empty())
	{
		float indent = image ? 48.0f : 8.0f;
		ImGui::SetCursorPosY(pos_y + 24.0f - (ImGui::GetTextLineHeight() / 2));
		ImGui::Indent(indent);
		ImGui::TextDisabled("%s", subtitle.c_str());
		ImGui::Unindent(indent);
	}
}

void CustomSelectableCard(std::function<void()> on_click, std::string label, bool disabled)
{
	float width = ImGui::GetColumnWidth(ImGui::GetColumnIndex());
	if (ImGui::Selectable(("##" + label).c_str(), false, disabled ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_None, ImVec2(width, 80)))
	{
		on_click();
	}
	ImGui::SameLine();
	ImVec2 pos = ImGui::GetCursorPos();
	ImVec2 text_size = ImGui::CalcTextSize(label.c_str());
	float center_x = pos.x - width / 2 - (text_size.x / 2);
	float center_y = pos.y + 40 - (text_size.y / 2);
	ImGui::SetCursorPos(ImVec2(center_x, center_y));
	if (disabled) ImGui::TextDisabled("%s", label.c_str());
	else ImGui::TextWrapped("%s", label.c_str());
}

void CustomSelectableBack(std::function<void()> on_click, std::string label)
{
	if (ImGui::Selectable(("##" + label).c_str(), false, 0, ImVec2(0, 32)))
	{
		on_click();
	}

	ImGui::SameLine();
	float sz = ImGui::GetFrameHeight();
	float pos_y = ImGui::GetCursorPosY();
	ImGui::SetCursorPosY(pos_y + 16.0f - (sz / 2) + 2);
	ImGui::ArrowButton("##BackBtn", ImGuiDir_Left);
	ImGui::SameLine();
	ImGui::SetCursorPosY(pos_y + 16.0f - (ImGui::GetTextLineHeight() / 2));
	ImGui::Text("%s", label.c_str());
}