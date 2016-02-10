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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = OffsetX)
	float offsetX = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Direction)
	bool ArneOnOtherSide = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Speed)
	float speed = 12;

	bool WhereHeWas = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ImpulseDirection, meta = (AllowPrivateAccess = "true"))
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
