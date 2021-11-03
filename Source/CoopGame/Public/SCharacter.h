
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASCharacter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USpringArmComponent* SpringArmComp;
    
	virtual void BeginPlay() override;

    void MoveForward(const float Amount);
    void MoveRight(const float Amount);
    void StartCrouch();
    void EndCrouch();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
