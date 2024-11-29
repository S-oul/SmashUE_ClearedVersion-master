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

	InitCamZoomParameters();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
}
void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	FVector Position = CalculateAveragePositionBetweenTargets();

	ClampPositionIntoCameraBounds(Position);
	
	CameraMain->SetWorldLocation(Position);
}



void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if(CameraMain == nullptr) return;
	float MaxDistance = CalculateGreatastDistanceBetweenTargets();

	float ClampedMaxDistance = FMath::Clamp(MaxDistance, CamZoomYMin,CamZoomYMax);
	FVector pos = CameraMain->GetComponentLocation();
	pos.Y = (ClampedMaxDistance - CamZoomDistanceMin)/(CamZoomDistanceMax - CamZoomDistanceMin);

	CameraMain->SetWorldLocation(pos);
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
	
	FVector WorldBoundsMin = CalculateWorldPositionViewportPosition(ViewportBoundsMax);
	FVector WorldBoundsMax = CalculateWorldPositionViewportPosition(ViewportBoundsMin);


	Position.X = FMath::Clamp(Position.X, -FMath::Abs(WorldBoundsMin.X/2) ,FMath::Abs(WorldBoundsMax.X/2));
	Position.Z = FMath::Clamp(Position.Z,FMath::Abs(WorldBoundsMin.Z/2),ViewportBoundsMax.Y);
	
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

void UCameraWorldSubsystem::InitCamZoomParameters()
{
	TArray<AActor*> Components;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraDistanceMin",Components);
	Components.Num() > 0 ? CamZoomYMin = Components[0]->GetActorLocation().Y : -1.f;

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "CameraDistanceMax",Components);
	Components.Num() > 0 ? CamZoomYMax = Components[0]->GetActorLocation().Y : -1.f;
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* Target)
{
	FollowTargets.Add(Target);
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* Target)
{
	FollowTargets.Remove(Target);
}

float UCameraWorldSubsystem::CalculateGreatastDistanceBetweenTargets()
{
	float MaxDistance = 0.0f;

	for (auto player : FollowTargets)
	{
		ICameraFollowTarget* Target1 = Cast<ICameraFollowTarget>(player);

		if(Target1 == nullptr) continue;
		
		if(Target1->IsFollowable())
		{
			for (auto player2 : FollowTargets)
			{
				if (player != player2)
				{
					ICameraFollowTarget* Target2 = Cast<ICameraFollowTarget>(player);

					if(Target2 == nullptr) continue;
					
					if(Target2->IsFollowable())
					{
						FVector difference = (Target1->GetFollowPosition() - Target2->GetFollowPosition());

						if(difference.Length() > MaxDistance) MaxDistance = difference.Length();
					}
				}
			}
		}
	}
	return MaxDistance;
}
