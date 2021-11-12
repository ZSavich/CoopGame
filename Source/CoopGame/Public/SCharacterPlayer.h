
#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "SCharacterPlayer.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class COOPGAME_API ASCharacterPlayer : public ASCharacter
{
	GENERATED_BODY()

public:
    ASCharacterPlayer();
    
protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UCameraComponent* CameraComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USpringArmComponent* SpringArmComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zoom")
    float ZoomedFOV;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zoom", meta = (ClampMin = 0.01f, ClampMax = 100.f))
    float ZoomInterpolatingSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
    
    float DefaultFOV;
    bool bWantsZoom;

    void MoveForward(const float Amount);
    void MoveRight(const float Amount);
    void StartCrouch();
    void EndCrouch();

    void OnWantsZoom(const bool bIsWants);
    void ZoomTick(const float DeltaTime);

public:
    virtual FVector GetPawnViewLocation() const override;
    
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

};
