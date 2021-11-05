
#pragma once

#include "CoreMinimal.h"
#include "SHealthComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USWeaponComponent;
class USHealthComponent;

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USWeaponComponent* WeaponComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USHealthComponent* HealthComp;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zoom")
    float ZoomedFOV;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Zoom", meta = (ClampMin = 0.01f, ClampMax = 100.f))
    float ZoomInterpolatingSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Widgets")
    TSubclassOf<UUserWidget> PlayerHUDWidgetClass;
    
    float DefaultFOV;
    bool bWantsZoom;
    
	virtual void BeginPlay() override;

    void MoveForward(const float Amount);
    void MoveRight(const float Amount);
    void StartCrouch();
    void EndCrouch();

    void OnWantsZoom(const bool bIsWants);
    void ZoomTick(const float DeltaTime);

    void OnDeath();

public:	
	virtual void Tick(float DeltaTime) override;
    virtual FVector GetPawnViewLocation() const override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    FORCEINLINE bool IsDead() const {return HealthComp->bIsDead;}

};
