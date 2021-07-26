// Fill out your copyright notice in the Description page of Project Settings.


#include "DefenceController.h"

#include "DefenceBase.h"
#include "EnemyBase.h"
#include "GeneralCharacter.h"
#include "Perception/PawnSensingComponent.h"


ADefenceController::ADefenceController()
{
    SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("SensingComponent");
    SensingComponent->SetPeripheralVisionAngle(180);
}


void ADefenceController::BeginPlay()
{
    Super::BeginPlay();    
}


void ADefenceController::MakeAShot()
{
    ADefenceBase* ControlledDefence = Cast<ADefenceBase>(GetPawn());
    if(!ControlledDefence)
    {
        return;
    }

    ControlledDefence->Fire();
}

bool ADefenceController::CanShootTarget(AActor* TargetActor)
{
    // Get Reference on controlled pawn!
    ADefenceBase* ControlledDefence = Cast<ADefenceBase>(GetPawn());
    if(!ControlledDefence)
    {
        return false;
    }

    AEnemyBase* Enemy = Cast<AEnemyBase>(TargetActor);
    if(Enemy)
    {
        // If Defence damage has Modifier 0 then Enemy should ignore it.
        //UE_LOG(LogTemp, Warning, TEXT("SENSED ENEMY! Modifier: %d"), ControlledDefence->DefenceData.DamageModifiersPerEnemyType.FindRef(Enemy->EnemyData.Type));
        return (ControlledDefence->DefenceData.DamageModifiersPerEnemyType.FindRef(Enemy->EnemyData.Type) > 0) ? true : false;
    }

    return false;
}
