#include <format>
#include <list>
#include <cmath>
#include "imgui/imgui.h"
#include "imgui/imgui_extensions.h"
#include "imgui/imgui_markdown.h"
#include "nlohmann/json.hpp"
#include "gui.h"
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
	APIDefs->QuickAccess.Add("SHORTCUT_ENCOUNTER_JOURNAL", "TEX_JOURNAL_ICON", "TEX_JOURNAL_ICON_HOVER", KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL, "Encounter Journal");
}

void DeregisterQuickAccessShortcut() {
	APIDefs->QuickAccess.Remove("SHORTCUT_ENCOUNTER_JOURNAL");
}

Mumble::EUIScale GetUIScale()
{
	if (!MumbleIdentity)
	{
		return Mumble::EUIScale::Normal;
	}

	return MumbleIdentity->UISize;
}

inline ImGui::MarkdownImageData ImageCallback( ImGui::MarkdownLinkCallbackData data_ )
{
	std::string image_link = data_.link;
	image_link = image_link.substr(0, data_.linkLength);
    ImTextureID image = nullptr;
	Texture* tex_from_url = APIDefs->Textures.GetOrCreateFromURL(std::format("TEX_{}", image_link).c_str(), "https://assets.gw2dat.com", image_link.c_str());

	if (tex_from_url != nullptr)
	{
		image = tex_from_url->Resource;
	}

    ImGui::MarkdownImageData imageData;
    imageData.isValid =         true;
    imageData.useLinkCallback = false;
    imageData.user_texture_id = image;
    imageData.size =            ImVec2( 16.0f, 16.0f );

    // For image resize when available size.x > image width, add
    ImVec2 const contentSize = ImGui::GetContentRegionAvail();
    if( imageData.size.x > contentSize.x )
    {
        float const ratio = imageData.size.y/imageData.size.x;
        imageData.size.x = contentSize.x;
        imageData.size.y = contentSize.x*ratio;
    }

    return imageData;
}

void LinkCallback( ImGui::MarkdownLinkCallbackData data_ )
{
    std::string url( data_.link, data_.linkLength );
    if( !data_.isImage )
    {
        ShellExecuteA( nullptr, "open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL );
    }
}

void MarkdownTooltipCallback( ImGui::MarkdownTooltipCallbackData data_ )
{
	if( data_.linkData.isImage )
	{
		return;
		//ImGui::SetTooltip( "%.*s", data_.linkData.linkLength, data_.linkData.link );
	}
	else
	{
		ImGui::SetTooltip( "Open in browser\n%.*s", data_.linkData.linkLength, data_.linkData.link );
	}
}

void Markdown(const std::string& markdown_)
{
    // You can make your own Markdown function with your prefered string container and markdown config.
    ImGui::MarkdownConfig mdConfig{ LinkCallback, MarkdownTooltipCallback, ImageCallback, nullptr, { { (ImFont*)NexusLink->FontBig, true }, { (ImFont*)NexusLink->FontBig, true }, { (ImFont*)NexusLink->FontUI, false } }, nullptr};
    ImGui::Markdown( markdown_.c_str(), markdown_.length(), mdConfig );
}

void RenderWindowEncounterJournal()
{
    if (!ShowWindowEncounterJournal) return;

    ImGui::PushFont((ImFont*)NexusLink->FontUI);
	ImGui::SetNextWindowSize(ImVec2(820, 380), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(NexusLink->Width/2 - 820/2, NexusLink->Height/3 - 380/2), ImGuiCond_FirstUseEver); // Center window
    if (ImGui::Begin("Encounter Journal", &ShowWindowEncounterJournal, ImGuiWindowFlags_NoCollapse + ImGuiWindowFlags_NoScrollbar + ImGuiWindowFlags_NoScrollWithMouse))
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
				for (auto it = j_encounters.begin(); it != j_encounters.end(); ++it)
				{
					if (filter.IsActive())
					{
						ImGui::SetNextItemOpen(true);
					}

					if (ImGui::CollapsingHeader(it.key().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						json childNode = it.value();

						for (auto childIt = childNode.begin(); childIt != childNode.end(); ++childIt, index++)
						{
							const std::string& bossName = childIt.key();
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
					ImGui::PopFont();
					ImGui::PushFont((ImFont*)NexusLink->FontBig);
					ImGui::Text(selected_boss_name.c_str());
					Markdown("***");
					ImGui::PopFont();
					ImGui::PushFont((ImFont*)NexusLink->FontUI);

					if (selected_boss.contains("desc"))
					{
						Markdown(selected_boss["desc"].get<std::string>());
					}

					if (selected_boss.contains("comm"))
					{
						if (ImGui::CollapsingHeader("Commander"))
						{
							Markdown(selected_boss["comm"].get<std::string>());
						}
					}

					if (selected_boss.contains("tank"))
					{
						if (ImGui::CollapsingHeader("Tank"))
						{
							Markdown(selected_boss["tank"].get<std::string>());
						}
					}

					if (selected_boss.contains("heal"))
					{
						if (ImGui::CollapsingHeader("Healer"))
						{
							Markdown(selected_boss["heal"].get<std::string>());
						}
					}

					if (selected_boss.contains("dps"))
					{
						if (ImGui::CollapsingHeader("DPS"))
						{
							Markdown(selected_boss["dps"].get<std::string>());
						}
					}

					if (selected_boss.contains("links"))
					{
						if (ImGui::CollapsingHeader("Links"))
						{
							Markdown(selected_boss["links"].get<std::string>());
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

void DrawBackground(ImVec2 small_size, ImVec2 normal_size, ImVec2 large_size, ImVec2 larger_size)
{
	if (!ShowWidgetBackground) return;

	ImVec2 cur_pos = ImGui::GetCursorPos();
	ImVec2 bg_size = small_size;

	switch (GetUIScale())
	{
		case Mumble::EUIScale::Normal:
			bg_size = normal_size;
			break;

		case Mumble::EUIScale::Large:
			bg_size = large_size;
			break;
		
		case Mumble::EUIScale::Larger:
			bg_size = larger_size;
			break;
	}

	ImGui::Image(texWidgetBG->Resource, bg_size, ImVec2(0, 0), ImVec2(1, 1), ImVec4(1, 1, 1, WidgetBackgroundOpacity));
	ImGui::SetCursorPos(ImVec2(cur_pos.x + 10, cur_pos.y + 10));
}

void RenderDhuumTable()
{
	DrawBackground(ImVec2(270, 220), ImVec2(280, 220), ImVec2(300, 220), ImVec2(300, 230));

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
	DrawBackground(ImVec2(270, 220), ImVec2(280, 220), ImVec2(300, 220), ImVec2(300, 230));

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
	DrawBackground(ImVec2(270, 220), ImVec2(280, 220), ImVec2(300, 220), ImVec2(300, 230));

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
	DrawBackground(ImVec2(270, 220), ImVec2(280, 220), ImVec2(300, 220), ImVec2(300, 230));
	
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
	if (ImGui::Begin("Boss widget", nullptr, widgetFlags))
	{
		float dist_from_sabetha = GetPlayerDistanceSquaredFromPoint(36651.1, 28924.5);
		float dist_from_trio = GetPlayerDistanceSquaredFromPoint(35893.1, 29866.6);
		float dist_from_dhuum = GetPlayerDistanceSquaredFromPoint(53390.7, 32350.9);
		float dist_from_sloth = GetPlayerDistanceSquaredFromPoint(36297.1, 29445.6);
		constexpr float dist_cutoff = 25000.0f;

		if (dist_from_sabetha < dist_cutoff) RenderSabethaTable();
		else if (dist_from_trio < dist_cutoff) RenderTrioTable();
		else if (dist_from_dhuum < dist_cutoff && MumbleLink->AvatarPosition.Y > 145.0f) RenderDhuumTable();
		else if (dist_from_sloth < dist_cutoff) RenderSlothTable();
		else ImGui::TextOutlined("No boss nearby or no widget");

		ImVec2 widgetSize = ImGui::GetWindowSize();
		ImGui::SetWindowPos("Boss widget", ImVec2(NexusLink->Width/2 - widgetSize.x/2, NexusLink->Height/2 - widgetSize.y/2), ImGuiCond_FirstUseEver);

		ImGui::End();
	}
	ImGui::PopStyleColor();
	ImGui::PopFont();
}

Texture* GetBossTexture(const std::string& boss_name)
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
