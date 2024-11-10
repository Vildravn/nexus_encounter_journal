#include "imgui_custom.h"
#include "imgui/imgui.h"

void CustomSelectable(std::function<void()> on_click, std::string label, bool selected , std::string subtitle)
{
	if (ImGui::Selectable(("##" + label).c_str(), selected, 0, ImVec2(0, 32)))
	{
		on_click();
	}
	ImGui::SameLine();
	float pos_y = ImGui::GetCursorPosY();
	float label_y = subtitle.empty() ? 16.0f : 8.0f;
	ImGui::SetCursorPosY(pos_y + label_y - (ImGui::GetTextLineHeight() / 2));
	ImGui::Text("%s", label.c_str());
	if (!subtitle.empty())
	{
		float indent = 8.0f;
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