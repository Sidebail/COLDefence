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
	/**
	* @brief AIPawn sensing component.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	class UPawnSensingComponent* SensingComponent;

	// Behaviour tree variables
	/**
	* @brief Pointer to blackboard asset that is going to be used for AI.
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour")
	UBlackboardData* BlackboardData = nullptr;
	/**
	* @brief Triggers the defence death sequence. Leads to Destroy()
	* 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour")
	UBehaviorTree* BehaviorTree = nullptr;
	/**
	* @brief FName to look-up in Blackboard.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_NukeTarget = FName("NukeTarget");
	/**
	* @brief FName to look-up in Blackboard.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_CurrentTarget = FName("CurrentTarget");
	/**
	* @brief FName to look-up in Blackboard.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_IsProperTargetFound = FName("bIsProperTargetFound");
	
	// Functions to trigger in EnemyPawn
	/**
	* @brief Triggers shooting in controlled enemy.
	* 
	*/
	UFUNCTION(BlueprintCallable)
	void MakeAShot();
	/**
	* @brief Function to use from Blueprint by BehaviorTree. Returns if Defence unit can shoot enemy.
	* Checks Modifier. If Modifier is 0 then it doesnt even target that type of Defence.
	*/
	UFUNCTION(BlueprintCallable)
	bool CanShootTarget(AActor* TargetActor);
	
};
