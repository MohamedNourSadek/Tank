// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankController.generated.h"

UCLASS()
class TANK_API ATankController : public APawn
{
	GENERATED_BODY()


	
protected:
	virtual void BeginPlay() override;
	
public:
	ATankController();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void InputYRecieved(float y);
	void InputXRecieved(float x);
	void Accelerate(int direction);
	void Declerate();
	void AccelerateRotationOverTime(int direction);
	void DecelerationRotationOverTime();
	void Translate();
	void Rotate();
private:
	UPROPERTY(EditAnywhere) int64 movementForce = 5000000000;
	UPROPERTY(EditAnywhere)	float movementAcceleration = 1;
	UPROPERTY(EditAnywhere) float movementDeceleration = 1;
	UPROPERTY(EditAnywhere) float movementSpeed = 1;
	UPROPERTY(EditAnywhere) float rotationAcceleration = 1;
	UPROPERTY(EditAnywhere) float rotationDeceleration = 1;
	UPROPERTY(EditAnywhere) float rotationSpeed = 1;

	UPROPERTY(EditAnywhere) float currentMovementSpeed = 0;
	UPROPERTY(EditAnywhere) float currentRotationSpeed = 0;
	float deltaTime = 0;
};
