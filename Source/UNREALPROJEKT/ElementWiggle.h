// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ElementWiggle.generated.h"

UCLASS()
class UNREALPROJEKT_API AElementWiggle : public AActor
{
	GENERATED_BODY()
	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* main_mesh;
	
public:	
	// Sets default values for this actor's properties
	AElementWiggle();

	UPROPERTY(Category = Offset, EditAnywhere)
	float offsetX;

	float direction = 1;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

private:
	void CalcPosDir();

	FVector StartingLocation;
	FVector EndLocation;
	FVector MovmSpan;
};
