

#include "Animations/SPlayerAnimInstance.h"
#include "SCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

class ASCharacter;

USPlayerAnimInstance::USPlayerAnimInstance()
{
    PlayerCharacter = nullptr;
    Speed = 0.f;
    Direction = 0.f;
    bIsCrouching = false;
    bIsJumping = false;
    bIsDead = false;
}

void USPlayerAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    GetPlayerCharacter();
}

void USPlayerAnimInstance::UpdateAnimationProperties()
{
    if(!PlayerCharacter)
    {
        GetPlayerCharacter();
        return;
    }
    /** Speed & Direction **/
    const auto Velocity = PlayerCharacter->GetVelocity();
    Speed = FVector(Velocity.X, Velocity.Y, 0.f).Size();
    Direction = CalculateDirection(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRotation());

    /** Crouch **/
    bIsCrouching = PlayerCharacter->bIsCrouched;

    /** Jump **/
    bIsJumping = PlayerCharacter->GetCharacterMovement()->IsFalling();

    /** Dead Status **/
    bIsDead = PlayerCharacter->IsDead();
}

void USPlayerAnimInstance::GetPlayerCharacter()
{
    if(!TryGetPawnOwner()) return;
    PlayerCharacter = Cast<ASCharacter>(TryGetPawnOwner());
}
