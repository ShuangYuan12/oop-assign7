#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>
#include "unit.h"
#include "icon.h"
#include <iostream>
class GameObject{

public:
    GameObject(Position pos, Icon icon): _pos(pos), _dir(Direction::NONE), _icon(icon) {};
    Position getPosition() const;
    Icon getIcon() const;
    void setDirection(Direction dir);
    virtual void update();

protected:
    Position _pos;
    Direction _dir;
    Icon _icon;
};

#endif
