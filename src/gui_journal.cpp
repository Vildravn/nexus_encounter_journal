#include "gui_journal.h"
#include "imgui/imgui.h"
#include "data/encounters.h"
#include "nlohmann/json.hpp"
#include "imgui_custom.h"
#include "shared.h"

using json = nlohmann::ordered_json;

json j_encounters = json::parse(str_encounters_json);

std::string selected_zone;
std::string selected_boss;
std::string selected_strike;
JournalSubmenu active_screen = JournalSubmenu::Main;

void ClearSelections()
{
	selected_boss.clear();
	selected_strike.clear();
	selected_zone.clear();
}

void RenderJournal()
{
	if (!ShowJournalWindow)
	{
		/*auto map_id_str = std::to_string(MumbleData->Context.MapID);
		if (j_encounters.contains(map_id_str)) selected_zone = map_id_str;
		else selected_zone.clear();*/

		return;
	}

	ImGui::SetNextWindowSize(ImVec2(820, 380), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(NexusLink->Width/2 - 820/2, NexusLink->Height/3 - 380/2), ImGuiCond_FirstUseEver); // Center window

	ImGui::Begin("Encounter Journal", &ShowJournalWindow, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoScrollbar + ImGuiWindowFlags_NoScrollWithMouse);
	
	switch (active_screen)
	{
		case JournalSubmenu::Main:
			RenderMainSubmenu();
			break;
		
		case JournalSubmenu::Raids:
			RenderRaidSubmenu();
			break;

		case JournalSubmenu::Strikes:
			RenderStrikeSubmenu();
	}

	ImGui::End();
}

void RenderMainSubmenu()
{
	ImGui::BeginTable("JournalTableMain", 4, ImGuiTableFlags_SizingStretchSame);
	ImGui::TableNextColumn();
	CustomSelectableCard([]() {}, "Open World", true);
	ImGui::TableNextColumn();
	CustomSelectableCard([]() {}, "Dungeons", true);
	ImGui::TableNextColumn();
	CustomSelectableCard([]() {}, "Fractals of the Mists", true);
	ImGui::TableNextColumn();
	CustomSelectableCard([]() { active_screen = JournalSubmenu::Raids; }, "Raids");
	ImGui::TableNextColumn();
	CustomSelectableCard([]() { active_screen = JournalSubmenu::Strikes; }, "Strike Missions");
	ImGui::EndTable();
}

void RenderRaidSubmenu()
{
	ImGui::BeginTable("JournalTableRaids", 2);
	ImGui::TableSetupColumn("Menu", ImGuiTableColumnFlags_WidthFixed, 270.0f);
	ImGui::TableSetupColumn("Content");

	ImGui::TableNextColumn();
	ImGui::BeginChild("Menu");
	if (!selected_zone.empty())
	{
		RenderJournalBossMenu();
	}
	else
	{
		RenderJournalZoneMenu("raid");
	}
	ImGui::EndChild();

	ImGui::TableNextColumn();
	ImGui::BeginChild("Content");
	if (!selected_zone.empty() && !selected_boss.empty())
	{
		if (j_encounters[selected_zone]["bosses"][selected_boss].contains("desc"))
		{
			std::string desc = j_encounters[selected_zone]["bosses"][selected_boss]["desc"];
			ImGui::TextWrapped("%s", desc.c_str());
		}
	}
	ImGui::EndChild();

	ImGui::EndTable();
}

void RenderStrikeSubmenu()
{
	ImGui::BeginTable("JournalTableStrikes", 2);
	ImGui::TableSetupColumn("Menu", ImGuiTableColumnFlags_WidthFixed, 270.0f);
	ImGui::TableSetupColumn("Content");

	ImGui::TableNextColumn();
	ImGui::BeginChild("Menu");
	if (selected_strike.empty())
	{
		CustomSelectable([]() { active_screen = JournalSubmenu::Main; }, "Back");
		CustomSelectable([]() { selected_strike = "ibs_strike"; }, "Icebrood Saga");
		CustomSelectable([]() { selected_strike = "eod_strike"; }, "End of Dragons");
		CustomSelectable([]() { selected_strike = "soto_strike"; }, "Secrets of the Obscure");
	}
	else
	{
		RenderJournalStrikeBossMenu(selected_strike);
	}
	ImGui::EndChild();

	ImGui::TableNextColumn();
	ImGui::BeginChild("Content");
	if (!selected_zone.empty() && !selected_boss.empty())
	{
		if (j_encounters[selected_zone]["bosses"][selected_boss].contains("desc"))
		{
			std::string desc = j_encounters[selected_zone]["bosses"][selected_boss]["desc"];
			ImGui::TextWrapped("%s", desc.c_str());
		}
	}
	ImGui::EndChild();

	ImGui::EndTable();
}

void RenderJournalZoneMenu(std::string type)
{
	CustomSelectable([]() { active_screen = JournalSubmenu::Main; }, "Back");
	for (auto j = j_encounters.begin(); j != j_encounters.end(); ++j)
	{
		auto zone_id = j.key();
		auto zone_name = j->at("name");
		std::string subtitle;
		if (j->contains("subtitle")) subtitle = j->at("subtitle");
		if (j->at("type") != type) continue;
		CustomSelectable([zone_id]() { selected_zone = zone_id; }, zone_name, false, subtitle);
	}
}

void RenderJournalBossMenu()
{
	CustomSelectable(ClearSelections, "Back");

	if (!selected_zone.empty())
	{
		auto bosses = j_encounters[selected_zone]["bosses"];
		for (auto j = bosses.begin(); j != bosses.end(); ++j)
		{
			auto boss_name = j.key();
			CustomSelectable([boss_name]() { selected_boss = boss_name; }, boss_name, selected_boss == boss_name);
		}
	}
}

void RenderJournalStrikeBossMenu(std::string type)
{
	CustomSelectable(ClearSelections, "Back");

	for (auto j = j_encounters.begin(); j != j_encounters.end(); ++j)
	{
		auto zone_id = j.key();
		std::string boss_name = j->at("name");
		std::string subtitle;
		if (j->contains("subtitle")) subtitle = j->at("subtitle");
		if (j->at("type") != type) continue;
		CustomSelectable([zone_id, boss_name]() { selected_zone = zone_id; selected_boss = boss_name; }, boss_name, selected_boss == boss_name, subtitle);
	}
}

void ToggleShowJournalWindow(const char* keybindIdentifier, bool isRelease)
{
	if (isRelease) return;
	ShowJournalWindow = !ShowJournalWindow;
}