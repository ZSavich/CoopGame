
#include "Weapons/SLauncherWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Weapons/SProjectile.h"

ASLauncherWeapon::ASLauncherWeapon()
{
    
}

void ASLauncherWeapon::Fire()
{
    if(!GetWorld() || !ProjectileClass) return;

    const auto MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
    
    const auto OwnerPawn = Cast<APawn>(GetOwner());
    if(!OwnerPawn) return;
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = GetOwner()->GetInstigator();
    SpawnParams.Owner = this;
    
    GetWorld()->SpawnActor<ASProjectile>(ProjectileClass,MuzzleLocation, OwnerPawn->GetViewRotation() ,SpawnParams);
    
    UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
    ShakeCamera();
}
