// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SMASHUE_API UCameraWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

#pragma region Subsystems Overrides
public:
	virtual void PostInitialize() override;
	
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

#pragma endregion

#pragma region Main Camera
public:

	virtual void Tick(float DeltaTime) override;
	
	void TickUpdateCameraPosition(float DeltaTime);

protected:
	UCameraComponent* FindCameraByTag(const FName& Tag) const;

protected:

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;
#pragma endregion
#pragma region Follow Targs
public:
	void AddFollowTarget(UObject* Target);
	void RemoveFollowTarget(UObject* Target);

protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;
#pragma endregion
#pragma region Misc
public:

	virtual TStatId GetStatId() const override {return TStatId();};
protected:
	FVector CalculateAveragePositionBetweenTargets();
#pragma endregion
	
#pragma region Bounds

protected:
	UPROPERTY()
	FVector2D CameraBoundsMin;

	UPROPERTY()
	FVector2D CameraBoundsMax;

	UPROPERTY()
	float CameraBoundsYProjectionCenter;
	
	AActor* FindCameraBoundActor() const;

	void InitCameraBounds(AActor* BoundsActor);

	void ClampPositionIntoCameraBounds(FVector& Position);

	void GetViewportBounds(FVector2D& OutMin, FVector2D& OutMax);

	FVector CalculateWorldPositionViewportPosition(const FVector2D& ViewportPosition);
	
#pragma  endregion


};
