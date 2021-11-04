
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class COOPGAME_API ASProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectile();

    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USphereComponent* SphereCollision;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UProjectileMovementComponent* ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile Properties")
    float BaseDamage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile Properties")
    float DamageRadius;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Projectile Properties")
    TSubclassOf<UDamageType> DamageTypeClass;

    UPROPERTY(EditDefaultsOnly, Category = "Effects")
    UParticleSystem* ImpactEffect;
    
	virtual void BeginPlay() override;

    
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
