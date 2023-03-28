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
	void RotateTank(float direction);
	void RotateTankTop(float direction);
	void RotateCannonY(float direction);
	void HandleMouseInput();
public:
	UPROPERTY(EditAnywhere) int64 movementForce = 5000000000;
	UPROPERTY(EditAnywhere) float rotationTorque= 5000000;
	UPROPERTY(EditAnywhere) float cannonAcceleration = 1;
	UPROPERTY(EditAnywhere) float tankTopMovementSpeed = 0.15;
	UPROPERTY(EditAnywhere) float cannonRotationSpeed = 5;
	UPROPERTY(EditAnywhere) UPrimitiveComponent* myBody;
	UPROPERTY(EditAnywhere) FVector tankRotationAxis;
	UPROPERTY(EditAnywhere) FVector tankTopRotationAxis;
	UPROPERTY(EditAnywhere) UStaticMeshComponent* tankTop;
	UPROPERTY(EditAnywhere) UChildActorComponent* canon;
	UPROPERTY(EditAnywhere) float shiftScreen;

private:
	UPROPERTY(EditAnywhere) AActor* bullet;
	UPROPERTY(EditAnywhere) AActor* firePosition;
	
	APlayerController* myController;
	USceneComponent* myCam;
	float currentCannonSpeed;
	float deltaTime = 0;
	float yInput = 0;
};
