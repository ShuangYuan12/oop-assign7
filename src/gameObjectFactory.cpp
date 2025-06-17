#include "gameObjectFactory.h"
#include "environment.h"

int map[20][20] = {0};

GameObject *GameObjectFactory::RPSObj(RPSType type, bool focus)
{
    int posX, posY;

    for (int i = 0; i < 10; i++)
    {
        posX = rand() % GAME_WINDOW_WIDTH;
        posY = rand() % GAME_WINDOW_HEIGHT;

        if (map[posY][posX] == 0)
        {
            map[posY][posX] = 1;
            map[posY][posX + 1] = 1;
            break;
        }
    }

    return new RPSGameObject(Position{posX, posY}, IconFactory::RPSIcon(type, focus), type, focus);
}

GameObject *GameObjectFactory::rndRPSObj()
{
    int type = rand() % 3;
    return RPSObj(static_cast<RPSType>(type));
}