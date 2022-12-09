// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

#include "Components/TimelineComponent.h"

#include "EnhancedInputComponent.h"


#include "WeaponComponent_Rifle.generated.h"

class AArenaCombatCharacter;
class UNiagaraSystem;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ARENACOMBAT_API UWeaponComponent_Rifle : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	///** Projectile class to spawn FROM ZOMIBEFPS */
//UPROPERTY(EditDefaultsOnly, Category = Projectile)
//TSubclassOf<class AZombieFPSProjectile> ProjectileClass;

/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float FireSoundVolume;


	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	// Should this just be set to the camera?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* FireAction;

	UWeaponComponent_Rifle();

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void AttachWeapon(AArenaCombatCharacter* TargetCharacter);

	/** Make the weapon Fire a Projectile (renamed from default Fire() */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void FireProjectile();


	//THIS IS HOW YOU GET A VALUE FROM C++ INTO BLUEPRINTS WITHOUT MAKING IT EDITABLE
	UPROPERTY(BlueprintReadOnly, Category = "Impact")
		FVector HitScanImpactLocation;

	UFUNCTION(BlueprintImplementableEvent)
		void GiveANumber();


protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
		virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	

private:
	/** The Character holding this weapon*/
	// can i make this so an ai could pick it up also?
	// is this hardcoding it to the player?
	AArenaCombatCharacter* Character;

public: // everything below this is what I added

	/*
	The DefaultMappingContext contains the Look input action, so we need to declare it here to bind it and access the values
	which are used to cancel the recoil timeline reversing.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
		class UInputAction* LookAction;

	/** Make the weapon Fire a HitScan */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void FireHitScan();

	/* Used to get the input values to cancel recoil timeline reversing */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void GetLookData(const FInputActionValue& Value);

	// does this need to be public?
	FTimeline RecoilTimeline;


protected:
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	// Used to cancel recoil
	FVector2D MostRecentLookValuesFromWeaponInputComponent;

	void OnStartFire();
	void OnStopFire();

	FTimerHandle AutomaticFireHandle;
	int32 CurrentAmmo;
	int32 DefaultAmmo = 30;

	FRotator RotationBeforeFiring;

	

	UFUNCTION()
		void StartHorizontalRecoil(float Value);

	UFUNCTION()
		void StartVerticalRecoil(float Value);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Advanbced Recoil | Curves")
		class UCurveFloat* VerticalCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Advanbced Recoil | Curves")
		class UCurveFloat* HorizontalCurve;

	// particle + vfx stuff

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AdvancedRecoil | FX")
		class UMaterialInterface* HitDecalMaterial;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AdvancedRecoil | FX")
		UNiagaraSystem* HitParticleFx;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AdvancedRecoil | FX")
		float CoefStrength;

};
