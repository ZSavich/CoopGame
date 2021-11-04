
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SPlayerAnimInstance.generated.h"

class ASCharacter;

UCLASS()
class COOPGAME_API USPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

    USPlayerAnimInstance();
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    ASCharacter* PlayerCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    float Speed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    float Direction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    bool bIsJumping;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    bool bIsDead;
    
    UFUNCTION(BlueprintCallable, Category="Animation Properties")
    void UpdateAnimationProperties();
    
    virtual void NativeInitializeAnimation() override;
    
    void GetPlayerCharacter();
};
