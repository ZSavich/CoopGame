
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;

UCLASS()
class COOPGAME_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    ASGameMode();

    virtual void StartPlay() override;
    virtual void Tick(float DeltaSeconds) override;
	
protected:
    FTimerHandle SpawnBotsTimer;
    FTimerHandle BetweenWavesTimer;

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
    
    void CheckWaveState();
    void CheckAnyPlayerAlive();
    
    void OnSpawnBotsTimer();

    void GameOver();
    
    void SetWaveState(const EWaveState& NewState) const;
};
