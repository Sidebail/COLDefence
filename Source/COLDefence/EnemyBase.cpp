// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"


#include "AIController.h"
#include "COLDefenceInstance.h"
#include "DefenceBase.h"
#include "GameplayTagContainer.h"
#include "GASComponent.h"
#include "GeneralCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	AbilitySystemComponent = CreateDefaultSubobject<UGASComponent>("AbilitySystemComponent");

	RotatingPart = CreateDefaultSubobject<USceneComponent>("RotatingPart");
	RotatingPart->SetupAttachment(RootComponent);
	ShootLocation = CreateDefaultSubobject<UArrowComponent>("ShootLocation");
	ShootLocation->SetupAttachment(RotatingPart);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireTick(DeltaTime);
}

void AEnemyBase::FireTick(float DeltaTime)
{
	TimeToNextShot-=DeltaTime;
	if(TimeToNextShot<=0)
	{
		TimeToNextShot = 0;
	}
		
}

void AEnemyBase::Fire()
{
	if(TimeToNextShot <= 0)
	{
		FGameplayTag ShootTag = FGameplayTag::RequestGameplayTag("Shot");
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(ShootTag);
		AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
		TimeToNextShot = 1/EnemyData.RateOfFire;
	}
}

bool AEnemyBase::LookAtTarget(AActor* TargetToLookAt, float DeltaTime)
{
	if(TargetToLookAt)
	{
		FRotator CurrentRotation = RotatingPart->GetComponentRotation();

		FVector TargetLocation = TargetToLookAt->GetActorLocation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(RotatingPart->GetComponentLocation(), TargetLocation );
		RotatingPart->SetWorldRotation(UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, EnemyData.ReactionLerpSpeed));
		FHitResult HitResult;
		
		GetWorld()->LineTraceSingleByChannel(HitResult, RotatingPart->GetComponentLocation(),
            RotatingPart->GetComponentLocation() + RotatingPart->GetForwardVector() * EnemyData.SightRange,
            ECollisionChannel::ECC_Visibility);

		if(HitResult.bBlockingHit)
		{
			AEnemyBase* Enemy = Cast<AEnemyBase>(HitResult.GetActor());
			if(Enemy)
			{
				return true;
			}
		}
	}

	return false;
}

void AEnemyBase::Initialize()
{
	if(AbilitySystemComponent && HasAuthority())
	{
		if(ShootingAbility)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(ShootingAbility.GetDefaultObject(), 1, 0));
			AbilitySystemComponent->InitAbilityActorInfo(this, this);
		}
	}
	
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	if(myGI)
	{
		if(myGI->GetEnemyStats(EnemyName, EnemyData))
		{
			CurrentHealth = EnemyData.Health;
			GetCharacterMovement()->MaxWalkSpeed = EnemyData.MovementSpeed;
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("AEnemyBase::Initialize -- Couldn't find Enemy while initiating enemy!"));
		}
	}

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyBase::EnemyCollisionSphereHit);

	GetComponents<UStaticMeshComponent>(StaticComps);
	for (UStaticMeshComponent* CompMesh : StaticComps)
	{
		DefaultMaterials.Add(CompMesh->GetMaterial(0));
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
                             AActor* DamageCauser)
{
	CurrentHealth-=DamageAmount;
	if(CurrentHealth<=0)
	{
		EnemyDeath();
	}

	if(ImHitMaterial)
	{
		for (UStaticMeshComponent* CompMesh : StaticComps)
		{
			CompMesh->SetMaterial(0, ImHitMaterial);
		}

		FTimerHandle MaterialReturnHandle;
		GetWorldTimerManager().SetTimer(MaterialReturnHandle, this, &AEnemyBase::ReturnDefaultMaterials, 0.1 , false);
	}
	return  DamageAmount;
}

void AEnemyBase::ReturnDefaultMaterials()
{
	for(int i = 0; i < StaticComps.Num(); i++)
	{
		StaticComps[i]->SetMaterial(0, DefaultMaterials[i]);
	}
}

void AEnemyBase::EnemyCollisionSphereHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// We deal damage to nuke if collided!
	// TODO: Deal damage to defences and player!
	ANukeBase* OverlappedNuke = Cast<ANukeBase>(OtherActor);
	if(OverlappedNuke)
	{
		UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
		if(myGI)
		{
			myGI->RegisterNukeDamage();
			Destroy();
		}
	}

	// Deal damage to defence
	ADefenceBase* OverlappedDefence = Cast<ADefenceBase>(OtherActor);
	if(OverlappedDefence)
	{
		if(OverlappedDefence->DefenceData.Type == EDefenceType::SOLDIER || EnemyData.Type == EEnemyType::VEHICLE)
			OverlappedDefence->DefenceDeath();
		else	
			OverlappedDefence->TakeDamage(EnemyData.DealingDamage * EnemyData.DamageModifiersPerDefenceType.FindRef(OverlappedDefence->DefenceData.Type), FDamageEvent(), GetInstigatorController(), this);
		if(EnemyData.Type == EEnemyType::HUMAN)
			Destroy();
	}
	// Deal damage to player
	AGeneralCharacter* OverlappedPlayer = Cast<AGeneralCharacter>(OtherActor);
	if(OverlappedPlayer)
	{
		OverlappedPlayer->TakeDamage(EnemyData.DealingDamage, FDamageEvent(), GetInstigatorController(), this);
		if(EnemyData.Type == EEnemyType::HUMAN)
			Destroy();
	}
}

void AEnemyBase::EnemyDeath()
{
	// TODO: Economy and effects
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	myGI->IncreaseRespect(EnemyData.RespectReward);
	Destroy();
}

