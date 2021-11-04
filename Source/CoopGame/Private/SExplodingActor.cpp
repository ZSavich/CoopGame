
#include "SExplodingActor.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplodingActor::ASExplodingActor()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
 	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
    HealthComp->SetMaxHealth(25.f);
    ExplosionDamage = 50.f;
    ExplosionRadius = 200.f;
    bIsExplosed = false;

    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce Component"));
    RadialForceComp->Radius = 250;
    RadialForceComp->bIgnoreOwningActor = true;
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->SetAutoActivate(false);
}

void ASExplodingActor::BeginPlay()
{
	Super::BeginPlay();
    check(MeshComp);
    HealthComp->OnDeath.AddUObject(this, &ASExplodingActor::OnBlowUp);
    if(BeforeExplosionMaterial) MeshComp->SetMaterial(0,BeforeExplosionMaterial);
}

void ASExplodingActor::OnBlowUp()
{
    if(bIsExplosed) return;
    bIsExplosed = true;
    
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplosionEffect,GetActorLocation());
    UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius,
        UDamageType::StaticClass(), {this});
    
    RadialForceComp->FireImpulse();
    
    MeshComp->AddImpulse(FVector(0,0,600.f),NAME_None, true);
    if(AfterExplosionMaterial) MeshComp->SetMaterial(0,AfterExplosionMaterial);
    SetLifeSpan(5.f);
}

