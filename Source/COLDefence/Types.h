// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Engine/TargetPoint.h"

#include "Types.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	HUMAN UMETA(DisplayName = "HUMAN"),
	VEHICLE UMETA(DisplayName = "VEHICLE")
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	WALK UMETA(DisplayName = "WALK"),
	RUN UMETA(DisplayName = "RUN"),
	FALL UMETA(DisplayName = "FALL")
};

UENUM(BlueprintType)
enum class EDefenceType : uint8
{
	SOLDIER UMETA(DisplayName="SOLDIER"),
	STRUCTURE UMETA(DisplayName="STRUCTURE")
};

USTRUCT(BlueprintType)
struct FPlayerStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		int32 MaxHealth = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float WalkSpeed = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float JumpScale = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float MaxBuildRange = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float PlayerCapsuleRadius = 42.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float CapsuleHalfHeight = 96.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death")
		float DeathTimeout = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death")
		float FlyLocationChangeSpeed = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Death")
		float FlyRotationChangeSpeed = 1.f;
};

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float Damage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float RateOfFire = 2.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		TSubclassOf<class AProjectileBase> ProjectileClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Reaction")
		TMap<EEnemyType, float> DamageModifiersPerEnemyType;
	
};

USTRUCT(BlueprintType)
struct FEnemyStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		int32 Health = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		EEnemyType Type = EEnemyType::HUMAN;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float MovementSpeed = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float DealingDamage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float RateOfFire = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float MaxRecoil = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		bool bCanTargetPlayer = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float ReactionLerpSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy")
		int32 RespectReward = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float SightRange = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		TSubclassOf<class AProjectileBase> ProjectileClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		TSubclassOf<class AEnemyBase> EnemyClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Reaction")
		TMap<EDefenceType, float> DamageModifiersPerDefenceType;
};

USTRUCT(BlueprintType)
struct FProjectileStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
		float ProjectileSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
		float GravityScale = 0.f;
};

USTRUCT(BlueprintType)
struct FDefenceStats : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		int32 Health = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		EDefenceType Type = EDefenceType::SOLDIER;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		float DealingDamage = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		FName NameOfDefenceForUI = FName("Red Army Guys");
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float RateOfFire = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float MaxRecoil = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float ReactionLerpSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Shooting")
		float SightRange = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy")
		int32 RespectCost = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defence Reaction")
		TMap<EEnemyType, float> DamageModifiersPerEnemyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		TSubclassOf<class AProjectileBase> ProjectileClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Base Stats")
		TSubclassOf<class ADefenceBase> DefenceClass = nullptr;
	
};

USTRUCT(BlueprintType)
struct FGameRules : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Nuke")
		float NukeLaunchTimeInSeconds = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Nuke")
		int32 NukeGateHealth = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Economy")
		int32 StartingRespect = 100;
	// TODO: Wave structure
};

USTRUCT()
struct FWaveInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
		TMap<FName, int32> EnemiesInWave;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
		float WaveTimeoutInSeconds = 10.f;
};
UCLASS()
class COLDEFENCE_API UTypes : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
