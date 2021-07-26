// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GASAbility.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UCLASS()
class COLDEFENCE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Stats")
		FEnemyStats EnemyData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Stats")
		float CurrentHealth = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy Data")
		FName EnemyName = FName("Melee");

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Shooting")
		float TimeToNextShot = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Animation")
		UMaterialInterface* ImHitMaterial = nullptr;

	TArray<UMaterialInterface*> DefaultMaterials;
	TArray<UStaticMeshComponent*> StaticComps;

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
	void FireTick(float DeltaTime);
	
	void Initialize();
	
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void ReturnDefaultMaterials();
	UFUNCTION()
    virtual void EnemyCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
	void Fire();
	UFUNCTION(BlueprintCallable)
    bool LookAtTarget(AActor* TargetToLookAt, float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void EnemyDeath();
};

