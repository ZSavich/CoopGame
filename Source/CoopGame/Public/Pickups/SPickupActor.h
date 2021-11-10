
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickupActor.generated.h"

class ASPowerupActor;
class UDecalComponent;
class USphereComponent;

UCLASS()
class COOPGAME_API ASPickupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASPickupActor();

    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

protected:
	virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USphereComponent* SphereComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UDecalComponent* DecalComp;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="PowerUp")
    TSubclassOf<ASPowerupActor> PowerUpClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="PowerUp")
    float CoolDownDuration;

    UPROPERTY()
    ASPowerupActor* PowerUpInstance;

    FTimerHandle RespawnTimer;

    void Respawn();

};
