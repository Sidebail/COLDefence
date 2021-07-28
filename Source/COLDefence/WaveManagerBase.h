// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GameFramework/Actor.h"
#include "WaveManagerBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNewWavePrepared);

UCLASS()
class COLDEFENCE_API AWaveManagerBase : public AActor
{
	GENERATED_BODY()
public:
	/**
	* @brief Array of waves to spawn
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="WaveManagement")
	TArray<FWaveInfo> Waves;
	/**
	* @brief Index of wave that is going to be spawned
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="WaveManagement")
	int32 CurrentWaveIndex = 0;
	/**
	* @brief TargetPoints with spawn locations
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WaveManagement")
	TArray<ATargetPoint*> SpawnLocations;
	/**
	* @brief Event delegate that is broadcasted when new wave is ready
	*/
	UPROPERTY(BlueprintAssignable, Category="Dispatchers")
	FOnNewWavePrepared NewWavePrepared;
	/**
	* @brief Timer to spawn new wave
	*/
	UPROPERTY(BlueprintReadWrite, Category="WaveManagement")
	FTimerHandle NewWaveTimerHandle;
	/**
	* @brief Actor that is generating wave notification
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WaveManagement")
	TSubclassOf<AActor> NotificationActorToSpawn;

	FWaveInfo CurrentWaveInfo;
	/**
    	* @brief Perform notification about wave auxillary function
    	*/
	void NotifyAboutWave();
public:	
	// Sets default values for this actor's properties
	AWaveManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* @brief Start wave spawning! West incoming!
	*/
	UFUNCTION(BlueprintCallable)
	void InitializeWaveSpawning();
	/**
	* @brief Auxillary, prepares new wave
	*/
	void SetNextWave();
	/**
	* @brief Spawns wave, recursive!!!
	*/
	void SpawnWave();
};
