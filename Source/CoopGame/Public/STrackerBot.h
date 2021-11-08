
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
class USphereComponent;

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
    USphereComponent* SphereComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USphereComponent* AllySphereComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USHealthComponent* HealthComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UAudioComponent* RollAudioComp;

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

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
    float SelfDamageInterval;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
    float MaxPowerLevel;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Effects")
    UParticleSystem* ExplosionEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sounds")
    USoundBase* SelfDestroySound;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Sounds")
    USoundBase* ExplosionSound;

    bool bExploded;
    bool bStartedSelfDamage;
    float PowerDamage;
    
    FVector NextPathPoint;
    FTimerHandle DestroyTimer;
    
    UFUNCTION()
    void HandleTakeDamage(const float Health, const float MaxHealth);
    
    void GetNextPathPoint();
    void SelfDestroy();
    void DamageSelf();
    void ChaseTarget();
    void ChangeRollVolume() const;
    void IncreasePower();
    UFUNCTION()
    void OnTargetOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
