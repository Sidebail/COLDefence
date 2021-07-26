// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types.h"
#include "WeaponBase.h"
#include "AbilitySystemInterface.h"
#include "DropPodBase.h"
#include "GeneralCharacter.generated.h"

UCLASS()
class COLDEFENCE_API AGeneralCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGeneralCharacter();

private:
	// COMPONENTS
	// Player Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* BuildDecal;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components,  meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* WeaponPoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components,  meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* PhonePoint;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UGASComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GunMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PhoneMesh;
	/**
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
		class UGASAttribute* GameplayAttributes;
		*/
public: 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defences")
		TSubclassOf<ADefenceBase> CurrentDefenceClass = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defences")
		int32 CurrentDefenceCost = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defences")
		TArray<FName> DefencesToCall = {"Grunt"};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Defences")
		int32 CurrentDefenceIndex = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Defences")
		FDefenceStats CurrentDefenceData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defences")
		UMaterialInterface* ValidDefenceMat = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defences")
		UMaterialInterface* InvalidDefenceMat = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		FVector PlayerRespawnLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		FVector DeathLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		FRotator DeathRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		float SpeedOfDeathFlyLocationChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		float SpeedOfDeathFlyRotationChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		float DeathTimeout;
	
	// Gameplay Classes and References
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
		EMovementState MovementState = EMovementState::WALK;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
		float CurrentCharacterSpeed = 100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stats")
		FPlayerStats PlayerStats;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stats")
		float CurrentHealth = 100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		FName FirstWeaponName;
	// TODO: CURRENT WEAPON
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		AWeaponBase* CurrentWeapon = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		TSubclassOf<AWeaponBase> InitialWeaponClass = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		bool bIsPhoneEquipped = false;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Death")
		APawn* DeathPawn;
	
	

	float AxisX = 0.0f;
	float AxisY = 0.0f;

	float LookAxisX = 0.0f;
	float LookAxisY = 0.0f;

	float bIsGamepadUsed = true;
	
	// Effect that initializes our default attributes
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gameplay Ability System")
		TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;
	
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DropPod Class")
		TSubclassOf<ADropPodBase> PodClass;

	bool bCanDecalMove = true;
	bool bCanCallDefenceHere = false;
	bool bPlayerIsDead = false;
	bool bPlayerIsDying = false;
	bool bPlayerIsRespawning = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Tick Func for movement
	void MovementTick(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Getter for AbilitySystemComponent
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// Initializer for GameplayAttributes
	virtual void InitializeAttributes();

	// Auzillary function to set the desired defence as Current and update everything related to it
	void EquipDefence(FName DefenceName);

	UFUNCTION()
	bool SpawnDefence();
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void LetDecalMove();

	// Input
	UFUNCTION()
        void InputAxisY(float Value);
	UFUNCTION()
        void InputAxisX(float Value);
	UFUNCTION()
        void InputLookAxisX(float Value);
	UFUNCTION()
        void InputLookAxisY(float Value);
	UFUNCTION()
        void TogglePhone();

	UFUNCTION(BlueprintCallable)
        void InitWeapon(FName WeaponDataRowName);
	UFUNCTION()
        void StartShooting();
	UFUNCTION()
        void EndShooting();
	UFUNCTION()
        void NextDefence();
	UFUNCTION()
        void PrevDefence();
	UFUNCTION()
        void InitializePlayerData();
	UFUNCTION()
		void PlayerDeath();
	UFUNCTION()
		void PlayerRespawn();
};
