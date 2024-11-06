


#include "MatchGameMode.h"

#include "Arena/ArenaPlayerStart.h"
#include "Kismet//GameplayStatics.h"


void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();

	TArray<AArenaPlayerStart*> PlayerStartPoints;
	FindPlayerStartActorInArena(PlayerStartPoints);

	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartPoints) {

		EAutoReceiveInput::Type InputType = PlayerStartPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;

		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Red,
			SmashCharacterClass->GetFName().ToString()
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

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const
{
	switch (InputType)
	{
	case EAutoReceiveInput::Player0:
		return SmashCharacterClassP0;

	case EAutoReceiveInput::Player1:
		return SmashCharacterClassP1;

	case EAutoReceiveInput::Player2:
		return SmashCharacterClassP2
			;
	case EAutoReceiveInput::Player3:
		return SmashCharacterClassP3;

	default:
		return nullptr;
	}
}
