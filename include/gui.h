#ifndef NEXUS_ENCOUNTER_JOURNAL_GUI_H
#define NEXUS_ENCOUNTER_JOURNAL_GUI_H

#include "nexus/Nexus.h"

enum SquadMarker {
    Arrow,
    Circle,
    Heart,
    Spiral,
    Square,
    Star,
    Triangle,
    X
};

void ToggleShowWindowEncounterJournal(const char* keybindIdentifier, bool isRelease);
void ToggleShowWindowEncounterWidget(const char* keybindIdentifier, bool isRelease);
void ToggleLockWindowEncounterWidget(const char* keybindIdentifier, bool isRelease);

void RegisterQuickAccessShortcut(void);
void DeregisterQuickAccessShortcut(void);

void RenderWindowEncounterJournal(void);
void RenderDhuumWidget(void);

Texture* GetBossTexture(std::string boss_name);
void DrawMarker(SquadMarker marker);

#endif // !NEXUS_ENCOUNTER_JOURNAL_GUI_H