
#pragma once

#include "CoreMinimal.h"
#include "Powerups/SPowerupActor.h"
#include "SSpeedPowerUp.generated.h"


UCLASS()
class COOPGAME_API ASSpeedPowerUp : public ASPowerupActor
{
	GENERATED_BODY()

public:
    ASSpeedPowerUp();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SpeedUp")
    float SpeedMultiplyValue;
    
    virtual void OnActivated() override;
    virtual void OnExpired() override;
};
