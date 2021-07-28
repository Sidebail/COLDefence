// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GASAbility.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class COLDEFENCE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();
	/**
	* @brief Keeps information about enemy unit.
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Stats")
		FEnemyStats EnemyData;
	/**
	* @brief If CurrentHealth goes below zero, enemy unit is destroy.
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Stats")
		float CurrentHealth = 100.f;
	/**
	* @brief Name of enemy that is looked up in DT_EnemyInfo.
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Data")
		FName EnemyName = FName("Melee");
	/**
	* @brief If this goes to 0, enemy unit can shoot with Fire() function.
	* 
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Shooting")
		float TimeToNextShot = 0.0f;
	/**
	* @brief Material to flick with when unit is hit.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
		UMaterialInterface* ImHitMaterial = nullptr;

	//  Auxillary
	TArray<UMaterialInterface*> DefaultMaterials;
	TArray<UStaticMeshComponent*> StaticComps;
	/**
	* @brief This is an ability from GameplayAbilitySystem.
	* Has to be set in Blueprint to an according ability.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay Ability System")
		TSubclassOf<class UGASAbility> ShootingAbility;
	/**
	* @brief GameplayAbilitySystem component to work with shooting.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
		class UGASComponent* AbilitySystemComponent;
	/**
	* @brief Arrow with location from where Enemy can shoot.
	* Can be modified in Blueprint.
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
		class UArrowComponent* ShootLocation;
	/**
	* @brief SceneComponent that holds all static meshes that rotate and shoot.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
		class USceneComponent* RotatingPart;
	
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/**
	* @brief Used for timing the shots
	* 
	*/
	void FireTick(float DeltaTime);
	/**
	* @brief Initializes the variables, checks data tables.
	* 
	*/
	void Initialize();

	/**
	* @brief Gets the get DamageAmount and subtracts it from CurrentHealthAmount.
	* 
	*/
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	/**
	* @brief Auxillary for Material flickering when hit.
	* 
	*/
	void ReturnDefaultMaterials();
	/**
	* @brief Overrided collision hit.
	* 
	*/
	UFUNCTION()
    virtual void EnemyCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	/**
	* @brief Triggers gameplay ability. Shooting written in its blueprint.
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void Fire();
	/**
	* @brief Interpolates the rotation towards the TargetToLookAt
	* 
	*/
	UFUNCTION(BlueprintCallable)
    bool LookAtTarget(AActor* TargetToLookAt, float DeltaTime);
	/**
	* @brief Triggers the enemy death sequence. Leads to Destroy()
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void EnemyDeath();
};

