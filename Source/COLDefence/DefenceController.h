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
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components)
	class UPawnSensingComponent* SensingComponent;
	
	// Behaviour tree variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour")
	UBlackboardData* BlackboardData = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Behaviour")
	UBehaviorTree* BehaviorTree = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_CurrentTarget = FName("CurrentTarget");
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Behaviour")
	FName BBKey_IsProperTargetFound = FName("bIsProperTargetFound");

	
	UFUNCTION(BlueprintCallable)
	virtual void BeginPlay() override;
	

	// Functions to trigger in DefencePawn
	UFUNCTION(BlueprintCallable)
    void MakeAShot();
	UFUNCTION(BlueprintCallable)
    bool CanShootTarget(AActor* TargetActor);
	
};
