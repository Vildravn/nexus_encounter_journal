#include <format>
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

bool ShowWindowEncounterJournal = false;

void ToggleShowWindowEncounterJournal(const char* keybindIdentifier, bool isRelease)
{
    if (isRelease) return;
    APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, std::format("Keybind {} was pressed.", keybindIdentifier).c_str());
    ShowWindowEncounterJournal = !ShowWindowEncounterJournal;
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

void RenderDhuumWidget()
{
    // Dhuum widget prototype
    ImGui::PushFont((ImFont*)NexusLink->FontUI);
	if (ImGui::Begin("Boss widget", NULL, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoScrollbar + ImGuiWindowFlags_NoScrollWithMouse + ImGuiWindowFlags_NoBackground + ImGuiWindowFlags_AlwaysAutoResize + ImGuiWindowFlags_NoTitleBar))
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
	else
	{
		return APIDefs->GetTextureOrCreateFromResource("TEX_UNDER_CONSTRUCTION", IDB_UNDER_CONSTRUCTION, hSelf);
	}
}

void DrawMarker(SquadMarker marker)
{
    //Texture* marker_texture = APIDefs->GetTextureOrCreateFromResource("TEX_MARKER_ARROW", IDB_MARKER_ARROW, hSelf);
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