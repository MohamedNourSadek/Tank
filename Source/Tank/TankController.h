// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankGameMode.h"
#include "TankController.generated.h"

UCLASS()
class TANK_API ATankController : public APawn
{
	GENERATED_BODY()

#pragma region InputFunctions
private:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void RecievedMoveX(float y);
	void RecievedMoveY(float x) ;
	void RecieveMouseX(float direction) ;
	void RecieveMouseY(float direction) ;
	void RecievedMouseLClick();
#pragma endregion

#pragma region Gameplay Functions
private:
	void FireCannon();
	void Translate(float direction) ;
	void RotateTank(float direction) ;
	void RotateTankTop(float direction) ;
	void RotateCannon(float direction) ;
#pragma endregion 

#pragma region Unreal Functions
protected:
	ATankController();
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
#pragma endregion 

#pragma region Helper functions
private:
	bool CanFire() const;
	void InitializeMyComponents();
	void UpdateTimeVariables(const float& DeltaTime);
	void UpdateUI();
#pragma endregion 

#pragma region Gameplay varialbes
	UPROPERTY(EditAnywhere) int64 movementForce = 5000000000;
	UPROPERTY(EditAnywhere) float rotationTorque= 5000000;
	UPROPERTY(EditAnywhere) float tankTopRotationalSpeed = 0.25;
	UPROPERTY(EditAnywhere) float tankCannonRotationSpeed = 0.25;
	UPROPERTY(EditAnywhere) FVector tankRotationAxis;
	UPROPERTY(EditAnywhere) FVector tankTopRotationAxis;
	UPROPERTY(EditAnywhere) float bulletForce;
	UPROPERTY(EditAnywhere) float reactionForceMultiplier;
	UPROPERTY(EditAnywhere) float fireTime = 1.0f;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float currentSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float currentRotationalSpeed = 0;
#pragma endregion 

#pragma region References
private:
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> bullet;
	UPROPERTY(EditAnywhere) UChildActorComponent* firePosition;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> fireParticles;
	UPrimitiveComponent* myBody;
	UStaticMeshComponent* tankTop;
	UChildActorComponent* canon;
	USceneComponent* myCam;
	ATankGameMode* gameMode;

#pragma endregion

#pragma region privateVariables
	float timeSinceStartUp = 0.0f;
	int timeSinceStartUpQuantize = 0;
	int lastFireSec = -1;
	float deltaTime = 0;
	float yInput = 0;
#pragma endregion 
};
