// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SmashCharacterStateMachine.generated.h"

/**
 * 
 */

enum class  ESmashCharacterState : uint8;
class ASmashCharacter;
class USmashCharacterState;

UCLASS()
class SMASHUE_API USmashCharacterStateMachine : public UObject
{
	GENERATED_BODY()

public:
	void Init(ASmashCharacter* InStateMachine);

	ASmashCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(ESmashCharacterStateID NextStateID);

	USmashCharacterState* GetState(ESmashCharacterStateID StateID) const;
	
protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TArray<USmashCharacterState*> AllStates;
	
	UPROPERTY(BlueprintReadWrite)
	ESmashCharacterStateID CurrentStateID;
	
	UPROPERTY()
	TObjectPtr<USmashCharacterState> CurrentState;

	void FindState();

	void InitState();
};
