#ifndef NEXUS_ENCOUNTER_JOURNAL_IMGUI_CUSTOM_H
#define NEXUS_ENCOUNTER_JOURNAL_IMGUI_CUSTOM_H

#include <functional>
#include <string>

extern void CustomSelectable(std::function<void()> on_click, std::string label, bool selected = false, std::string subtitle = "", std::string imageEndpoint = "");
extern void CustomSelectableCard(std::function<void()> on_click, std::string label, bool disabled = false);
extern void CustomSelectableBack(std::function<void()> on_click, std::string label);

#endif // !NEXUS_ENCOUNTER_JOURNAL_IMGUI_CUSTOM_H