// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMove.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UPlayerMove::UPlayerMove()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UInputAction> TempTurnInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Turn.IA_Turn'"));
	if (TempTurnInput.Succeeded())
	{
		ia_Turn = TempTurnInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempLookupInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Lookup.IA_Lookup'"));
	if (TempLookupInput.Succeeded())
	{
		ia_Lookup = TempLookupInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempMoveInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (TempMoveInput.Succeeded())
	{
		ia_Move = TempMoveInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempJumpInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (TempJumpInput.Succeeded())
	{
		ia_Jump = TempJumpInput.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempRunInput(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Run.IA_Run'"));
	if (TempRunInput.Succeeded())
	{
		ia_Run = TempRunInput.Object;
	}
	
}


// Called when the game starts
void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	// 초기 속도는 걷는 속도로 설정
	moveComp->MaxWalkSpeed = walkSpeed;
	
}


// Called every frame
void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType,
                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 바라보는 방향으로 이동하고 싶다.
	// 1. 캐릭터가 바라보는 방향
	// 2. 카메라가 바라보는 방향(컨트롤러)
	direction = FTransform(me->GetControlRotation()).TransformVector(direction);

	me->AddMovementInput(direction);
	direction = FVector::ZeroVector;

	//				v = v0 + at : 등가속운동
	// 수직속도 계산 v = v0 + at :
	// zVelocity += gravity * DeltaTime;
	// // 수직이동 P = P0 + vt
	// FHitResult result;
	// SetActorLocation(GetActorLocation() + FVector(0, 0, zVelocity) * walkSpeed * DeltaTime, true, &result);
	//
	// // 이동하고 싶다. P = P0 + vt : 등속운동
	// FVector P0 = GetActorLocation();
	// FVector vt = direction * walkSpeed * DeltaTime;
	// FVector P = P0 + vt;
	// SetActorLocation(P, true);
	//
	// // zvelocity 가 언제 0이 되어야 하는가?
	// // 바닥에 있다면 ZVelocity 를 0으로 초기화
	// if (result.IsValidBlockingHit())
	// {
	// 	zVelocity = 0;
	// 	currentJumpCount = 0;
	// }

	// 점프 : 사용자가 점프 버튼을 누르면 점프하고 싶다. 최대 점프 횟수만큼 점프 가능
	
}

void UPlayerMove::SetupInputBinding(class UEnhancedInputComponent* playerInput)
{
	Super::SetupInputBinding(playerInput);

	playerInput->BindAction(ia_Turn, ETriggerEvent::Triggered, this, &UPlayerMove::Turn);
	playerInput->BindAction(ia_Lookup, ETriggerEvent::Triggered, this, &UPlayerMove::Lookup);
	//------------------------------
	playerInput->BindAction(ia_Move, ETriggerEvent::Triggered, this, &UPlayerMove::PlayerMove);

	playerInput->BindAction(ia_Jump, ETriggerEvent::Started, this, &UPlayerMove::PlayerJump);

	playerInput->BindAction(ia_Run, ETriggerEvent::Started, this, &UPlayerMove::ChangeSpeedInput);
	playerInput->BindAction(ia_Run, ETriggerEvent::Completed, this, &UPlayerMove::ChangeSpeedInput);
}

void UPlayerMove::Turn(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	me->AddControllerYawInput(value);
}

void UPlayerMove::Lookup(const struct FInputActionValue& inputValue)
{
	float value = inputValue.Get<float>();
	me->AddControllerPitchInput(value);
}

void UPlayerMove::PlayerMove(const struct FInputActionValue& inputValue)
{
	// 상하좌우 입력을 받으면 이동하고 싶다.
	// -> 입력을 받아서 방향을 만들자.
	FVector2d value = inputValue.Get<FVector2d>();
	direction.X = value.X;
	direction.Y = value.Y;
}


void UPlayerMove::PlayerJump(const struct FInputActionValue& inputValue)
{
	// 최대 점프 횟수보다 작게 뛰었다면
	// if (currentJumpCount < jumpMax)
	// {
	// 	// 수직속도를 점프힘으로 설정한다.
	// 	zVelocity = jumpPower;
	// 	// 현재 점프 횟수를 늘려준다.
	// 	currentJumpCount++;
	// }
	me->Jump();
}

void UPlayerMove::ChangeSpeedInput(const struct FInputActionValue& inputValue)
{
	if (moveComp->MaxWalkSpeed > walkSpeed)
	{
		moveComp->MaxWalkSpeed = walkSpeed;
	}
	else
	{
		moveComp->MaxWalkSpeed = runSpeed;
	}
}