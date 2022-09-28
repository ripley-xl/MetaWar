// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACameraPawn::Move_X()
{
	// ScrollSpeed
	auto PlayerController = GetWorld()->GetFirstPlayerController();
	float Pos_X,_;
	PlayerController->GetMousePosition(Pos_X,_);
	FVector2D Size;
	GetWorld()->GetGameViewport()->GetViewportSize(Size);
	auto Rate = Pos_X/Size.X;
	auto Direction = 0;
	if (Rate >=0.98)
	{
		Direction = 1;
	}
	else if (Rate <=0.02)
	{
		Direction = -1;
	}
	auto DeltaLocation = FVector(0,ScrollSpeed*Direction*GetWorld()->GetDeltaSeconds(),0);
	AddActorWorldOffset(DeltaLocation);
}

void ACameraPawn::Move_Y()
{
	// ScrollSpeed
	auto PlayerController = GetWorld()->GetFirstPlayerController();
	float _,Pos_Y;
	PlayerController->GetMousePosition(_,Pos_Y);
	FVector2D Size;
	GetWorld()->GetGameViewport()->GetViewportSize(Size);
	auto Rate = Pos_Y/Size.Y;
	auto Direction = 0;
	if (Rate >=0.98)
	{
		Direction = -1;
	}
	else if (Rate <=0.02)
	{
		Direction = 1;
	}
	auto DeltaLocation = FVector(ScrollSpeed*Direction*GetWorld()->GetDeltaSeconds(),0,0);
	AddActorWorldOffset(DeltaLocation);
}

