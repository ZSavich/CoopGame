

#include "SPlayerState.h"

void ASPlayerState::AddScore(const float Amount)
{
    SetScore(GetScore() + Amount);
}
