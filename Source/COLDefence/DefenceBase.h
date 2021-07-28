// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GASAbility.h"
#include "GASComponent.h"
#include "GameFramework/Character.h"

#include "DefenceBase.generated.h"

UCLASS()
class COLDEFENCE_API ADefenceBase : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenceBase();

	/**
	* @brief Keeps information about defence unit.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Defence Stats")
	FDefenceStats DefenceData;
	/**
	* @brief If CurrentHealth goes below zero, defence unit is destroy.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Defence Stats")
	float CurrentHealth = 100.f;
	/**
	* @brief Name of defence that is looked up in DT_DefenceInfo.
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defence Data")
	FName DefenceName = FName("Grunt");
	/**
	* @brief If this goes to 0, defence unit can shoot with Fire() function.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooting")
	float TimeToNextShot = 0.0f;
	/**
	* @brief Material to flick with when unit is hit.
	* 
	*/
	// TODO: Make flicking when hit 
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
	* @brief Arrow with location from where Defence can shoot.
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
	* @brief Triggers the defence death sequence. Leads to Destroy()
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void DefenceDeath();
	

};
