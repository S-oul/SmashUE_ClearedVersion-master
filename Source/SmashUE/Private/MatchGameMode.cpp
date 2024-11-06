


#include "MatchGameMode.h"

#include "ArenaSettings.h"
#include "Arena/ArenaPlayerStart.h"
#include "Kismet//GameplayStatics.h"
#include "SmashCharacter.h"


void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AArenaPlayerStart*> PlayerStartPoints;
	FindPlayerStartActorInArena(PlayerStartPoints);
	SpawnCharacters(PlayerStartPoints);

	
	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartPoints) {
		
		EAutoReceiveInput::Type InputType = PlayerStartPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;

		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Red,
			PlayerStartPoint->GetFName().ToString() + " " + SmashCharacterClass->GetFName().ToString()
			);
	}
}

void AMatchGameMode::FindPlayerStartActorInArena(TArray<AArenaPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); ++i) {
		AArenaPlayerStart* ArenanPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if (ArenanPlayerStartActor == nullptr) continue;

		ResultsActors.Add(ArenanPlayerStartActor);
	}
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{
	for(AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if(SmashCharacterClass == nullptr) continue;

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
			SmashCharacterClass,
			SpawnPoint->GetTransform()
			);

		if(NewCharacter == nullptr) continue;
		NewCharacter-> AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
		
		CharactersInsideArena.Add(NewCharacter);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	
	switch (InputType)
	{
		case EAutoReceiveInput::Player0:
			return  ArenaSettings->SmashCharacterClassP0;
		
		case EAutoReceiveInput::Player1:
			return ArenaSettings->SmashCharacterClassP1;
		
		case EAutoReceiveInput::Player2:
			return ArenaSettings->SmashCharacterClassP2;

		case EAutoReceiveInput::Player3:
			return ArenaSettings->SmashCharacterClassP3;

		default:
			return nullptr;
	}
}
