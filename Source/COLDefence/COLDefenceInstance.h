// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NukeBase.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Types.h"
#include "COLDefenceInstance.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespectIncrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespectDecrease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRespectInsufficient);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDefenceUIUpdate);

UCLASS()
class COLDEFENCE_API UCOLDefenceInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	// We are holding game data in the tables
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WaveSetting")
	UDataTable* WavesInfoTable = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="PlayerSetting")
	UDataTable* PlayerInfoTable = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponSetting")
	UDataTable* WeaponInfoTable = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponSetting")
	UDataTable* ProjectileInfoTable = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EnemySetting")
	UDataTable* EnemyInfoTable = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="DefencesSetting")
	UDataTable* DefencesInfoTable = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="DefencesSetting")
	FGameRules Rules;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Economy")
	int32 CurrentRespect;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Economy")
	int32 MaxRespect = 500;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Nuke")
	int32 CurrentNukeGateHealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Nuke")
	ANukeBase* Nuke = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Nuke")
	float CurrentNukeTime;

	UPROPERTY(BlueprintAssignable, Category = "Respect Notification")
	FOnRespectIncrease RespIncrease;
	UPROPERTY(BlueprintAssignable, Category = "Respect Notification")
	FOnRespectDecrease RespDecrease;
	UPROPERTY(BlueprintAssignable, Category = "Respect Notification")
	FOnRespectInsufficient RespInsufficient;
	UPROPERTY(BlueprintAssignable, Category = "Defence Notification")
	FOnDefenceUIUpdate DefenceUIUpdate;
	
	// Getters for the game data
	UFUNCTION(BlueprintCallable)
	bool GetPlayerStats(FName PlayerName, FPlayerStats& OutInfo);
	UFUNCTION(BlueprintCallable)
    bool GetWeaponStats(FName WeaponName, FWeaponStats& OutInfo);
	UFUNCTION(BlueprintCallable)
    bool GetProjectileStats(FName ProjectileName, FProjectileStats& OutInfo);
	UFUNCTION(BlueprintCallable)
    bool GetDefenceStats(FName DefenceName, FDefenceStats& OutInfo);
	UFUNCTION(BlueprintCallable)
    bool GetEnemyStats(FName EnemyName, FEnemyStats& OutInfo);
	UFUNCTION(BlueprintCallable)
	bool GetWaveInfo(int32 WaveIndex, FWaveInfo& OutInfo);

	UFUNCTION(BlueprintCallable)
	void InitializeGame();
	
	void NukeTick(float DeltaTime);

	UFUNCTION(BlueprintCallable)
	void LaunchNuke();
	UFUNCTION(BlueprintCallable)
    void BlowUpNuke();
	UFUNCTION(BlueprintCallable)
	void RegisterNukeDamage();
	UFUNCTION(BlueprintCallable)
	void IncreaseRespect(float ChangeVal);
	UFUNCTION(BlueprintCallable)
	bool DecreaseRespect(float ChangeVal);
	UFUNCTION(BlueprintCallable)
	void UpdateDefenceUI(FDefenceStats DefenceData);	
	
	
};
