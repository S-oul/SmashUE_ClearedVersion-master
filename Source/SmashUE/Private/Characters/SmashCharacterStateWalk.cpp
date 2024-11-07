// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateWalk.h"


// Sets default values for this component's properties
USmashCharacterStateWalk::USmashCharacterStateWalk()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashCharacterStateWalk::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

ESmashCharacterStateID USmashCharacterStateWalk::GetStateID()
{
	return ESmashCharacterStateID::Walk;
}

void USmashCharacterStateWalk::StateEnter(ESmashCharacterStateID OldStateID)
{
	Super::StateEnter(OldStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("State Walk Enter")
		);
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NewStateID)
{
	Super::StateExit(NewStateID);
	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Red,
	TEXT("State Walk Exit")
	);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick State Walk")
		);
}



