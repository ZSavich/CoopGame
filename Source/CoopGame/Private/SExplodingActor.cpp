
#include "SExplodingActor.h"
#include "SHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ASExplodingActor::ASExplodingActor()
{
    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
 	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
    HealthComp->SetIsReplicated(true);
    HealthComp->SetMaxHealth(25.f);
    ExplosionDamage = 50.f;
    ExplosionRadius = 200.f;
    bIsExplosed = false;

    RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce Component"));
    RadialForceComp->Radius = 250;
    RadialForceComp->bIgnoreOwningActor = true;
    RadialForceComp->bImpulseVelChange = true;
    RadialForceComp->SetAutoActivate(false);
    bReplicates = true;
}

void ASExplodingActor::OnRep_OnExplosed()
{
    PlayEffects();
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
    
    PlayEffects();
    UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius,
        UDamageType::StaticClass(), {this});
    SetLifeSpan(5.f);
}

void ASExplodingActor::PlayEffects()
{
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplosionEffect,GetActorLocation());
    RadialForceComp->FireImpulse();
    MeshComp->AddImpulse(FVector(0,0,600.f),NAME_None, true);
    if(AfterExplosionMaterial) MeshComp->SetMaterial(0,AfterExplosionMaterial);
}

void ASExplodingActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME_CONDITION(ASExplodingActor, bIsExplosed, COND_SkipOwner);
}

