
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

    virtual void BeginPlay() override;
};
