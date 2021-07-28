// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DefenceController.generated.h"

/**
 * 
 */
UCLASS()
class COLDEFENCE_API ADefenceController : public AAIController
{
	GENERATED_BODY()

public:
	// Constructor
	ADefenceController();

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
	FName BBKey_CurrentTarget = FName("CurrentTarget");
	/**
	* @brief FName to look-up in Blackboard.
	* 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_IsProperTargetFound = FName("bIsProperTargetFound");

	
	UFUNCTION(BlueprintCallable)
	virtual void BeginPlay() override;
	

	/**
	* @brief Triggers shooting in controlled defence.
	* 
	*/
	UFUNCTION(BlueprintCallable)
    void MakeAShot();
	/**
	* @brief Function to use from Blueprint by BehaviorTree. Returns if Defence unit can shoot enemy.
	* Checks Modifier. If Modifier is 0 then it doesnt even target that type of Enemy.
	*/
	UFUNCTION(BlueprintCallable)
    bool CanShootTarget(AActor* TargetActor);
	
};
