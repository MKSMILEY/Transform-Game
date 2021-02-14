// Fill out your copyright notice in the Description page of Project Settings.


#include "TVanishPlatform.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATVanishPlatform::ATVanishPlatform()
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
void ATVanishPlatform::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ATVanishPlatform::HandleOverlap);
}

void ATVanishPlatform::HandleOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GetWorldTimerManager().SetTimer(VanishTimerHandle, this, &ATVanishPlatform::Vanish, VanishRate, false);
	UE_LOG(LogTemp, Warning, TEXT("Vanish Overlap"))
}

void ATVanishPlatform::Vanish()
{
	Destroy();
}

