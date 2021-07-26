// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GASAbility.h"
#include "GameFramework/Actor.h"
#include "GASComponent.h"
#include "Types.h"

#include "WeaponBase.generated.h"

UCLASS()
class COLDEFENCE_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay Ability System")
	TSubclassOf<class UGASAbility> ShootingAbility;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapon Data")
		FWeaponStats WeaponData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Weapon Data")
	FName WeaponName = FName("StandartIssueRifle");
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay Ability System")
	FGameplayTagContainer TagContainer;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	class UGASComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class UArrowComponent* ShootLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class UStaticMeshComponent* StaticMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void FireTick(float DeltaTime);
	float TimeToNextShot = 0.0f;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsFiring = false;

	UFUNCTION()
		void Initialize();
	UFUNCTION(BlueprintCallable)
		void Fire();
};
