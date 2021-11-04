
#pragma once

#include "CoreMinimal.h"
#include "Weapons/SBaseWeapon.h"
#include "SRifleWeapon.generated.h"

UCLASS()
class COOPGAME_API ASRifleWeapon : public ASBaseWeapon
{
	GENERATED_BODY()

public:
    ASRifleWeapon();

    virtual void BeginPlay() override;
    virtual void Fire() override;
    virtual void StartFire() override;
    virtual void StopFire() override;
    
protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    float BaseDamage;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    float ShotDistance;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    FName TracerEffectEndPointName;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    TSubclassOf<UDamageType> DamageTypeClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    float FireRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* DefaultImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* FleshImpactEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* TracerEffect;
    
    FTimerHandle FireTimer;

    float LastFireTime;
    float TimeBetweenShots;
};
