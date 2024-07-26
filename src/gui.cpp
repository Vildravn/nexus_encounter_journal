#include <format>
#include <list>
#include <cmath>
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

void RenderSlothTable()
{
	if (ImGui::BeginTable("Sloth Table", 2))
	{
		ImGui::TableSetupColumn("Time");
		ImGui::TableSetupColumn("Shroom");
		ImGui::TableNextColumn();
		DrawMarker(Arrow);
		ImGui::TextOutlined("Mushroom 1\t");
		DrawMarker(Circle);
		ImGui::TextOutlined("Mushroom 2\t");
		DrawMarker(Heart);
		ImGui::TextOutlined("Mushroom 3\t");
		DrawMarker(Square);
		ImGui::TextOutlined("Mushroom 4\t");
		DrawMarker(Arrow);
		ImGui::TextOutlined("Mushroom 1\t");
		DrawMarker(Circle);
		ImGui::TextOutlined("Mushroom 2\t");
		DrawMarker(Heart);
		ImGui::TextOutlined("Mushroom 3\t");
		ImGui::TableNextColumn();
		ImGui::TextOutlined("7:00");
		ImGui::TextOutlined("6:00");
		ImGui::TextOutlined("5:00");
		ImGui::TextOutlined("4:00");
		ImGui::TextOutlined("3:00");
		ImGui::TextOutlined("2:00");
		ImGui::TextOutlined("1:00");
		ImGui::EndTable();
	}
}

float GetPlayerDistanceSquaredFromPoint(float x, float y)
{
	return pow(x - MumbleLink->Context.Compass.PlayerPosition.X, 2) + pow(y - MumbleLink->Context.Compass.PlayerPosition.Y, 2);
}

void RenderWidget()
{
	if (!NexusLink || !MumbleLink) return;
	if (!ShowWindowEncounterWidget) return;
	if (!NexusLink->IsGameplay) return;
	if (OnlyShowWidgetInstanced && MumbleLink->Context.MapType != Mumble::EMapType::Instance) return;

    ImGui::PushFont((ImFont*)NexusLink->FontUI);

	ImGuiWindowFlags widgetFlags = WidgetFlagsLocked;
	if (!LockWindowEncounterWidget) widgetFlags = WidgetFlagsUnlocked;

	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	if (ImGui::Begin("Boss widget", NULL, widgetFlags))
	{
		float dist_from_sabetha = GetPlayerDistanceSquaredFromPoint(36651.1, 28924.5);
		float dist_from_trio = GetPlayerDistanceSquaredFromPoint(35893.1, 29866.6);
		float dist_from_dhuum = GetPlayerDistanceSquaredFromPoint(53247.4, 32344.7);
		float dist_from_sloth = GetPlayerDistanceSquaredFromPoint(36297.1, 29445.6);
		const float dist_cutoff = 25000.0f;

		if (dist_from_sabetha < dist_cutoff) RenderSabethaTable();
		else if (dist_from_trio < dist_cutoff) RenderTrioTable();
		//else if (dist_from_dhuum < dist_cutoff) RenderDhuumTable();
		else if (MumbleLink->Context.MapID == 1264) RenderDhuumTable(); //Workaround until we know the Z value of the PoI
		else if (dist_from_sloth < dist_cutoff) RenderSlothTable();
		else ImGui::TextOutlined("No boss nearby or no widget");

		ImGui::End();
	}
	ImGui::PopStyleColor();
	ImGui::PopFont();
}

Texture* GetBossTexture(std::string boss_name)
{
	if (boss_name == "Vale Guardian") return texVG;
	else if (boss_name == "Gorseval") return texGors;
	else if (boss_name == "Sabetha") return texSab;
	else if (boss_name == "Slothasor") return texSloth;
	else if (boss_name == "Prison Camp") return texTrio;
	else if (boss_name == "Matthias") return texMatthias;
	else if (boss_name == "Escort") return texEscort;
	else if (boss_name == "Keep Construct") return texKC;
	else if (boss_name == "Twisted Castle") return texTC;
	else if (boss_name == "Xera") return texXera;
	else if (boss_name == "Cairn the Indomitable") return texCairn;
	else if (boss_name == "Mursaat Overseer") return texMO;
	else if (boss_name == "Samarog") return texSamarog;
	else if (boss_name == "Deimos") return texDeimos;
	else if (boss_name == "Soulless Horror") return texSH;
	else if (boss_name == "River of Souls") return texRoS;
	else if (boss_name == "Eater of Souls") return texEoS;
	else if (boss_name == "Broken King") return texBK;
	else if (boss_name == "Eyes of Judgment and Fate") return texEyes;
	else if (boss_name == "Dhuum") return texDhuum;
	else if (boss_name == "Conjured Amalgamate") return texCA;
	else if (boss_name == "Twin Largos") return texTL;
	else if (boss_name == "Qadim") return texQadim;
	else if (boss_name == "Cardinal Adina") return texAdina;
	else if (boss_name == "Cardinal Sabir") return texSabir;
	else if (boss_name == "Qadim the Peerless") return texQTP;
	else if (boss_name == "Shiverpeaks Pass")  return texSP;
	else if (boss_name == "Voice and Claw of the Fallen") return texVoiceClaw;
	else if (boss_name == "Fraenir of Jormag") return texFoJ;
	else if (boss_name == "Boneskinner") return texBS;
	else if (boss_name == "Whisper of Jormag") return texWoJ;
	else if (boss_name == "Aetherblade Hideout") return texAH;
	else if (boss_name == "Xunlai Jade Junkyard") return texXJJ;
	else if (boss_name == "Kaineng Overlook") return texKO;
	else if (boss_name == "Harvest Temple") return texHT;
	else if (boss_name == "Cosmic Observatory") return texCO;
	else if (boss_name == "Temple of Febe") return texFebe;
	else if (boss_name == "Old Lion's Court") return texOLC;
	else return texWIP;
}

void DrawMarker(SquadMarker marker)
{
    Texture* marker_texture = nullptr;

    switch (marker)
    {
        case Arrow:
            marker_texture = texArrow;
            break;
        
        case Circle:
            marker_texture = texCircle;
            break;
        
        case Heart:
            marker_texture = texHeart;
            break;

        case Spiral:
            marker_texture = texSpiral;
            break;
        
        case Square:
            marker_texture = texSquare;
            break;

        case Star:
            marker_texture = texStar;
            break;
        
        case Triangle:
            marker_texture = texTriangle;
            break;
        
        case X:
            marker_texture = texX;
            break;
    }

	if (marker_texture != nullptr)
	{
		ImGui::Image(marker_texture->Resource, ImVec2(24, 24));
		ImGui::SameLine();
	}
}
