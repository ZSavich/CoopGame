

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SWeaponComponent.h"

ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
    SpringArmComp->SetupAttachment(GetRootComponent());
    SpringArmComp->bUsePawnControlRotation = true;
    
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    CameraComp->SetupAttachment(SpringArmComp);

    WeaponComponent = CreateDefaultSubobject<USWeaponComponent>(TEXT("Weapon Component"));

    bWantsZoom = false;
    ZoomedFOV = 50.f;
    ZoomInterpolatingSpeed = 20.f;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
    DefaultFOV = CameraComp->FieldOfView;
}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
    PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
    PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

    PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::StartCrouch);
    PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &USWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &USWeaponComponent::StopFire);

    DECLARE_DELEGATE_OneParam(FOnWantsZoomSignature, bool)
    PlayerInputComponent->BindAction<FOnWantsZoomSignature>("Zoom", IE_Pressed, this, &ASCharacter::OnWantsZoom, true);
    PlayerInputComponent->BindAction<FOnWantsZoomSignature>("Zoom", IE_Released, this, &ASCharacter::OnWantsZoom, false);
}

void ASCharacter::MoveForward(const float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASCharacter::MoveRight(const float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASCharacter::StartCrouch()
{
    Crouch();
}

void ASCharacter::EndCrouch()
{
    UnCrouch();
}

void ASCharacter::OnWantsZoom(const bool bIsWants)
{
    bWantsZoom = bIsWants;
}

void ASCharacter::ZoomTick(const float DeltaTime)
{
    const auto TargetFOV = bWantsZoom ? ZoomedFOV : DefaultFOV;
    if(FMath::IsNearlyEqual(TargetFOV,CameraComp->FieldOfView,0.1f)) return;
    
    const auto InterpFOV = FMath::FInterpTo(CameraComp->FieldOfView,TargetFOV,DeltaTime,ZoomInterpolatingSpeed);
    CameraComp->SetFieldOfView(InterpFOV);
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    ZoomTick(DeltaTime);
}

FVector ASCharacter::GetPawnViewLocation() const
{
    if(!CameraComp) return Super::GetPawnViewLocation();
    return CameraComp->GetComponentLocation();
}

