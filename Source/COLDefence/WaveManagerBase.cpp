// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveManagerBase.h"

#include "COLDefenceInstance.h"
#include "EnemyBase.h"

// Sets default values
AWaveManagerBase::AWaveManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaveManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaveManagerBase::InitializeWaveSpawning()
{
	FWaveInfo WaveInfo;
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	int32 Index = 0;
	if(myGI)
	{
		while(myGI->GetWaveInfo(Index, WaveInfo))
		{
			Waves.Add(WaveInfo);
			Index++;
		}
	}

	CurrentWaveIndex = 0;
	if(Waves.Num() > 0)
		SetNextWave();
	
}

void AWaveManagerBase::SetNextWave()
{
	if(Waves.Num() > CurrentWaveIndex)
	{
		CurrentWaveInfo = Waves[CurrentWaveIndex];
		GetWorldTimerManager().SetTimer(NewWaveTimerHandle, this, &AWaveManagerBase::SpawnWave, CurrentWaveInfo.WaveTimeoutInSeconds, false);
		FTimerHandle NotificationTimerHandle;
		GetWorldTimerManager().SetTimer(NotificationTimerHandle, this, &AWaveManagerBase::NotifyAboutWave, CurrentWaveInfo.WaveTimeoutInSeconds, false);
		NewWavePrepared.Broadcast();
		CurrentWaveIndex++;
	}else
	{
		CurrentWaveIndex--;
		SetNextWave();
	}
}

void AWaveManagerBase::SpawnWave() // THIS IS RECURSIVE
{
	if(SpawnLocations.Num() > 0)
	{
		//int32 SpawnLocationIndex = FMath::RandRange(0, SpawnLocations.Num() - 1);
		TArray<FName> KeyArr;
		CurrentWaveInfo.EnemiesInWave.GetKeys(KeyArr);
		if(KeyArr.Num() > 0)
		{
			int32 EnemyTypeIndex = FMath::RandRange(0, KeyArr.Num() - 1);
			int32 EnemyCount = CurrentWaveInfo.EnemiesInWave.FindRef(KeyArr[EnemyTypeIndex]);
			if(EnemyCount > 0)
			{
				EnemyCount--;
				CurrentWaveInfo.EnemiesInWave.Add(KeyArr[EnemyTypeIndex], EnemyCount);
				UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
				if(myGI)
				{
					FEnemyStats EnemyData;
					if(myGI->GetEnemyStats(KeyArr[EnemyTypeIndex], EnemyData))
					{
						for (ATargetPoint* SpawnLocation : SpawnLocations)
						{
							FActorSpawnParameters SpawnParams;
							SpawnParams.Instigator = GetInstigator();
							SpawnParams.Owner = GetOwner();
							SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
							FVector SpawnLoc = SpawnLocation->GetActorLocation();
							FRotator SpawnRot = SpawnLocation->GetActorRotation();
							GetWorld()->SpawnActor(EnemyData.EnemyClass, &SpawnLoc, &SpawnRot, SpawnParams);
						}
						
						

						FTimerHandle RecursionSpawnTimerHandle;
						GetWorldTimerManager().SetTimer(RecursionSpawnTimerHandle, this, &AWaveManagerBase::SpawnWave, 1.5f, false);
						return;
					}
				}
			}else
			{
				CurrentWaveInfo.EnemiesInWave.Remove(KeyArr[EnemyTypeIndex]);
				SpawnWave();
				return;
			}
		}
	}
	SetNextWave();
	return;
}

void AWaveManagerBase::NotifyAboutWave()
{
	for (ATargetPoint* SpawnLocation : SpawnLocations)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = GetInstigator();
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		FVector SpawnLoc = SpawnLocation->GetActorLocation();
		FRotator SpawnRot = SpawnLocation->GetActorRotation();
		GetWorld()->SpawnActor(NotificationActorToSpawn, &SpawnLoc, &SpawnRot, SpawnParams);
	}
}

