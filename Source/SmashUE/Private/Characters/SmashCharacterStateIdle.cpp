// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateIdle.h"


// Sets default values for this component's properties
USmashCharacterStateIdle::USmashCharacterStateIdle()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashCharacterStateIdle::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

ESmashCharacterStateID USmashCharacterStateIdle::GetStateID()
{
	return ESmashCharacterStateID::Idle;
}

void USmashCharacterStateIdle::StateEnter(ESmashCharacterStateID OldStateID)
{
	Super::StateEnter(OldStateID);
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("State Idle Enter")
		);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NewStateID)
{
	Super::StateExit(NewStateID);
	GEngine->AddOnScreenDebugMessage(
	-1,
	3.f,
	FColor::Red,
	TEXT("State Idle Exit")
	);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(
		-1,
		DeltaTime,
		FColor::Green,
		TEXT("Tick StateIdle")
		);
}

