

#include "Weapons/SProjectile.h"
#include "DrawDebugHelpers.h"
#include "SBaseWeapon.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ASProjectile::ASProjectile()
{
    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereCollision->InitSphereRadius(15.f);
    SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereCollision->SetCollisionResponseToAllChannels(ECR_Block);
    SphereCollision->IgnoreActorWhenMoving(GetOwner(), true);
    SphereCollision->bReturnMaterialOnMove = true;
    SetRootComponent(SphereCollision);

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetupAttachment(GetRootComponent());
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
    ProjectileMovement->InitialSpeed = 4000.f;
    ProjectileMovement->MaxSpeed = 10000.f;
    ProjectileMovement->ProjectileGravityScale = 0.5f;
    
    BaseDamage = 75.f;
    DamageRadius = 50.f;

}

void ASProjectile::BeginPlay()
{
	Super::BeginPlay();
    SphereCollision->OnComponentHit.AddDynamic(this, &ASProjectile::OnHit);
    SetLifeSpan(5.f);
	
}

void ASProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
    const FHitResult& Hit)
{
    const auto HitLocation = Hit.ImpactPoint;
    UGameplayStatics::ApplyRadialDamage(GetWorld(),BaseDamage,HitLocation, DamageRadius,DamageTypeClass,
        {GetInstigator()},GetOwner(), GetInstigator()->GetInstigatorController());

    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitLocation);
    MeshComp->DestroyComponent();
    Destroy();
}

