

#include "Weapons/SRifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame/CoopGame.h"

static int32 DebugWeaponDrawing = 0;

FAutoConsoleVariableRef CVARDebugWeaponDrawing (TEXT("COOP.DebugWeapon"), DebugWeaponDrawing,
    TEXT("Draw Debug Lines for Weapon"), ECVF_Cheat);

ASRifleWeapon::ASRifleWeapon()
{
    BaseDamage = 15.f;
    ShotDistance = 1500.f;
    TracerEffectEndPointName = "BeamEnd";
}

void ASRifleWeapon::Fire()
{
    if(!GetOwner() || !GetWorld()) return;
    
    FVector EyesLocation;
    FRotator EyesRotation;
    GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);
    
    const auto ShotDirection = EyesRotation.Vector();
    auto EndTrace = EyesLocation + (ShotDirection * ShotDistance);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.bTraceComplex = true;
    CollisionParams.bReturnPhysicalMaterial = true;
    
    FHitResult HitResult;
    if(GetWorld()->LineTraceSingleByChannel(HitResult,EyesLocation,EndTrace, WEAPON_TRACE, CollisionParams))
    {
        EndTrace = HitResult.ImpactPoint;
        
        UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), BaseDamage, ShotDirection, HitResult,
            GetOwner()->GetInstigatorController(), this, DamageTypeClass);

        const auto PhysSurface = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
        auto ActualImpactEffect = DefaultImpactEffect;
        
        switch(PhysSurface)
        {
            case FLESH_SURFACE:
            case HEAD_SURFACE:
                ActualImpactEffect = FleshImpactEffect;
                break;
            default: ;
        }
        
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActualImpactEffect, EndTrace, HitResult.ImpactNormal.Rotation());
        
    }
    const auto MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
    const auto Tracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
    Tracer->SetVectorParameter(TracerEffectEndPointName, EndTrace);
    UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
    ShakeCamera();
    
    if(DebugWeaponDrawing > 0)
    {
        DrawDebugLine(GetWorld(),MuzzleLocation,EndTrace,FColor::Red,false,3.f,2.f,2.f);
        DrawDebugSphere(GetWorld(),EndTrace,5.f,12.f,FColor::Red,false,3.f,2.f,2.f);
    }
}
