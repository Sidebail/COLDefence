// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneralCharacter.h"

#include "COLDefenceInstance.h"
#include "DropPodBase.h"
#include "GASComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGeneralCharacter::AGeneralCharacter()
{
	// Set initial capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	// Create Camera
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	// Create point where gun will be spawned
	WeaponPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("GunPoint"));
	WeaponPoint->SetupAttachment(FirstPersonCameraComponent);
	// Create point where Phone will be located
	PhonePoint = CreateDefaultSubobject<UArrowComponent>(TEXT("PhonePoint"));
	PhonePoint->SetupAttachment(FirstPersonCameraComponent);
	// TODO: Decal!

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(100.f, 100.f, 0.f);

	AbilitySystemComponent = CreateDefaultSubobject<UGASComponent>(TEXT("AbilitySystemComponent"));

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>("GunMesh");
	GunMesh->SetupAttachment(RootComponent);
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GunMesh->SetVisibility(true);
	GunMesh->SetHiddenInGame(false);

	PhoneMesh = CreateDefaultSubobject<UStaticMeshComponent>("PhoneMesh");
	PhoneMesh->SetupAttachment(RootComponent);
	PhoneMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PhoneMesh->SetVisibility(true);
	PhoneMesh->SetHiddenInGame(true);

	BuildDecal = CreateDefaultSubobject<UDecalComponent>("BuildDecal");
	BuildDecal->SetHiddenInGame(true);
	//BuildDecal->SetupAttachment(RootComponent);
	//GameplayAttributes = CreateDefaultSubobject<UGASAttribute>(TEXT("Attributes"));
	
	
}

// Called when the game starts or when spawned
void AGeneralCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializePlayerData();
	InitWeapon(FirstWeaponName);
}

// Called every frame
void AGeneralCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MovementTick(DeltaTime);
	// Is phone is used, draw decal before player
	if(bIsPhoneEquipped && bCanDecalMove)
	{
		BuildDecal->SetWorldRotation(FRotator(BuildDecal->GetComponentRotation().Pitch,GetActorRotation().Yaw,BuildDecal->GetComponentRotation().Roll));
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, FirstPersonCameraComponent->GetComponentLocation(),
                FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * PlayerStats.MaxBuildRange,
                ECollisionChannel::ECC_Visibility);
		if(HitResult.bBlockingHit)
		{
			if(BuildDecal->bHiddenInGame == true)
				BuildDecal->SetHiddenInGame(false);

			BuildDecal->SetWorldLocation(HitResult.Location);

			if(HitResult.Normal.Z > 0.5f)
			{
				if(!bCanCallDefenceHere)
					bCanCallDefenceHere = true;
				if(BuildDecal->GetDecalMaterial() != ValidDefenceMat)
					BuildDecal->SetDecalMaterial(ValidDefenceMat);
			}else
			{
				if(bCanCallDefenceHere)
					bCanCallDefenceHere = false;
				if(BuildDecal->GetDecalMaterial() != InvalidDefenceMat)
					BuildDecal->SetDecalMaterial(InvalidDefenceMat);
			}
			//BuildDecal->SetWorldRotation(FRotator(0,0,0));
		}else
		{
			if(BuildDecal->bHiddenInGame == false)
				BuildDecal->SetHiddenInGame(true);
			bCanCallDefenceHere = false;
		}
	}
}


void AGeneralCharacter::MovementTick(float DeltaTime)
{
	if(!bPlayerIsDead)
	{
		AddMovementInput(GetActorForwardVector(), AxisX);
		AddMovementInput(GetActorRightVector(), AxisY);

		AddControllerYawInput(LookAxisX);
		AddControllerPitchInput(LookAxisY);
		SetActorRotation(FRotator(0, GetControlRotation().Yaw, 0));
	}
}


// Called to bind functionality to input
void AGeneralCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AGeneralCharacter::InputAxisX);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AGeneralCharacter::InputAxisY);
	PlayerInputComponent->BindAxis(TEXT("LookXAxis"), this, &AGeneralCharacter::InputLookAxisX);
	PlayerInputComponent->BindAxis(TEXT("LookYAxis"), this, &AGeneralCharacter::InputLookAxisY);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AGeneralCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Pressed, this, &AGeneralCharacter::StartShooting);
	PlayerInputComponent->BindAction(TEXT("Shoot"), IE_Released, this, &AGeneralCharacter::EndShooting);
	PlayerInputComponent->BindAction(TEXT("NextDefence"), IE_Pressed, this, &AGeneralCharacter::NextDefence);
	PlayerInputComponent->BindAction(TEXT("PrevDefence"), IE_Pressed, this, &AGeneralCharacter::PrevDefence);
	PlayerInputComponent->BindAction(TEXT("TogglePhone"), IE_Pressed, this, &AGeneralCharacter::TogglePhone);

}

UAbilitySystemComponent* AGeneralCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGeneralCharacter::InitializeAttributes()
{
	if(AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if(SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle GEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

bool AGeneralCharacter::SpawnDefence()
{
	// Checks if there is a place to spawn the Defence. In range of building.
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, FirstPersonCameraComponent->GetComponentLocation(),
            FirstPersonCameraComponent->GetComponentLocation() + FirstPersonCameraComponent->GetForwardVector() * PlayerStats.MaxBuildRange,
            ECollisionChannel::ECC_Visibility);

	if(HitResult.bBlockingHit)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = GetInstigator();
		SpawnParameters.Owner = GetOwner();
		FVector SpawnLocation = HitResult.Location;
		// TODO: Set the height of where to spawn the Pod.
		SpawnLocation.Z = SpawnLocation.Z + 3000;
		FRotator SpawnRotation = FirstPersonCameraComponent->GetComponentRotation();
		SpawnRotation.Pitch = 0;
		SpawnRotation.Roll = 0;
		ADropPodBase* Pod = Cast<ADropPodBase>(GetWorld()->SpawnActor(PodClass, &SpawnLocation, &SpawnRotation, SpawnParameters));
		if(Pod)
		{
			bCanDecalMove = false;
			bCanCallDefenceHere = false;
			Pod->InitializeDefence(CurrentDefenceClass, (CurrentDefenceData.Type == EDefenceType::SOLDIER)?true:false);
			FTimerHandle DelayHandle;
			// TODO: Set delay timer time to be taken from DataTable
			GetWorldTimerManager().SetTimer(DelayHandle, this, &AGeneralCharacter::LetDecalMove, Pod->SpawnDelay + 1, false);
			return true;
		}
			
	}

	return false;
}

float AGeneralCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	CurrentHealth-=DamageAmount;
	if(CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		PlayerDeath();
	}

	return DamageAmount;
}

void AGeneralCharacter::LetDecalMove()
{
	bCanDecalMove = true;
	if(BuildDecal->bHiddenInGame == false)
		BuildDecal->SetHiddenInGame(true);
}

// Input Auxillary
void AGeneralCharacter::InputAxisY(float Value)
{
	AxisY = Value;

}

void AGeneralCharacter::InputAxisX(float Value)
{
	AxisX = Value;
}

void AGeneralCharacter::InputLookAxisX(float Value)
{
	LookAxisX = Value;
}

void AGeneralCharacter::InputLookAxisY(float Value)
{
	LookAxisY = Value;
}
void AGeneralCharacter::TogglePhone()
{
	// Plays with visibilities 
	bIsPhoneEquipped = !bIsPhoneEquipped;
	if(bIsPhoneEquipped)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
		PhoneMesh->SetHiddenInGame(false);
		BuildDecal->SetHiddenInGame(false);
	}else
	{
		CurrentWeapon->SetActorHiddenInGame(false);
		PhoneMesh->SetHiddenInGame(true);
		BuildDecal->SetHiddenInGame(true);
	}
}

void AGeneralCharacter::InitWeapon(FName WeaponDataRowName)
{
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	if(!myGI)
	{
		UE_LOG(LogTemp, Error, TEXT("AGeneralCharacter::InitWeapon -- Couldn't get GameInstance while initiating weapon!"));
		return;
	}

	FWeaponStats WeaponStats;
	// Creates Weapon actor
	if(myGI->GetWeaponStats(WeaponDataRowName, WeaponStats))
	{
		// Weapon is found, let's spawn it in!
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		SpawnParams.Instigator = GetInstigator();

		FVector SpawnLocation = WeaponPoint->GetComponentLocation();
		FRotator SpawnRotation = WeaponPoint->GetComponentRotation();
		AWeaponBase* NewWeapon = Cast<AWeaponBase>(GetWorld()->SpawnActor(InitialWeaponClass));
		if(NewWeapon)
		{
			CurrentWeapon = NewWeapon;
			NewWeapon->AttachToComponent(WeaponPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
	}else
	{
		UE_LOG(LogTemp, Error, TEXT("AGeneralCharacter::InitWeapon -- Couldn't find Weapon while initiating weapon!"));
        return;
	}
	
		
}

void AGeneralCharacter::StartShooting()
{
	if(!bIsPhoneEquipped)
		CurrentWeapon->bIsFiring = true;
	else
	{
		UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
		if(myGI)
		{
			if(bCanCallDefenceHere)
			{
				if(myGI->DecreaseRespect(CurrentDefenceCost))
				{
					SpawnDefence();
				}
			}
		}
	}
		
}

void AGeneralCharacter::EndShooting()
{
	CurrentWeapon->bIsFiring = false;
}

void AGeneralCharacter::NextDefence()
{
	if(DefencesToCall.Num() - 1 > CurrentDefenceIndex)
	{
		CurrentDefenceIndex++;
		EquipDefence(DefencesToCall[CurrentDefenceIndex]);
	}
}

void AGeneralCharacter::PrevDefence()
{
	if(CurrentDefenceIndex > 0)
	{
		CurrentDefenceIndex--;
		EquipDefence(DefencesToCall[CurrentDefenceIndex]);
	}
}

void AGeneralCharacter::EquipDefence(FName DefenceName)
{
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	if(myGI)
	{
		FDefenceStats DefenceData;
		if(myGI->GetDefenceStats(DefenceName, DefenceData))
		{
			CurrentDefenceClass = DefenceData.DefenceClass;
			CurrentDefenceCost = DefenceData.RespectCost;
			CurrentDefenceData = DefenceData;
			myGI->UpdateDefenceUI(DefenceData);
		}
	}
}

void AGeneralCharacter::InitializePlayerData()
{
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	if(myGI)
	{
		myGI->GetPlayerStats(FName("DEFAULT"), PlayerStats);
		GetCharacterMovement()->MaxWalkSpeed = PlayerStats.WalkSpeed;
		GetCharacterMovement()->JumpZVelocity = PlayerStats.JumpScale;
		GetCapsuleComponent()->SetCapsuleSize(PlayerStats.PlayerCapsuleRadius, PlayerStats.CapsuleHalfHeight);
		DeathTimeout = PlayerStats.DeathTimeout;
		SpeedOfDeathFlyLocationChange = PlayerStats.FlyLocationChangeSpeed;
		SpeedOfDeathFlyRotationChange = PlayerStats.FlyRotationChangeSpeed;
	}

	if(DefencesToCall.Num() > 0)
		EquipDefence(DefencesToCall[0]);
}

void AGeneralCharacter::PlayerDeath()
{
	if(!bPlayerIsDead)
	{
		bPlayerIsDead = true;
		if(bIsPhoneEquipped)
			TogglePhone();
		if(CurrentWeapon)
			CurrentWeapon->SetActorHiddenInGame(true);
		FTimerHandle RespawnTimerHandle;
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &AGeneralCharacter::PlayerRespawn, DeathTimeout);

		UGameplayStatics::GetPlayerController(GetWorld(), 0)->UnPossess();
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(DeathPawn);
	}
}

void AGeneralCharacter::PlayerRespawn()
{
	bPlayerIsDead = false;
	CurrentWeapon->SetActorHiddenInGame(false);
	SetActorLocation(PlayerRespawnLocation);
	SetActorRotation(FRotator(0,-90,0));
	CurrentHealth = PlayerStats.MaxHealth;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->UnPossess();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
}

