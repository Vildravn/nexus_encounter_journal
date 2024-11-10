#include "shared.h"

HMODULE hSelf				= nullptr;
AddonAPI* APIDefs			= nullptr;
const char* ADDON_NAME 		= "EncounterJournal";
NexusLinkData* NexusLink	= nullptr;
Mumble::Data* MumbleData	= nullptr;

const char* KB_TOGGLE_SHOW_WINDOW_JOURNAL = "KB_TOGGLE_SHOW_WINDOW_JOURNAL";
bool ShowJournalWindow = true;