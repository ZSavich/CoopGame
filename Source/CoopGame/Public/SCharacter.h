
#pragma once

#include "CoreMinimal.h"
#include "SHealthComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USWeaponComponent;
class USHealthComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USWeaponComponent* WeaponComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USHealthComponent* HealthComp;
    
	virtual void BeginPlay() override;
    
    void OnDeath();

public:	
    FORCEINLINE USHealthComponent* GetHealthComponent() const {return HealthComp;}
    UFUNCTION(BlueprintCallable)
    FORCEINLINE USWeaponComponent* GetWeaponComponent() const {return WeaponComp;}
    FORCEINLINE bool IsDead() const {return HealthComp->bIsDead;}
};
