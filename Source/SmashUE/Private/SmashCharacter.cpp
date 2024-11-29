


#include "SmashCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "SmashCharacterStateMachine.h"
#include "Camera/CameraWorldSubsystem.h"
#include "GameFramework/CharacterMovementComponent.h"

int ASmashCharacter::CharacterNumber = 0;

// Sets default values
ASmashCharacter::ASmashCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmashCharacter::BeginPlay()
{

	if(CharacterNumber > 4 ) CharacterNumber = 0;
	
	CharacterId = CharacterNumber++; 
	
	Super::BeginPlay();
	CreateStateMachine();
	InitStateMachine();

	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
}

// Called every frame
void ASmashCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();

}

// Called to bind functionality to input
void ASmashCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if(EnhancedInputComponent == nullptr) return;

	BindInputMoveAxisAndActions(EnhancedInputComponent);
}

float ASmashCharacter::GetOrientX() const
{
	return OrientX;
}

void ASmashCharacter::SetOrientX(float NewOrientX)
{
	OrientX = NewOrientX;
}

void ASmashCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.0f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ASmashCharacter::CreateStateMachine()
{
	StateMachine = NewObject<USmashCharacterStateMachine>(this);
}

void ASmashCharacter::InitStateMachine()
{
	if(StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ASmashCharacter::TickStateMachine(float DeltaTime) const
{
	if(StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

void ASmashCharacter::SetupMappingContextIntoController() const
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController == nullptr) return;

	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if(Player == nullptr) return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = Player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(InputSystem == nullptr) return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

float ASmashCharacter::GetInputMoveX() const
{
	return InputMoveX;
}

float ASmashCharacter::GetInputMoveY() const
{
	return InputMoveY;
}

void ASmashCharacter::BindInputMoveAxisAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if(InputMappingContext == nullptr) return;

	if(InputData->InputActionMoveX)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveX
			);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ASmashCharacter::OnInputMoveX
			);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
,
			&ASmashCharacter::OnInputMoveX
			);
	}

	if(InputData->InputActionMoveFastX)
	{
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveFastX,
			ETriggerEvent::Started,
			this,
			&ASmashCharacter::OnInputMoveXFast
			);
	}

	if(InputData->InputActionJump)
	{
		EnhancedInputComponent->BindAction(
		InputData->InputActionJump,
		ETriggerEvent::Started,
		this,
		&ASmashCharacter::OnInputJump
		);

		EnhancedInputComponent->BindAction(
		InputData->InputActionJump,
		ETriggerEvent::Completed,
		this,
		&ASmashCharacter::OnInputJump
		);
		
		EnhancedInputComponent->BindAction(
		InputData->InputActionJump,
		ETriggerEvent::Triggered,
		this,
		&ASmashCharacter::OnInputJump
		);
	}
}

void ASmashCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
}

void ASmashCharacter::OnInputMoveXFast(const FInputActionValue& InputActionValue)
{
	InputMoveX = InputActionValue.Get<float>();
	InputMoveXFastEvent.Broadcast(InputMoveX);
}

void ASmashCharacter::OnInputJump(const FInputActionValue& InputActionValue)
{
	InputMoveY = InputActionValue.Get<float>();
}

FVector ASmashCharacter::GetFollowPosition()
{
	return GetActorLocation();
}

bool ASmashCharacter::IsFollowable()
{
	return true; //isFollowable;
}


