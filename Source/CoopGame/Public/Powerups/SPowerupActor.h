
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "SPowerupActor.generated.h"

class UPointLightComponent;

UCLASS()
class COOPGAME_API ASPowerupActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASPowerupActor();

    UFUNCTION(BlueprintCallable)
    void ActivatePowerUp();

protected:
	virtual void BeginPlay() override;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USceneComponent* SceneComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* StaticMeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UPointLightComponent* PointLightComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    URotatingMovementComponent* RotationComp;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tick")
    float PowerUpInterval;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Tick")
    int32 TotalNrOfTicks;

    int32 TicksProcessed;

    FTimerHandle TickTimer;

    virtual void OnActivated() {};
    virtual void OnExpired() {};
    void OnTicked();
};
