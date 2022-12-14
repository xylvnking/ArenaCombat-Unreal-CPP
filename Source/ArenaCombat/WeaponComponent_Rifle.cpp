// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent_Rifle.h"

#include "ArenaCombatCharacter.h"

#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/KismetMathLibrary.h"


#include "Engine/EngineTypes.h"

UWeaponComponent_Rifle::UWeaponComponent_Rifle()
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = false;
	CurrentAmmo = 9999;

	// this should be from the camera instead?
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	FireSoundVolume = 1.0f;

}

void UWeaponComponent_Rifle::BeginPlay()
{
	FOnTimelineFloat XRecoilCurve;
	FOnTimelineFloat YRecoilCurve;

	XRecoilCurve.BindUFunction(this, FName("StartHorizontalRecoil"));
	YRecoilCurve.BindUFunction(this, FName("StartVerticalRecoil"));

	if (!HorizontalCurve || !VerticalCurve) { return; }

	RecoilTimeline.AddInterpFloat(HorizontalCurve, XRecoilCurve);
	RecoilTimeline.AddInterpFloat(VerticalCurve, YRecoilCurve);


}





void UWeaponComponent_Rifle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
			Subsystem->RemoveMappingContext(DefaultMappingContext);
		}
	}
}

void UWeaponComponent_Rifle::AttachWeapon(AArenaCombatCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	//AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	//AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("weapon_r_muzzle")));
	

	// should check to see if they have this first?
	Character->SetHasRifle(true);

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UWeaponComponent_Rifle::OnStartFire);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UWeaponComponent_Rifle::OnStopFire);

			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UWeaponComponent_Rifle::GetLookData);
		}
	}
}

void UWeaponComponent_Rifle::FireProjectile()
{
}

void UWeaponComponent_Rifle::FireHitScan()
{
	if (Character == nullptr || Character->GetController() == nullptr) { return; }
	UWorld* const World = GetWorld();
	if (CurrentAmmo > 0 && World != nullptr)
	{
		RotationBeforeFiring = Character->GetControlRotation();
		CurrentAmmo--;
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		FHitResult Hit;
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
		FCollisionQueryParams Params;
		Params.AddIgnoredComponent(this);
		FVector TraceStart = CameraLocation;
		FVector TraceEnd = TraceStart + CameraRotation.Vector() * 10000;
		bool bHasHitSomething = World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, Params);
		if (bHasHitSomething)
		{
			AActor* HitActor = Hit.GetActor();
			//const FDamageEvent;
			if (!HitParticleFx) { return; }
			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(World, HitParticleFx, Hit.Location, Hit.ImpactNormal.Rotation(), FVector(0.2f), true, true);
			NiagaraComp->SetNiagaraVariableFloat(FString("StrengthCoef"), CoefStrength);

			if (!HitDecalMaterial) { return; }
			UGameplayStatics::SpawnDecalAtLocation(World, HitDecalMaterial, FVector(15.0f), Hit.Location, Hit.ImpactNormal.Rotation(), 10.0f);

			//if (IsValid(Hit.GetActor()))
			if (IsValid(HitActor))
			{
				//AActor* HitActor = Hit.GetActor();
				if (HitActor->IsA(ACharacter::StaticClass()))
				{
					//UE_LOG(LogTemp, Log, TEXT("Trace hit AN ACTUAL ACTOR"));
					// The trace hit a character
					// (do something with the character, such as applying damage)
				}
				//else if (HitActor->IsA(AStaticMeshActor::StaticClass()))
				//{
				//	// The trace hit a static mesh actor
				//	// (do something with the static mesh actor, such as applying a force)
				//}
				//UE_LOG(LogTemp, Log, TEXT("Trace hit actor: %s"), *Hit.GetActor()->GetName());
				UGameplayStatics::ApplyDamage(Hit.GetActor(), 50.0f, Character->GetController(), Character, NULL);
				//UE_LOG(LogTemp, Warning, TEXT("MyVector = %s"), *Hit.Location.ToString());
				HitScanImpactLocation = Hit.Location;



				//AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(ActorClass, Hit.Location, FRotator::ZeroRotator);
				
				
				// i used this when i was trying to get chaos physics working
				/*TArray<AActor*> ActorsToIgnore;
				UGameplayStatics::ApplyRadialDamage(
					World, 
					200000.0f, 
					Hit.Location, 
					100.0f, 
					UDamageType::StaticClass(),
					ActorsToIgnore,
					Character,
					Character->GetController(),
					true
				);*/



				
				
				
			}

		}
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation(), FireSoundVolume);
		}
		//if (FireAnimation != nullptr)
		//{
		//	// Get the animation object for the arms mesh
		//	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
		//	if (AnimInstance != nullptr)
		//	{
		//		AnimInstance->Montage_Play(FireAnimation, 1.f);
		//	}
		//}
	}


}


void UWeaponComponent_Rifle::GetLookData(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	MostRecentLookValuesFromWeaponInputComponent = FVector2D(LookAxisVector.X, LookAxisVector.Y);
}



void UWeaponComponent_Rifle::OnStartFire()
{
	if (CurrentAmmo <= 0) {
		RecoilTimeline.Stop();
		return;
	}
	RecoilTimeline.PlayFromStart();
	FireHitScan();
	GetWorld()->GetTimerManager().SetTimer(AutomaticFireHandle, this, &UWeaponComponent_Rifle::FireHitScan, 0.1, true);
}

void UWeaponComponent_Rifle::OnStopFire()
{
	if (CurrentAmmo <= 0) {
		RecoilTimeline.Stop();
		return;
	}
	RecoilTimeline.ReverseFromEnd();
	GetWorld()->GetTimerManager().ClearTimer(AutomaticFireHandle);
}

void UWeaponComponent_Rifle::StartHorizontalRecoil(float Value)
{
	if (RecoilTimeline.IsReversing()) { return; }
	Character->AddControllerYawInput(Value);
}

void UWeaponComponent_Rifle::StartVerticalRecoil(float Value)
{
	if (RecoilTimeline.IsReversing()) { return; }
	Character->AddControllerPitchInput(Value);
}

void UWeaponComponent_Rifle::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, LEVELTICK_All, ThisTickFunction);
	//Super::TickComponent(DeltaTime, LEVELTICK_PauseTick, ThisTickFunction);
	if (RecoilTimeline.IsPlaying())
	{
		RecoilTimeline.TickTimeline(DeltaTime);
	}

	if (RecoilTimeline.IsReversing())
	{
		if (FMath::Abs(MostRecentLookValuesFromWeaponInputComponent.X) > 0.08 || FMath::Abs(MostRecentLookValuesFromWeaponInputComponent.Y) > 0.08)
		{
			RecoilTimeline.Stop();
			return;
		}

		RecoilTimeline.TickTimeline(DeltaTime);

		// this check wasn't required before piecing the project back together. It makes sense, but I don't get why I didn't need it before.
		if (Character == nullptr || Character->GetController() == nullptr) { return; }
		FRotator NewRotation = UKismetMathLibrary::RInterpTo(Character->GetControlRotation(), RotationBeforeFiring, DeltaTime, 2.0f);
		Character->Controller->ClientSetRotation(NewRotation);
	}
}
