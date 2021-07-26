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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Components, meta=(AllowPrivateAccess="true"))
	class USphereComponent* ProjectileCollision = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	class UProjectileMovementComponent* MovementComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visuals", meta=(AllowPrivateAccess="true"))
	class UParticleSystemComponent* ProjectileTrace = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Visuals", meta=(AllowPrivateAccess="true"))
	class UParticleSystemComponent* ProjectileDestroyParticle = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Projectile Data")
		FProjectileStats ProjectileData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Projectile Data")
		FName ProjectileName = FName("DefaultProjectile");
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProjectileData")
		float CurrrentDamage = 0.f; // Has to be set right after spawn!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProjectileData")
		TMap<EEnemyType, float> DamageModifiersPerEnemyType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="ProjectileData")
		TMap<EDefenceType, float> DamageModifiersPerDefenceType;// This one too!e
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

	// Initializing the projectile and setting the data from DataTables
	UFUNCTION()
		void Initialize();
	
	// Functions to trigger on collision responses for Projectile
	UFUNCTION()
    virtual void BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
    void BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
    void BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
