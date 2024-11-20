// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/SmashCharactersStateJump.h"

#include "SmashCharacter.h"
#include "SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"


// Sets default values for this component's properties
USmashCharactersStateJump::USmashCharactersStateJump()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USmashCharactersStateJump::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

ESmashCharacterStateID USmashCharactersStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}


void USmashCharactersStateJump::StateEnter(ESmashCharacterStateID OldStateID)
{
	Super::StateEnter(OldStateID);
	UMovementComponent* MovementComponent = Cast<UMovementComponent>(GetOwner());

	float v =  2.f * JumpMaxHeight / JumpMaxDuration;

	float g = -2.f * JumpMaxHeight / (JumpMaxDuration*JumpMaxDuration);

	Character->GetCharacterMovement()->JumpZVelocity =v;
	Character->GetCharacterMovement()->GravityScale = -g / GravityForce;
	Character->GetCharacterMovement()->MaxWalkSpeed = JumpWalkSpeed;
	Character->GetCharacterMovement()->AirControl = 200.f;

	Character->Jump();
}

void USmashCharactersStateJump::StateExit(ESmashCharacterStateID NewStateID)
{
	Super::StateExit(NewStateID);
}

void USmashCharactersStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	if(Character->GetVelocity().Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}
	if(FMath::Abs(Character->GetInputMoveX()) > 0)
	{
		Character->AddMovementInput(FVector::ForwardVector, Character->GetInputMoveX());
	}
}

