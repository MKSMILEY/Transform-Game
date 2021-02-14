// Fill out your copyright notice in the Description page of Project Settings.


#include "TLevelClearZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATLevelClearZone::ATLevelClearZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	RootComponent = OverlapComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = OverlapComp->GetUnscaledBoxExtent();
	DecalComp->SetupAttachment(OverlapComp);

}

// Called when the game starts or when spawned
void ATLevelClearZone::BeginPlay()
{
	Super::BeginPlay();

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &ATLevelClearZone::HandleOverlap);
	
}

void ATLevelClearZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult) 
{
	// Call LevelClear in the BP.
	LevelClear();
	UE_LOG(LogTemp, Warning, TEXT("Overlap on LevelClearZone."));
}

FString ATLevelClearZone::GetLevelToLoad()
{
	// Returns the name of the level that this clear zone is set to load.
	return LevelToLoad;
}

