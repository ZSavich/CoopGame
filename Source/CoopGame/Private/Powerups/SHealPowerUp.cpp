

#include "Powerups/SHealPowerUp.h"

#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"

ASHealPowerUp::ASHealPowerUp()
{
    HealAmount = 20.f;
}

void ASHealPowerUp::OnActivated()
{
    if(!GetWorld()) return;
    const auto PlayerSCharacter = Cast<ASCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
    
    if(!PlayerSCharacter || !PlayerSCharacter->GetHealthComponent()) return;
    const auto HealResult = PlayerSCharacter->GetHealthComponent()->Heal(HealAmount);
    
    StaticMeshComp->SetVisibility(!HealResult,true);
}

void ASHealPowerUp::OnExpired()
{
    Destroy();
}
