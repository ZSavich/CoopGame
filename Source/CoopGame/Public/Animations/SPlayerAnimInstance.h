
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SPlayerAnimInstance.generated.h"


UCLASS()
class COOPGAME_API USPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

    USPlayerAnimInstance();
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    ACharacter* PlayerCharacter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    float Speed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    float Direction;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    bool bIsCrouching;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Animation Properties")
    bool bIsJumping;
    
    UFUNCTION(BlueprintCallable, Category="Animation Properties")
    void UpdateAnimationProperties();
    
    virtual void NativeInitializeAnimation() override;
    
    void GetPlayerCharacter();
};
