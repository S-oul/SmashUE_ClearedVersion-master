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
	CharacterSettings = GetDefault<USmashCharacterSettings>();
}

void USmashCharacterState::StateEnter(ESmashCharacterStateID OldStateID)
{
	Character->PlayAnimMontage(StateAnimation);
}

void USmashCharacterState::StateExit(ESmashCharacterStateID NewStateID)
{
}

void USmashCharacterState::StateTick(float DeltaTime)
{
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	DeltaTime,
	FColor::Green,
this->GetName() + FString(" ") + FString::FromInt(Character->CharacterId)
	);
	
}

