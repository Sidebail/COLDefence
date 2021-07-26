// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "COLDefenceInstance.h"
#include "GameplayTagsManager.h"
#include "Components/ArrowComponent.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UGASComponent>("AbilitySystemComponent");

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("GunMesh");
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = StaticMeshComponent;
	ShootLocation = CreateDefaultSubobject<UArrowComponent>("ShootLocation");
	ShootLocation->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FireTick(DeltaTime);
}

void AWeaponBase::FireTick(float DeltaTime)
{
	
	TimeToNextShot-=DeltaTime;
	if(TimeToNextShot < 0)
			TimeToNextShot = 0.0f;
	
	if(bIsFiring)
	{
		if(TimeToNextShot <= 0.0f)
		{
			Fire();
			TimeToNextShot = 1/WeaponData.RateOfFire;
		}
	}
}

void AWeaponBase::Initialize()
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
		if(myGI->GetWeaponStats(WeaponName, WeaponData))
		{
			
		}else
		{
			UE_LOG(LogTemp, Error, TEXT("AWeaponBase::Initialize -- Couldn't find Weapon while initiating weapon!"));
		}
	}
}

void AWeaponBase::Fire()
{
	FGameplayTag ShootTag = FGameplayTag::RequestGameplayTag("Shot");
	TagContainer.AddTag(ShootTag);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
}
