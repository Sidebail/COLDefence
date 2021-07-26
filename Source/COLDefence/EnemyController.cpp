// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"



#include "DefenceBase.h"
#include "EnemyBase.h"
#include "GeneralCharacter.h"
#include "Perception/PawnSensingComponent.h"

AEnemyController::AEnemyController()
{
    SensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("SensingComponent");
    SensingComponent->SetPeripheralVisionAngle(120);
}

void AEnemyController::MakeAShot()
{
    AEnemyBase* ControlledEnemy = Cast<AEnemyBase>(GetPawn());
    if(!ControlledEnemy)
    {
        return;
    }

    ControlledEnemy->Fire();
}

bool AEnemyController::CanShootTarget(AActor* TargetActor)
{
    // Get Reference on controlled pawn!
    AEnemyBase* ControlledEnemy = Cast<AEnemyBase>(GetPawn());
    if(!ControlledEnemy)
    {
        return false;
    }
    // First check if its a player
    AGeneralCharacter* Player = Cast<AGeneralCharacter>(TargetActor);
    if(Player)
    {
        return ControlledEnemy->EnemyData.bCanTargetPlayer;
    }

    // Not the player? Check if its the defence then
    ADefenceBase* Defence = Cast<ADefenceBase>(TargetActor);
    if(Defence)
    {
        // If Defence damage has Modifier 0 then Enemy should ignore it.
        return (ControlledEnemy->EnemyData.DamageModifiersPerDefenceType.FindRef(Defence->DefenceData.Type) > 0) ? true : false;
    }

    return false;
}
