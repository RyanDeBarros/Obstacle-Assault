// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	PlatformVelocity = GetTransform().Rotator().RotateVector(PlatformVelocity);
	//GetTransform().Rotator().RotateVector(PlatformVelocity);
	StartLocation = GetActorLocation() + RelativeStartDist * PlatformVelocity.GetSafeNormal();
	EndLocation = GetActorLocation() + RelativeEndDist * PlatformVelocity.GetSafeNormal();
	MoveDistance = FVector::Dist(StartLocation, EndLocation);
	CurrentRelativePoint = EndLocation;
	CalculateCurrentMoveProportion();
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MoveDistance > 0) MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	if (ShouldPlatformReturn())
	{
		CurrentRelativePoint = CurrentRelativePoint == StartLocation ? EndLocation : StartLocation;
		SetActorLocation(CurrentRelativePoint);
		PlatformVelocity *= -1;
	}
	else
	{
		SetActorLocation(GetActorLocation() + DeltaTime * PlatformVelocity);
	}
	CalculateCurrentMoveProportion();
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	AddActorLocalRotation(DeltaTime * RotationVelocity);
}

bool AMovingPlatform::ShouldPlatformReturn() const
{
	return CurrentMoveProportion > 1;
}

float AMovingPlatform::GetDistanceMoved() const
{
	return FVector::Dist(StartLocation, GetActorLocation());
}

void AMovingPlatform::CalculateCurrentMoveProportion()
{
	if (MoveDistance != 0) CurrentMoveProportion = FVector::Dist(CurrentRelativePoint, GetActorLocation()) / MoveDistance;
}
