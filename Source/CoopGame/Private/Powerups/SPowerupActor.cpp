
#include "Powerups/SPowerupActor.h"

#include "SCharacter.h"
#include "Components/PointLightComponent.h"
#include "Net/UnrealNetwork.h"

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
    bIsPowerUpActivated = false;
    InstigatorCharacter = nullptr;
    
    bReplicates = true;
}

void ASPowerupActor::ActivatePowerUp()
{
    bIsPowerUpActivated = true;
    
    if(PowerUpInterval > 0.f)
        GetWorldTimerManager().SetTimer(TickTimer, this, &ASPowerupActor::OnTicked, PowerUpInterval, true, 0.f);
    else
        OnTicked();

    OnActivated();
}

void ASPowerupActor::OnRep_PowerUpActivated()
{
    ActivatePowerUp();
}

void ASPowerupActor::OnTicked()
{
    if(++TicksProcessed >= TotalNrOfTicks)
    {
        GetWorldTimerManager().ClearTimer(TickTimer);
        OnExpired();
    }
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ASPowerupActor, bIsPowerUpActivated);
}