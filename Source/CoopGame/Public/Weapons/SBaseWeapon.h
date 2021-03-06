
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseWeapon.generated.h"


UCLASS()
class COOPGAME_API ASBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
    ASBaseWeapon();
    
    virtual void StartFire() {};
    virtual void StopFire() {};
    void ShakeCamera();
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USkeletalMeshComponent* MeshComp;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    FName MuzzleSocketName;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* MuzzleEffect;

    UPROPERTY(EditDefaultsOnly, Category="Effects")
    TSubclassOf<UCameraShakeBase> CameraShakeClass;
    
    virtual void Fire() {};
};
