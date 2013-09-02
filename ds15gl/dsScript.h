#include <string>
#include "dsVector2f.h"

enum ScriptType {
    next_turn,
    soldier_move,
    soldier_fight,
    soldier_die,
};

class Script {
    ScriptType type;
    std::string object_name;

};