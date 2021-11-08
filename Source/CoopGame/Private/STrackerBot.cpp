
#include "STrackerBot.h"

#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SHealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogTrackerBot, All, Log)

ASTrackerBot::ASTrackerBot()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetCanEverAffectNavigation(false);
    MeshComp->SetSimulatePhysics(true);
    SetRootComponent(MeshComp);

    HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
    HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);
    HealthComp->OnDeath.AddUObject(this, &ASTrackerBot::SelfDestroy);

    MovementForce = 1000.f;
    bUseVelocityChange = true;
    RequiredDistanceToTarget = 100.f;

    bExploded = false;
    BaseDamage = 50.f;
    DamageRadius = 200.f;
}

void ASTrackerBot::GetNextPathPoint()
{
    const auto Target = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    const auto NavPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(),Target);

    if(NavPath->PathPoints.Num() <= 1) NextPathPoint = GetActorLocation();
    NextPathPoint = NavPath->PathPoints[1];    
}

void ASTrackerBot::SelfDestroy()
{
    if(bExploded) return;
    if(ExplosionEffect)
        UGameplayStatics::SpawnEmitterAtLocation(this,ExplosionEffect,GetActorLocation());

    UGameplayStatics::ApplyRadialDamage(GetWorld(), BaseDamage, GetActorLocation(), DamageRadius, nullptr, {this},this, GetInstigatorController());
    bExploded = true;
    Destroy();
}

void ASTrackerBot::HandleTakeDamage(const float Health, const float MaxHealth)
{
    if(!MaterialInst)
        MaterialInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));

    if(MaterialInst)
        MaterialInst->SetScalarParameterValue("LastTimeDamaged",GetWorld()->GetTimeSeconds());
    
    UE_LOG(LogTrackerBot, Display, TEXT("Health is %f for %s"), Health, *GetName());
}

void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	GetNextPathPoint();
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    const auto DistanceToTarget = (GetActorLocation() - NextPathPoint).Size();
    if(DistanceToTarget <= RequiredDistanceToTarget)
    {
        GetNextPathPoint();
        DrawDebugString(GetWorld(), GetActorLocation(), "Target Reached!");
    }
    else
    {
        const auto DirectionToTarget = (NextPathPoint - GetActorLocation()).GetSafeNormal();
        MeshComp->AddForce(DirectionToTarget * MovementForce, NAME_None, bUseVelocityChange);
        DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(),NextPathPoint,MovementForce,FColor::Red,false,0.f,0.f,2.f);
    }
    DrawDebugSphere(GetWorld(),NextPathPoint,25.f,12.f,FColor::Red,false,0.f,0.f,2.f);
}
