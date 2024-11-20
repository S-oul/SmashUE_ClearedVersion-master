// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateFall.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterStateFall::USmashCharacterStateFall()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashCharacterStateFall::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID OldStateID)
{
	Super::StateEnter(OldStateID);
	
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NewStateID)
{
	Super::StateExit(NewStateID);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
		
	if(Character->GetVelocity().Z == 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
		return;
	}
	if(Character->GetVelocity().Z > 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
		return;
	}

	if(FMath::Abs(Character->GetInputMoveX()) > 0)
	{
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
	}
	
}

