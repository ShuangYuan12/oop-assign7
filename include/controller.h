#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <termios.h>
#include <vector>
#include "view.h"
#include "gameObject.h"
#include "RPSGameObject.h"

class Controller{

public:
       
    Controller(View&);
    void run();
private:

    void handleInput(int);
    void checkCollision();
    void changePlayer();
    bool checkGameOver();
    void update();

    // Model
    std::vector<GameObject*> _objs;

    // View
    View& _view;

    int _currentPlayerIndex = 0;
    int _currentCount[3] = {0, 0, 0};
};

static struct termios old_termios, new_termios;
void reset_terminal();
void configure_terminal();
int read_input();

#endif
