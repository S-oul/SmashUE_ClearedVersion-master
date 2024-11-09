// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterState.h"
#include "SmashCharacter.h"
#include "SmashCharacterStateMachine.h"


// Sets default values for this component's properties
USmashCharacterState::USmashCharacterState()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

ESmashCharacterStateID USmashCharacterState::GetStateID()
{
	return ESmashCharacterStateID::None;
}

void USmashCharacterState::StateInit(USmashCharacterStateMachine* InStateMachine)
{
	StateMachine = InStateMachine;
	Character = InStateMachine->GetCharacter();
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	3.f,
	FColor::Blue,
	FString::Printf(TEXT("Init State %d"), GetStateID())
	);
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID OldStateID)
{
	Character->PlayAnimMontage(StateAnimation);
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	3.f,
	FColor::Purple,
	StateAnimation.GetFullName()
	);
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NewStateID)
{
	Character->StopAnimMontage();
}

void USmashCharacterState::StateTick(float DeltaTime)
{
	
}

