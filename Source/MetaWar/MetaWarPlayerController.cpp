// Copyright Epic Games, Inc. All Rights Reserved.

#include "MetaWarPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "MetaWarCharacter.h"

AMetaWarPlayerController::AMetaWarPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void AMetaWarPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// lock mouse always
	FInputModeGameAndUI  InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	SetInputMode(InputMode);
}


void AMetaWarPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (!GetPawn()->IsA(AMetaWarCharacter::StaticClass()))
	{
		return;
	}

	if(bInputPressed)
	{
		FollowTime += DeltaTime;

		// Look for the touch location
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		if(bIsTouch)
		{
			GetHitResultUnderFinger(ETouchIndex::Touch1, ECC_Visibility, true, Hit);
		}
		else
		{
			GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		}
		HitLocation = Hit.Location;

		// Direct the Pawn towards that location
		if(APawn* const MyPawn = GetPawn())
		{
			FVector WorldDirection = (HitLocation - MyPawn->GetActorLocation()).GetSafeNormal();
			MyPawn->AddMovementInput(WorldDirection, 1.f, false);
		}
	}
	else
	{
		FollowTime = 0.f;
	}
}

void AMetaWarPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AMetaWarPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AMetaWarPlayerController::OnSetDestinationReleased);

	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMetaWarPlayerController::OnTouchPressed);
	InputComponent->BindTouch(EInputEvent::IE_Released, this, &AMetaWarPlayerController::OnTouchReleased);

}

void AMetaWarPlayerController::OnSetDestinationPressed()
{
	if (!GetPawn()->IsA(AMetaWarCharacter::StaticClass()))
	{
		return;
	}
	// We flag that the input is being pressed
	bInputPressed = true;
	// Just in case the character was moving because of a previous short press we stop it
	StopMovement();
}

void AMetaWarPlayerController::OnSetDestinationReleased()
{
	if (!GetPawn()->IsA(AMetaWarCharacter::StaticClass()))
	{
		return;
	}
	// Player is no longer pressing the input
	bInputPressed = false;

	// If it was a short press
	if(FollowTime <= ShortPressThreshold)
	{
		// We look for the location in the world where the player has pressed the input
		FVector HitLocation = FVector::ZeroVector;
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, true, Hit);
		HitLocation = Hit.Location;

		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, HitLocation);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, HitLocation, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}
}

void AMetaWarPlayerController::OnTouchPressed(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!GetPawn()->IsA(AMetaWarCharacter::StaticClass()))
	{
		return;
	}
	bIsTouch = true;
	OnSetDestinationPressed();
}

void AMetaWarPlayerController::OnTouchReleased(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (!GetPawn()->IsA(AMetaWarCharacter::StaticClass()))
	{
		return;
	}
	bIsTouch = false;
	OnSetDestinationReleased();
}
