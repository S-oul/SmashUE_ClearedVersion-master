// Fill out your copyright notice in the Description page of Project Settings.


#include "SmashCharacterStateMachine.h"

#include "SmashCharacter.h"
#include "Characters/SmashCharacterState.h"

void USmashCharacterStateMachine::Init(ASmashCharacter* InCharacter)
{
	Character = InCharacter;
	FindState();
	InitState();

	ChangeState(ESmashCharacterStateID::Idle);
}

ASmashCharacter* USmashCharacterStateMachine::GetCharacter()const
{
	return Character;
}

void USmashCharacterStateMachine::ChangeState(ESmashCharacterStateID NextStateID)
{
	USmashCharacterState* NewState = GetState(NextStateID);
	if(NewState == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(
		-1,
		1.f,
		FColor::Blue,
		TEXT("NEW IS NULL CONNARD")
		);
		return;
	}

	if(CurrentState != nullptr) CurrentState->StateExit(NextStateID);

	ESmashCharacterStateID PreviousStateID = CurrentStateID;
	CurrentStateID = NextStateID;
	CurrentState = NewState;

	GEngine->AddOnScreenDebugMessage(
		-1,
		1,
		FColor::Green,
		CurrentState.GetFullName()
		);
	
	if(CurrentState != nullptr) CurrentState->StateEnter(PreviousStateID);
}

USmashCharacterState* USmashCharacterStateMachine::GetState(ESmashCharacterStateID StateID) const
{
	for(USmashCharacterState* state : AllStates)
	{
		if(StateID == state->GetStateID()) return state;
	}
	return nullptr;
}

void USmashCharacterStateMachine::FindState()
{
	TArray<UActorComponent*> FoundComponents = Character->K2_GetComponentsByClass(USmashCharacterState::StaticClass());
	for (UActorComponent* StateComponent : FoundComponents)
	{
		USmashCharacterState* State = Cast<USmashCharacterState>(StateComponent);
		if (State == nullptr) continue;
		if(State->GetStateID() == ESmashCharacterStateID::None) continue;

		AllStates.Add(State);
	}
}

void USmashCharacterStateMachine::InitState()
{
	for(USmashCharacterState* State : AllStates)
	{
		State->StateInit(this);
	}
}

void USmashCharacterStateMachine::Tick(float DeltaTime)
{
	if(CurrentState == nullptr) return;
	CurrentState->StateTick(DeltaTime);
}
