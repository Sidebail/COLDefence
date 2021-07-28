// Fill out your copyright notice in the Description page of Project Settings.


#include "DropPodBase.h"

#include "Components/ArrowComponent.h"

// Sets default values
ADropPodBase::ADropPodBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//StaticMesh->SetEnableGravity(true);
	//StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetGenerateOverlapEvents(true);
	RootComponent = StaticMesh;

	FirstTroopSpawnLocation = CreateDefaultSubobject<UArrowComponent>("FirstTroopSpawnLocation");
	FirstTroopSpawnLocation->SetupAttachment(RootComponent);

	SecondTroopSpawnLocation = CreateDefaultSubobject<UArrowComponent>("SecondTroopSpawnLocation");
	SecondTroopSpawnLocation->SetupAttachment(RootComponent);

	SpawnEffect = CreateDefaultSubobject<UParticleSystemComponent>("SpawnEffect");
}

// Called when the game starts or when spawned
void ADropPodBase::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ADropPodBase::OnPodOverlapBegin);
	StaticMesh->OnComponentHit.AddDynamic(this, &ADropPodBase::PodMeshHit);
}

// Called every frame
void ADropPodBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADropPodBase::PodMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if(!bSpawned)
	{
		StaticMesh->SetEnableGravity(false);
		StaticMesh->SetSimulatePhysics(false);
		FTimerHandle DelayHandle;
		GetWorldTimerManager().SetTimer(DelayHandle, this, &ADropPodBase::SpawnDefence, SpawnDelay, false);
		bSpawned = true;
	}
	
}

void ADropPodBase::OnPodOverlapBegin(class UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if(!bSpawned)
	{
		StaticMesh->SetEnableGravity(false);
		StaticMesh->SetSimulatePhysics(false);
		FTimerHandle DelayHandle;
		GetWorldTimerManager().SetTimer(DelayHandle, this, &ADropPodBase::SpawnDefence, SpawnDelay, false);
		bSpawned = true;
	}
}

void ADropPodBase::InitializeDefence(TSubclassOf<ADefenceBase> DefenceClass, bool bIsSpawningGrunts)
{
	DefenceClassToSpawn = DefenceClass;
	bSpawnsGrunts = bIsSpawningGrunts;
	StaticMesh->SetMassScale(NAME_None,GravityScale);
}

void ADropPodBase::SpawnDefence()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Instigator = GetInstigator();
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if(bSpawnsGrunts)
	{
		FVector SpawnLocation = FirstTroopSpawnLocation->GetComponentLocation();
		FRotator SpawnRotation = FirstTroopSpawnLocation->GetComponentRotation();
		GetWorld()->SpawnActor(DefenceClassToSpawn, &SpawnLocation, &SpawnRotation, SpawnParameters);
		SpawnLocation = SecondTroopSpawnLocation->GetComponentLocation();
		SpawnRotation = SecondTroopSpawnLocation->GetComponentRotation();
		GetWorld()->SpawnActor(DefenceClassToSpawn, &SpawnLocation, &SpawnRotation, SpawnParameters);
	}else
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor(DefenceClassToSpawn, &SpawnLocation, &SpawnRotation, SpawnParameters);
	}

	// Plays Particle
	if(SpawnEffect->Template)
		SpawnEffect->Activate();
	// Allows the drop-pod to fall through ground.
	StaticMesh->SetEnableGravity(true);
	StaticMesh->SetMassScale(NAME_None, GravityScale/3);
	StaticMesh->SetSimulatePhysics(true);
}

