#include "format"
#include "gui_journal.h"
#include "imgui/imgui.h"
#include "imgui/imgui_markdown.h"
#include "data/encounters.h"
#include "nexus/Nexus.h"
#include "nlohmann/json.hpp"
#include "imgui_custom.h"
#include "shared.h"
#include <cstring>
#include <string>

using json = nlohmann::ordered_json;

json j_encounters = json::parse(str_encounters_json);

std::string selected_zone;
std::string selected_boss;
std::string selected_strike;
bool copy_mode = false;
JournalSubmenu active_screen = JournalSubmenu::Main;

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
	std::string url(data_.linkData.link, data_.linkData.linkLength);
	if( data_.linkData.isImage )
	{
		return;
	}
	else if (url.find("text:") == 0)
	{
		ImGui::SetTooltip( "%s", url.substr(5).c_str() );
	}
	else if (url.find("image:") == 0)
	{
		ImTextureID image = nullptr;
		Texture* tex_from_url = APIDefs->Textures.GetOrCreateFromResource("EJ_HORSE", 101, hSelf);

		if (tex_from_url != nullptr)
		{
			image = tex_from_url->Resource;
		}

		ImGui::BeginTooltip();
		ImGui::Image(image, ImVec2(64, 64));
		ImGui::TextDisabled("A relevant preview goes here");
		ImGui::TextDisabled("In the meantime, enjoy horse");
		ImGui::EndTooltip();
	}
	else
	{
		ImGui::SetTooltip( "Open in browser\n%.*s", data_.linkData.linkLength, data_.linkData.link );
	}
}

void Markdown(const std::string& markdown_)
{
	ImFont* default_font = ImGui::GetFont();
	// You can make your own Markdown function with your prefered string container and markdown config.
	ImGui::MarkdownConfig mdConfig
	{ 
		LinkCallback,
		MarkdownTooltipCallback,
		ImageCallback,
		nullptr,
		{
			// { (ImFont*)NexusLink->FontBig, true },
			// { (ImFont*)NexusLink->FontBig, true },
			// { (ImFont*)NexusLink->FontUI, false }
			{ default_font, true },
			{ default_font, true },
			{ default_font, false }
		},
		nullptr};
	ImGui::Markdown( markdown_.c_str(), markdown_.length(), mdConfig );
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
		auto map_id_str = std::to_string(MumbleData->Context.MapID);
		if (j_encounters.contains(map_id_str)) selected_zone = map_id_str;
		else selected_zone.clear();

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
	RenderContent();

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
		CustomSelectableBack([]() { active_screen = JournalSubmenu::Main; }, "Back to Menu");
		CustomSelectable([]() { selected_strike = "s1_strike"; }, "Living World Season 1");
		CustomSelectable([]() { selected_strike = "wintersday_strike"; }, "Wintersday");
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
	RenderContent();

	ImGui::EndTable();
}

void RenderJournalZoneMenu(std::string type)
{
	CustomSelectableBack([]() { active_screen = JournalSubmenu::Main; }, "Back to Menu");
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
	CustomSelectableBack(ClearSelections, "Back to Raids");

	if (!selected_zone.empty())
	{
		auto bosses = j_encounters[selected_zone]["bosses"];
		for (auto j = bosses.begin(); j != bosses.end(); ++j)
		{
			auto boss_name = j.key();
			int icon = 0;
			if (j->contains("icon")) icon = j->at("icon");
			CustomSelectable([boss_name]() { selected_boss = boss_name; }, boss_name, selected_boss == boss_name, "", icon);
		}
	}
}

void RenderJournalStrikeBossMenu(std::string type)
{
	CustomSelectableBack(ClearSelections, "Back to Strike Missions");

	for (auto j = j_encounters.begin(); j != j_encounters.end(); ++j)
	{
		auto zone_id = j.key();
		std::string boss_name = j->at("name");
		std::string subtitle;
		std::string icon_path = std::format("/bosses/{}/icon", boss_name);
		json::json_pointer icon_ptr (icon_path);
		int icon = 0;
		if (j->contains("subtitle")) subtitle = j->at("subtitle");
		if (j->contains(icon_ptr)) icon = j->at(icon_ptr);
		if (j->at("type") != type) continue;
		CustomSelectable([zone_id, boss_name]() { selected_zone = zone_id; selected_boss = boss_name; }, boss_name, selected_boss == boss_name, subtitle, icon);
	}
}

void RenderContent()
{
	ImGui::BeginChild("Content");
	if (!selected_zone.empty() && !selected_boss.empty())
	{
		auto boss_json = j_encounters[selected_zone]["bosses"][selected_boss];

		ImGui::Text("%s", selected_boss.c_str());
		Markdown("***");

		if (boss_json.contains("requirements"))
		{
			std::string reqs = boss_json["requirements"];
			Markdown("Requirements");
			Markdown(reqs);
		}

		if (boss_json.contains("overview"))
		{
			std::string overview = boss_json["overview"];
			Markdown("Overview");
			Markdown(overview);
		}

		if (boss_json.contains("tank"))
		{
			std::string tank = boss_json["tank"];
			if (ImGui::CollapsingHeader("Tanking"))
			{
				Markdown(tank);
			}
		}

		if (boss_json.contains("heal"))
		{
			std::string heal = boss_json["heal"];
			if (ImGui::CollapsingHeader("Healing"))
			{
				Markdown(heal);
			}
		}

		if (boss_json.contains("dps"))
		{
			std::string dps = boss_json["dps"];
			if (ImGui::CollapsingHeader("Damage Dealers"))
			{
				Markdown(dps);
			}
		}

		if (boss_json.contains("links"))
		{
			std::string links = boss_json["links"];
			if (ImGui::CollapsingHeader("Links"))
			{
				Markdown(links);
			}
		}
	}
	ImGui::EndChild();
}

void ToggleShowJournalWindow(const char* keybindIdentifier, bool isRelease)
{
	if (isRelease) return;
	ShowJournalWindow = !ShowJournalWindow;
}