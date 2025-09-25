// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalPlatform.h"

// Sets default values
ANormalPlatform::ANormalPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANormalPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANormalPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

