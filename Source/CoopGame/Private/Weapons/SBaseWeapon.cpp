

#include "Weapons/SBaseWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

ASBaseWeapon::ASBaseWeapon()
{
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    BaseDamage = 15.f;
    ShotDistance = 1500.f;
}

void ASBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBaseWeapon::Fire()
{
    if(!GetOwner() || !GetWorld()) return;
    
    FVector EyesLocation;
    FRotator EyesRotation;
    GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);
    
    const auto ShotDirection = EyesRotation.Vector();
    const auto EndTrace = EyesLocation + (ShotDirection * ShotDistance);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.bTraceComplex = true;
    
    FHitResult HitResult;
    if(GetWorld()->LineTraceSingleByChannel(HitResult,EyesLocation,EndTrace,
        ECC_Visibility, CollisionParams))
    {
        UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), BaseDamage, ShotDirection, HitResult,
            GetOwner()->GetInstigatorController(), this, DamageTypeClass);
    }
    DrawDebugLine(GetWorld(), EyesLocation, EndTrace,FColor::Red,false,2.f,2.f,2.f);
}
