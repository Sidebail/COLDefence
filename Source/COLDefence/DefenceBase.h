// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GASAbility.h"
#include "GASComponent.h"
#include "GameFramework/Character.h"

#include "DefenceBase.generated.h"

UCLASS()
class COLDEFENCE_API ADefenceBase : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefenceBase();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Defence Stats")
	FDefenceStats DefenceData;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Defence Stats")
	float CurrentHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Defence Data")
	FName DefenceName = FName("Grunt");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Shooting")
	float TimeToNextShot = 0.0f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Gameplay Ability System")
	TSubclassOf<class UGASAbility> ShootingAbility;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Gameplay Ability System", meta = (AllowPrivateAccess = "true"))
	class UGASComponent* AbilitySystemComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class UArrowComponent* ShootLocation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class USceneComponent* RotatingPart;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Used for timing out shots
	void FireTick(float DeltaTime);

	void Initialize();
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION(BlueprintCallable)
	void Fire();
	UFUNCTION(BlueprintCallable)
	bool LookAtTarget(AActor* TargetToLookAt, float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void DefenceDeath();
	

};
