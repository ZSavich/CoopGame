
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"


UCLASS()
class COOPGAME_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
    void AddScore(const float Amount);
};
