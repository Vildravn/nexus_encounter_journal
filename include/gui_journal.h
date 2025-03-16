#ifndef NEXUS_ENCOUNTER_JOURNAL_GUI_JOURNAL_H
#define NEXUS_ENCOUNTER_JOURNAL_GUI_JOURNAL_H

#include <string>

enum JournalSubmenu
{
	Main,
	OpenWorld,
	Dungeons,
	Fractals,
	Raids,
	Strikes
};

extern void ClearSelections();
extern void RenderJournal();
extern void RenderJournalZoneMenu(std::string type);
extern void RenderJournalBossMenu();
extern void RenderJournalStrikeBossMenu(std::string type);
extern void RenderMainSubmenu();
extern void RenderRaidSubmenu();
extern void RenderStrikeSubmenu();
extern void RenderContent();
extern void ToggleShowJournalWindow(const char* keybindIdentifier, bool isRelease);

extern std::string selected_zone;
extern std::string selected_boss;
extern std::string selected_strike;
extern JournalSubmenu active_screen;

#endif // !NEXUS_ENCOUNTER_JOURNAL_GUI_JOURNAL_H