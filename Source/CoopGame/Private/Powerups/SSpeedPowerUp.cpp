

#include "Powerups/SSpeedPowerUp.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASSpeedPowerUp::ASSpeedPowerUp()
{
    SpeedMultiplyValue = 2.f;
}

void ASSpeedPowerUp::OnActivated()
{
    if(!GetWorld()) return;
    const auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    
    if(!PlayerCharacter || !PlayerCharacter->GetCharacterMovement()) return;
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed *= SpeedMultiplyValue;
    
    StaticMeshComp->SetVisibility(false,true);
}

void ASSpeedPowerUp::OnExpired()
{
    if(!GetWorld()) return;
    const auto PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
    
    if(!PlayerCharacter || !PlayerCharacter->GetCharacterMovement()) return;
    PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed /= SpeedMultiplyValue;

    Destroy();
}
