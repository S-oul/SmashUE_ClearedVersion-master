// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharactersStateJump.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharactersStateJump : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharactersStateJump();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	const float GravityForce = 981.f;

public:
	
	UFUNCTION()
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID OldStateID) override;

	virtual void StateExit(ESmashCharacterStateID NewStateID) override;
	virtual void StateTick(float DeltaTime) override;

	
	UPROPERTY(EditAnywhere)
	float JumpMaxHeight = 300.f;

	UPROPERTY(EditAnywhere)
	float JumpMaxDuration = 1.f;

	UPROPERTY(EditAnywhere)
	float JumpWalkSpeed = 400.f;

};
