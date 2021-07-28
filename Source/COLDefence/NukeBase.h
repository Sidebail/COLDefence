// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NukeBase.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNukeTriggered, bool, bIsNukeSent);

UCLASS()
class COLDEFENCE_API ANukeBase : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANukeBase();
	/**
	* @brief 3D model of mesh and base.
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Components)
	class UStaticMeshComponent* NukeMesh = nullptr;
	/**
	* @brief Camera that player will be linked to when possessed into Nuke.
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Camera)
	class UCameraComponent* NukeFlyCameraComponent = nullptr;
	/**
	* @brief Particle system for engine fire
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Components)
	class UParticleSystemComponent* EngineFireParticle = nullptr;
	/**
	* @brief Speed for for sending the nuke.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flying")
	float TargetFlyVelocity = 100.f;
	/**
	* @brief Acceleration to send the nuke
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flying")
	float FlyingAcceleration = 15.f;
	/**
	* @brief Camera shake asset to play
	* @note VLAD: That I forgot to use, damn...
	* TODO: Use the shake!
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visuals")
	class UCameraShakeBase* CameraShakeWhenFlying = nullptr;
	/**
	* @brief Effect of nuke destruction
	* @note VLAD: Which is not uuuuused... daaaamn..
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	class UParticleSystemComponent* NukeDestructionEffect = nullptr;
	/**
	* @brief Delegate for events to bind, for Widgets.
	*/
	UPROPERTY(BlueprintAssignable, Category="Flying")
	FOnNukeTriggered NukeTriggered;
	/**
	* @brief Variables says for itself (。_。)
	*/
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Flying")
	bool bIsNukeSent = false;
	/**
    * @brief Variables says for itself (。_。)
    */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Destruction")
	bool bIsNukeDestroyed = false;
	/**
	* @brief Speed that nuke moves with when launched
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Flying")
	float CurrentSpeed = 0.f;
	/**
	* @brief If timer is 0, then Nuke is sent
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GameTimer")
	float CurrentNukeTimer = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/**
	* @brief Flying movement on tick
	*/
	void FlyingTick(float DeltaTime);
	/**
	* @brief Nuke is sent - game is victorious!
	*/
	UFUNCTION(BlueprintCallable)
	void SendNuke();
	/**
	* @brief Nuke is destroyed - game is failed! Try again!
	*/
	UFUNCTION(BlueprintCallable)
	void NukeDestruction();

};
