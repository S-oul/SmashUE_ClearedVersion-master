

#pragma once

#include "CoreMinimal.h"
#include "ArenaSettings.h"
#include "GameFramework/GameModeBase.h"
#include "Inputs/SmashCharacterInputData.h"
#include "MatchGameMode.generated.h"

class ASmashCharacter;
class AArenaPlayerStart;

/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY()
	TArray<ASmashCharacter*> CharactersInsideArena;
	
private:
	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();
	
	void FindPlayerStartActorInArena(TArray<AArenaPlayerStart*>& ResultsActors);

	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);
	
	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InpoutType) const;
};
