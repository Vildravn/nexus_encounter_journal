#ifndef NEXUS_ENCOUNTER_JOURNAL_IMGUI_CUSTOM_H
#define NEXUS_ENCOUNTER_JOURNAL_IMGUI_CUSTOM_H

#include <functional>
#include <string>

extern void CustomSelectable(std::function<void()> on_click, std::string label, bool selected = false, std::string subtitle = "");
extern void CustomSelectableCard(std::function<void()> on_click, std::string label, bool disabled = false);

#endif // !NEXUS_ENCOUNTER_JOURNAL_IMGUI_CUSTOM_H