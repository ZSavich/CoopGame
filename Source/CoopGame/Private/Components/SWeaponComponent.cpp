

#include "Components/SWeaponComponent.h"
#include "SCharacter.h"
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
	EquipWeapon();
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
