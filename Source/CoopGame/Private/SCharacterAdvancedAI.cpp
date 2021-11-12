
#include "SCharacterAdvancedAI.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"

void ASCharacterAdvancedAI::BeginPlay()
{
    const auto AIController = GetController<AAIController>();
    if(!AIController) return;
    AIController->RunBehaviorTree(BehaviorTreeClass);
    
    Super::BeginPlay();
}
