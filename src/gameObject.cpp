#include "gameObject.h"
#include "view.h"
#include "environment.h"

Position GameObject::getPosition() const{
    return _pos;
}

Icon GameObject::getIcon() const{
    return _icon;
}

void GameObject::setDirection(Direction dir){
    _dir = dir;
}

void GameObject::update(){
    if (_dir == UP && _pos.y() > 0){
        _pos.y()--;
    }
    else if (_dir == DOWN && _pos.y() < GAME_WINDOW_HEIGHT - 1){
        _pos.y()++;
    }
    else if (_dir == LEFT && _pos.x() > 0){
        _pos.x()--;
    }
    else if (_dir == RIGHT && _pos.x() < GAME_WINDOW_WIDTH - 2){
        _pos.x()++;
    }
}