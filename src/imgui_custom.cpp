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

void CustomSelectableCard(std::function<void()> on_click, const std::string label, bool disabled)
{
	float width = ImGui::GetColumnWidth(ImGui::GetColumnIndex());
	float height = 80.0f;
	if (ImGui::Selectable(("##" + label).c_str(), false, disabled ? ImGuiSelectableFlags_Disabled : ImGuiSelectableFlags_None, ImVec2(width, height)))
	{
		on_click();
	}

	ImVec2 itemRectMin = ImGui::GetItemRectMin();
	ImVec2 itemRectMax = ImGui::GetItemRectMax();
	ImVec2 itemSize = ImGui::GetItemRectSize();
	ImVec2 textSize = ImGui::CalcTextSize(label.c_str(), NULL, true, itemSize.x);
	float textPosX = itemRectMin.x + (itemSize.x - textSize.x) * 0.5f;
	float textPosY = itemRectMin.y + (itemSize.y - textSize.y) * 0.5f;
	ImGui::SetCursorScreenPos(ImVec2(textPosX, textPosY));
	ImGui::PushTextWrapPos(textPosX + itemSize.x);

	if (disabled) ImGui::TextDisabled("%s", label.c_str());
	else ImGui::TextWrapped("%s", label.c_str());

	ImGui::PopTextWrapPos();
	ImGui::SetCursorScreenPos(ImVec2(itemRectMin.x, itemRectMax.y));
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