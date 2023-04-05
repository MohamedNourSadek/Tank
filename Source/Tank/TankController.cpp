#include "TankController.h"

#include "TankGameMode.h"
#include "Kismet/GameplayStatics.h"


#pragma region HelperFunctions

float Remap(float value, float a, float b, float c, float d)
{
	return (value * ((d-c)/(b-a))) + c;
}

bool ATankController::CanFire()
{
	return timeSinceStartUpQuantize != lastFireSec;
}

#pragma endregion 


#pragma region Unreal Delgates
ATankController::ATankController()
{
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
		else if(component->GetName().Contains("Camera"))
			myCam = Cast<USceneComponent>(component);
		else if(component->GetName().Contains("FirePoint"))
			firePosition = Cast<UChildActorComponent>(component);
	}
	
	myController = Cast<APlayerController>(GetController());
}
void ATankController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	deltaTime = DeltaTime;
	timeSinceStartUp += DeltaTime;
	timeSinceStartUpQuantize = (int)(timeSinceStartUp/fireTime);
	HandleMouseInput();

	if(CanFire())
		Cast<ATankGameMode>(UGameplayStatics::GetGameMode(this))->canFire = true;
}
void ATankController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveY", this, &ATankController::InputYRecieved);
	PlayerInputComponent->BindAxis("MoveX", this, &ATankController::InputXRecieved);
	PlayerInputComponent->BindAction("MouseClick", IE_Pressed, this, &ATankController::MouseInput);
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
			RotateTank(-direction);
		else
			RotateTank(direction);
	}
}
void ATankController::MouseInput()
{
	if(CanFire())
	{
		lastFireSec = timeSinceStartUpQuantize;
		Cast<ATankGameMode>(UGameplayStatics::GetGameMode(this))->canFire = false;

		const AActor* myBullet = Cast<AActor>(GetWorld()->SpawnActor(bullet, &firePosition->GetComponentTransform()));
		UPrimitiveComponent* component = Cast<UPrimitiveComponent>(myBullet->GetComponentByClass(UPrimitiveComponent::StaticClass()));

		const FVector force = BulletForce * canon->GetRightVector();
		component->AddImpulse(force);
		myBody->AddImpulse(-reactionForceMultiplier * force);

		AActor* nigraFire = Cast<AActor>(GetWorld()->SpawnActor(fireParticles, &component->GetComponentTransform()));
		nigraFire->AttachToComponent(component, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
	}
}
#pragma endregion 

#pragma region Movement Functions
void ATankController::Translate(float direction)
{
	myBody->AddForce(GetActorForwardVector() * direction * movementForce);
}
void ATankController::RotateTank(float direction)
{
	myBody->AddTorqueInRadians(tankRotationAxis * direction * rotationTorque);
}
void ATankController::RotateTankTop(float direction)
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
void ATankController::HandleMouseInput()
{
	float xPosition = 0;
	float yPosition = 0;
	myController->GetMousePosition(xPosition,yPosition);

	FVector2D mouseRelativePosition;

	if(GEngine && GEngine->GameViewport)
	{
		FVector2D mousePosition;
		FVector2D screenSize;

		GEngine->GameViewport->GetViewportSize(screenSize);
		GEngine->GameViewport->GetMousePosition(mousePosition);

		mouseRelativePosition = FVector2D(mousePosition.X/screenSize.X, mousePosition.Y/screenSize.Y);
	}

	float directionX = Remap(mouseRelativePosition.X, 0,1, -1,1);
	float directionY = Remap(mouseRelativePosition.Y+shiftScreen, 0,1,-1,1);

	RotateTankTop(directionX);
	RotateCannonY(-directionY);
}
#pragma endregion 



