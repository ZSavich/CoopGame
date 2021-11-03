

#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
    
    SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm Component"));
    SpringArmComp->SetupAttachment(GetRootComponent());
    SpringArmComp->bUsePawnControlRotation = true;
    
    CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
    CameraComp->SetupAttachment(SpringArmComp);
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
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

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

