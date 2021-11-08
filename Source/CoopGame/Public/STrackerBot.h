
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;

UCLASS()
class COOPGAME_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	ASTrackerBot();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USHealthComponent* HealthComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float MovementForce;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    bool bUseVelocityChange;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    float RequiredDistanceToTarget;

    UPROPERTY()
    UMaterialInstanceDynamic* MaterialInst;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
    float BaseDamage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
    float DamageRadius;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* ExplosionEffect;

    bool bExploded;
    FVector NextPathPoint;
    
    UFUNCTION()
    void HandleTakeDamage(const float Health, const float MaxHealth);
    
    void GetNextPathPoint();
    void SelfDestroy();
    
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
