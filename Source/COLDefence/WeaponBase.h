// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASAbility.h"
#include "GameFramework/Actor.h"
#include "GASComponent.h"
#include "Types.h"

#include "WeaponBase.generated.h"

UCLASS()
class COLDEFENCE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	/**
	* @brief Ability that projectiles are created with
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay Ability System")
	TSubclassOf<class UGASAbility> ShootingAbility;
	/**
	* @brief Struct that holds information about weapon
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapon Data")
	FWeaponStats WeaponData;
	/**
	* @brief Name of weapon to search in DT_WeaponDataTable
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapon Data")
	FName WeaponName = FName("StandartIssueRifle");
	/**
	* @brief Container that holds ability tag to Shoot
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay Ability System")
	FGameplayTagContainer TagContainer;

private:
	/**
	* @brief Name of variable (。_。)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	class UGASComponent* AbilitySystemComponent;
	/**
	* @brief Projectile will be spawned with location of that ArrowComponent
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class UArrowComponent* ShootLocation;
	/**
	* @brief Static mesh of gun.. not used
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class UStaticMeshComponent* StaticMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/**
	* @brief Tick function to count down the shoot delay
	*/
	void FireTick(float DeltaTime);
	/**
	* @brief Figures (。_。)
	*/
	float TimeToNextShot = 0.0f;

	/**
	* @brief When its true, weapon is shooting
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsFiring = false;
	/**
	* @brief Gets data from DataTable and sets variables.
	*/
	UFUNCTION()
		void Initialize();
	/**
	* @brief Calls GameplayAbilitySystem ability
	*/
	UFUNCTION(BlueprintCallable)
		void Fire();
};
