#include "TankController.h"

#include "TankGameMode.h"
#include "Kismet/GameplayStatics.h"

#pragma region HelperFunctions
bool ATankController::CanFire() const
{
	return timeSinceStartUpQuantize != lastFireSec;
}
void ATankController::InitializeMyComponents()
{
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

	gameMode = Cast<ATankGameMode>(UGameplayStatics::GetGameMode(this));
}
void ATankController::UpdateTimeVariables(const float& DeltaTime)
{
	deltaTime = DeltaTime;
	timeSinceStartUp += DeltaTime;
	timeSinceStartUpQuantize = (int)(timeSinceStartUp/fireTime);
	currentSpeed = myBody->GetComponentVelocity().Size();
	currentRotationalSpeed = myBody->GetPhysicsAngularVelocityInRadians().Size();
}
void ATankController::UpdateUI()
{
	if(CanFire() && gameMode != nullptr)
		gameMode->canFire = true;
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
	InitializeMyComponents();
}
void ATankController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTimeVariables(DeltaTime);
	UpdateUI();
}
void ATankController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveX", this, &ATankController::RecievedMoveY);
	PlayerInputComponent->BindAxis("MoveY", this, &ATankController::RecievedMoveX);
	PlayerInputComponent->BindAxis("MouseX", this, &ATankController::RecieveMouseX);
	PlayerInputComponent->BindAxis("MouseY", this, &ATankController::RecieveMouseY);
	PlayerInputComponent->BindAction("MouseClick", IE_Pressed, this, &ATankController::RecievedMouseLClick);
}  
#pragma endregion

#pragma region Input Callbacks
void ATankController::RecievedMoveX(float direction)
{
	yInput = direction;
	
	if(abs(direction) > 0)
		Translate(direction);
}
void ATankController::RecievedMoveY(float direction) {

	if(abs(direction) > 0)
	{
		if(yInput < 0)
			RotateTank(-direction);
		else
			RotateTank(direction);
	}
}
void ATankController::RecieveMouseX(float direction) 
{
	RotateTankTop(direction);
}
void ATankController::RecieveMouseY(float direction) 
{
	RotateCannon(direction);
}
void ATankController::RecievedMouseLClick()
{
	if(CanFire())
	{
		FireCannon();
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
	const FVector angle = tankTopRotationalSpeed * direction * FVector::RightVector;
	tankTop->AddWorldRotation(FRotator(angle.X, angle.Y, angle.Z));
}
void ATankController::RotateCannon(float direction) 
{
	float currentAngle = canon->GetRelativeRotation().Roll;
	bool outOfNegativeBounds = (currentAngle < -10) && direction > 0;
	bool outOfPositionBounds = (currentAngle > 10) && direction < 0;

	if(!outOfNegativeBounds && !outOfPositionBounds)
	{
		const FVector angle = - FVector(0,0,1) * direction * tankCannonRotationSpeed;
		canon->AddRelativeRotation(FRotator(angle.X, angle.Y, angle.Z));
	}
}
void ATankController::FireCannon()
{
	lastFireSec = timeSinceStartUpQuantize;
	
	if(gameMode != nullptr)
		gameMode->canFire = false;

	const AActor* myBullet = Cast<AActor>(GetWorld()->SpawnActor(bullet, &firePosition->GetComponentTransform()));
	UPrimitiveComponent* component = Cast<UPrimitiveComponent>(myBullet->GetComponentByClass(UPrimitiveComponent::StaticClass()));

	const FVector forceVector = bulletForce * canon->GetRightVector();
	component->AddImpulse(forceVector);
	myBody->AddImpulse(-reactionForceMultiplier * forceVector);

	AActor* nigraFire = Cast<AActor>(GetWorld()->SpawnActor(fireParticles, &component->GetComponentTransform()));
	nigraFire->AttachToComponent(component, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
}
#pragma endregion 



