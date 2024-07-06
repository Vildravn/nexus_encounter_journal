#include <format>
#include <list>
#include "imgui/imgui.h"
#include "imgui/imgui_extensions.h"
#include "nlohmann/json.hpp"
#include "gui.h"
#include "resource.h"
#include "shared.h"
#include "data/encounters.h"

using json = nlohmann::ordered_json;

json j_encounters = json::parse(str_encounters_json);
json::json_pointer selected_pointer;
std::string selected_boss_name;
std::list<std::string> widget_bosses = {"Dhuum", "Sabetha", "Trio"};
std::string selected_widget_boss = "Dhuum";

ImGuiWindowFlags WidgetFlagsLocked = 
	ImGuiWindowFlags_NoCollapse + 
	ImGuiWindowFlags_NoScrollbar + 
	ImGuiWindowFlags_NoScrollWithMouse + 
	ImGuiWindowFlags_NoBackground + 
	ImGuiWindowFlags_AlwaysAutoResize + 
	ImGuiWindowFlags_NoTitleBar +
	ImGuiWindowFlags_NoMove +
	ImGuiWindowFlags_NoInputs;

ImGuiWindowFlags WidgetFlagsUnlocked = 
	ImGuiWindowFlags_NoCollapse + 
	ImGuiWindowFlags_NoScrollbar + 
	ImGuiWindowFlags_NoScrollWithMouse + 
	ImGuiWindowFlags_AlwaysAutoResize + 
	ImGuiWindowFlags_NoTitleBar;

void ToggleShowWindowEncounterJournal(const char* keybindIdentifier, bool isRelease)
{
    if (isRelease) return;
    APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", keybindIdentifier).c_str());
    ShowWindowEncounterJournal = !ShowWindowEncounterJournal;
}

void ToggleShowWindowEncounterWidget(const char* keybindIdentifier, bool isRelease)
{
    if (isRelease) return;
    APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", keybindIdentifier).c_str());
    ShowWindowEncounterWidget = !ShowWindowEncounterWidget;
}

void ToggleLockWindowEncounterWidget(const char* keybindIdentifier, bool isRelease)
{
    if (isRelease) return;
    APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", keybindIdentifier).c_str());
    LockWindowEncounterWidget = !LockWindowEncounterWidget;
}

void RegisterQuickAccessShortcut() {
	APIDefs->AddShortcut("SHORTCUT_ENCOUNTER_JOURNAL", "TEX_JOURNAL_ICON", "TEX_JOURNAL_ICON_HOVER", KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL, "Encounter Journal");
}

void DeregisterQuickAccessShortcut() {
	APIDefs->RemoveShortcut("SHORTCUT_ENCOUNTER_JOURNAL");
}

void RenderWindowEncounterJournal()
{
    if (!ShowWindowEncounterJournal) return;

    ImGui::PushFont((ImFont*)NexusLink->FontUI);
    if (ImGui::Begin(ADDON_NAME, &ShowWindowEncounterJournal, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoScrollbar + ImGuiWindowFlags_NoScrollWithMouse))
	{
		if (ImGui::BeginTable("MainTable", 2))
		{
			ImGui::TableSetupColumn("Bosses", ImGuiTableColumnFlags_WidthFixed, 270.0f);
			ImGui::TableSetupColumn("Content");
			//ImGui::TableHeadersRow();

			ImGui::TableNextColumn();
			if (ImGui::BeginChild("Bosses"))
			{
				static ImGuiTextFilter filter;
				ImGui::Text("Search");
				ImGui::SameLine();
				filter.Draw("##filter");
				int index = 0;
				for (json::iterator it = j_encounters.begin(); it != j_encounters.end(); it++)
				{
					if (filter.IsActive())
					{
						ImGui::SetNextItemOpen(true);
					}

					if (ImGui::CollapsingHeader(it.key().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						json childNode = it.value();

						for (json::iterator childIt = childNode.begin(); childIt != childNode.end(); childIt++, index++)
						{
							auto bossName = childIt.key();
							if (filter.PassFilter(bossName.c_str()))
							{
								ImGui::Indent(10.0f);
								
								if (ImGui::Selectable(("##" + bossName).c_str(), bossName == selected_boss_name, 0, ImVec2(0, 32)))
								{
									selected_pointer = json::json_pointer("/" + it.key() + "/" + bossName);
									selected_boss_name = bossName;
								}
								ImGui::SameLine();
								
								Texture* bossTexture = GetBossTexture(bossName);

								if (bossTexture != nullptr)
								{
									ImGui::Image(bossTexture->Resource, ImVec2(32, 32));
								}
								ImGui::SameLine();
								float pos_y = ImGui::GetCursorPosY();
								ImGui::SetCursorPosY(pos_y + 16.0f - (ImGui::GetTextLineHeight() / 2));
								ImGui::Text(bossName.c_str());
								ImGui::SetCursorPosY(pos_y + 40.0f);
								ImGui::Unindent(10.0f);
							}
						}
					}	
				}
				ImGui::EndChild();
			}

			ImGui::TableNextColumn();
			if (ImGui::BeginChild("Content"))
			{
				json selected_boss;
				if (selected_pointer.empty())
				{
					selected_boss = NULL;
				}
				else
				{
					selected_boss = j_encounters[selected_pointer];
				}

				if (selected_boss == NULL)
				{
					ImGui::TextWrapped("Select a boss from the left menu");	
				}
				else
				{
					if (selected_boss.contains("desc"))
					{
						ImGui::TextWrapped(selected_boss["desc"].get<std::string>().c_str());
					}

					if (selected_boss.contains("comm"))
					{
						if (ImGui::CollapsingHeader("Commander"))
						{
							ImGui::TextWrapped(selected_boss["comm"].get<std::string>().c_str());
						}
					}

					if (selected_boss.contains("tank"))
					{
						if (ImGui::CollapsingHeader("Tank"))
						{
							ImGui::TextWrapped(selected_boss["tank"].get<std::string>().c_str());
						}
					}

					if (selected_boss.contains("heal"))
					{
						if (ImGui::CollapsingHeader("Healer"))
						{
							ImGui::TextWrapped(selected_boss["heal"].get<std::string>().c_str());
						}
					}

					if (selected_boss.contains("dps"))
					{
						if (ImGui::CollapsingHeader("DPS"))
						{
							ImGui::TextWrapped(selected_boss["dps"].get<std::string>().c_str());
						}
					}
				}

				ImGui::EndChild();
			}
			ImGui::EndTable();
		}
		ImGui::End();
	}
    ImGui::PopFont();
}

void RenderDhuumTable()
{
	if (ImGui::BeginTable("Dhuum Table", 3))
	{
		ImGui::TableSetupColumn("G1");
		ImGui::TableSetupColumn("G2");
		ImGui::TableSetupColumn("G3");
		//ImGui::TableHeadersRow();
		ImGui::TableNextColumn();
		ImGui::TextOutlined("\tG1\t");
		DrawMarker(Arrow);
		ImGui::TextOutlined("9:30\t");
		DrawMarker(Square);
		ImGui::TextOutlined("8:00");
		DrawMarker(Triangle);
		ImGui::TextOutlined("6:30");
		DrawMarker(Heart);
		ImGui::TextOutlined("5:00");
		DrawMarker(Spiral);
		ImGui::TextOutlined("3:30");
		DrawMarker(Circle);
		ImGui::TextOutlined("2:00");
		DrawMarker(Star);
		ImGui::TextOutlined("0:30");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("\tG2\t");
		DrawMarker(Circle);
		ImGui::TextOutlined("9:00\t");
		DrawMarker(Star);
		ImGui::TextOutlined("7:30");
		DrawMarker(Arrow);
		ImGui::TextOutlined("6:00");
		DrawMarker(Square);
		ImGui::TextOutlined("4:30");
		DrawMarker(Triangle);
		ImGui::TextOutlined("3:00");
		DrawMarker(Heart);
		ImGui::TextOutlined("1:30");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("\tG3\t");
		DrawMarker(Heart);
		ImGui::TextOutlined("8:30");
		DrawMarker(Spiral);
		ImGui::TextOutlined("7:00");
		DrawMarker(Circle);
		ImGui::TextOutlined("5:30");
		DrawMarker(Star);
		ImGui::TextOutlined("4:00");
		DrawMarker(Arrow);
		ImGui::TextOutlined("2:30");
		DrawMarker(Square);
		ImGui::TextOutlined("1:00");
		ImGui::EndTable();
	}
}

void RenderSabethaTable()
{
	if (ImGui::BeginTable("Sabetha Table", 2))
	{
		ImGui::TableSetupColumn("1,3");
		ImGui::TableSetupColumn("2,4");
		//ImGui::TableHeadersRow();
		ImGui::TableNextColumn();
		ImGui::TextOutlined("\t1,3\t");
		DrawMarker(Arrow);
		ImGui::TextOutlined("8:30\t");
		DrawMarker(Heart);
		ImGui::TextOutlined("7:30\t");
		DrawMarker(Arrow);
		ImGui::TextOutlined("6:30\t");
		DrawMarker(Circle);
		ImGui::TextOutlined("5:30\t");
		DrawMarker(Arrow);
		ImGui::TextOutlined("4:30\t");
		DrawMarker(Heart);
		ImGui::TextOutlined("3:30\t");
		DrawMarker(Arrow);
		ImGui::TextOutlined("2:30\t");
		DrawMarker(Circle);
		ImGui::TextOutlined("1:30\t");
		DrawMarker(Arrow);
		ImGui::TextOutlined("0:30\t");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("\t2,4\t");
		DrawMarker(Circle);
		ImGui::TextOutlined("8:00\t");
		DrawMarker(Square);
		ImGui::TextOutlined("7:00\t");
		DrawMarker(Heart);
		ImGui::TextOutlined("6:00\t");
		DrawMarker(Square);
		ImGui::TextOutlined("5:00\t");
		DrawMarker(Circle);
		ImGui::TextOutlined("4:00\t");
		DrawMarker(Square);
		ImGui::TextOutlined("3:00\t");
		DrawMarker(Heart);
		ImGui::TextOutlined("2:00\t");
		DrawMarker(Square);
		ImGui::TextOutlined("1:00\t");
		ImGui::EndTable();
	}
}

void RenderTrioTable()
{
	DrawMarker(Arrow);
	ImGui::TextOutlined("6:50 - Berg");
	DrawMarker(Circle);
	ImGui::TextOutlined("4:50 - Zane");
	DrawMarker(Heart);
	ImGui::TextOutlined("2:45 - Narella");
	ImGui::NewLine();
	ImGui::TextOutlined("Mortars");
	if (ImGui::BeginTable("Mortars Table", 4))
	{
		ImGui::TableSetupColumn("1");
		ImGui::TableSetupColumn("2");
		ImGui::TableSetupColumn("3");
		ImGui::TableSetupColumn("4");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("5:20");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("4:20");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("3:20");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("2:20");
		ImGui::EndTable();
	}
}

void RenderWidget()
{
	if (!NexusLink || !MumbleLink) return;
	if (!ShowWindowEncounterWidget) return;
	if (!NexusLink->IsGameplay) return;
	if (OnlyShowWidgetInstanced && MumbleLink->Context.MapType != Mumble::EMapType::Instance) return;

    // Dhuum widget prototype
    ImGui::PushFont((ImFont*)NexusLink->FontUI);

	ImGuiWindowFlags widgetFlags = WidgetFlagsLocked;
	if (!LockWindowEncounterWidget) widgetFlags = WidgetFlagsUnlocked;

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	if (ImGui::Begin("Boss widget", NULL, widgetFlags))
	{
		if (!LockWindowEncounterWidget)
		{
			if (ImGui::BeginCombo("##widgetCombo", selected_widget_boss.c_str()))
			{
				//ImGui::Selectable("Sabetha");
				for (std::string boss : widget_bosses)
				{
					if (selected_widget_boss != boss)
					{
						if (ImGui::Selectable(boss.c_str()))
						{
							selected_widget_boss = boss;
						}
					}
				}
				ImGui::EndCombo();
			}
		}

		if (selected_widget_boss == "Sabetha") RenderSabethaTable();
		else if (selected_widget_boss == "Dhuum") RenderDhuumTable();
		else if (selected_widget_boss == "Trio") RenderTrioTable();

		ImGui::End();
	}
	ImGui::PopStyleColor();
	ImGui::PopFont();
}

Texture* GetBossTexture(std::string boss_name)
{
	if (boss_name == "Vale Guardian")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_VALE_GUARDIAN", IDB_VALE_GUARDIAN, hSelf);
	}
	else if (boss_name == "Gorseval")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_GORSEVAL", IDB_GORSEVAL, hSelf);
	}
	else if (boss_name == "Sabetha")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_SABETHA", IDB_SABETHA, hSelf);
	}
	else if (boss_name == "Slothasor")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_SLOTHASOR", IDB_SLOTHASOR, hSelf);
	}
	else if (boss_name == "Prison Camp")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_ZANE", IDB_ZANE, hSelf);
	}
	else if (boss_name == "Matthias")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_MATTHIAS", IDB_MATTHIAS, hSelf);
	}
	else if (boss_name == "Escort")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_ESCORT", IDB_ESCORT, hSelf);
	}
	else if (boss_name == "Keep Construct")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_KEEP_CONSTRUCT", IDB_KEEP_CONSTRUCT, hSelf);
	}
	else if (boss_name == "Twisted Castle")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_TWISTED_CASTLE", IDB_TWISTED_CASTLE, hSelf);
	}
	else if (boss_name == "Xera")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_XERA", IDB_XERA, hSelf);
	}
	else if (boss_name == "Cairn the Indomitable")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_CAIRN", IDB_CAIRN, hSelf);
	}
	else if (boss_name == "Mursaat Overseer")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_MURSAAT_OVERSEER", IDB_MURSAAT_OVERSEER, hSelf);
	}
	else if (boss_name == "Samarog")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_SAMAROG", IDB_SAMAROG, hSelf);
	}
	else if (boss_name == "Deimos")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_DEIMOS", IDB_DEIMOS, hSelf);
	}
	else if (boss_name == "Soulless Horror")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_SOULLESS_HORROR", IDB_SOULLESS_HORROR, hSelf);
	}
	else if (boss_name == "River of Souls")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_RIVER_OF_SOULS", IDB_RIVER_OF_SOULS, hSelf);
	}
	else if (boss_name == "Eater of Souls")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_EATER_OF_SOULS", IDB_EATER_OF_SOULS, hSelf);
	}
	else if (boss_name == "Broken King")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_BROKEN_KING", IDB_BROKEN_KING, hSelf);
	}
	else if (boss_name == "Eyes of Judgment and Fate")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_EYES_OF_FATE", IDB_EYES_OF_FATE, hSelf);
	}
	else if (boss_name == "Dhuum")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_DHUUM", IDB_DHUUM, hSelf);
	}
	else if (boss_name == "Conjured Amalgamate")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_CONJURED_AMALGAMATE", IDB_CONJURED_AMALGAMATE, hSelf);
	}
	else if (boss_name == "Twin Largos")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_NIKARE", IDB_NIKARE, hSelf);
	}
	else if (boss_name == "Qadim")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_QADIM", IDB_QADIM, hSelf);
	}
	else if (boss_name == "Cardinal Adina")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_ADINA", IDB_ADINA, hSelf);
	}
	else if (boss_name == "Cardinal Sabir")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_SABIR", IDB_SABIR, hSelf);
	}
	else if (boss_name == "Qadim the Peerless")
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_QADIM_THE_PEERLESS", IDB_QADIM_THE_PEERLESS, hSelf);
	}
	else
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_UNDER_CONSTRUCTION", IDB_UNDER_CONSTRUCTION, hSelf);
	}
}

void DrawMarker(SquadMarker marker)
{
    Texture* marker_texture = nullptr;

    switch (marker)
    {
        case Arrow:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_ARROW", IDB_MARKER_ARROW, hSelf);
            break;
        
        case Circle:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_CIRCLE", IDB_MARKER_CIRCLE, hSelf);
            break;
        
        case Heart:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_HEART", IDB_MARKER_HEART, hSelf);
            break;

        case Spiral:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_SPIRAL", IDB_MARKER_SPIRAL, hSelf);
            break;
        
        case Square:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_SQUARE", IDB_MARKER_SQUARE, hSelf);
            break;

        case Star:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_STAR", IDB_MARKER_STAR, hSelf);
            break;
        
        case Triangle:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_TRIANGLE", IDB_MARKER_TRIANGLE, hSelf);
            break;
        
        case X:
            marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_X", IDB_MARKER_X, hSelf);
            break;
    }

	if (marker_texture != nullptr)
	{
		ImGui::Image(marker_texture->Resource, ImVec2(24, 24));
		ImGui::SameLine();
	}
}