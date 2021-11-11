

#include "SGameMode.h"

ASGameMode::ASGameMode()
{
    NrOfBotsToSpawn = 4;
    WaveCount = 2;
    TimerBetweenWaves = 5.f;
    NrOfSpawnedBots = 0;
}

void ASGameMode::StartPlay()
{
    Super::StartPlay();
    PrepareForNextWave();
}

void ASGameMode::PrepareForNextWave()
{
    if(--WaveCount < 0) return;
    FTimerHandle BetweenWavesTimer;
    GetWorldTimerManager().SetTimer(BetweenWavesTimer, this, &ASGameMode::StartWave, TimerBetweenWaves);
}

void ASGameMode::StartWave()
{
    GetWorldTimerManager().SetTimer(SpawnBotsTimer, this, &ASGameMode::OnSpawnBotsTimer,1.f, true, 0.f);
}

void ASGameMode::OnSpawnBotsTimer()
{
    if(++NrOfSpawnedBots >= NrOfBotsToSpawn) EndWave();
    SpawnBot();
}


void ASGameMode::EndWave()
{
    GetWorldTimerManager().ClearTimer(SpawnBotsTimer);
    NrOfSpawnedBots = 0;
    PrepareForNextWave();
}