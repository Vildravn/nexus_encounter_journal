#include "shared.h"

const char* ADDON_NAME = "EncounterJournal";
AddonAPI* APIDefs			= nullptr;
NexusLinkData* NexusLink	= nullptr;
Mumble::Data* MumbleLink	= nullptr;
Mumble::Identity* MumbleIdentity = nullptr;
HMODULE hSelf				= nullptr;

const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL = "KB_EJ_TOGGLE_SHOW_JOURNAL";
const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET = "KB_EJ_TOGGLE_SHOW_WIDGET";
const char* KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET = "KB_EJ_TOGGLE_LOCK_WIDGET";

bool ShowWindowEncounterJournal = false;
bool ShowWindowEncounterWidget = true;
bool LockWindowEncounterWidget = true;
bool OnlyShowWidgetInstanced = true;
bool ShowWidgetBackground = true;
float WidgetBackgroundOpacity = 1.0;

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

Texture* texArrow = nullptr;
Texture* texCircle = nullptr;
Texture* texHeart = nullptr;
Texture* texSpiral = nullptr;
Texture* texSquare = nullptr;
Texture* texStar = nullptr;
Texture* texTriangle = nullptr;
Texture* texX = nullptr;

Texture* texWidgetBG = nullptr;
