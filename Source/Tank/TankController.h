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
	void Translate(float direction);
	void Rotate(float direction);
	
private:
	UPROPERTY(EditAnywhere) int64 movementForce = 5000000000;
	UPROPERTY(EditAnywhere) float rotationTorque= 5000000;
	UPROPERTY(EditAnywhere) UPrimitiveComponent* myBody;
	UPROPERTY(EditAnywhere) FVector rotationAxis;

	float deltaTime = 0;
	float yInput = 0;
};
