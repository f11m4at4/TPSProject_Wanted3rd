
#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMove();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction*
	                           ThisTickFunction) override;

	virtual void SetupInputBinding(class UEnhancedInputComponent* playerInput) override;

public:
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Turn;
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Lookup;

	// 입력 처리 함수
	// 좌우 회전 입력 처리
	void Turn(const struct FInputActionValue& inputValue);
	// 상하 회전 입력 처리
	void Lookup(const struct FInputActionValue& inputValue);

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Move;
	// 이동 입력 처리 함수
	void PlayerMove(const struct FInputActionValue& inputValue);
	FVector direction;

	// ---------- 점프 --------------
	// 중력가속도
	float gravity = -10;
	// 수직속도
	float zVelocity = 0;
	// 필요한 속성 : 점프 입력, 점프파워, 최대 점프 횟수, 현재 점프 횟수
	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Jump;
	// 점프 입력 처리 함수
	void PlayerJump(const struct FInputActionValue& inputValue);

	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float jumpPower = 5;
	// 최대 점프횟수
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	int32 jumpMax = 2;
	// 현재 점프횟수
	int32 currentJumpCount = 0;

	// --------------- Run ------------------
	//걸을때 속도
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float walkSpeed = 200;
	//달릴때 속도
	UPROPERTY(EditAnywhere, Category=PlayerStats)
	float runSpeed = 600;

	UPROPERTY(EditDefaultsOnly, Category=Input)
	class UInputAction* ia_Run;
	// 이동 입력 처리 함수
	void ChangeSpeedInput(const struct FInputActionValue& inputValue);
};
