#include "gameObject.h"
#include "iconFactory.h"

class GameObjectFactory{
    public:
        static GameObject* RPSObj(RPSType type, bool focus = false);
        static GameObject* rndRPSObj();
};