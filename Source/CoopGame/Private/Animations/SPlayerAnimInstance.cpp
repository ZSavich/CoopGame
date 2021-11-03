

#include "Animations/SPlayerAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USPlayerAnimInstance::USPlayerAnimInstance()
{
    PlayerCharacter = nullptr;
    Speed = 0.f;
    Direction = 0.f;
    bIsCrouching = false;
    bIsJumping = false;
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
}

void USPlayerAnimInstance::GetPlayerCharacter()
{
    if(!TryGetPawnOwner()) return;
    PlayerCharacter = Cast<ACharacter>(TryGetPawnOwner());
}
