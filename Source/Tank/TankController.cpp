// Fill out your copyright notice in the Description page of Project Settings.


#include "TankController.h"

#include "Algo/Rotate.h"

#pragma region Unreal Delgates
ATankController::ATankController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void ATankController::BeginPlay()
{
	Super::BeginPlay();
	
}
void ATankController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Display, TEXT("Tank Tick"));
}
void ATankController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveY", this, &ATankController::InputYRecieved);
	PlayerInputComponent->BindAxis("MoveX", this, &ATankController::InputXRecieved);
}

#pragma endregion

#pragma region private functions
void ATankController::InputYRecieved(float y)
{
	if(y == 0)
	{
		if(currentMovementSpeed >= 0)
			currentMovementSpeed -= movementAcceleration;
		else
			currentMovementSpeed = 0;
	}
	else
	{
		if(abs(currentMovementSpeed) <= movementSpeed)
			currentMovementSpeed += (y*movementAcceleration);
	}
	
	auto currentLocation = GetActorLocation();
	auto currentDirection = GetActorForwardVector();
	
	SetActorLocation(currentLocation + (currentDirection * currentMovementSpeed));		
}

void ATankController::InputXRecieved(float x)
{
	SetActorRotation(GetActorRotation() + FRotator(0, rotationSpeed * x * .01f,0));
}
#pragma endregion 

