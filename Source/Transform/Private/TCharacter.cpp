// Fill out your copyright notice in the Description page of Project Settings.


#include "TCharacter.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "..\Public\TCharacter.h"

// Sets default values
ATCharacter::ATCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bGliding = false;
	bGlideSwap = false;
	bClimbing = false;
	bFormChanged = false;
}

// Called when the game starts or when spawned
void ATCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurForm = TForm::TF_Default;
	PrevForm = TForm::TF_Default;
	CharMoveComp = GetCharacterMovement();

	TransformJump();
}

// Called every frame
void ATCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurZ = CurVelocity.Z;

	UE_LOG(LogTemp, Warning, TEXT("CurZ: %f"), CurZ);

	CheckGlide();

	if (CurForm == TForm::TF_Glide && bGliding) {
		if (bClimbing) {
			AddMovementInput(GetActorUpVector(), 0.33f);
		}
		else {
			AddMovementInput(GetActorUpVector(), -0.33f);
		}
	}

	bFormChanged = false;
}

// Called to bind functionality to input
void ATCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATCharacter::MoveRight);

	PlayerInputComponent->BindAxis("ViewPitch", this, &ATCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("ViewYaw", this, &ATCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("TransformDefault", IE_Pressed, this, &ATCharacter::TransformJump);
	PlayerInputComponent->BindAction("TransformSpeed", IE_Pressed, this, &ATCharacter::TransformSpeed);
	PlayerInputComponent->BindAction("TransformGlide", IE_Pressed, this, &ATCharacter::TransformGlide);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATCharacter::Jump);
	PlayerInputComponent->BindAction("StopGlide", IE_Released, this, &ATCharacter::StopGlide);

}

// Called by PlayerPawn BP to set CurVelocity for use in the gliding mechanics.
void ATCharacter::SetVelocity(FVector Velocity)
{
	CurVelocity = Velocity;
}

// Called by WBP_HUD to get and display the name of the current form.
FString ATCharacter::GetFormName()
{
	FString Ret = "";
	if (CurForm == TForm::TF_Jump) {
		Ret = "Jump";
	}
	else if (CurForm == TForm::TF_Speed) {
		Ret = "Speed";
	}
	else if (CurForm == TForm::TF_Glide) {
		Ret = "Glide";
	}
	else {
		Ret = "Default";
	}

	return Ret;
}

// Sets the PlayerController settings to allow UI interactions.
void ATCharacter::UIInput()
{
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	this->DisableInput(nullptr);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
}

// Sets the PlayerController settings to allow Game interactions.
void ATCharacter::GameInput()
{
	APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	this->EnableInput(nullptr);
	PC->SetInputMode(FInputModeGameOnly());
	PC->bShowMouseCursor = false;
}

void ATCharacter::SetClimbing(bool bClimb)
{
	bClimbing = bClimb;
}

// Called when the button to move forward/backward is pressed.
void ATCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector(), Value, true);
}

// Called when the button to move right/left is pressed.
void ATCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector(), Value, true);
}

void ATCharacter::Jump() {
	ACharacter::Jump();
	bGliding = true;
}

void ATCharacter::StopGlide() {
	bGliding = false;
}

void ATCharacter::CheckGlide() {

	float DeltaZ = 0.001f;
	float AbsZ = UKismetMathLibrary::Abs(CurVelocity.Z);
	if (CurForm == TForm::TF_Glide && bGliding && !(AbsZ < DeltaZ)) {
		if(CharMoveComp->MovementMode != EMovementMode::MOVE_Flying) {
			CharMoveComp->MovementMode = EMovementMode::MOVE_Flying;
		}
		bGlideSwap = true;
	}
	else {
		if(bGlideSwap) {
			if (CharMoveComp->MovementMode != EMovementMode::MOVE_Falling) {
				CharMoveComp->MovementMode = EMovementMode::MOVE_Falling;
			}
			bGlideSwap = false;
		}
	}
}

// Called when the button to transform to Default form is pressed. Transforms to Default form if not already in this form.
void ATCharacter::TransformJump()
{
	if (CurForm != TForm::TF_Jump) {
		PrevForm = CurForm;
		CurForm = TForm::TF_Jump;
		bFormChanged = true;

		// Adjust current transform to account for the change in height of capsule.
		if (PrevForm == TForm::TF_Speed) {
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 50.0f);
			FTransform TransformAdjustment = FTransform(NewLocation);
			//AddActorLocalTransform(TransformAdjustment);
			SetActorLocation(NewLocation);
		}
		else if (PrevForm == TForm::TF_Glide) {
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 75.0f);
			FTransform TransformAdjustment = FTransform(NewLocation);
			//AddActorLocalTransform(TransformAdjustment);
			SetActorLocation(NewLocation);
		}

		// Set the relevant variables for this form.
		GetCharacterMovement()->JumpZVelocity = 600.0f;
		GetCharacterMovement()->MaxWalkSpeed = 750.0f;
		GetCharacterMovement()->AirControl = 1.0f;
		GetCapsuleComponent()->SetCapsuleHalfHeight(100.0f, true);

		// TODO Make the transformation take time.
		// Create a for loop and add/subtract one point to CapsuleHalfHeight until it is the right size.
	}
}

// Called when the button to transform to Speed form is pressed. Transforms to Speed form if not already in this form.
void ATCharacter::TransformSpeed()
{
	if (CurForm != TForm::TF_Speed) {
		PrevForm = CurForm;
		CurForm = TForm::TF_Speed;
		bFormChanged = true;

		// Adjust current transform to account for the change in height of capsule.
		if (PrevForm == TForm::TF_Glide) {
			FVector NewLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 25.0f);
			FTransform TransformAdjustment = FTransform(NewLocation);
			//AddActorLocalTransform(TransformAdjustment);
			SetActorLocation(NewLocation);
		}

		// Set the relevant variables for this form.
		GetCharacterMovement()->JumpZVelocity = 450.0f;
		GetCharacterMovement()->MaxWalkSpeed = 1750.0f;
		GetCharacterMovement()->AirControl = 0.1f;
		GetCapsuleComponent()->SetCapsuleHalfHeight(50.0f, true);

		// TODO Make the transformation take time.
		// Create a for loop and add/subtract one point to CapsuleHalfHeight until it is the right size.
	}
}

// Called when the button to transform to Glide form is pressed. Transforms to Glide form if not already in this form.
void ATCharacter::TransformGlide()
{
	if (CurForm != TForm::TF_Glide) {
		PrevForm = CurForm;
		CurForm = TForm::TF_Glide;
		bFormChanged = true;

		// Set the relevant variables for this form.
		GetCharacterMovement()->JumpZVelocity = 300.0f;
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
		GetCharacterMovement()->AirControl = 1.0f;
		GetCapsuleComponent()->SetCapsuleHalfHeight(25.0f, true);

		// TODO Make the transformation take time.
		// Create a for loop and add/subtract one point to CapsuleHalfHeight until it is the right size.
	}
}
