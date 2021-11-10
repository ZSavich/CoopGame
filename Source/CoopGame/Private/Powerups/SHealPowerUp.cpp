

#include "Powerups/SHealPowerUp.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"

ASHealPowerUp::ASHealPowerUp()
{
    HealAmount = 20.f;
}

void ASHealPowerUp::OnActivated()
{
    if(GetLocalRole() == ROLE_Authority)
    {
        if(!GetWorld()) return;
        TArray<AActor*> PlayerActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASCharacter::StaticClass(),PlayerActors);
        for(auto Player : PlayerActors)
        {
            const auto PlayerSCharacter = Cast<ASCharacter>(Player);
            if(!PlayerSCharacter) continue;
            
            if(!PlayerSCharacter || !PlayerSCharacter->GetHealthComponent()) return;
            PlayerSCharacter->GetHealthComponent()->Heal(HealAmount);
        }
    }
    
    StaticMeshComp->SetVisibility(false,true);
}

void ASHealPowerUp::OnExpired()
{
    Destroy();
}
