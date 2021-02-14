// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TCharacter.generated.h"


UCLASS()
class TRANSFORM_API ATCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetClimbing(bool bClimb);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector CurVelocity;
	FVector JumpVelocity;

	TForm CurForm;
	TForm PrevForm;

	UCharacterMovementComponent *CharMoveComp;

	bool bGliding;
	bool bGlideSwap;
	bool bClimbing;
	bool bFormChanged;

	UFUNCTION(BlueprintCallable)
		void SetVelocity(FVector Velocity);

	UFUNCTION(BlueprintCallable)
		FString GetFormName();

	UFUNCTION(BlueprintCallable)
		void UIInput();

	UFUNCTION(BlueprintCallable)
		void GameInput();

	void MoveForward(float Value);
	void MoveRight(float Value);

	void Jump();
	void StopGlide();
	void CheckGlide();

	void TransformJump();
	void TransformSpeed();
	void TransformGlide();
};

UENUM()
enum class TForm : uint8
{
	TF_Default		UMETA(DisplayName = "Default"),
	TF_Jump			UMETA(DisplayName = "Jump"),
	TF_Speed		UMETA(DisplayName = "Speed"),
	TF_Glide		UMETA(DisplayName = "Glide")
};