#include <functional>
#include <string>
#include <windows.h>

#include "mumble/Mumble.h"
#include "nexus/Nexus.h"
#include "imgui/imgui.h"
#include "nlohmann/json.hpp"

/* proto */
void AddonLoad(AddonAPI* aApi);
void AddonUnload();
void AddonRender();
void AddonOptions();

/* Move */
void ClearSelections();
void RenderJournal();
void RenderJournalZoneMenu(std::string type);
void RenderJournalBossMenu();
void RenderJournalStrikeBossMenu(std::string type);
void RenderMainSubmenu();
void RenderRaidSubmenu();
void RenderStrikeSubmenu();
void ToggleShowJournalWindow(const char* keybindIdentifier, bool isRelease);
void CustomSelectable(std::function<void()> on_click, std::string label, bool selected = false, std::string subtitle = "");
void CustomSelectableCard(std::function<void()> on_click, std::string label, bool disabled = false);
std::string selected_zone;
std::string selected_boss;
std::string selected_strike;
bool ShowJournalWindow = true;
const char* KB_TOGGLE_SHOW_WINDOW_JOURNAL = "KB_TOGGLE_SHOW_WINDOW_JOURNAL";

enum JournalSubmenu
{
	Main,
	OpenWorld,
	Dungeons,
	Fractals,
	Raids,
	Strikes
};

JournalSubmenu active_screen;

using json = nlohmann::ordered_json;

/* Temp */
inline std::string tmp_encounters_json = R"###({
	"1062": {
		"name": "Spirit Vale",
		"subtitle": "Wing 1",
		"type": "raid",
		"bosses": {
			"Vale Guardian": {
				"desc": "**Health:** 22 021 440\n**Enrage Timer:** 8 minutes\n**Enrage Effect:** Gains ![Enraged](/961403.png) [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%\n**Fixation Mechanics:** Highest toughness\n\n"
			},
			"Gorseval": {
				"desc": "**Health:** 21 628 200\n**Enrage Timer**: 7 minute\n**Enrage Effect:** Removes all updrafts and gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%.\n**Fixation Mechanics:** Highest toughness\n\n"
			},
			"Sabetha": {}
		}
	},
	"1149": {
		"name": "Salvation Pass",
		"subtitle": "Wing 2",
		"type": "raid",
		"bosses": {
			"Slothasor": {},
			"Prison Camp": {},
			"Matthias": {}
		}
	},
	"1332": {
		"name": "Icebrood Construct",
		"subtitle": "Shiverpeaks Pass",
		"type": "ibs_strike",
		"bosses": {
			"Icebrood Construct": {
				"desc": "**Health**: 11 698 890"
			}
		}
	},
	"1432": {
		"name": "Mai Trin",
		"subtitle": "Aetherblade Hideout",
		"type": "eod_strike",
		"bosses": {
			"Mai Trin": {
				"desc": "**Health**: 5 898 600"
			}
		}
	}
})###";

json j_encounters = json::parse(tmp_encounters_json);

/* globals */
AddonDefinition AddonDef	= {};
HMODULE hSelf				= nullptr;
AddonAPI* APIDefs			= nullptr;
const char* ADDON_NAME 		= "EncounterJournal";
NexusLinkData* NexusLink	= nullptr;
Mumble::Data* MumbleData	= nullptr;

///----------------------------------------------------------------------------------------------------
/// DllMain:
/// 	Main entry point for DLL.
/// 	We are not interested in this, all we get is our own HMODULE in case we need it.
///----------------------------------------------------------------------------------------------------
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH: hSelf = hModule; break;
		case DLL_PROCESS_DETACH: break;
		case DLL_THREAD_ATTACH: break;
		case DLL_THREAD_DETACH: break;
	}
	return TRUE;
}

///----------------------------------------------------------------------------------------------------
/// GetAddonDef:
/// 	Export needed to give Nexus information about the addon.
///----------------------------------------------------------------------------------------------------
extern "C" __declspec(dllexport) AddonDefinition* GetAddonDef()
{
	AddonDef.Signature = -89350; // set to random unused negative integer
	AddonDef.APIVersion = NEXUS_API_VERSION;
	AddonDef.Name = "Encounter Journal (Beta)";
	AddonDef.Version.Major = 0;
	AddonDef.Version.Minor = 0;
	AddonDef.Version.Build = 0;
	AddonDef.Version.Revision = 1;
	AddonDef.Author = "Ravenheart.8935";
	AddonDef.Description = "Cliffnotes about dungeon, strike and raid bosses";
	AddonDef.Load = AddonLoad;
	AddonDef.Unload = AddonUnload;
	AddonDef.Flags = EAddonFlags_None;

	return &AddonDef;
}

///----------------------------------------------------------------------------------------------------
/// AddonLoad:
/// 	Load function for the addon, will receive a pointer to the API.
/// 	(You probably want to store it.)
///----------------------------------------------------------------------------------------------------
void AddonLoad(AddonAPI* aApi)
{
	APIDefs = aApi; // store the api somewhere easily accessible

	ImGui::SetCurrentContext((ImGuiContext*)APIDefs->ImguiContext); // cast to ImGuiContext*
	ImGui::SetAllocatorFunctions((void* (*)(size_t, void*))APIDefs->ImguiMalloc, (void(*)(void*, void*))APIDefs->ImguiFree); // on imgui 1.80+

	NexusLink = (NexusLinkData*)APIDefs->DataLink.Get("DL_NEXUS_LINK");
	MumbleData = (Mumble::Data*)APIDefs->DataLink.Get("DL_MUMBLE_LINK");

	// Add an options window and a regular render callback
	APIDefs->Renderer.Register(ERenderType_Render, AddonRender);
	APIDefs->Renderer.Register(ERenderType_OptionsRender, AddonOptions);

	APIDefs->InputBinds.RegisterWithString(KB_TOGGLE_SHOW_WINDOW_JOURNAL, ToggleShowJournalWindow, "CTRL+J");

	active_screen = JournalSubmenu::Main;

	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#00ff00>Encounter Journal</c> was loaded.");
}

///----------------------------------------------------------------------------------------------------
/// AddonUnload:
/// 	Everything you registered in AddonLoad, you should "undo" here.
///----------------------------------------------------------------------------------------------------
void AddonUnload()
{
	/* let's clean up after ourselves */
	APIDefs->Renderer.Deregister(AddonRender);
	APIDefs->Renderer.Deregister(AddonOptions);

	APIDefs->InputBinds.Deregister(KB_TOGGLE_SHOW_WINDOW_JOURNAL);

	MumbleData = nullptr;
	NexusLink = nullptr;

	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#ff0000>Signing off</c>, it was an honor commander.");
}

///----------------------------------------------------------------------------------------------------
/// AddonRender:
/// 	Called every frame. Safe to render any ImGui.
/// 	You can control visibility on loading screens with NexusLink->IsGameplay.
///----------------------------------------------------------------------------------------------------
void AddonRender()
{
	RenderJournal();
}

///----------------------------------------------------------------------------------------------------
/// AddonOptions:
/// 	Basically an ImGui callback that doesn't need its own Begin/End calls.
///----------------------------------------------------------------------------------------------------
void AddonOptions()
{

}

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