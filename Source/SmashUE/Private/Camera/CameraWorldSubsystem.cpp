// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Kismet/GameplayStatics.h"

void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(TEXT("CameraMain"));
	
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	CameraMain->SetWorldLocation(CalculateAveragePositionBetweenTargets());
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector Average = FVector::ZeroVector;
	for (auto Actor: FollowTargets)
	{
		Average = Average + Actor->GetActorLocation();
	}
	
	Average /= FollowTargets.Num();

	Average.Y = CameraMain->GetComponentLocation().Y;

	return Average;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> Components;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag,Components);
	return Components.Num() > 0 ? Components[0]->FindComponentByClass<UCameraComponent>() : nullptr;
}

void UCameraWorldSubsystem::AddFollowTarget(AActor* Target)
{
	FollowTargets.Add(Target);
}

void UCameraWorldSubsystem::RemoveFollowTarget(AActor* Target)
{
	FollowTargets.Remove(Target);
}