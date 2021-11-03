

#include "SCharacter.h"

ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;

}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(const float Amount)
{
    AddMovementInput(GetActorForwardVector(), Amount);
}

void ASCharacter::MoveRight(const float Amount)
{
    AddMovementInput(GetActorRightVector(), Amount);
}

void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
}

