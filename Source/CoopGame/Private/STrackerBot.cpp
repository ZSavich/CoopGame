
#include "STrackerBot.h"

#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "SHealthComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASTrackerBot::ASTrackerBot()
{
    PrimaryActorTick.bCanEverTick = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComp->SetCanEverAffectNavigation(false);
    MeshComp->SetSimulatePhysics(true);
    MeshComp->SetCollisionObjectType(ECC_Pawn);
    SetRootComponent(MeshComp);

    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    SphereComp->SetSphereRadius(50.f);
    SphereComp->SetupAttachment(GetRootComponent());

    AllySphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Ally Collision"));
    AllySphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    AllySphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
    AllySphereComp->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
    AllySphereComp->SetSphereRadius(200.f);
    AllySphereComp->IgnoreComponentWhenMoving(MeshComp,true);
    AllySphereComp->SetupAttachment(GetRootComponent());

    RollAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("Rool Audio"));
    RollAudioComp->SetupAttachment(GetRootComponent());

    HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
    HealthComp->SetIsReplicated(true);
    
    MovementForce = 1000.f;
    bUseVelocityChange = true;
    RequiredDistanceToTarget = 100.f;

    bExploded = false;
    bStartedSelfDamage = false;
    BaseDamage = 50.f;
    DamageRadius = 200.f;
    SelfDamageInterval = 0.25f;
    MaxPowerLevel = 3.f;
}

void ASTrackerBot::BeginPlay()
{
    Super::BeginPlay();
    PowerDamage = BaseDamage;
    SphereComp->OnComponentBeginOverlap.AddDynamic(this,&ASTrackerBot::OnTargetOverlapped);
    if(GetLocalRole() == ROLE_Authority)
    {
        HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);
        GetNextPathPoint();
    }
}

void ASTrackerBot::GetNextPathPoint()
{
    const auto Target = Cast<AActor>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    if(!Target) return;
    const auto NavPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(),Target);

    if(NavPath->PathPoints.Num() <= 1) NextPathPoint = GetActorLocation();
    NextPathPoint = NavPath->PathPoints[1];
}

void ASTrackerBot::SelfDestroy()
{
    if(bExploded) return;
    if(ExplosionEffect)
        UGameplayStatics::SpawnEmitterAtLocation(this,ExplosionEffect,GetActorLocation());

    UGameplayStatics::PlaySoundAtLocation(GetWorld(),ExplosionSound,GetActorLocation());
    bExploded = true;
    MeshComp->SetVisibility(false);
    MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    if(GetLocalRole() == ROLE_Authority)
    {
        TArray<AActor*> IgnoredActors;
        UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASTrackerBot::StaticClass(),IgnoredActors);
        UGameplayStatics::ApplyRadialDamage(GetWorld(), PowerDamage, GetActorLocation(), DamageRadius, nullptr, IgnoredActors,this, GetInstigatorController());
        SetLifeSpan(2.f);
    }
}

void ASTrackerBot::HandleTakeDamage(const float Health, const float MaxHealth)
{
    if(!MaterialInst)
        MaterialInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));

    if(MaterialInst)
        MaterialInst->SetScalarParameterValue("LastTimeDamageTaken",GetWorld()->GetTimeSeconds());
    
    if(Health<=0)
        SelfDestroy();
}

void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if(GetLocalRole() == ROLE_Authority)
    {
        ChaseTarget();
    }
    ChangeRollVolume();
    IncreasePower();
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
    
}

void ASTrackerBot::DamageSelf()
{
    UGameplayStatics::ApplyDamage(this,20.f,nullptr,this,nullptr);
}

void ASTrackerBot::ChaseTarget()
{
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

void ASTrackerBot::ChangeRollVolume() const
{
    if(!RollAudioComp) return;
    const auto Speed = GetVelocity().Size();
    const auto Volume = UKismetMathLibrary::MapRangeClamped(Speed,10.f,1000.f,0.1f,2.f);
    RollAudioComp->SetVolumeMultiplier(Volume);
}

void ASTrackerBot::IncreasePower()
{
    TArray<AActor*> OverlappingActors;
    AllySphereComp->GetOverlappingActors(OverlappingActors,ASTrackerBot::StaticClass());
    if(OverlappingActors.Num())
    {
        if(!MaterialInst)
            MaterialInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
        if(!MaterialInst) return;
        
        const auto PowerLevel = OverlappingActors.Num() / MaxPowerLevel;
        PowerDamage = BaseDamage * OverlappingActors.Num();
        MaterialInst->SetScalarParameterValue("PowerLevelAlpha",FMath::Clamp(PowerLevel,0.f,1.f));
    }
    else
    {
        PowerDamage = BaseDamage;
        if(!MaterialInst) return;
        MaterialInst->SetScalarParameterValue("PowerLevelAlpha",0.f);
    }
}

void ASTrackerBot::OnTargetOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if(bStartedSelfDamage) return;
    const auto TargetCharacter = Cast<ASCharacter>(OtherActor);
    if(!TargetCharacter) return;

    if(GetLocalRole() == ROLE_Authority)
    {
        GetWorldTimerManager().SetTimer(DestroyTimer, this, &ASTrackerBot::DamageSelf, SelfDamageInterval, true, 0.f);
    }
    
    bStartedSelfDamage = true;
    UGameplayStatics::SpawnSoundAttached(SelfDestroySound,MeshComp);
}
