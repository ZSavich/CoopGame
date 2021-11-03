
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SWeaponComponent.generated.h"

class ASBaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COOPGAME_API USWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USWeaponComponent();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Info")
    TSubclassOf<ASBaseWeapon> WeaponClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weapon Info")
    ASBaseWeapon* CurrentWeapon;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Info")
    FName EquipWeaponSocketName;
    
	virtual void BeginPlay() override;

    void EquipWeapon();
    void AttachWeaponToSocket(ASBaseWeapon* Weapon, USceneComponent* Component, const FName SocketName) const;
};
