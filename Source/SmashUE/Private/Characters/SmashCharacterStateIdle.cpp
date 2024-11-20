// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateIdle.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateMachine.h"


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

	Character->InputMoveXFastEvent.AddDynamic(this,&USmashCharacterStateIdle::OnInputMoveXFast);
}

void USmashCharacterStateIdle::StateExit(ESmashCharacterStateID NewStateID)
{
	Super::StateExit(NewStateID);
	
	Character->InputMoveXFastEvent.RemoveDynamic(this,&USmashCharacterStateIdle::OnInputMoveXFast);
}

void USmashCharacterStateIdle::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	MoveXTreshHold = CharacterSettings->MoveXTreshHold;

	if(Character->GetInputMoveY() > 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
	if(FMath::Abs(Character->GetInputMoveX()) > MoveXTreshHold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Walk);
	}
}

void USmashCharacterStateIdle::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}

