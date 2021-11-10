
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

UCLASS()
class COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASPowerupActor();

protected:
	virtual void BeginPlay() override;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tick")
    float PowerUpInterval;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tick")
    int32 TotalNrOfTicks;

    int32 TicksProcessed;

    FTimerHandle TickTimer;

    void OnActivated();
    void OnExpired();
    void OnTicked();
    void ActivatePowerUp();
};
