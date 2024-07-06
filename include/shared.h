#ifndef NEXUS_ENCOUNTER_JOURNAL_SHARED_H
#define NEXUS_ENCOUNTER_JOURNAL_SHARED_H

#include "nexus/Nexus.h"
#include "mumble/Mumble.h"

extern const char* ADDON_NAME;
extern AddonAPI *APIDefs;
extern NexusLinkData *NexusLink;
extern Mumble::Data *MumbleLink;
extern HMODULE hSelf;

extern const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL;
extern const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET;
extern const char* KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET;

extern bool ShowWindowEncounterJournal;
extern bool ShowWindowEncounterWidget;
extern bool LockWindowEncounterWidget;
extern bool OnlyShowWidgetInstanced;

#endif // !NEXUS_ENCOUNTER_JOURNAL_SHARED_H
