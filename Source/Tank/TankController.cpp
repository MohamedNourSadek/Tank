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
}
#pragma endregion

#pragma region Input Callbacks
void ATankController::InputYRecieved(float direction)
{
	if(abs(direction) > 0)
		Accelerate(direction);
	else
		Declerate();
}
void ATankController::InputXRecieved(float direction)
{
	if(abs(direction) > 0)
		AccelerateRotationOverTime(direction);
	else
		DecelerationRotationOverTime();
}
#pragma endregion 

#pragma region Movement Functions
void ATankController::Accelerate(int direction)
{
	if(abs(currentMovementSpeed) <= movementSpeed)
		currentMovementSpeed += (direction  * movementAcceleration);

	if(direction != 0)
		Translate();
}
void ATankController::Declerate()
{
	const int DirectionOfDeceleration = currentMovementSpeed / abs(currentMovementSpeed);

	if(abs(currentMovementSpeed) > 0.01*movementSpeed)
		currentMovementSpeed -= (DirectionOfDeceleration  *movementDeceleration);
	else
		currentMovementSpeed = 0;
}
void ATankController::AccelerateRotationOverTime(int direction)
{
	if(abs(currentRotationSpeed) <= rotationSpeed)
	{
		currentRotationSpeed += (direction  *rotationAcceleration);
	}

	Rotate();
}
void ATankController::DecelerationRotationOverTime()
{
	const int DirectionOfDeceleration = currentRotationSpeed / abs(currentRotationSpeed);

	if(abs(currentRotationSpeed) >= .05*rotationSpeed)
		currentRotationSpeed -= (DirectionOfDeceleration * rotationDeceleration);
	else
		currentRotationSpeed = 0;

	Rotate();
}
void ATankController::Translate()
{
	auto currentLocation = GetActorLocation();
	auto currentDirection = GetActorForwardVector();

	UPrimitiveComponent* myBody = Cast<UPrimitiveComponent>(RootComponent);
	//myBody->AddImpulse((currentDirection * currentMovementSpeed * deltaTime));
	myBody->AddForce(FVector(0,1,0) * movementForce * deltaTime);
	UE_LOG(LogTemp, Display, TEXT("Force is added"));
}
void ATankController::Rotate()
{
	SetActorRotation(GetActorRotation() + FRotator(0, currentRotationSpeed * deltaTime,0));
}
#pragma endregion 

