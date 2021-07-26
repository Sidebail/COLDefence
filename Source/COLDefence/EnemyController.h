// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class COLDEFENCE_API AEnemyController : public AAIController
{
	GENERATED_BODY()
	public:
	AEnemyController();
	// AI components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	class UPawnSensingComponent* SensingComponent;

	// Behaviour tree variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour")
	UBlackboardData* BlackboardData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour")
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_NukeTarget = FName("NukeTarget");
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_CurrentTarget = FName("CurrentTarget");
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_IsProperTargetFound = FName("bIsProperTargetFound");
	
	// Functions to trigger in EnemyPawn
	UFUNCTION(BlueprintCallable)
	void MakeAShot();
	UFUNCTION(BlueprintCallable)
	bool CanShootTarget(AActor* TargetActor);
	
};
