// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterStateFall();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual ESmashCharacterStateID GetStateID() override;

	virtual void StateEnter(ESmashCharacterStateID OldStateID) override;

	virtual void StateExit(ESmashCharacterStateID NewStateID) override;
	virtual void StateTick(float DeltaTime) override;
};
