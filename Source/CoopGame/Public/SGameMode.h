
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"


UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    ASGameMode();

    virtual void StartPlay() override;
	
protected:
    FTimerHandle SpawnBotsTimer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Waves")
    int32 NrOfBotsToSpawn;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Waves")
    int32 WaveCount;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Waves")
    float TimerBetweenWaves;

    int32 NrOfSpawnedBots;
    
    UFUNCTION(BlueprintImplementableEvent, Category="Waves")
    void SpawnBot();
    void StartWave();
    void EndWave();
    void PrepareForNextWave();
    void OnSpawnBotsTimer();
    
};
