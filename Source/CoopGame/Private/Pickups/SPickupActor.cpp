

#include "Pickups/SPickupActor.h"

#include "Components/DecalComponent.h"

ASPickupActor::ASPickupActor()
{
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SphereComp->SetSphereRadius(75.f);
    SetRootComponent(SphereComp);

    DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
    DecalComp->SetRelativeRotation(FRotator(90.f,0.f,0.f));
    DecalComp->DecalSize = FVector(90.f, 75.f, 75.f);
    DecalComp->SetupAttachment(GetRootComponent());
}

void ASPickupActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
    Super::NotifyActorBeginOverlap(OtherActor);
    
}

void ASPickupActor::BeginPlay()
{
	Super::BeginPlay();
	
}

