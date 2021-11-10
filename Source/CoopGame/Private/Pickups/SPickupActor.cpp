

#include "Pickups/SPickupActor.h"

#include "SCharacter.h"
#include "SPowerupActor.h"
#include "SCharacter.h"
#include "Components/DecalComponent.h"
#include "Components/SphereComponent.h"

ASPickupActor::ASPickupActor()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereComp->SetSphereRadius(75.f);
    SetRootComponent(SphereComp);

    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
    DecalComp->SetRelativeRotation(FRotator(90.f,0.f,0.f));
    DecalComp->DecalSize = FVector(90.f, 75.f, 75.f);
    DecalComp->SetupAttachment(GetRootComponent());

    CoolDownDuration = 5.f;
    PowerUpInstance = nullptr;
    
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    
    if(!OtherActor->IsA<ASCharacter>()) return;
    GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASPickupActor::Respawn, CoolDownDuration);
    
    if(!PowerUpInstance) return;
    PowerUpInstance->ActivatePowerUp();
    PowerUpInstance = nullptr;
}

void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
    Respawn();
}

void ASPickupActor::Respawn()
{
    if(!PowerUpClass || !GetWorld()) return;
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    PowerUpInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerUpClass, GetTransform(), SpawnInfo);
}

