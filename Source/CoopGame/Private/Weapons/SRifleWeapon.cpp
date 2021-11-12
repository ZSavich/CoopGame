

#include "Weapons/SRifleWeapon.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopGame/CoopGame.h"
#include "Net/UnrealNetwork.h"

static int32 DebugWeaponDrawing = 0;

FAutoConsoleVariableRef CVARDebugWeaponDrawing (TEXT("COOP.DebugWeapon"), DebugWeaponDrawing,
    TEXT("Draw Debug Lines for Weapon"), ECVF_Cheat);

ASRifleWeapon::ASRifleWeapon()
{
    BaseDamage = 15.f;
    ShotDistance = 1500.f;
    TracerEffectEndPointName = "BeamEnd";
    FireRate = 600.f;
    FireSpread = 5.f;

    NetUpdateFrequency = 66.f;
    MinNetUpdateFrequency = 33.f;
}

void ASRifleWeapon::BeginPlay()
{
    Super::BeginPlay();
    TimeBetweenShots = 60 / FireRate;
}

void ASRifleWeapon::Fire()
{
    if(GetLocalRole() < ROLE_Authority)
    {
        ServerFire();
    }
    
    if(!GetOwner() || !GetWorld()) return;
    
    FVector EyesLocation;
    FRotator EyesRotation;
    GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);
    
    auto ShotDirection = EyesRotation.Vector();

    // Fire Spread 
    const auto SpreadInRadians = FMath::DegreesToRadians(FireSpread);
    ShotDirection = FMath::VRandCone(ShotDirection, SpreadInRadians, SpreadInRadians);
    
    auto EndTrace = EyesLocation + (ShotDirection * ShotDistance);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.bTraceComplex = true;
    CollisionParams.bReturnPhysicalMaterial = true;
    
    FHitResult HitResult;
    
    auto ActualDamage = BaseDamage;

    EPhysicalSurface PhysSurface = SurfaceType_Default;
    
    if(GetWorld()->LineTraceSingleByChannel(HitResult,EyesLocation,EndTrace, WEAPON_TRACE, CollisionParams))
    {
        EndTrace = HitResult.ImpactPoint;
        
        PhysSurface = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
        PlayImpactEffects(PhysSurface, EndTrace);

        switch(PhysSurface)
        {
            case HEAD_SURFACE:
                ActualDamage *= 4.f;
                break;
            default: ;
        }
        
        UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), ActualDamage, ShotDirection, HitResult,
            GetOwner()->GetInstigatorController(), this, DamageTypeClass);
    }
    PlayEmitterEffects(EndTrace);
    ShakeCamera();
    LastFireTime = GetWorld()->TimeSeconds;

    if(GetLocalRole() == ROLE_Authority)
    {
        HitScanTrace.TraceTo = EndTrace;
        HitScanTrace.SurfaceType = PhysSurface;
    }

    /** COOP.DebugWeapon = 1 **/
    if(DebugWeaponDrawing > 0)
    {
        DrawDebugLine(GetWorld(),MeshComp->GetSocketLocation(MuzzleSocketName),EndTrace,FColor::Red,false,3.f,2.f,2.f);
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

void ASRifleWeapon::PlayEmitterEffects(const FVector& EndPoint) const
{
    const auto MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
    const auto Tracer = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
    if(Tracer) Tracer->SetVectorParameter(TracerEffectEndPointName, EndPoint);
    UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
}

void ASRifleWeapon::PlayImpactEffects(const EPhysicalSurface& SurfaceType, const FVector& ImpactPoint) const
{
    auto ActualImpactEffect = DefaultImpactEffect;
        
    switch(SurfaceType)
    {
        case HEAD_SURFACE:
        case FLESH_SURFACE:
            ActualImpactEffect = FleshImpactEffect;
        break;
        default: ;
    }

    const auto MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
    const auto ShotDirection = (ImpactPoint - MuzzleLocation).GetSafeNormal();

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActualImpactEffect, ImpactPoint, ShotDirection.Rotation());
        
}

void ASRifleWeapon::OnRep_HitScanTrace()
{
    PlayEmitterEffects(HitScanTrace.TraceTo);
    PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASRifleWeapon::ServerFire_Implementation()
{
    Fire();
}

bool ASRifleWeapon::ServerFire_Validate()
{
    return true;
}

void ASRifleWeapon::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    // COND_SkipOwner - Not Replicate To Called Client
    DOREPLIFETIME_CONDITION(ASRifleWeapon, HitScanTrace, COND_SkipOwner);
}