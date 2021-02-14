// Fill out your copyright notice in the Description page of Project Settings.


#include "TClimbingWall.h"
#include "TCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

// Sets default values
ATClimbingWall::ATClimbingWall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	RootComponent = MeshComp;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetupAttachment(MeshComp);

	
}

// Called when the game starts or when spawned
void ATClimbingWall::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ATClimbingWall::OnOverlapBegin);
	OverlapComp->OnComponentEndOverlap.AddDynamic(this, &ATClimbingWall::OnOverlapEnd);
	
}

void ATClimbingWall::OnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Begin Climbing Overlap"));
	ATCharacter *Char = Cast<ATCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Char->SetClimbing(true);
}

void ATClimbingWall::OnOverlapEnd(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//UE_LOG(LogTemp, Warning, TEXT("End Climbing Overlap"));
	ATCharacter *Char = Cast<ATCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	Char->SetClimbing(false);
}
