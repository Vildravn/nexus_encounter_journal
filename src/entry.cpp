#include <stdint.h>
#include <stdio.h>
#include <windows.h>
#include <format>

#include "shared.h"
#include "gui.h"
#include "resource.h"

#include "nexus/Nexus.h"
#include "imgui/imgui.h"

/* proto */
void AddonLoad(AddonAPI* aApi);
void AddonUnload();
void AddonRender();
void AddonOptions();

/* globals */
AddonDefinition AddonDef = {};

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
	AddonDef.Signature = -8935; // set to random unused negative integer
	AddonDef.APIVersion = NEXUS_API_VERSION;
	AddonDef.Name = "Encounter Journal";
	AddonDef.Version.Major = 0;
	AddonDef.Version.Minor = 0;
	AddonDef.Version.Build = 0;
	AddonDef.Version.Revision = 1;
	AddonDef.Author = "Ravenheart.8935";
	AddonDef.Description = "Cliffnotes about dungeon, strike and raid bosses.";
	AddonDef.Load = AddonLoad;
	AddonDef.Unload = AddonUnload;
	AddonDef.Flags = EAddonFlags_None;

	/* not necessary if hosted on Raidcore, but shown anyway for the example also useful as a backup resource */
	//AddonDef.Provider = EUpdateProvider_GitHub;
	//AddonDef.UpdateLink = "https://github.com/RaidcoreGG/GW2Nexus-AddonTemplate";

	return &AddonDef;
}

void ReceiveTexture(const char* aIdentifier, Texture* aTexture)
{
	std::string str = aIdentifier;

	if (str == "TEX_UNDER_CONSTRUCTION") texWIP = aTexture;
	if (str == "TEX_VALE_GUARDIAN") texVG = aTexture;
	if (str == "TEX_GORSEVAL") texGors = aTexture;
	if (str == "TEX_SABETHA") texSab = aTexture;
	if (str == "TEX_SLOTHASOR") texSloth = aTexture;
	if (str == "TEX_ZANE") texTrio = aTexture;
	if (str == "TEX_MATTHIAS") texMatthias = aTexture;
	if (str == "TEX_ESCORT") texEscort = aTexture;
	if (str == "TEX_KEEP_CONSTRUCT") texKC = aTexture;
	if (str == "TEX_TWISTED_CASTLE") texTC = aTexture;
	if (str == "TEX_XERA") texXera = aTexture;
	if (str == "TEX_CAIRN") texCairn = aTexture;
	if (str == "TEX_MURSAAT_OVERSEER") texMO = aTexture;
	if (str == "TEX_SAMAROG") texSamarog = aTexture;
	if (str == "TEX_DEIMOS") texDeimos = aTexture;
	if (str == "TEX_SOULLESS_HORROR") texSH = aTexture;
	if (str == "TEX_RIVER_OF_SOULS") texRoS = aTexture;
	if (str == "TEX_EATER_OF_SOULS") texEoS = aTexture;
	if (str == "TEX_BROKEN_KING") texBK = aTexture;
	if (str == "TEX_EYES_OF_FATE") texEyes = aTexture;
	if (str == "TEX_DHUUM") texDhuum = aTexture;
	if (str == "TEX_CONJURED_AMALGAMATE") texCA = aTexture;
	if (str == "TEX_NIKARE") texTL = aTexture;
	if (str == "TEX_QADIM") texQadim = aTexture;
	if (str == "TEX_ADINA") texAdina = aTexture;
	if (str == "TEX_SABIR") texSabir = aTexture;
	if (str == "TEX_QADIM_THE_PEERLESS") texQTP = aTexture;
	if (str == "TEX_WATCHKNIGHT_MARK_II") texOLC = aTexture;
	if (str == "TEX_ICEBROOD_CONSTRUCT") texSP = aTexture;
	if (str == "TEX_KODANS") texVoiceClaw = aTexture;
	if (str == "TEX_FRAENIR_OF_JORMAG") texFoJ = aTexture;
	if (str == "TEX_BONESKINNER") texBS = aTexture;
	if (str == "TEX_WHISPER_OF_JORMAG") texWoJ = aTexture;
	if (str == "TEX_CAPTAIN_MAI_TRIN") texAH = aTexture;
	if (str == "TEX_ANKKA") texXJJ = aTexture;
	if (str == "TEX_MINISTER_LI") texKO = aTexture;
	if (str == "TEX_HARVEST_TEMPLE") texHT = aTexture;
	if (str == "TEX_DAGDA") texCO = aTexture;
	if (str == "TEX_CERUS") texFebe = aTexture;

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

	NexusLink = (NexusLinkData*)APIDefs->GetResource("DL_NEXUS_LINK");
	MumbleLink = (Mumble::Data*)APIDefs->GetResource("DL_MUMBLE_LINK");

	APIDefs->GetTextureOrCreateFromResource("TEX_JOURNAL_ICON", IDB_JOURNAL_ICON, hSelf);
	APIDefs->GetTextureOrCreateFromResource("TEX_JOURNAL_ICON_HOVER", IDB_JOURNAL_ICON_HOVER, hSelf);

	APIDefs->LoadTextureFromResource("TEX_UNDER_CONSTRUCTION", IDB_UNDER_CONSTRUCTION, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_VALE_GUARDIAN", IDB_VALE_GUARDIAN, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_GORSEVAL", IDB_GORSEVAL, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_SABETHA", IDB_SABETHA, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_SLOTHASOR", IDB_SLOTHASOR, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_ZANE", IDB_ZANE, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_MATTHIAS", IDB_MATTHIAS, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_ESCORT", IDB_ESCORT, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_KEEP_CONSTRUCT", IDB_KEEP_CONSTRUCT, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_TWISTED_CASTLE", IDB_TWISTED_CASTLE, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_XERA", IDB_XERA, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_CAIRN", IDB_CAIRN, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_MURSAAT_OVERSEER", IDB_MURSAAT_OVERSEER, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_SAMAROG", IDB_SAMAROG, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_DEIMOS", IDB_DEIMOS, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_SOULLESS_HORROR", IDB_SOULLESS_HORROR, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_RIVER_OF_SOULS", IDB_RIVER_OF_SOULS, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_EATER_OF_SOULS", IDB_EATER_OF_SOULS, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_BROKEN_KING", IDB_BROKEN_KING, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_EYES_OF_FATE", IDB_EYES_OF_FATE, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_DHUUM", IDB_DHUUM, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_CONJURED_AMALGAMATE", IDB_CONJURED_AMALGAMATE, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_NIKARE", IDB_NIKARE, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_QADIM", IDB_QADIM, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_ADINA", IDB_ADINA, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_SABIR", IDB_SABIR, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_QADIM_THE_PEERLESS", IDB_QADIM_THE_PEERLESS, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_WATCHKNIGHT_MARK_II", IDB_WATCHKNIGHT_MARK_II, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_ICEBROOD_CONSTRUCT", IDB_ICEBROOD_CONSTRUCT, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_KODANS", IDB_KODANS, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_FRAENIR_OF_JORMAG", IDB_FRAENIR_OF_JORMAG, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_BONESKINNER", IDB_BONESKINNER, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_WHISPER_OF_JORMAG", IDB_WHISPER_OF_JORMAG, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_CAPTAIN_MAI_TRIN", IDB_CAPTAIN_MAI_TRIN, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_ANKKA", IDB_ANKKA, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_MINISTER_LI", IDB_MINISTER_LI, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_HARVEST_TEMPLE", IDB_HARVEST_TEMPLE, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_DAGDA", IDB_DAGDA, hSelf, ReceiveTexture);
	APIDefs->LoadTextureFromResource("TEX_CERUS", IDB_CERUS, hSelf, ReceiveTexture);

	APIDefs->RegisterKeybindWithString(KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL, ToggleShowWindowEncounterJournal, "CTRL+J");
	APIDefs->RegisterKeybindWithString(KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET, ToggleShowWindowEncounterWidget, "CTRL+SHIFT+J");
	APIDefs->RegisterKeybindWithString(KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET, ToggleLockWindowEncounterWidget, "CTRL+ALT+J");
	RegisterQuickAccessShortcut();

	// Add an options window and a regular render callback
	APIDefs->RegisterRender(ERenderType_Render, AddonRender);
	APIDefs->RegisterRender(ERenderType_OptionsRender, AddonOptions);

	APIDefs->Log(ELogLevel_DEBUG, ADDON_NAME, "<c=#00ff00>Encounter Journal</c> was loaded.");
}

///----------------------------------------------------------------------------------------------------
/// AddonUnload:
/// 	Everything you registered in AddonLoad, you should "undo" here.
///----------------------------------------------------------------------------------------------------
void AddonUnload()
{
	/* let's clean up after ourselves */
	APIDefs->DeregisterRender(AddonRender);
	APIDefs->DeregisterRender(AddonOptions);

	DeregisterQuickAccessShortcut();
	APIDefs->DeregisterKeybind(KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL);
	APIDefs->DeregisterKeybind(KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET);
	APIDefs->DeregisterKeybind(KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET);

	texWIP = nullptr;
	texVG = nullptr;
	texGors = nullptr;
	texSab = nullptr;
	texSloth = nullptr;
	texTrio = nullptr;
	texMatthias = nullptr;
	texEscort = nullptr;
	texKC = nullptr;
	texTC = nullptr;
	texXera = nullptr;
	texCairn = nullptr;
	texMO = nullptr;
	texSamarog = nullptr;
	texDeimos = nullptr;
	texSH = nullptr;
	texRoS = nullptr;
	texEoS = nullptr;
	texBK = nullptr;
	texEyes = nullptr;
	texDhuum = nullptr;
	texCA = nullptr;
	texTL = nullptr;
	texQadim = nullptr;
	texAdina = nullptr;
	texSabir = nullptr;
	texQTP = nullptr;
	texOLC = nullptr;
	texSP = nullptr;
	texVoiceClaw = nullptr;
	texFoJ = nullptr;
	texBS = nullptr;
	texWoJ = nullptr;
	texAH = nullptr;
	texXJJ = nullptr;
	texKO = nullptr;
	texHT = nullptr;
	texCO = nullptr;
	texFebe = nullptr;

	MumbleLink = nullptr;
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
	RenderWindowEncounterJournal();
	RenderWidget();
}

///----------------------------------------------------------------------------------------------------
/// AddonOptions:
/// 	Basically an ImGui callback that doesn't need its own Begin/End calls.
///----------------------------------------------------------------------------------------------------
void AddonOptions()
{
	ImGui::TextDisabled("Boss Widget");
	ImGui::Checkbox("Show Widget", &ShowWindowEncounterWidget);
	//ImGui::SetTooltip("Unlocking the widget lets you move it and change the content.");
	ImGui::Checkbox("Lock Widget", &LockWindowEncounterWidget);
	ImGui::Checkbox("Only show widget inside instances", &OnlyShowWidgetInstanced);
}