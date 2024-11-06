


#include "MatchGameMode.h"

#include "Arena/ArenaPlayerStart.h"
#include "Kismet//GameplayStatics.h"


void AMatchGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AArenaPlayerStart*> PlayerStartPoints;
	FindPlayerStartActorInArena(PlayerStartPoints);
	
	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartPoints) {
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Cyan,
			PlayerStartPoint->GetFName().ToString()
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
