

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterSettings.h"
#include "GameFramework/Character.h"
#include "Inputs/SmashCharacterInputData.h"
#include "SmashCharacter.generated.h"

class USmashCharacterStateMachine;

UCLASS()
class SMASHUE_API ASmashCharacter : public ACharacter
{
	GENERATED_BODY()
	
#pragma region Unreal Default
public:
	// Sets default values for this character's properties
	ASmashCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	static int CharacterNumber;
	int CharacterId = 0;

#pragma endregion

#pragma region Orient
public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadWrite)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;

#pragma endregion
	
#pragma region State Machine
public:
	void CreateStateMachine();
	void InitStateMachine();
	void TickStateMachine(float DeltaTime) const;
protected:
	UPROPERTY(BlueprintReadOnly);
	TObjectPtr<USmashCharacterStateMachine> StateMachine;

	
#pragma endregion

#pragma region Input Data / Mapping

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<USmashCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;

	
#pragma endregion

#pragma region Input Move 

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputMoveXEvent, float, InputMoveX);
	
public:
	float GetInputMoveX() const;

	float GetInputMoveY() const;
	UPROPERTY()
	FInputMoveXEvent InputMoveXFastEvent;

protected:
	UPROPERTY()
	float InputMoveX = 0.f;
	
	UPROPERTY()
	float InputMoveY = 0.f;

private:
	void BindInputMoveAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent);

	void OnInputMoveX(const FInputActionValue& InputActionValue);
	
	void OnInputMoveXFast(const FInputActionValue& InputActionValue);
	
	void OnInputJump(const FInputActionValue& InputActionValue);

	
#pragma endregion	
};
