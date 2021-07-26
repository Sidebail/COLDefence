// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"

#include "COLDefenceInstance.h"
#include "DefenceBase.h"
#include "EnemyBase.h"
#include "GeneralCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileCollision = CreateDefaultSubobject<USphereComponent>("ProjectileCollision");
	ProjectileCollision->SetSphereRadius(16.0f);
	ProjectileCollision->SetCanEverAffectNavigation(false);

	// Set the collision as root
	RootComponent = ProjectileCollision;

	ProjectileTrace = CreateDefaultSubobject<UParticleSystemComponent>("ProjectileTraceParticle");
	ProjectileTrace->SetupAttachment(RootComponent);
	ProjectileTrace->SetAutoActivate(true);

	ProjectileDestroyParticle = CreateDefaultSubobject<UParticleSystemComponent>("ProjectileDestroyParticle");
	ProjectileDestroyParticle->SetupAttachment(RootComponent);
	ProjectileDestroyParticle->SetAutoActivate(false);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	MovementComponent->UpdatedComponent = RootComponent;

	
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
	// Mapping functions for onHit, onOverlap and onEndOverlap
	ProjectileCollision->OnComponentHit.AddDynamic(this,&AProjectileBase::BulletCollisionSphereHit);
	ProjectileCollision->OnComponentBeginOverlap.AddDynamic(this,&AProjectileBase::BulletCollisionSphereBeginOverlap);
	ProjectileCollision->OnComponentEndOverlap.AddDynamic(this, &AProjectileBase::BulletCollisionSphereEndOverlap);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::Initialize()
{
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	if(myGI)
	{
		if(myGI->GetProjectileStats(ProjectileName, ProjectileData))
		{
			MovementComponent->Velocity = GetActorForwardVector() * ProjectileData.ProjectileSpeed;
			MovementComponent->ProjectileGravityScale = ProjectileData.GravityScale;
		}else
		{
			UE_LOG(LogTemp, Error, TEXT(" AProjectileBase::Initialize -- Couldn't find Projectile while initiating projectile!"));
		}
	}
}

void AProjectileBase::BulletCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if(bIsFriendlyProjectile)
	{
		AEnemyBase* HittedEnemy = Cast<AEnemyBase>(OtherActor);
		if(HittedEnemy)
		{
			//UGameplayStatics::ApplyDamage(HittedEnemy, CurrrentDamage * DamageModifiersPerEnemyType.FindRef(HittedEnemy->EnemyData.Type), GetInstigatorController(), this, NULL);
			OtherActor->TakeDamage(CurrrentDamage * DamageModifiersPerEnemyType.FindRef(HittedEnemy->EnemyData.Type), FDamageEvent(), GetInstigatorController(), this);
		}
	}else
	{
		// Deal damage to defence
		ADefenceBase* HittedDefence = Cast<ADefenceBase>(OtherActor);
		if(HittedDefence)
		{
			OtherActor->TakeDamage(CurrrentDamage * DamageModifiersPerDefenceType.FindRef(HittedDefence->DefenceData.Type), FDamageEvent(), GetInstigatorController(), this);
		}
		// Deal damage to player
		AGeneralCharacter* HittedPlayer = Cast<AGeneralCharacter>(OtherActor);
		if(HittedPlayer)
		{
			OtherActor->TakeDamage(CurrrentDamage, FDamageEvent(), GetInstigatorController(), this);
		}
	}
	Destroy();
}

void AProjectileBase::BulletCollisionSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectileBase::BulletCollisionSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

