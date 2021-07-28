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
	/**
	* @brief 3D model of DropPod base.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Components, meta=(AllowPrivateAccess=true))
	class UStaticMeshComponent* StaticMesh;
	/**
	* @brief Arrow with location to spawn grunt defence unit at.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Components, meta=(AllowPrivateAccess=true))
	class UArrowComponent* FirstTroopSpawnLocation;
	/**
	* @brief Arrow with location to spawn grunt defence unit at.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Components, meta=(AllowPrivateAccess=true))
	class UArrowComponent* SecondTroopSpawnLocation;
	/**
	* @brief ParticleEffect to play on spawn
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category=Components, meta=(AllowPrivateAccess=true))
	class UParticleSystemComponent* SpawnEffect;
	/**
	* @brief Spawns two defence units if true and one if false.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	bool bSpawnsGrunts = true;
	/**
	* @brief Spawns actor from THIS class.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	TSubclassOf<class ADefenceBase> DefenceClassToSpawn;
	/**
	* @brief Delay of Defence unit before they actually spawned.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	float SpawnDelay = 2.f;
	/**
	* @brief Changes the mass, according to that scale.
	* 
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere,Category="Behaviour")
	float GravityScale = 1.f;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* @brief Triggered when mesh hits the ground.
	* @note VLAD: Can be changed to prevent from colliding with characters, structures or ground.
	* 
	*/
	UFUNCTION()
    virtual void PodMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	/**
	* @brief Triggered when mesh overlaps the ground. 
	* @note VLAD: Can be changed to prevent from colliding with characters, structures or ground.
	*/
	UFUNCTION()
	void OnPodOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	bool bSpawned = false;
	/**
	* @brief Sets the local variables, get values from arguments.
	* If bIsSpawningGrunts is true then two defences spawned. Otherwise just one.
	*/
	UFUNCTION(BlueprintCallable)
	void InitializeDefence(TSubclassOf<ADefenceBase> DefenceClass, bool bIsSpawningGrunts);
	/**
	* @brief Spawns defence unit actors according to variables stored in DropPod.
	* @note VLAD: Amount of units is based on bIsSpawningGrunts.
	*/
	UFUNCTION(BlueprintCallable)
	void SpawnDefence();
};
