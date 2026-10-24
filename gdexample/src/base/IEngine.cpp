#include "IEngine.h"

void IEngine::SetGame(sptr<IGame> g)
{
    game =g;
}

sptr<IGame> IEngine::GetGame()
{
    return game;
}
