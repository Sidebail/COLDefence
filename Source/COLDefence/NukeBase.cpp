// Fill out your copyright notice in the Description page of Project Settings.


#include "NukeBase.h"



#include "COLDefenceInstance.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ANukeBase::ANukeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NukeMesh = CreateDefaultSubobject<UStaticMeshComponent>("NukeMesh");
	RootComponent = NukeMesh;
	
	NukeFlyCameraComponent = CreateDefaultSubobject<UCameraComponent>("FlyCamera");
	NukeFlyCameraComponent->SetupAttachment(RootComponent);

	EngineFireParticle = CreateDefaultSubobject<UParticleSystemComponent>("EngineFireEffect");
	EngineFireParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANukeBase::BeginPlay()
{
	Super::BeginPlay();
	UCOLDefenceInstance* myGI = Cast<UCOLDefenceInstance>(GetGameInstance());
	if(myGI)
	{
		FTimerHandle SendNukeTimer;
		GetWorldTimerManager().SetTimer(SendNukeTimer, this, &ANukeBase::SendNuke, myGI->Rules.NukeLaunchTimeInSeconds, false);
	}
}

// Called every frame
void ANukeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!bIsNukeSent)
	{
		CurrentNukeTimer+=DeltaTime;
	}
	FlyingTick(DeltaTime);
}

void ANukeBase::FlyingTick(float DeltaTime)
{
	if(bIsNukeSent)
	{
		//FMath::FInterpTo(CurrentSpeed, TargetFlyVelocity, DeltaTime,FlyingAcceleration);
		if(CurrentSpeed < TargetFlyVelocity)
			CurrentSpeed +=FlyingAcceleration;
		AddActorWorldOffset(FVector(0,0,CurrentSpeed * DeltaTime));
	}
}

void ANukeBase::SendNuke()
{
	if(!bIsNukeDestroyed && !bIsNukeSent)
	{
		NukeTriggered.Broadcast(true);
		bIsNukeSent = true;
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->UnPossess();
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->Possess(this);
		bIsNukeSent = true;
	}
}

void ANukeBase::NukeDestruction()
{
	if(!bIsNukeDestroyed && !bIsNukeSent)
	{
		NukeTriggered.Broadcast(false);
		bIsNukeDestroyed = true;
		
	}
}

