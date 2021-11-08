
#include "Components/SHealthComponent.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"

USHealthComponent::USHealthComponent()
{
    Health = 0.f;
    MaxHealth = 100.f;
    bIsDead = false;
    OwnerCharacter = nullptr;
    OwnerActor = nullptr;
    bForCharacter = true;
}


void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();
    Health = MaxHealth;
    if(GetOwnerRole() == ROLE_Authority)
    {
        if(bForCharacter)
        {
            if(!FindOwnerCharacter()) return;
            OwnerCharacter->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
        }
        else
        {
            if(!FindOwnerActor()) return;
            OwnerActor->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
        } 
    }
}

bool USHealthComponent::FindOwnerCharacter()
{
    if(OwnerCharacter) return true;
    OwnerCharacter = GetOwner<ASCharacter>();
    return OwnerCharacter ? true : false;
}

bool USHealthComponent::FindOwnerActor()
{
    if(OwnerActor) return true;
    OwnerActor = GetOwner<AActor>();
    return OwnerActor ? true : false;
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
    OnHealthChanged.Broadcast(Health, MaxHealth);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USHealthComponent, bIsDead);
}