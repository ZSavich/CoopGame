
#pragma once

#include "CoreMinimal.h"
#include "Weapons/SBaseWeapon.h"
#include "SRifleWeapon.generated.h"

USTRUCT()
struct FHitScanTrace
{
    GENERATED_BODY()

    UPROPERTY()
    TEnumAsByte<EPhysicalSurface> SurfaceType; 

    UPROPERTY()
    FVector_NetQuantize TraceTo;
};

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

    void PlayEmitterEffects(const FVector& EndPoint) const;
    
    void PlayImpactEffects(const EPhysicalSurface& SurfaceType, const FVector& ImpactPoint) const;
    
    /** Network Replication **/
    UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
    FHitScanTrace HitScanTrace;

    UFUNCTION()
    void OnRep_HitScanTrace();

    UFUNCTION(Server,Reliable,Withvalidation)
    void ServerFire();
};
