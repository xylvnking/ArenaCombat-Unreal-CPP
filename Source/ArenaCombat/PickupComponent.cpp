// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SphereComponent.h"
#include "ArenaCombatCharacter.h"
#include "Components/ActorComponent.h"
#include "PickupComponent.h"

// Sets default values for this component's properties
UPickupComponent::UPickupComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;


	// Setup the Sphere Collision
	//SphereRadius = 32.f;

	// ...
}


// Called when the game starts
void UPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnSphereBeginOverlap);
}

void UPickupComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AArenaCombatCharacter* Character = Cast<AArenaCombatCharacter>(OtherActor);
	if (Character != nullptr)
	{

		// Notify that the actor is being picked up
		//OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		//OnComponentBeginOverlap.RemoveAll(this);
	}
}


// Called every frame
//void UPickupComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

