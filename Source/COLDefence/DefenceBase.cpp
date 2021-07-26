// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenceBase.h"


#include "AIController.h"
#include "COLDefenceInstance.h"
#include "DefenceController.h"
#include "EnemyBase.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ADefenceBase::ADefenceBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGASComponent>("AbilitySystemComponent");

	RotatingPart = CreateDefaultSubobject<USceneComponent>("HeadOrTurret");
	RotatingPart->SetupAttachment(RootComponent);
	ShootLocation = CreateDefaultSubobject<UArrowComponent>("ShootLocation");
	ShootLocation->SetupAttachment(RotatingPart);

}

// Called when the game starts or when spawned
void ADefenceBase::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

// Called every frame
void ADefenceBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireTick(DeltaTime);
}

void ADefenceBase::FireTick(float DeltaTime)
{
	TimeToNextShot -= DeltaTime;
	if(TimeToNextShot<=0)
	{
		TimeToNextShot = 0.f;
	}
}

void ADefenceBase::Initialize()
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
		if(myGI->GetDefenceStats(DefenceName, DefenceData))
		{
			CurrentHealth = DefenceData.Health;
			// Set the range of controller sensing!
			ADefenceController* myController = Cast<ADefenceController>(GetController());
			if(myController)
			{
				myController->SensingComponent->SightRadius = DefenceData.SightRange;
			}
			
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("ADefenceBase::Initialize -- Couldn't find Defence while initiating defence!"));
		}
	}
}

float ADefenceBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth-=DamageAmount;
	if(CurrentHealth<=0)
	{
		DefenceDeath();
	}

	return  DamageAmount;
}

void ADefenceBase::Fire()
{
	if(TimeToNextShot <= 0.0f)
	{
		FGameplayTag ShootTag = FGameplayTag::RequestGameplayTag("Shot");
		FGameplayTagContainer TagContainer;
		TagContainer.AddTag(ShootTag);
		AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
		TimeToNextShot = 1/DefenceData.RateOfFire;
	}
}

bool ADefenceBase::LookAtTarget(AActor* TargetToLookAt, float DeltaTime)
{
	if(TargetToLookAt)
	{
		FRotator CurrentRotation = RotatingPart->GetComponentRotation();

		FVector TargetLocation = TargetToLookAt->GetActorLocation() + TargetToLookAt->GetActorForwardVector() * 20;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(RotatingPart->GetComponentLocation(), TargetLocation );
		RotatingPart->SetWorldRotation(UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, DefenceData.ReactionLerpSpeed));
		FHitResult HitResult;
		
		GetWorld()->LineTraceSingleByChannel(HitResult, RotatingPart->GetComponentLocation(),
			RotatingPart->GetComponentLocation() + RotatingPart->GetForwardVector() * DefenceData.SightRange,
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

void ADefenceBase::DefenceDeath()
{
	Destroy();
}

