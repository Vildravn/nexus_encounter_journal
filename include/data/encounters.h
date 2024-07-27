#ifndef NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H
#define NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H

std::string str_encounters_json = R"###({
    "Spirit Vale": {
        "Vale Guardian": {
            "desc": "**Health:** 22 021 440\n**Enrage Timer:** 8 minutes\n**Enrage Effect:** Gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%\n**Fixation Mechanics:** Highest toughness\n\n",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Spirit_Vale#Vale_Guardian)\n[Snowcrows](https://snowcrows.com/guides/raids/spirit-vale/vale-guardian)"
        },
        "Gorseval": {
            "desc": "**Health:** 21 628 200\n**Enrage Timer**: 7 minute\n**Enrage Effect:** Removes all updrafts and gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%.\n**Fixation Mechanics:** Highest toughness\n\n",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Spirit_Vale#Gorseval_the_Multifarious)\n[Snowcrows](https://snowcrows.com/guides/raids/spirit-vale/gorseval)"
        },
        "Sabetha": {
            "desc": "**Health:** 34 015 256\n**Enrage Timer:** 9 minutes\n**Enrage Effect:** All four cannons become active and gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(500%25)), increasing outgoing damage by 500%\n**Fixation Mechanics:** None\n\nSee the Encounter Widget for bomb timings and locations",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Spirit_Vale#Sabetha_the_Saboteur)\n[Snowcrows](https://snowcrows.com/guides/raids/spirit-vale/sabetha)"
        }
    },
    "Salvation Pass": {
        "Slothasor": {
            "desc": "**Health:** 18 973 828\n**Enrage Timer:** 7 minutes\n**Enrage Effect:** Imbued Mushrooms stop spawning and gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)), increasing damage dealt by 200%\n**Fixation Mechanics:** Random\n\nSee the Encounter Widget for the mushroom timings and locations",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Salvation_Pass#Slothasor_(Boss))\n[Snowcrows](https://snowcrows.com/guides/raids/salvation-pass/slothasor)"
        },
        "Prison Camp": {
            "desc": "See the Encounter Widget for timings and locations",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Salvation_Pass#Protect_the_caged_prisoners)\n[Snowcrows](https://snowcrows.com/guides/raids/salvation-pass/bandit-trio)"
        },
        "Matthias": {
            "desc": "**Health:** 25 953 840\n**Enrage Timer:** 10 minutes\n**Enrage Effect:** Gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25), increasing damage dealt by 200%. Will no longer use Blood Sacrifice.\n**Fixation Mechanics:** Moves towards furthest player\n\n",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Salvation_Pass#Matthias_Gabrel)\n[Snowcrows](https://snowcrows.com/guides/raids/salvation-pass/matthias-gabrel)"
        }
    },
    "Stronghold of the Faithful": {
        "Escort": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Keep Construct": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Twisted Castle": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Xera": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    },
    "Bastion of the Penitent": {
        "Cairn the Indomitable": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Mursaat Overseer": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Samarog": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Deimos": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    },
    "Hall of Chains": {
        "Soulless Horror": {
            "desc": "**Health:** 35 391 600\n**Enrage Timer:** 8 minutes\n**Enrage Effect:** [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(200%25)) - Increases damage dealt by 200%\n**Fixation Mechanic:** Player that activates the [Issue Challenge](https://wiki.guildwars2.com/wiki/Issue_Challenge) Special Action Key\n\nTwo players will need to pick up the red and green rune, they will be the designated tanks for the fight. The boss should be tanked facing away from the group as it does a lot of damage.",
            "tank": "Use [Issue Challenge](https://wiki.guildwars2.com/wiki/Issue_Challenge) (Special Action Key) to \"taunt\" the boss. First \"taunt\" should be used when the other tank has 3 stacks of [Necrosis](https://wiki.guildwars2.com/wiki/Necrosis). After that, it can be used on cooldown.",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Hall_of_Chains#Soulless_Horror)\n[Snowcrows](https://snowcrows.com/guides/raids/hall-chains/soulless-horror)"
        },
        "River of Souls": {
            "desc": "Protect Desmina. The general strategy is for two to three players to stay with Desmina, to grant her healing, Superspeed and Aegis to protect from Hollowed Bombers.\nThe rest of the squad should have multiple healers to outheal the aura damage, and some DPS to clear out the Enervators, and run ahead of Desmina.",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Hall_of_Chains#River_of_Souls)\n[Snowcrows](https://snowcrows.com/guides/raids/hall-chains/river-of-souls)"
        },
        "Eater of Souls": {
            "desc": "**Health:** 1 720 425\n**Fixation Mechanics:** Highest toughness\n\nKeep an eye on his defiance bar to break it as fast as possible. The group should largely stay close together, except when they must split to gather orbs of light or take greens. Collecting a 2nd light orb when you are holding one will destroy the 2nd orb.",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Hall_of_Chains#Statue_of_Death)\n[Snowcrows](https://snowcrows.com/guides/raids/hall-chains/statues-of-grenth)"
        },
        "Broken King": {
            "desc": "**Health:** 9 831 000\n**Enrage Timer:** 3 minutes 30 seconds\n**Enrage Effect:** Defeats all players\n**Fixation Mechanics:** Highest toughness\n\nEvery 2% of the Broken King's health, he spwans a green circle with a slowly descending ice shard above it. A squad member must be standing in the green circle when the ice touches the ground or the whole squad takes a massive amount of damage. Absorbing a shard gives the player a stack of [Frozen Wind](https://wiki.guildwars2.com/wiki/Frozen_Wind) that lasts 45 seconds. Players can only have 3 stacks of [Frozen Wind](https://wiki.guildwars2.com/wiki/Frozen_Wind) at most.",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Hall_of_Chains#Statue_of_Ice)\n[Snowcrows](https://snowcrows.com/guides/raids/hall-chains/statues-of-grenth)"
        },
        "Eyes of Judgment and Fate": {
            "desc": "The squad splits, with 4 people going to the north side and 4 people going to the south. Two people will be on top of the wall in the middle.\nThe two players in the middle will throw orbs to the north and south groups. Usually each player is dedicated to throwing to one side only, taking turns with the middle orb. Mobility skills are recommended.\nThe 4 players on each side designate one player to use the orbs, to keep the eye permanently stunned using the [Flare](https://wiki.guildwars2.com/wiki/Flare) skill. When there are 3 orbs ready on their side, the group can engage.",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Hall_of_Chains#Statue_of_Darkness)\n[Snowcrows](https://snowcrows.com/guides/raids/hall-chains/statues-of-grenth)"
        },
        "Dhuum": {
            "desc": "**Health:** 32 000 000 (40 000 000 if the Challenge Mote is active)\n**Enrage Timer:** 100%-10% HP: 10 minutes | 10-10% HP: 1 minute 45 seconds\n**Enrage Effect:** 100%-10% HP: Gains [Enraged](https://wiki.guildwars2.com/wiki/Enraged_(500%25)), increasing outgoing damage by 500% | 10-10% HP: Defeats all players\n**Fixation Mechanics:** Higest tougnes\n\nSee the Encounter Widget for the timings and positions of Greens.",
            "links": "[Wiki](https://wiki.guildwars2.com/wiki/Hall_of_Chains#The_Voice_in_the_Void)\n[Snowcrows](https://snowcrows.com/guides/raids/hall-chains/dhuum)"
        }
    },
    "Mythwright Gambit": {
        "Conjured Amalgamate": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Twin Largos": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Qadim": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    },
    "The Key of Ahdashim": {
        "Cardinal Adina": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Cardinal Sabir": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Qadim the Peerless": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    },
    "Core Tyria Strikes": {
        "Old Lion's Court": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    },
    "Icebrood Saga Strikes": {
        "Shiverpeaks Pass": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Voice and Claw of the Fallen": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Fraenir of Jormag": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Boneskinner": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Whisper of Jormag": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    },
    "End of Dragons Strikes": {
        "Aetherblade Hideout": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Xunlai Jade Junkyard": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Kaineng Overlook": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Harvest Temple": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    },
    "Secrets of the Obscure Strikes": {
        "Cosmic Observatory": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        },
        "Temple of Febe": {
            "desc": "**Health:** \n**Enrage Timer:** \n**Enrage Effect:** \n**Fixation Mechanics:** \n\n",
            "links": "[Wiki]()\n[Snowcrows]()"
        }
    }
})###";

#endif // !NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H