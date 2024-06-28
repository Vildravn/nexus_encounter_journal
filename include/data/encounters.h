#ifndef NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H
#define NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H

std::string str_encounters_json = R"###({
    "Spirit Vale": {
        "Vale Guardian": {
            "desc": "The Vale Guardian is the first boss in the Spirit Vale raid wing. It is an elemental of mysterious origin, powered or summoned by the pylons in the Vale.",
            "tank": "Tanking is toughness based",
            "heal": "Unless group dps is low, watch out for damage spikes from greens"
        },
        "Gorseval": {
            "desc": "Gorseval the Multifarious is the second boss in the Spirit Vale raid wing. It is a being formed from the angered ghosts trapped within the Spirit Woods. "
        },
        "Sabetha": {
            "desc": "Sabetha the Saboteur is the third and final boss in the Spirit Vale raid wing. She the leader of the bandits found in the Vale. "
        }
    },
    "Salvation Pass": {
        "Slothasor": {
            "desc": "Slothasor serves as the first boss of the Salvation Pass raid wing. It is a giant sloth that was affected and mutated by the heavy concentration of magic released by the Elder Dragon Mordremoth's death or the White Mantle's experiments, turning into a sloth-slub hybrid.",
            "comm": "Four people need to pick up mushrooms",
            "tank": "Tanking is based on a random focus"
        },
        "Prison Camp": {
            "desc": "This is a temporary description."
        },
        "Matthias": {}
    },
    "Stronghold of the Faithful": {
        "Escort": {},
        "Keep Construct": {},
        "Twisted Castle": {},
        "Xera": {}
    }
})###";

#endif // !NEXUS_ENCOUNTER_JOURNAL_ENCOUNTERS_H