
#pragma once

#include "CoreMinimal.h"
#include "Powerups/SPowerupActor.h"
#include "SHealPowerUp.generated.h"


UCLASS()
class COOPGAME_API ASHealPowerUp : public ASPowerupActor
{
	GENERATED_BODY()

public:
    ASHealPowerUp();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Heal")
    float HealAmount;
    
    virtual void OnActivated() override;
    virtual void OnExpired() override;
};
