

#include "SCharacter.h"

#include "SHealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SWeaponComponent.h"
#include "CoopGame/CoopGame.h"

ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    GetCapsuleComponent()->SetCollisionResponseToChannel(WEAPON_TRACE, ECR_Ignore);
    
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
    SpringArmComp->SetupAttachment(GetRootComponent());
    SpringArmComp->bUsePawnControlRotation = true;
    
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    CameraComp->SetupAttachment(SpringArmComp);

    WeaponComp = CreateDefaultSubobject<USWeaponComponent>(TEXT("Weapon Component"));
    HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));

    bWantsZoom = false;
    ZoomedFOV = 50.f;
    ZoomInterpolatingSpeed = 20.f;
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
    check(WeaponComp);
    check(HealthComp);
    DefaultFOV = CameraComp->FieldOfView;
    HealthComp->OnDeath.AddUObject(this,&ASCharacter::OnDeath);
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
    PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComp, &USWeaponComponent::StartFire);
    PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComp, &USWeaponComponent::StopFire);

    DECLARE_DELEGATE_OneParam(FOnWantsZoomSignature, bool)
    PlayerInputComponent->BindAction<FOnWantsZoomSignature>("Zoom", IE_Pressed, this, &ASCharacter::OnWantsZoom, true);
    PlayerInputComponent->BindAction<FOnWantsZoomSignature>("Zoom", IE_Released, this, &ASCharacter::OnWantsZoom, false);
}

void ASCharacter::MoveForward(const float Amount)
{
    if(!Amount) return;
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASCharacter::MoveRight(const float Amount)
{
    if(!Amount) return;
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

void ASCharacter::OnDeath()
{
    //GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //GetMesh()->SetSimulatePhysics(true);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponComp->StopFire();
    WeaponComp->DestroyComponent();
    DetachFromControllerPendingDestroy();
    SetLifeSpan(5.f);
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

