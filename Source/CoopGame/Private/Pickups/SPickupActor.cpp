

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
    
    bReplicates = true;
    
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    if(GetLocalRole() == ROLE_Authority)
    {
        Super::NotifyActorBeginOverlap(OtherActor);
    
        const auto PlayerCharacter = Cast<ASCharacter>(OtherActor);
        if(!PlayerCharacter) return;
        GetWorldTimerManager().SetTimer(RespawnTimer, this, &ASPickupActor::Respawn, CoolDownDuration);
    
        if(!PowerUpInstance) return;
        PowerUpInstance->SetInstigatorCharacter(PlayerCharacter);
        PowerUpInstance->ActivatePowerUp();
        PowerUpInstance = nullptr;
    }
}

void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
    if(GetLocalRole() == ROLE_Authority)
        Respawn();
}

void ASPickupActor::Respawn()
{
    if(!PowerUpClass || !GetWorld()) return;
    FActorSpawnParameters SpawnInfo;
    SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    PowerUpInstance = GetWorld()->SpawnActor<ASPowerupActor>(PowerUpClass, GetTransform(), SpawnInfo);
}

