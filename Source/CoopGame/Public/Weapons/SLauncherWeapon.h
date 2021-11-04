
#pragma once

#include "CoreMinimal.h"
#include "Weapons/SBaseWeapon.h"
#include "SLauncherWeapon.generated.h"

class ASProjectile;

UCLASS()
class COOPGAME_API ASLauncherWeapon : public ASBaseWeapon
{
	GENERATED_BODY()

public:
    ASLauncherWeapon();
    
    virtual void Fire() override;
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    TSubclassOf<ASProjectile> ProjectileClass;
    
};
