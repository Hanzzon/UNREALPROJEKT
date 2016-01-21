// Fill out your copyright notice in the Description page of Project Settings.

#include "UNREALPROJEKT.h"
#include "ElementWiggle.h"


// Sets default values
AElementWiggle::AElementWiggle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	main_mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("ElementMesh"));
	main_mesh->AttachTo(RootComponent);	
}

// Called when the game starts or when spawned
void AElementWiggle::BeginPlay()
{
	Super::BeginPlay();

	MovmSpan = FVector(1290, 0, 0);
	CalcPosDir();

	StartingLocation = GetActorLocation();

	SetActorLocation(StartingLocation + FVector(offsetX, 0, 0));
	EndLocation = StartingLocation + MovmSpan;
}

// Called every frame
void AElementWiggle::Tick( float DeltaTime )
{
	Super::Tick(DeltaTime);

	if (GetActorLocation().X > EndLocation.X)
	{
		direction = -1;
	}
	if (GetActorLocation().X < StartingLocation.X)
	{
		direction = 1;
	}

	AddActorLocalOffset(FVector(direction * 5, 0, 0));
}
void AElementWiggle::CalcPosDir()
{
	int SurpassedSpans;
	SurpassedSpans = (int)offsetX / (int)MovmSpan.X;

	if (SurpassedSpans % 2 == 0)
	{
		direction = 1;
		offsetX -= (SurpassedSpans * MovmSpan.X);
	}
	else
	{
		direction = -1;
		offsetX -= ((SurpassedSpans - 1) * MovmSpan.X - (int)offsetX / (int)MovmSpan.X);
	}
}

