#include <iostream>
#include <ctime>
#include <thread>
#include <unistd.h>
#include <termios.h>

#include "environment.h"
#include "controller.h"
#include "gameObjectFactory.h"


Controller::Controller(View& view) : _view(view){
    _objs.emplace_back(GameObjectFactory::RPSObj(ROCK, true));
    _currentCount[static_cast<int>(RPSType::ROCK)] = 1;
    
    for (int i = 0; i < 25; i++)
    {
        GameObject* obj = GameObjectFactory::rndRPSObj();
        _objs.emplace_back(obj);

        RPSType t = dynamic_cast<RPSGameObject*>(obj)->getType();
        _currentCount[static_cast<int>(t)]++;
    }
}

void Controller::run() {
    // initial setup
    std::cin.tie(0);
    std::ios::sync_with_stdio(0);
    configure_terminal();

    // init state
    int input = -1;
    clock_t start, end;
    
    // Main loop
    while (true) {
        start = clock();
        // game loop goes here
        input = read_input();

        // ESC to exit program
        if(input==27)break;

        this->handleInput(input);

        _view.resetLatest();
        for(GameObject* obj : _objs) 
        {
            obj->update();
        }

        this->checkCollision();

        for (GameObject* obj : _objs) {
            _view.updateGameObject(obj);
        }

        _view.render();

        if(checkGameOver())break;

        end = clock();

        // frame rate normalization
        double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
        if (time_taken > SPF) continue;
        int frameDelay = int((SPF - time_taken) * 1000); // 0.1 seconds
        if(frameDelay > 0) std::this_thread::sleep_for(std::chrono::milliseconds(frameDelay)); // frame delay
    }

}

void Controller::handleInput(int keyInput){

    // If there is no input, do nothing.
    if(keyInput==-1)return;
    
    // handle key events.
    RPSGameObject* player = dynamic_cast<RPSGameObject*>(_objs[_currentPlayerIndex]);

    switch (keyInput) {
        case 9: // Tab
            changePlayer();
            break;
        case 'w': // 上
        case 'W':
            player->setDirection(UP);
            break;
        case 's': // 下
        case 'S':
            player->setDirection(DOWN);
            break;
        case 'a': // 左
        case 'A':
            player->setDirection(LEFT);
            break;
        case 'd': // 右
        case 'D':
            player->setDirection(RIGHT);
            break;
        default:
            break;
    }
}

void Controller::checkCollision(){
    int objSize = _objs.size();

    for(int i = 0; i < objSize; i++) {
        RPSGameObject* obj1 = dynamic_cast<RPSGameObject*>(_objs[i]);
        for (int j = i + 1; j < objSize; j++) {
            RPSGameObject* obj2 = dynamic_cast<RPSGameObject*>(_objs[j]);
            if (obj1->intersect(obj2)) {
                RPSType obj1TypeBefore = obj1->getType();
                RPSType obj2TypeBefore = obj2->getType();

                obj1->onCollision(obj2);
                obj2->onCollision(obj1);

                if(obj1->getType() != obj1TypeBefore){
                    _currentCount[static_cast<int>(obj1TypeBefore)]--;
                    _currentCount[static_cast<int>(obj1->getType())]++;
                }
                if(obj2->getType() != obj2TypeBefore){
                    _currentCount[static_cast<int>(obj2TypeBefore)]--;
                    _currentCount[static_cast<int>(obj2->getType())]++;
                }
            }
        }
    }

    // check player
    RPSGameObject* currentPlayer = dynamic_cast<RPSGameObject*>(_objs[_currentPlayerIndex]);
    if(currentPlayer->getType() != RPSType::ROCK)
        changePlayer();
}

void Controller::changePlayer(){
    RPSGameObject* currentPlayer = dynamic_cast<RPSGameObject*>(_objs[_currentPlayerIndex]);
    currentPlayer->setFocus(false);

    int objSize = _objs.size();
    for (int i = 1; i < objSize; i++)
    {
        int nextIndex = (_currentPlayerIndex + i) % objSize;
        RPSGameObject* nextPlayer = dynamic_cast<RPSGameObject*>(_objs[nextIndex]);
        if(nextPlayer->getType() == RPSType::ROCK){
            nextPlayer->setFocus(true);
            nextPlayer->setDirection(NONE);
            _currentPlayerIndex = nextIndex;
            break;
        }
    }

    if(currentPlayer->getType() != RPSType::ROCK) {
        currentPlayer->setFocus(false);
    }
}

bool Controller::checkGameOver() {
    if(_currentCount[0] == 0 || _currentCount[1] == 0 || _currentCount[2] == 0){
        std::cout << "Game Over. ";
        if(_currentCount[static_cast<int>(RPSType::PAPER)] == 0) {
            std::cout << "Rock wins!" << std::endl;
        } else if(_currentCount[static_cast<int>(RPSType::ROCK)] == 0) {
            std::cout << "Scissors wins!" << std::endl;
        } else if(_currentCount[static_cast<int>(RPSType::SCISSORS)] == 0) {
            std::cout << "Paper wins!" << std::endl;
        }
        
        return true;
    }

    return false;
}

void Controller::update(){

}
void reset_terminal() {
    printf("\e[m"); // reset color changes
    printf("\e[?25h"); // show cursor
    fflush(stdout);
    tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
}

// terminal initial configuration setup
void configure_terminal() {
    tcgetattr(STDIN_FILENO, &old_termios);
	new_termios = old_termios; // save it to be able to reset on exit
    
    new_termios.c_lflag &= ~(ICANON | ECHO); // turn off echo + non-canonical mode
    new_termios.c_cc[VMIN] = 0;
    new_termios.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    printf("\e[?25l"); // hide cursor
    std::atexit(reset_terminal);
}


int read_input() {
    fflush(stdout);
   	char buf[4096]; // maximum input buffer
	int n = read(STDIN_FILENO, buf, sizeof(buf));
    return n > 0 ? buf[n - 1] : -1;
}

