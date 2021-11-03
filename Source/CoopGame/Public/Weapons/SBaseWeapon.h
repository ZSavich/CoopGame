
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
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USkeletalMeshComponent* MeshComp;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    float BaseDamage;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    float ShotDistance;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Weapon Properties")
    TSubclassOf<UDamageType> DamageTypeClass;
    
    UFUNCTION(BlueprintCallable)
    void Fire();

    
    
    virtual void BeginPlay() override;
};
