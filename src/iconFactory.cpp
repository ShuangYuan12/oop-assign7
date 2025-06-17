#include "iconFactory.h"
#include "icon.h"

Icon IconFactory::RPSIcon(RPSType type, bool focus)
{
    switch (type)
    {
    case ROCK:
        if(focus)
            return {{Cell(Color::RED, "[R"), Cell(Color::RED, "R]")}};
        else
            return {{Cell(Color::RED, "R"), Cell(Color::RED, "R")}};
    case PAPER:
        return {{Cell(Color::GREEN, "P"), Cell(Color::GREEN, "P")}};
    case SCISSORS:
        return {{Cell(Color::BLUE, "S"), Cell(Color::BLUE, "S")}};
    }
}