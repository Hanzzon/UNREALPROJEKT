// Fill out your copyright notice in the Description page of Project Settings.

#include "UNREALPROJEKT.h"
#include "Base.h"


// Sets default values
ABase::ABase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	main_mesh = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	main_mesh->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void ABase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABase::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

