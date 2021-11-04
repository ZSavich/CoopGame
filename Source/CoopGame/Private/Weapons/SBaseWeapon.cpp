

#include "Weapons/SBaseWeapon.h"

#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ASBaseWeapon::ASBaseWeapon()
{
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    MuzzleSocketName = "S_MuzzleFlash";
}

void ASBaseWeapon::ShakeCamera()
{
    if(!GetOwner() || !CameraShakeClass) return;
    const auto OwnerCharacter = GetOwner<ASCharacter>();
    const auto OwnerController = OwnerCharacter->GetController<APlayerController>();
    if(!OwnerController) return;
    OwnerController->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
