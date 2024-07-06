#include "shared.h"

const char* ADDON_NAME = "Encounter Journal";
AddonAPI* APIDefs			= nullptr;
NexusLinkData* NexusLink	= nullptr;
Mumble::Data* MumbleLink	= nullptr;
HMODULE hSelf				= nullptr;

const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL = "KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL";
const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET = "KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET";
const char* KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET = "KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET";

bool ShowWindowEncounterJournal = false;
bool ShowWindowEncounterWidget = false;
bool LockWindowEncounterWidget = true;
bool OnlyShowWidgetInstanced = true;

Texture* texWIP = nullptr;
Texture* texVG = nullptr;
Texture* texGors = nullptr;
Texture* texSab = nullptr;
Texture* texSloth = nullptr;
Texture* texTrio = nullptr;
Texture* texMatthias = nullptr;
Texture* texEscort = nullptr;
Texture* texKC = nullptr;
Texture* texTC = nullptr;
Texture* texXera = nullptr;
Texture* texCairn = nullptr;
Texture* texMO = nullptr;
Texture* texSamarog = nullptr;
Texture* texDeimos = nullptr;
Texture* texSH = nullptr;
Texture* texRoS = nullptr;
Texture* texEoS = nullptr;
Texture* texBK = nullptr;
Texture* texEyes = nullptr;
Texture* texDhuum = nullptr;
Texture* texCA = nullptr;
Texture* texTL = nullptr;
Texture* texQadim = nullptr;
Texture* texAdina = nullptr;
Texture* texSabir = nullptr;
Texture* texQTP = nullptr;
Texture* texOLC = nullptr;
Texture* texSP = nullptr;
Texture* texVoiceClaw = nullptr;
Texture* texFoJ = nullptr;
Texture* texBS = nullptr;
Texture* texWoJ = nullptr;
Texture* texAH = nullptr;
Texture* texXJJ = nullptr;
Texture* texKO = nullptr;
Texture* texHT = nullptr;
Texture* texCO = nullptr;
Texture* texFebe = nullptr;