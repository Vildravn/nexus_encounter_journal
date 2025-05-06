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
        "icon": "/1301792.png",
        "requirements": "  * Tank - Highest Toughness\n  * 2 Condition DPS players",
        "overview": "  * At 66% and 33% the Vale Guardian becomes invulnerable and spawns 3 guardians.\n    * Condition DPS players go to [red](image:Red Guardian), everyone else goes to [blue](text:Blue Guardian) and one ranged player hits [green](text: Green Guardian) to pull it to main group\n  * After 66% the boss starts using [Magic Storm](text:Magic Storm) that needs to be CCed. One section of the area will become [Glowing Section](text:Glowing Section).\n  * After 33% there are 2 [Glowing Section](text:Glowing Section).",
        "tank": "  * Face boss away from the group\n  * Tank closer to the edge of the line between two sections\n  * After 33% tank closer to the middle to be able to quickly change sections",
        "heal": "  * Pay attention to the whole arena\n  * Barrier up for or out-heal [Distributed Magic](text:Description for Distributed Magic)",
        "dps": "  * Sidestep or dodge out of [Unstable Magic Spike](text:Description for Unstable Magic Spike)\n  * Try to push [Seekers](text:Description for Seekers) away from the group\n  * CC Vale Guardian as soon as [Magic Storm](text:Description for Magic Storm) starts",
        "links": "  * [Snowcrows](https://snowcrows.com/guides/raids/spirit-vale/vale-guardian) - detailed written guide\n  * [Hardstuck](https://hardstuck.gg/gw2/guides/w1-spirit-vale/vale-guardian/) - detailed written guide\n  * [Mukluk](https://www.youtube.com/watch?v=qtzMDCVHlLg) - summarized video guide"
      },
      "Gorseval": {
        "desc": "Health: 21 628 200\nEnrage Timer: 7 minutes\nEnrage Effect: Removes all updrafts and gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%.\nFixation Mechanics: Highest toughness",
        "icon": "/1301787.png"
      },
      "Sabetha": {
        "desc": "",
        "icon": "/1301795.png"
      }
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
        "desc": "**Health**: 11 698 890",
        "icon": "/2221486.png"
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