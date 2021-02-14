// Fill out your copyright notice in the Description page of Project Settings.


#include "TCheckpointZone.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATCheckpointZone::ATCheckpointZone()
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
void ATCheckpointZone::BeginPlay()
{
	Super::BeginPlay();
	
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ATCheckpointZone::HandleOverlap);
}

void ATCheckpointZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	SetCheckpoint();
}

void ATCheckpointZone::SetCheckpoint()
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap on CheckpointZone"));
}



