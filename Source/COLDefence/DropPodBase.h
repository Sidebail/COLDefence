// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefenceBase.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystemComponent.h"
#include "DropPodBase.generated.h"

UCLASS()
class COLDEFENCE_API ADropPodBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropPodBase();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components, meta=(AllowPrivateAccess=true))
	class UStaticMeshComponent* StaticMesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Components, meta=(AllowPrivateAccess=true))
	class UArrowComponent* FirstTroopSpawnLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Components, meta=(AllowPrivateAccess=true))
	class UArrowComponent* SecondTroopSpawnLocation;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Components, meta=(AllowPrivateAccess=true))
	class UParticleSystemComponent* SpawnEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	bool bSpawnsGrunts = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	TSubclassOf<class ADefenceBase> DefenceClassToSpawn;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	float SpawnDelay = 2.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	float GravityScale = 1.f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    virtual void PodMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnPodOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	bool bSpawned = false;
	
	UFUNCTION(BlueprintCallable)
	void InitializeDefence(TSubclassOf<ADefenceBase> DefenceClass, bool bIsSpawningGrunts);
	UFUNCTION(BlueprintCallable)
	void SpawnDefence();
};
