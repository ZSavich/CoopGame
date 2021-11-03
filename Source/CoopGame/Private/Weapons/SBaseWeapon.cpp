

#include "Weapons/SBaseWeapon.h"

ASBaseWeapon::ASBaseWeapon()
{
    MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));
    
}

void ASBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}
