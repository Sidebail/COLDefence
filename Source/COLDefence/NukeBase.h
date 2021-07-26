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

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Components)
	class UStaticMeshComponent* NukeMesh = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Camera)
	class UCameraComponent* NukeFlyCameraComponent = nullptr;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category=Components)
	class UParticleSystemComponent* EngineFireParticle = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flying")
	float TargetFlyVelocity = 100.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Flying")
	float FlyingAcceleration = 15.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Visuals")
	class UCameraShakeBase* CameraShakeWhenFlying = nullptr;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Destruction")
	class UParticleSystemComponent* NukeDestructionEffect = nullptr;
	UPROPERTY(BlueprintAssignable, Category="Flying")
	FOnNukeTriggered NukeTriggered;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Flying")
	bool bIsNukeSent = false;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Destruction")
	bool bIsNukeDestroyed = false;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Flying")
	float CurrentSpeed = 0.f;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="GameTimer")
	float CurrentNukeTimer = 0;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FlyingTick(float DeltaTime);
	
	UFUNCTION(BlueprintCallable)
	void SendNuke();
	UFUNCTION(BlueprintCallable)
	void NukeDestruction();

};
