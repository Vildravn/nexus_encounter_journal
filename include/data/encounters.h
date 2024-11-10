#ifndef NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H
#define NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H

#include <string>

inline std::string str_encounters_json = R"###({
	"1062": {
		"name": "Spirit Vale",
		"subtitle": "Wing 1",
		"type": "raid",
		"bosses": {
			"Vale Guardian": {
				"desc": "**Health:** 22 021 440\n**Enrage Timer:** 8 minutes\n**Enrage Effect:** Gains ![Enraged](/961403.png) [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%\n**Fixation Mechanics:** Highest toughness\n\n"
			},
			"Gorseval": {
				"desc": "**Health:** 21 628 200\n**Enrage Timer**: 7 minute\n**Enrage Effect:** Removes all updrafts and gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%.\n**Fixation Mechanics:** Highest toughness\n\n"
			},
			"Sabetha": {}
		}
	},
	"1149": {
		"name": "Salvation Pass",
		"subtitle": "Wing 2",
		"type": "raid",
		"bosses": {
			"Slothasor": {},
			"Prison Camp": {},
			"Matthias": {}
		}
	},
	"1332": {
		"name": "Icebrood Construct",
		"subtitle": "Shiverpeaks Pass",
		"type": "ibs_strike",
		"bosses": {
			"Icebrood Construct": {
				"desc": "**Health**: 11 698 890"
			}
		}
	},
	"1432": {
		"name": "Mai Trin",
		"subtitle": "Aetherblade Hideout",
		"type": "eod_strike",
		"bosses": {
			"Mai Trin": {
				"desc": "**Health**: 5 898 600"
			}
		}
	}
})###";

#endif // !NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H