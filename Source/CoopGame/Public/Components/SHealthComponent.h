
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

class ASCharacter;

DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);

UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USHealthComponent();

    FOnDeathSignature OnDeath;

    UFUNCTION(BlueprintPure)
    FORCEINLINE float GetHealth() const {return Health;}
    FORCEINLINE void SetMaxHealth(const float Amount) {MaxHealth = Amount;}
    UFUNCTION(BlueprintPure)
    FORCEINLINE bool IsDead() const {return bIsDead;}

protected:
    UPROPERTY()
    ASCharacter* OwnerCharacter;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Health")
    float MaxHealth;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
    bool bIsDead;
    
    float Health;
    
    UFUNCTION()
    void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

    virtual void BeginPlay() override;

    bool FindOwnerCharacter();
};
