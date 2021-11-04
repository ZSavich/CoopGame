
#include "Components/SHealthComponent.h"
#include "SCharacter.h"

USHealthComponent::USHealthComponent()
{
    Health = 0.f;
    MaxHealth = 100.f;
    bIsDead = false;
    OwnerCharacter = nullptr;
}


void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    Health = MaxHealth;
    if(!FindOwnerCharacter()) return;
    OwnerCharacter->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
}

bool USHealthComponent::FindOwnerCharacter()
{
    if(OwnerCharacter) return true;
    OwnerCharacter = GetOwner<ASCharacter>();
    return OwnerCharacter ? true : false;
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    if(Health - Damage <= 0)
    {
        bIsDead = true;
        OnDeath.Broadcast();
    }
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}
