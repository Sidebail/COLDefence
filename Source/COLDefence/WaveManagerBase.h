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
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="WaveManagement")
	TArray<FWaveInfo> Waves;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="WaveManagement")
	int32 CurrentWaveIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="WaveManagement")
	TArray<ATargetPoint*> SpawnLocations;
	UPROPERTY(BlueprintAssignable, Category="Dispatchers")
	FOnNewWavePrepared NewWavePrepared;
	UPROPERTY(BlueprintReadWrite, Category="WaveManagement")
	FTimerHandle NewWaveTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="WaveManagement")
	TSubclassOf<AActor> NotificationActorToSpawn;
	
	FWaveInfo CurrentWaveInfo;
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

	UFUNCTION(BlueprintCallable)
	void InitializeWaveSpawning();
	void SetNextWave();
	void SpawnWave();
};
