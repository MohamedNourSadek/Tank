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
	myBody->AddTorqueInRadians(rotationAxis * direction * rotationTorque);
}
#pragma endregion 

