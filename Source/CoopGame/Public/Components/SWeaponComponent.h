
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SWeaponComponent.generated.h"

class ASBaseWeapon;

UCLASS( ClassGroup=(COOP), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USWeaponComponent();

    UFUNCTION(BlueprintCallable)
    void StartFire();
    UFUNCTION(BlueprintCallable)
    void StopFire();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Info")
    TSubclassOf<ASBaseWeapon> WeaponClass;

    // Replicated - Share Pointer to Client Side from Server
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category="Weapon Info")
    ASBaseWeapon* CurrentWeapon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Info")
    FName EquipWeaponSocketName;
    
	virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    void EquipWeapon();
    void AttachWeaponToSocket(ASBaseWeapon* Weapon, USceneComponent* Component, const FName SocketName) const;
};
