// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class COLDEFENCE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

	// ProjectileMovement and Bounds
	/**
	* @brief Collision of projectile (。_。)
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class USphereComponent* ProjectileCollision = nullptr;
	/**
	* @brief Movement data, projectile behaviour. 
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* MovementComponent = nullptr;
	/**
	* @brief VFX for projectile trace, not used :(
	* TODO: Use it goddammit
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visuals", meta=(AllowPrivateAccess="true"))
	class UParticleSystemComponent* ProjectileTrace = nullptr;
	/**
	* @brief VFX for projectile trace, not used :(
	* TODO: Use it goddammit
	*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visuals", meta=(AllowPrivateAccess="true"))
	class UParticleSystemComponent* ProjectileDestroyParticle = nullptr;
	/**
	* @brief Struct with Projectile information, FProjectileStats
	*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Projectile Data")
		FProjectileStats ProjectileData;
	/**
	* @brief Name of projectile to get data of from DT_ProjectileData
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile Data")
		FName ProjectileName = FName("DefaultProjectile");
	/**
	* @brief Damage it deals
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProjectileData")
		float CurrrentDamage = 0.f; // Has to be set right after spawn!
	/**
	* @brief Map that hold damage modifiers for enemies
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProjectileData")
		TMap<EEnemyType, float> DamageModifiersPerEnemyType;
	/**
    	* @brief Map that hold damage modifiers for enemies
    	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProjectileData")
		TMap<EDefenceType, float> DamageModifiersPerDefenceType;// This one too!e
	/**
	* @brief When projectile is friendly it deals damage to enemies and vice versa.
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProjectileData")
		bool bIsFriendlyProjectile = true;
	// TODO: Make recoil and dispersion
	// TODO: Make projectile targeting to centre of screen

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* @brief Initializing the projectile and setting the data from DataTables
	*/
	UFUNCTION()
		void Initialize();
	
	UFUNCTION()
    virtual void BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
    void BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
    void BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
