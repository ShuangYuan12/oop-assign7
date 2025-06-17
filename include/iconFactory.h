#include "icon.h"
#include "RPSGameObject.h"

class IconFactory{
    public:
        static Icon RPSIcon(RPSType type, bool focus = false);
};