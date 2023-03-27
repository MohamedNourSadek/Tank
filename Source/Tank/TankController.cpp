#include "TankController.h"

#pragma region Unreal Delgates
ATankController::ATankController()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
void ATankController::BeginPlay()
{
	Super::BeginPlay();

	myBody = Cast<UPrimitiveComponent>(RootComponent);

	auto myComponents = GetComponents();

	for(auto component : myComponents)
	{
		if(component->GetName().Contains("tank top"))
			tankTop = Cast<UStaticMeshComponent>(component);
		else if(component->GetName().Contains("cannon"))
			canon = Cast<UChildActorComponent>(component);
	}

	Cast<APlayerController>(GetController())->bShowMouseCursor = true;
}
void ATankController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime = DeltaTime;
}
void ATankController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveY", this, &ATankController::InputYRecieved);
	PlayerInputComponent->BindAxis("MoveX", this, &ATankController::InputXRecieved);
	PlayerInputComponent->BindAxis("RotateX", this, &ATankController::RotateCannon);
	PlayerInputComponent->BindAxis("RotateY", this, &ATankController::RotateCannonY);
	PlayerInputComponent->BindAxis("MouseX", this, &ATankController::MouseXInputRecieved);
}
#pragma endregion

#pragma region Input Callbacks
void ATankController::InputYRecieved(float direction)
{
	yInput = direction; 
	if(abs(direction) > 0)
		Translate(direction);
}
void ATankController::InputXRecieved(float direction)
{
	if(abs(direction) > 0)
	{
		if(yInput < 0)
			Rotate(-direction);
		else
			Rotate(direction);
	}
}
#pragma endregion 

#pragma region Movement Functions
void ATankController::Translate(float direction)
{
	myBody->AddForce(GetActorForwardVector() * direction * movementForce);
}
void ATankController::Rotate(float direction)
{
	myBody->AddTorqueInRadians(tankRotationAxis * direction * rotationTorque);
}
void ATankController::RotateCannon(float direction)
{
	if(abs(direction) != 0)
	{
		if(currentCannonSpeed < 1)
			currentCannonSpeed += cannonAcceleration * deltaTime;
		else
			currentCannonSpeed = 1;
	}
	else
	{
		if(currentCannonSpeed > 0)
			currentCannonSpeed -= cannonAcceleration * deltaTime;
		else
			currentCannonSpeed = 0;
	}

	const FVector angle = currentCannonSpeed * FVector(0,1,0) * direction * tankTopMovementSpeed;
	tankTop->AddWorldRotation(FRotator(angle.X, angle.Y, angle.Z));
}
void ATankController::RotateCannonY(float direction)
{
	float currentAngle = canon->GetRelativeRotation().Roll;
	bool outOfNegativeBounds = (currentAngle < -10) && direction > 0;
	bool outOfPositionBounds = (currentAngle > 10) && direction < 0;

	if(!outOfNegativeBounds && !outOfPositionBounds)
	{
		const FVector angle = - FVector(0,0,1) * direction * cannonRotationSpeed;
		canon->AddRelativeRotation(FRotator(angle.X, angle.Y, angle.Z));
	}
}
void ATankController::MouseXInputRecieved(float direction)
{
	if(direction != 0)
		UE_LOG(LogTemp, Display, TEXT("%f"), direction);
}

#pragma endregion 

