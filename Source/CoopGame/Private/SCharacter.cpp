

#include "SCharacter.h"
#include "SHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SWeaponComponent.h"
#include "CoopGame/CoopGame.h"

ASCharacter::ASCharacter()
{
 	PrimaryActorTick.bCanEverTick = false;

    GetCapsuleComponent()->SetCollisionResponseToChannel(WEAPON_TRACE, ECR_Ignore);

    WeaponComp = CreateDefaultSubobject<USWeaponComponent>(TEXT("Weapon Component"));
    WeaponComp->SetIsReplicated(true);
    
    HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
    HealthComp->SetIsReplicated(true);
}

void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
    
    check(WeaponComp);
    check(HealthComp);
    HealthComp->OnDeath.AddUObject(this,&ASCharacter::OnDeath);
}

void ASCharacter::OnDeath()
{
    //GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    //GetMesh()->SetSimulatePhysics(true);

    GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
    WeaponComp->StopFire();
    WeaponComp->DestroyComponent();
    DetachFromControllerPendingDestroy();
    SetLifeSpan(5.f);
}