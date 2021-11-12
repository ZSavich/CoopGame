
#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "SCharacterAdvancedAI.generated.h"

class UBehaviorTree;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;

UCLASS()
class COOPGAME_API ASCharacterAdvancedAI : public ASCharacter
{
	GENERATED_BODY()

public:
    ASCharacterAdvancedAI();

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="AI")
    UBehaviorTree* BehaviorTreeClass;
    
public:
    virtual void BeginPlay() override;
};
