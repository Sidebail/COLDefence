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
	/**
	* @brief Player camera.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FirstPersonCameraComponent;
	/**
	* @brief Decal to show building position to player.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UDecalComponent* BuildDecal;
	/**
	* @brief Point where to spawn equipped weapon.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components,  meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* WeaponPoint;
	/**
	* @brief Point where to locate phone mesh.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components,  meta = (AllowPrivateAccess = "true"))
		class UArrowComponent* PhonePoint;
	/**
	* @brief AbilitySystemComponent that is... not used, lol. I was thinking how to use GAS system and testing some stuff.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UGASComponent* AbilitySystemComponent;
	/**
	* @brief Pointer to gun mesh, weeeell, not used. I decided to do gun through another actor. Modularity, baby!
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* GunMesh;
	/**
	* @brief Pointer to phone mesh asset.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* PhoneMesh;
	/**
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
		class UGASAttribute* GameplayAttributes;
		*/
public:
	/**
	* @brief Reference to a class of currently chosen defence unit to call.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defences")
		TSubclassOf<ADefenceBase> CurrentDefenceClass = nullptr;
	/**
	* @brief Cost of currently chosen defence to call.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Defences")
		int32 CurrentDefenceCost = 0;
	/**
	* @brief Array of defences that player can choose from.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defences")
		TArray<FName> DefencesToCall = {"Grunt"};
	/**
	* @brief Currently chosen defence index.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Defences")
		int32 CurrentDefenceIndex = 0;
	/**
	* @brief Currently chosen defence data.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Defences")
		FDefenceStats CurrentDefenceData;
	/**
	* @brief Material for decal if defence is valid.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defences")
		UMaterialInterface* ValidDefenceMat = nullptr;
	/**
	* @brief Material for decal if defence is invalid and there is no place to build.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defences")
		UMaterialInterface* InvalidDefenceMat = nullptr;
	/**
	* @brief Location to respawn enemy at
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		FVector PlayerRespawnLocation;
	/**
	* @brief DEPRECATED, not used
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		FVector DeathLocation;
	/**
    * @brief DEPRECATED, not used
    */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		FRotator DeathRotation;
	/**
	* @brief DEPRECATED, not used
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		float SpeedOfDeathFlyLocationChange;
	/**
	* @brief DEPRECATED, not used
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		float SpeedOfDeathFlyRotationChange;
	/**
	* @brief Time to respawn
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Animation")
		float DeathTimeout;
	
	// Gameplay Classes and References
	/**
	* @brief Welll, made for future animations but not even used anywhere...
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
		EMovementState MovementState = EMovementState::WALK;
	/**
	* @brief Speed that character walks with
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
		float CurrentCharacterSpeed = 100.f;
	/**
	* @brief Reference to FPlayerStats struct with player data.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stats")
		FPlayerStats PlayerStats;
	/**
	* @brief If it goes to 0, player is dead.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Stats")
		float CurrentHealth = 100.f;
	/**
	* @brief Name of weapon to equip on game's start.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		FName FirstWeaponName;
	/**
	* @brief Pointer to current weapon actor that is equipped. 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		AWeaponBase* CurrentWeapon = nullptr;
	/**
	* @brief Class of first weapon to equip.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		TSubclassOf<AWeaponBase> InitialWeaponClass = nullptr;
	/**
	* @brief Checker if phone is equipped at the moment.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Equipment")
		bool bIsPhoneEquipped = false;
	/**
	* @brief Reference to pawn with cameras to make death fly over scene.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Death")
		APawn* DeathPawn;
	
	
	/**
	* @brief Used for movement
	*/
	float AxisX = 0.0f;
	/**
    * @brief Used for movement
    */
	float AxisY = 0.0f;
	/**
    * @brief Used for movement
    */
	float LookAxisX = 0.0f;
	/**
    * @brief Used for movement
    */
	float LookAxisY = 0.0f;

	/**
	* @brief Determining if input is from gamepad. Well, not used, tbh.
	*/
	float bIsGamepadUsed = true;
	
	// Effect that initializes our default attributes
	/**
	* @brief Not used. :(
	*/
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Gameplay Ability System")
		TSubclassOf<class UGameplayEffect> DefaultAttributeEffect;
	/**
    * @brief Class of DropPod to call out.
    */
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="DropPod Class")
		TSubclassOf<ADropPodBase> PodClass;

	// Auxillary checkers
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
	
	/**
	* @brief Tick function for movement
	*/
	void MovementTick(float DeltaTime);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**
	* @brief Getter for ability system component
	*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/**
    * @brief Initializer for GameplayAttributes
    */
	virtual void InitializeAttributes();

	/**
	* @brief Auzillary function to set the desired defence as Current and update everything related to it
	*/
	void EquipDefence(FName DefenceName);
	/**
	* @brief Spawns DropPod and sets the spawn information to it.
	*/
	UFUNCTION()
	bool SpawnDefence();
	/**
	* @brief Decreases CurrentHealth.
	*/
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/**
	* @brief Auxillary for decal movement
	*/
	void LetDecalMove();

	// Input
	/**
	* @brief Input - movement
	*/
	UFUNCTION()
        void InputAxisY(float Value);
	/**
	* @brief Input - movement
	*/
	UFUNCTION()
        void InputAxisX(float Value);
	/**
	* @brief Input - rotation
	*/
	UFUNCTION()
        void InputLookAxisX(float Value);
	/**
	* @brief Input - rotation
	*/
	UFUNCTION()
        void InputLookAxisY(float Value);
	/**
	* @brief Equip and un-equip phone, also turn visibility of weapon
	*/
	UFUNCTION()
        void TogglePhone();
	/**
	* @brief Initialize weapon data from DT_WeaponsDataTable
	*/
	UFUNCTION(BlueprintCallable)
        void InitWeapon(FName WeaponDataRowName);
	/**
	* @brief Let weapon shoot. Or spawn defence
	*/
	UFUNCTION()
        void StartShooting();
	/**
	* @brief Prevent weapon from shooting
	*/
	UFUNCTION()
        void EndShooting();
	/**
	* @brief Cycle through defences, next.
	*/
	UFUNCTION()
        void NextDefence();
	/**
    * @brief Cycle through defences, next.
    */
	UFUNCTION()
        void PrevDefence();
	/**
	* @brief Signing to input.
	*/
	UFUNCTION()
        void InitializePlayerData();
	/**
	* @brief Player death logic sequence. Unpossesses the player and possesses him to DeathPawn.
	*/
	UFUNCTION()
		void PlayerDeath();
	/**
	* @brief Player respawn logic sequence, change Character location,
	* unpossesses the player from DeathPawn and possesses him to this character.
	*/
	UFUNCTION()
		void PlayerRespawn();
};
