// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateRun.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterStateRun::USmashCharacterStateRun()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashCharacterStateRun::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID OldStateID)
{
	Super::StateEnter(OldStateID);

}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NewStateID)
{
	Super::StateExit(NewStateID);

		
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("State Run")
		);

	if(FMath::Abs(Character->GetInputMoveX()) < CharacterSettings->MoveXTreshHold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}
	
	Character->SetActorLocation(Character->GetTransform().GetLocation() += FVector(RunSpeed*DeltaTime, 0, 0) * Character->GetOrientX());
}



