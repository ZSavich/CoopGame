

#include "Components/SWeaponComponent.h"
#include "SCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Weapons/SBaseWeapon.h"

USWeaponComponent::USWeaponComponent()
{
    EquipWeaponSocketName = "S_WeaponEquip";
}

void USWeaponComponent::StartFire()
{
    if(!CurrentWeapon) return;
    CurrentWeapon->StartFire();
}

void USWeaponComponent::StopFire()
{
    if(!CurrentWeapon) return;
    CurrentWeapon->StopFire();
}

void USWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
    if(GetOwnerRole() == ROLE_Authority) // Spawn Actor Only on Server
    {
        EquipWeapon();
    }
}

void USWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if(!CurrentWeapon) return;
    WeaponClass = nullptr;
    CurrentWeapon->SetLifeSpan(5.f);
    CurrentWeapon = nullptr;
    Super::EndPlay(EndPlayReason);
}

void USWeaponComponent::EquipWeapon()
{
    if(!WeaponClass || !GetWorld()) return;
    
    const auto Owner = GetOwner<ASCharacter>();
    CurrentWeapon = GetWorld()->SpawnActor<ASBaseWeapon>(WeaponClass);
    
    if(!Owner || !CurrentWeapon) return;
    
    AttachWeaponToSocket(CurrentWeapon,Owner->GetMesh(),EquipWeaponSocketName);
}

void USWeaponComponent::AttachWeaponToSocket(ASBaseWeapon* Weapon, USceneComponent* Component, const FName SocketName) const
{
    if(!Weapon || !Component) return;
    
    const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget,false);

    Weapon->AttachToComponent(Component,AttachmentRules,SocketName);
    Weapon->SetOwner(GetOwner());
}

// Replicate CurrentWeapon for Client
void USWeaponComponent::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
    Super::GetLifetimeReplicatedProps( OutLifetimeProps );
    DOREPLIFETIME(USWeaponComponent, CurrentWeapon);
}
