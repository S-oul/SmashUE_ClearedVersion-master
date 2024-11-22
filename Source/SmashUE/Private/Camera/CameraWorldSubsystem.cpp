// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"

#include "Camera/CameraFollowTarget.h"
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
	FVector aa = CalculateAveragePositionBetweenTargets();

	ClampPositionIntoCameraBounds(aa);
	
	CameraMain->SetWorldLocation(aa);
}


void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector Average = FVector::ZeroVector;
	
	for (auto Object: FollowTargets)
	{
		ICameraFollowTarget* CameraFollowTarget = Cast<ICameraFollowTarget>(Object);

		if(CameraFollowTarget == nullptr) continue;
		
		if(CameraFollowTarget->IsFollowable())
		{
			Average += CameraFollowTarget->GetFollowPosition();
		}
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

AActor* UCameraWorldSubsystem::FindCameraBoundActor() const
{
	TArray<AActor*> Components;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraBounds",Components);

	return Components.Num() > 0 ? Components[0] : nullptr;
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* BoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;

	BoundsActor->GetActorBounds(false,BoundsCenter,BoundsExtents);
		
	CameraBoundsMin = FVector2D(BoundsCenter.X - BoundsExtents.X,BoundsCenter.Z - BoundsExtents.Z);
	CameraBoundsMax = FVector2D(BoundsCenter.X + BoundsExtents.X,BoundsCenter.Z + BoundsExtents.Z);

	CameraBoundsYProjectionCenter = 1;
	
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	GetViewportBounds(ViewportBoundsMin,ViewportBoundsMax);

	GEngine->AddOnScreenDebugMessage
	(
	-1,
	.01f,
	FColor::Purple,
	FString::Printf(TEXT("ViewportBound \n min : %f %f \n max : %f %f"),
		ViewportBoundsMin.X, ViewportBoundsMin.Y,ViewportBoundsMax.X,ViewportBoundsMax.Y)  
	);

	
	
	FVector WorldBoundsMin = CalculateWorldPositionViewportPosition(ViewportBoundsMax);
	FVector WorldBoundsMax = CalculateWorldPositionViewportPosition(ViewportBoundsMin);
	
	GEngine->AddOnScreenDebugMessage
	(
	-1,
	.01f,
	FColor::Yellow,
	FString::Printf(TEXT("WorldBounds \n min : %f %f %f \n max : %f %f %f"),
		WorldBoundsMin.X, WorldBoundsMin.Y,WorldBoundsMin.Z,WorldBoundsMax.X,WorldBoundsMax.Y,WorldBoundsMax.Z)  
	);

	FVector WorldBoundsOffset = FVector(WorldBoundsMax - WorldBoundsMin);
	Position.X = FMath::Clamp(Position.X, CameraBoundsMin.X + FMath::Abs(WorldBoundsOffset.X/2) ,CameraBoundsMax.X - FMath::Abs(WorldBoundsOffset.X/2));
	Position.Z = FMath::Clamp(Position.Z, CameraBoundsMin.Y + FMath::Abs(WorldBoundsOffset.Z/2),CameraBoundsMax.Y - FMath::Abs(WorldBoundsOffset.X/2));
	//Position.Z = FMath::Clamp(Position.Z,WorldBoundsMin.Z,WorldBoundsMax.Z);

	GEngine->AddOnScreenDebugMessage
		(
		-1,
		.01f,
		FColor::Red,
		FString::Printf(TEXT("Position \n %f %f %f"),
			Position.X, Position.Y,Position.Z)  
		);
}

void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutMin, FVector2D& OutMax)
{
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if(ViewportClient == nullptr) return;
	
	FViewport* Viewport = ViewportClient->Viewport;

	if(Viewport == nullptr) return;
	
	FIntRect ViewRect(
		Viewport->GetInitialPositionXY(),
		Viewport->GetInitialPositionXY() + Viewport->GetSizeXY()
    );
	
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);
	
	OutMin.X = ViewportRect.Min.X;
	OutMin.Y = ViewportRect.Min.Y;

	OutMax.X = ViewportRect.Max.X;
	OutMax.Y = ViewportRect.Max.Y;

}

FVector UCameraWorldSubsystem::CalculateWorldPositionViewportPosition(const FVector2D& ViewportPosition)
{
	if(CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if(PlayerController == nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir, WorldPosition;

	UGameplayStatics::DeprojectScreenToWorld(
		PlayerController,
		ViewportPosition,
		WorldPosition,
		CameraWorldProjectDir
    );

	WorldPosition += CameraWorldProjectDir * YDistanceToCenter;
	return WorldPosition;
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* Target)
{
	FollowTargets.Add(Target);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* Target)
{
	FollowTargets.Remove(Target);
}
