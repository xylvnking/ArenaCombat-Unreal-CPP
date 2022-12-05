// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupComponentSphere.h"

UPickupComponentSphere::UPickupComponentSphere()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	//PrimaryComponentTick.bCanEverTick = false;


	// Setup the Sphere Collision
	SphereRadius = 32.f;

	// ...
}

void UPickupComponentSphere::BeginPlay()
{
	Super::BeginPlay();
	OnComponentBeginOverlap.AddDynamic(this, &UPickupComponentSphere::OnSphereBeginOverlap);
}

void UPickupComponentSphere::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AArenaCombatCharacter* Character = Cast<AArenaCombatCharacter>(OtherActor);
	if (Character != nullptr)
	{

		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
