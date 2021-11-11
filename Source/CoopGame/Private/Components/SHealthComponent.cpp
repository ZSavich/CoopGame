
#include "Components/SHealthComponent.h"
#include "SCharacter.h"
#include "SGameMode.h"
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

bool USHealthComponent::Heal(const float& Amount)
{
    if(Amount <= 0.f || bIsDead || Health == MaxHealth) return false;
    Health = FMath::Clamp(Health + Amount, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(Health,MaxHealth);
    return true;
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

void USHealthComponent::OnRep_Health() const
{
    OnHealthChanged.Broadcast(Health,MaxHealth);
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
    AActor* DamageCauser)
{
    if(Health - Damage <= 0)
    {
        bIsDead = true;
        OnDeath.Broadcast();

        const auto SGameMode = GetWorld()->GetAuthGameMode<ASGameMode>();
        
        if(SGameMode && InstigatedBy)
            SGameMode->OnActorKilled.Broadcast(GetOwner(), InstigatedBy->GetPawn<AActor>());
    }
    Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
    OnHealthChanged.Broadcast(Health, MaxHealth);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(USHealthComponent, bIsDead);
    DOREPLIFETIME(USHealthComponent, Health);
}