

#include "SGameState.h"

#include "Net/UnrealNetwork.h"

void ASGameState::SetWaveState(const EWaveState NewState)
{
    if(NewState == WaveState) return;
    const auto OldWave = WaveState;
    WaveState = NewState;
    OnRep_WaveState(OldWave);
}

void ASGameState::OnRep_WaveState(const EWaveState OldState) const
{
    WaveStateChanged(WaveState, OldState);
}

void ASGameState::WaveStateChanged(const EWaveState NewState, const EWaveState OldState) const
{
    const auto DebugMessage = UEnum::GetValueAsString(NewState);
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, DebugMessage, true,FVector2D(1.5f));
}

void ASGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASGameState, WaveState);
}