

#include "SGameMode.h"
#include "EngineUtils.h"
#include "SCharacter.h"
#include "STrackerBot.h"
#include "SGameState.h"
#include "SPlayerState.h"

ASGameMode::ASGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 1.f;

    GameStateClass = ASGameState::StaticClass();
    PlayerStateClass = ASPlayerState::StaticClass();
    
    NrOfBotsToSpawn = 4;
    WaveCount = 2;
    TimerBetweenWaves = 5.f;
    NrOfSpawnedBots = 0;
    PointsByKilling = 20.f;
    AdvancedAIRate = .4;

    OnActorKilled.AddDynamic(this, &ASGameMode::OnActorKilledHandle);
}

void ASGameMode::StartPlay()
{
    Super::StartPlay();
}

void ASGameMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    CheckWaveState();
    CheckAnyPlayerAlive();
}

void ASGameMode::PrepareForNextWave()
{
    if(--WaveCount < 0) return;
    GetWorldTimerManager().SetTimer(BetweenWavesTimer, this, &ASGameMode::StartWave, TimerBetweenWaves);
    SetWaveState(EWaveState::EWS_WaitingToStart);
    RestartDeadPlayers();
}

void ASGameMode::RestartDeadPlayers()
{
    for(auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        const auto CurrentPlayerController = It->Get();
        if(CurrentPlayerController && !CurrentPlayerController->GetPawn()) RestartPlayer(It->Get());
    }
}

void ASGameMode::StartWave()
{
    GetWorldTimerManager().SetTimer(SpawnBotsTimer, this, &ASGameMode::OnSpawnBotsTimer,1.f, true, 0.f);
    SetWaveState(EWaveState::EWS_WaveInProgress);
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
    SetWaveState(EWaveState::EWS_WaitingToComplete);
}

void ASGameMode::CheckWaveState()
{
    if(NrOfSpawnedBots != 0 || GetWorldTimerManager().IsTimerActive(BetweenWavesTimer)) return;
    
    for(TActorIterator<ASTrackerBot> It(GetWorld()); It; ++It)
    {
        const auto CurrentBot = *It;
        if(!CurrentBot->IsDead()) return;
    }
    SetWaveState(EWaveState::EWS_WaveCompleted);
    PrepareForNextWave();
}

void ASGameMode::CheckAnyPlayerAlive()
{
    for(auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
    {
        const auto CurrentPlayer = It->Get()->GetPawn<ASCharacter>();
        if(CurrentPlayer && !CurrentPlayer->IsDead()) return;
    }
    GameOver();
}


void ASGameMode::GameOver()
{
    EndWave();
    UE_LOG(LogTemp, Display, TEXT("--------------   GAME OVER   ----------------"));
    SetWaveState(EWaveState::EWS_GameOver);
}

void ASGameMode::SetWaveState(const EWaveState& NewState) const
{
    if(GetLocalRole() != ROLE_Authority) return;
    
    const auto SGameState = GetGameState<ASGameState>();
    if(!SGameState) return;

    SGameState->SetWaveState(NewState);
}

void ASGameMode::OnActorKilledHandle(const AActor* SVictim, const AActor* SInstigator)
{    
    const auto InstigatorPlayer = Cast<ASCharacter>(SInstigator);
    if(InstigatorPlayer)
    {
        const auto SPlayerState = InstigatorPlayer->GetPlayerState<ASPlayerState>();
        if(!SPlayerState) return;

        if(SVictim->IsA<ASTrackerBot>())
            SPlayerState->AddScore(PointsByKilling);
        else if(SVictim->IsA<ASCharacter>())
            SPlayerState->AddScore(-PointsByKilling);
        else
            SPlayerState->AddScore(1.f);
        
        return;
    }
}


