#ifndef NEXUS_ENCOUNTER_JOURNAL_SHARED_H
#define NEXUS_ENCOUNTER_JOURNAL_SHARED_H

#include "nexus/Nexus.h"
#include "mumble/Mumble.h"

extern const char* ADDON_NAME;
extern AddonAPI* APIDefs;
extern NexusLinkData* NexusLink;
extern Mumble::Data* MumbleLink;
extern Mumble::Identity* MumbleIdentity;
extern HMODULE hSelf;

extern const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_JOURNAL;
extern const char* KB_TOGGLE_SHOW_WINDOW_ENCOUNTER_WIDGET;
extern const char* KB_TOGGLE_LOCK_WINDOW_ENCOUNTER_WIDGET;

extern bool ShowWindowEncounterJournal;
extern bool ShowWindowEncounterWidget;
extern bool LockWindowEncounterWidget;
extern bool OnlyShowWidgetInstanced;
extern bool ShowWidgetBackground;
extern float WidgetBackgroundOpacity;

extern Texture* texWIP;
extern Texture* texVG;
extern Texture* texGors;
extern Texture* texSab;
extern Texture* texSloth;
extern Texture* texTrio;
extern Texture* texMatthias;
extern Texture* texEscort;
extern Texture* texKC;
extern Texture* texTC;
extern Texture* texXera;
extern Texture* texCairn;
extern Texture* texMO;
extern Texture* texSamarog;
extern Texture* texDeimos;
extern Texture* texSH;
extern Texture* texRoS;
extern Texture* texEoS;
extern Texture* texBK;
extern Texture* texEyes;
extern Texture* texDhuum;
extern Texture* texCA;
extern Texture* texTL;
extern Texture* texQadim;
extern Texture* texAdina;
extern Texture* texSabir;
extern Texture* texQTP;
extern Texture* texOLC;
extern Texture* texSP;
extern Texture* texVoiceClaw;
extern Texture* texFoJ;
extern Texture* texBS;
extern Texture* texWoJ;
extern Texture* texAH;
extern Texture* texXJJ;
extern Texture* texKO;
extern Texture* texHT;
extern Texture* texCO;
extern Texture* texFebe;

extern Texture* texArrow;
extern Texture* texCircle;
extern Texture* texHeart;
extern Texture* texSpiral;
extern Texture* texSquare;
extern Texture* texStar;
extern Texture* texTriangle;
extern Texture* texX;

extern Texture* texWidgetBG;

#endif // !NEXUS_ENCOUNTER_JOURNAL_SHARED_H
