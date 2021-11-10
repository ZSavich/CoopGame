

#include "Powerups/SSpeedPowerUp.h"
#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASSpeedPowerUp::ASSpeedPowerUp()
{
    SpeedMultiplyValue = 2.f;
}

void ASSpeedPowerUp::OnActivated()
{
    if(GetLocalRole() == ROLE_Authority)
    {        
        if(!InstigatorCharacter || !InstigatorCharacter->GetCharacterMovement()) return;
        InstigatorCharacter->GetCharacterMovement()->MaxWalkSpeed *= SpeedMultiplyValue;
    }
    
    StaticMeshComp->SetVisibility(false,true);
}

void ASSpeedPowerUp::OnExpired()
{
    if(GetLocalRole() == ROLE_Authority)
    {        
        if(!InstigatorCharacter || !InstigatorCharacter->GetCharacterMovement()) return;
        InstigatorCharacter->GetCharacterMovement()->MaxWalkSpeed /= SpeedMultiplyValue;
    }
    
    Destroy();
}