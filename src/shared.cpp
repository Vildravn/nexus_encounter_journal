#include "shared.h"

const char* ADDON_NAME = "Encounter Journal";
AddonAPI* APIDefs			= nullptr;
NexusLinkData* NexusLink	= nullptr;
HMODULE hSelf				= nullptr;

const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL = "KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL";
const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET = "KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET";
const char* KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET = "KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET";

bool ShowWindowEncounterJournal = false;
bool ShowWindowEncounterWidget = false;
bool LockWindowEncounterWidget = true;