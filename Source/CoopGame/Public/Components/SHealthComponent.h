
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

class ASCharacter;

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, Health, float, MaxHealth);

UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USHealthComponent();

    FOnDeathSignature OnDeath;
    
    UPROPERTY(BlueprintAssignable)
    FOnHealthChangedSignature OnHealthChanged;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Health")
    bool bIsDead;

    UFUNCTION(BlueprintPure)
    FORCEINLINE float GetHealth() const {return Health;}
    UFUNCTION(BlueprintPure)
    FORCEINLINE float GetHealthByPercent() const {return Health/MaxHealth;}
    FORCEINLINE void SetMaxHealth(const float Amount) {MaxHealth = Amount;}

protected:
    UPROPERTY()
    ASCharacter* OwnerCharacter;

    UPROPERTY()
    AActor* OwnerActor;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    float MaxHealth;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    bool bForCharacter;

    UPROPERTY(ReplicatedUsing=OnRep_Health);
    float Health;
    
    UFUNCTION()
    void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    virtual void BeginPlay() override;

    bool FindOwnerCharacter();
    bool FindOwnerActor();

    UFUNCTION()
    void OnRep_Health() const;
};
