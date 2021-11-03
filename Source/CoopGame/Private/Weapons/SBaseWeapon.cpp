

#include "Weapons/SBaseWeapon.h"

#include "DrawDebugHelpers.h"

ASBaseWeapon::ASBaseWeapon()
{
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    
}

void ASBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBaseWeapon::Fire() const
{
    if(!GetOwner() || !GetWorld()) return;
    
    FVector EyesLocation;
    FRotator EyesRotation;
    GetOwner()->GetActorEyesViewPoint(EyesLocation, EyesRotation);

    const auto EndTrace = EyesLocation + (EyesRotation.Vector() * 1000);

    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
    CollisionParams.AddIgnoredActor(this);
    CollisionParams.bTraceComplex = true;
    
    FHitResult HitResult;
    if(GetWorld()->LineTraceSingleByChannel(HitResult,EyesLocation,EndTrace,
        ECC_Visibility, CollisionParams))
    {
        // Blocking Object Damage
    }
    DrawDebugLine(GetWorld(), EyesLocation, EndTrace,FColor::Red,false,5.f,5.f,5.f);
}
