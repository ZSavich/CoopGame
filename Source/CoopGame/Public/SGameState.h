
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(Blueprintable)
enum class EWaveState : uint8
{
    EWS_WaitingToStart      UMETA(DisplayName="WaitingToStart"),
    EWS_WaveInProgress      UMETA(DisplayName="WaveInProgress"),
    EWS_WaitingToComplete   UMETA(DisplayName="WaitingToComplete"),
    EWS_WaveCompleted       UMETA(DisplayName="WaveCompleted"),
    EWS_GameOver            UMETA(DisplayName="GameOver"),

    EWS_MAX                 UMETA(DisplayName="MaxDefault")
};

UCLASS()
class COOPGAME_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
    void SetWaveState(const EWaveState NewState);

protected:
    UPROPERTY(ReplicatedUsing=OnRep_WaveState, VisibleAnywhere, BlueprintReadOnly, Category="Wave")
    EWaveState WaveState;

    UFUNCTION()
    void OnRep_WaveState(const EWaveState OldState) const;

    void WaveStateChanged(const EWaveState NewState, const EWaveState OldState) const;
};
