
#include "Powerups/SPowerupActor.h"
#include "Components/PointLightComponent.h"

ASPowerupActor::ASPowerupActor()
{
    SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
    SetRootComponent(SceneComp);
    
    StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
    StaticMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    StaticMeshComp->SetupAttachment(GetRootComponent());

    PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("Point Light"));
    PointLightComp->SetupAttachment(StaticMeshComp);

    RotationComp = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("Rotation Movement Component"));
    RotationComp->RotationRate = FRotator(0.f, 15.f, 0.f);
    
    PowerUpInterval = 1.f;
    TotalNrOfTicks = 5;
    TicksProcessed = 0;
}

void ASPowerupActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASPowerupActor::ActivatePowerUp()
{
    OnActivated();
    
    if(PowerUpInterval > 0.f)
        GetWorldTimerManager().SetTimer(TickTimer, this, &ASPowerupActor::OnTicked, PowerUpInterval, true, 0.f);
    else
        OnTicked();
}

void ASPowerupActor::OnTicked()
{
    if(++TicksProcessed >= TotalNrOfTicks)
    {
        GetWorldTimerManager().ClearTimer(TickTimer);
        OnExpired();
    }
}
