#ifndef NEXUS_ENCOUNTER_JOURNAL_SHARED_H
#define NEXUS_ENCOUNTER_JOURNAL_SHARED_H

#include "nexus/Nexus.h"
#include "mumble/Mumble.h"

extern HMODULE hSelf;
extern AddonAPI* APIDefs;
extern const char* ADDON_NAME;
extern NexusLinkData* NexusLink;
extern Mumble::Data* MumbleData;

extern const char* KB_TOGGLE_SHOW_WINDOW_JOURNAL;
extern bool ShowJournalWindow;

#endif // !NEXUS_ENCOUNTER_JOURNAL_SHARED_H