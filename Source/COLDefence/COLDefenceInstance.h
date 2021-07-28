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
	/**
	 * @brief  Reference to DataTable with collection of Wave data structs
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WaveSetting")
	UDataTable* WavesInfoTable = nullptr;
	
	/**
	* @brief  Reference to DataTable with collection of Player data structs
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="PlayerSetting")
	UDataTable* PlayerInfoTable = nullptr;

	/**
	* @brief  Reference to DataTable with collection of Weapon data structs
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponSetting")
	UDataTable* WeaponInfoTable = nullptr;

	/**
	* @brief  Reference to DataTable with collection of Projectile data structs
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WeaponSetting")
	UDataTable* ProjectileInfoTable = nullptr;

	/**
	* @brief  Reference to DataTable with collection of Enemy data structs
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="EnemySetting")
	UDataTable* EnemyInfoTable = nullptr;

	/**
	* @brief  Reference to DataTable with collection of Defence data structs
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="DefencesSetting")
	UDataTable* DefencesInfoTable = nullptr;

	/**
	* @brief  Rules of game that define win and loose conditions, Nuke timer, Nuke health etc.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="DefencesSetting")
	FGameRules Rules;

	/**
	* @brief  Current amount of main economy value in the game
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Economy")
	int32 CurrentRespect;
	/**
	* @brief  Maximum possible economy value amount that player can hold
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Economy")
	int32 MaxRespect = 500;
	/**
	* @brief  This variable tracks the Nuke health. If it goes to zero, game is lost.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Nuke")
	int32 CurrentNukeGateHealth;
	/**
	* @brief  Reference to the Nuke actor, which is the main target of the enemies and the thing Player has to defend
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Nuke")
	ANukeBase* Nuke = nullptr;
	/**
	* @brief  This variable is Nuke countdown.
	* 		  It's set to the time of countdown in InitializeGame().
	* 		  When it reaches zero - Nuke is launched, Player achieves victory
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Nuke")
	float CurrentNukeTime;

	/**
	* @brief  Delegate for binding when Respect amount is increased.
	* Used by Widget Blueprints for UI updates.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Respect Notification")
	FOnRespectIncrease RespIncrease;
	/**
    * @brief  Delegate for binding when Respect amount is decreased.
    * Used by Widget Blueprints for UI updates.
    */
	UPROPERTY(BlueprintAssignable, Category = "Respect Notification")
	FOnRespectDecrease RespDecrease;
	/**
    * @brief  Delegate for binding when it's not enough Respect to build Defence.
    * Used by Widget Blueprints for UI updates.
    */
	UPROPERTY(BlueprintAssignable, Category = "Respect Notification")
	FOnRespectInsufficient RespInsufficient;
	/**
	* @brief  Delegate for binding any other . Used by Widget Blueprints for UI updates.
	*/
	UPROPERTY(BlueprintAssignable, Category = "Defence Notification")
	FOnDefenceUIUpdate DefenceUIUpdate;
	
	// Getters for the game data
	/**
	* @brief  Writes player stats to OutInfo variable, passed by reference.
	* Looked-up in DT_PlayersDataTable by PlayerName.
	* Returns bool variable if entry with PlayerName row name.
	*/
	UFUNCTION(BlueprintCallable)
	bool GetPlayerStats(FName PlayerName, FPlayerStats& OutInfo);
	/**
	* @brief  Writes weapon stats to OutInfo variable, passed by reference.
	* Looked-up in DT_WeaopnsDataTable by WeaponName.
	* Returns bool variable if entry with WeaponName row name.
	*/
	UFUNCTION(BlueprintCallable)
    bool GetWeaponStats(FName WeaponName, FWeaponStats& OutInfo);
	/**
	* @brief  Writes projectile stats to OutInfo variable, passed by reference.
	* Looked-up in DT_ProjectilesDataTable by ProjectileName.
	* Returns bool variable if entry with ProjectileName row name.
	*/
	UFUNCTION(BlueprintCallable)
    bool GetProjectileStats(FName ProjectileName, FProjectileStats& OutInfo);
	/**
	* @brief  Writes defence stats to OutInfo variable, passed by reference.
	* Looked-up in DT_DefencesDataTable by DefenceName.
	* Returns bool variable if entry with DefenceName row name.
	*/
	UFUNCTION(BlueprintCallable)
    bool GetDefenceStats(FName DefenceName, FDefenceStats& OutInfo);
	/**
	* @brief  Writes enemy stats to OutInfo variable, passed by reference.
	* Looked-up in DT_EnemyDataTable by EnemyName.
	* Returns bool variable if entry with EnemyName row name.
	*/
	UFUNCTION(BlueprintCallable)
    bool GetEnemyStats(FName EnemyName, FEnemyStats& OutInfo);
	/**
	* @brief  Writes waves information to OutInfo variable, passed by reference.
	* Looked-up in DT_Waves by WaveIndex, index is compared with row indexes.
	* Returns bool variable if entry with WaveIndex index.
	* Turns all entries from DT_Waves into array and cycles through it.
	*/
	UFUNCTION(BlueprintCallable)
	bool GetWaveInfo(int32 WaveIndex, FWaveInfo& OutInfo);

	/**
	 * @brief Sets the inner variables, gets information from Blueprint assigned variable values.
	 *  Works with current Respect, sets the health and countdown timer from variables.
	 *
	 */
	UFUNCTION(BlueprintCallable)
	void InitializeGame();

	/**
	 * @brief Counts down the DeltaTime from CurrentNukeTime variable.
	 *
	 */
	void NukeTick(float DeltaTime);

	/**
	* @brief Nuke launch sequence triggered! 
	* Victory!!
	*/
	UFUNCTION(BlueprintCallable)
	void LaunchNuke();
	/**
	* @brief Nuke destruction sequence triggered! 
	* Defeat! Try again!
	*/
	UFUNCTION(BlueprintCallable)
    void BlowUpNuke();
	/**
	* @brief Nuke takes single damage! 
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void RegisterNukeDamage();
	/**
	* @brief Give more Respect to player.
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void IncreaseRespect(float ChangeVal);
	/**
	* @brief Steal Respect from plyaer. 
	* 
	*/
	UFUNCTION(BlueprintCallable)
	bool DecreaseRespect(float ChangeVal);
	/**
	* @brief Delegate for event in UI (WBP_HUD) to trigger and change stuff.
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void UpdateDefenceUI(FDefenceStats DefenceData);	
	
	
};
