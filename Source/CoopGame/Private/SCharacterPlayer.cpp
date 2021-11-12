

#include "SCharacterPlayer.h"

#include "SWeaponComponent.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASCharacterPlayer::ASCharacterPlayer()
{
    PrimaryActorTick.bCanEverTick = true;
    
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
    SpringArmComp->SetupAttachment(GetRootComponent());
    SpringArmComp->bUsePawnControlRotation = true;

    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    CameraComp->SetupAttachment(SpringArmComp);

    bWantsZoom = false;
    ZoomedFOV = 50.f;
    ZoomInterpolatingSpeed = 20.f;
}

void ASCharacterPlayer::BeginPlay()
{
    Super::BeginPlay();
    
    DefaultFOV = CameraComp->FieldOfView;
    const auto PlayerWidget = CreateWidget(GetController<APlayerController>(), PlayerHUDWidgetClass,TEXT("PlayerHUDWidget"));
    if(PlayerWidget) PlayerWidget->AddToViewport();
}

void ASCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacterPlayer::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacterPlayer::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ACharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Turn", this, &ACharacter::AddControllerYawInput);

    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacterPlayer::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacterPlayer::EndCrouch);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComp, &USWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComp, &USWeaponComponent::StopFire);

    DECLARE_DELEGATE_OneParam(FOnWantsZoomSignature, bool)
    PlayerInputComponent->BindAction<FOnWantsZoomSignature>("Zoom", IE_Pressed, this, &ASCharacterPlayer::OnWantsZoom, true);
    PlayerInputComponent->BindAction<FOnWantsZoomSignature>("Zoom", IE_Released, this, &ASCharacterPlayer::OnWantsZoom, false);
}


void ASCharacterPlayer::MoveForward(const float Amount)
{
    if(!Amount) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASCharacterPlayer::MoveRight(const float Amount)
{
    if(!Amount) return;
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASCharacterPlayer::StartCrouch()
{
    Crouch();
}

void ASCharacterPlayer::EndCrouch()
{
    UnCrouch();
}

void ASCharacterPlayer::OnWantsZoom(const bool bIsWants)
{
    bWantsZoom = bIsWants;
}

void ASCharacterPlayer::ZoomTick(const float DeltaTime)
{
    const auto TargetFOV = bWantsZoom ? ZoomedFOV : DefaultFOV;
    if(FMath::IsNearlyEqual(TargetFOV,CameraComp->FieldOfView,0.1f)) return;
    
    const auto InterpFOV = FMath::FInterpTo(CameraComp->FieldOfView,TargetFOV,DeltaTime,ZoomInterpolatingSpeed);
    CameraComp->SetFieldOfView(InterpFOV);
}

void ASCharacterPlayer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ZoomTick(DeltaTime);
}

FVector ASCharacterPlayer::GetPawnViewLocation() const
{
    if(!CameraComp) return Super::GetPawnViewLocation();
    return CameraComp->GetComponentLocation();
}

