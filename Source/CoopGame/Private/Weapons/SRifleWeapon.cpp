

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
    FireRate = 600.f;
}

void ASRifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    TimeBetweenShots = 60 / FireRate;
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
    auto ActualDamage = BaseDamage;
    
    if(GetWorld()->LineTraceSingleByChannel(HitResult,EyesLocation,EndTrace, WEAPON_TRACE, CollisionParams))
    {
        EndTrace = HitResult.ImpactPoint;
        
        const auto PhysSurface = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
        auto ActualImpactEffect = DefaultImpactEffect;
        
        switch(PhysSurface)
        {
            case HEAD_SURFACE:
                ActualDamage *= 4.f;
            case FLESH_SURFACE:
                ActualImpactEffect = FleshImpactEffect;
                break;
            default: ;
        }
        
        UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), ActualDamage, ShotDirection, HitResult,
            GetOwner()->GetInstigatorController(), this, DamageTypeClass);
        
        UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActualImpactEffect, EndTrace, HitResult.ImpactNormal.Rotation());
        
    }
    const auto MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
    const auto Tracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
    Tracer->SetVectorParameter(TracerEffectEndPointName, EndTrace);
    UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
    ShakeCamera();
    LastFireTime = GetWorld()->TimeSeconds;
    
    if(DebugWeaponDrawing > 0)
    {
        DrawDebugLine(GetWorld(),MuzzleLocation,EndTrace,FColor::Red,false,3.f,2.f,2.f);
        DrawDebugSphere(GetWorld(),EndTrace,5.f,12.f,FColor::Red,false,3.f,2.f,2.f);
        DrawDebugString(GetWorld(),EndTrace, FString::SanitizeFloat(ActualDamage),0,FColor::White,3.f);
    }
}

void ASRifleWeapon::StartFire()
{
    if(!GetWorld()) return;
    
    float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->TimeSeconds,0.0f);
    GetWorld()->GetTimerManager().SetTimer(FireTimer, this, &ASRifleWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ASRifleWeapon::StopFire()
{
    if(!GetWorld()) return;
    GetWorld()->GetTimerManager().ClearTimer(FireTimer);
}