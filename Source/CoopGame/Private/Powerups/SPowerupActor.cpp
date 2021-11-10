
#include "Powerups/SPowerupActor.h"

#include "SHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASPowerupActor::ASPowerupActor()
{
    PowerUpInterval = 0.5f;
    TotalNrOfTicks = 3;
    TicksProcessed = 0;
}


void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerupActor::ActivatePowerUp()
{
    if(PowerUpInterval > 0.f)
        GetWorldTimerManager().SetTimer(TickTimer, this, &ASPowerupActor::OnTicked, PowerUpInterval, true, 0.f);
    else
        OnTicked();
}

void ASPowerupActor::OnTicked()
{
    if(++TicksProcessed >= TotalNrOfTicks)
    {
        GetWorldTimerManager().ClearTimer(TickTimer);
        OnExpired();
    }

    // Main Functional for Tick event
}

void ASPowerupActor::OnExpired()
{
    if(!GetWorld()) return;
    const auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    
    if(!PlayerCharacter || !PlayerCharacter->GetCharacterMovement()) return;
    
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed /= 2.f;
}

void ASPowerupActor::OnActivated()
{
    if(!GetWorld()) return;
    const auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    
    if(!PlayerCharacter || !PlayerCharacter->GetCharacterMovement()) return;
    
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= 2.f;
}
