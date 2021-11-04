
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplodingActor.generated.h"

class USHealthComponent;
class URadialForceComponent;

UCLASS()
class COOPGAME_API ASExplodingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASExplodingActor();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    UStaticMeshComponent* MeshComp;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    USHealthComponent* HealthComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    URadialForceComponent* RadialForceComp;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Details")
    UMaterialInterface* BeforeExplosionMaterial;
    
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Details")
    UMaterialInterface* AfterExplosionMaterial;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Details")
    UParticleSystem* ExplosionEffect;

    

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Details")
    float ExplosionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Details")
    float ExplosionDamage;

    bool bIsExplosed;
    
	virtual void BeginPlay() override;
    
    void OnBlowUp();
};
