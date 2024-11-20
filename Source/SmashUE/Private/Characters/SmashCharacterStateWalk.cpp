// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharacterStateWalk.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"


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
	Character->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	Character->InputMoveXFastEvent.AddDynamic(this,&USmashCharacterStateWalk::OnInputMoveXFast);

	MoveXTreshHold = CharacterSettings->MoveXTreshHold;
}

void USmashCharacterStateWalk::StateExit(ESmashCharacterStateID NewStateID)
{
	Super::StateExit(NewStateID);

	Character->InputMoveXFastEvent.RemoveDynamic(this,&USmashCharacterStateWalk::OnInputMoveXFast);
}

void USmashCharacterStateWalk::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if(Character->GetInputMoveY() > 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Jump);
	}
	if(FMath::Abs(Character->GetInputMoveX()) < MoveXTreshHold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
		
	}
}

void USmashCharacterStateWalk::OnInputMoveXFast(float InputMoveX)
{
	StateMachine->ChangeState(ESmashCharacterStateID::Run);
}
















