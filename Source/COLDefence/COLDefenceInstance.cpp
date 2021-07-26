// Fill out your copyright notice in the Description page of Project Settings.


#include "COLDefenceInstance.h"

bool UCOLDefenceInstance::GetPlayerStats(FName PlayerName, FPlayerStats& OutInfo)
{
    bool bIsFound = false;
    if(PlayerInfoTable)
    {
        FPlayerStats* PlayerInfoRow = PlayerInfoTable->FindRow<FPlayerStats>(PlayerName, "", false);
        if(PlayerInfoRow)
        {
            bIsFound = true;
            OutInfo = *PlayerInfoRow;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetPlayerStats -- Player entry not found!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetPlayerStats -- Missing PlayerTable pointer! NullException!"));
    }

    return bIsFound;
}

bool UCOLDefenceInstance::GetWeaponStats(FName WeaponName, FWeaponStats& OutInfo)
{
    bool bIsFound = false;
    if(WeaponInfoTable)
    {
        FWeaponStats* WeaponInfoRow = WeaponInfoTable->FindRow<FWeaponStats>(WeaponName, "", false);
        if(WeaponInfoRow)
        {
            bIsFound = true;
            OutInfo = *WeaponInfoRow;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetWeaponStats -- Weapon entry not found!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetWeaponStats -- Missing WeaponTable pointer! NullException!"));
    }

    return bIsFound;
}

bool UCOLDefenceInstance::GetProjectileStats(FName ProjectileName, FProjectileStats& OutInfo)
{
    bool bIsFound = false;
    if(ProjectileInfoTable)
    {
        FProjectileStats* ProjectileInfoRow = ProjectileInfoTable->FindRow<FProjectileStats>(ProjectileName, "", false);
        if(ProjectileInfoRow)
        {
            bIsFound = true;
            OutInfo = *ProjectileInfoRow;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UCOLDefenceInstance::GetProjectileStats -- Projectile entry not found!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UCOLDefenceInstance::GetProjectileStats -- Missing ProjectileTable pointer! NullException!"));
    }

    return bIsFound;
}

bool UCOLDefenceInstance::GetDefenceStats(FName DefenceName, FDefenceStats& OutInfo)
{
    bool bIsFound = false;
    if(DefencesInfoTable)
    {
        FDefenceStats* DefenceInfoRow = DefencesInfoTable->FindRow<FDefenceStats>(DefenceName, "", false);
        if(DefenceInfoRow)
        {
            bIsFound = true;
            OutInfo = *DefenceInfoRow;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetDefenceStats -- Defence entry not found!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetDefenceStats -- Missing DefenceTable pointer! NullException!"));
    }

    return bIsFound;
}

bool UCOLDefenceInstance::GetEnemyStats(FName EnemyName, FEnemyStats& OutInfo)
{
    bool bIsFound = false;
    if(EnemyInfoTable)
    {
        FEnemyStats* EnemyInfoRow = EnemyInfoTable->FindRow<FEnemyStats>(EnemyName, "", false);
        if(EnemyInfoRow)
        {
            bIsFound = true;
            OutInfo = *EnemyInfoRow;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetEnemyStats -- Enemy entry not found!"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("COLDefenceInstance::GetEnemyStats -- Missing EnemyTable pointer! NullException!"));
    }

    return bIsFound;
}

bool UCOLDefenceInstance::GetWaveInfo(int32 WaveIndex, FWaveInfo& OutInfo)
{
    bool bIsFound = false;
    if(WavesInfoTable)
    {
        TArray<FWaveInfo*> Rows;
        
        WavesInfoTable->GetAllRows(TEXT(""), Rows);
        if(WaveIndex <= Rows.Num()-1)
        {
            OutInfo = *Rows[WaveIndex];
            bIsFound = true;
        }      
             
        /**
        FWaveInfo* WaveInfoRow = WavesInfoTable->FindRow<FWaveInfo>(FName("" + WaveIndex), "", false);
        if(WaveInfoRow)
        {
            bIsFound = true;
            OutInfo = *WaveInfoRow;
            UE_LOG(LogTemp, Warning,TEXT("SUCCESS"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UCOLDefenceInstance::GetWaveInfo -- Wave entry not found!"));
        }
        */
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UCOLDefenceInstance::GetWaveInfo -- Missing WaveTable pointer! NullException!"));
    }

    return bIsFound;
}

void UCOLDefenceInstance::InitializeGame()
{
    CurrentRespect = Rules.StartingRespect;
    CurrentNukeGateHealth = Rules.NukeGateHealth;
    CurrentNukeTime = Rules.NukeLaunchTimeInSeconds;
    // TODO: Wave set-up
}

void UCOLDefenceInstance::NukeTick(float DeltaTime)
{
    CurrentNukeTime-=DeltaTime;
    if(CurrentNukeTime<=0)
    {
        LaunchNuke();
        CurrentNukeTime = 0;
    }
}

void UCOLDefenceInstance::LaunchNuke()
{
    Nuke->SendNuke();
}

void UCOLDefenceInstance::BlowUpNuke()
{
    Nuke->NukeDestruction();
}

void UCOLDefenceInstance::RegisterNukeDamage()
{
    CurrentNukeGateHealth--;
    if(CurrentNukeGateHealth<=0)
        BlowUpNuke();
}

void UCOLDefenceInstance::IncreaseRespect(float ChangeVal)
{
    CurrentRespect+=ChangeVal;
    if(CurrentRespect > MaxRespect)
        CurrentRespect = MaxRespect;
    // Do UI animation
    RespIncrease.Broadcast();
}

bool UCOLDefenceInstance::DecreaseRespect(float ChangeVal)
{
    if(CurrentRespect < ChangeVal)
    {
        RespInsufficient.Broadcast();
        return false;
    }
        
    
    CurrentRespect-=ChangeVal;
    if(CurrentRespect <= 0)
        CurrentRespect = 0;
    
    RespDecrease.Broadcast();
    return true;
    
}

void UCOLDefenceInstance::UpdateDefenceUI(FDefenceStats DefenceData)
{
    DefenceUIUpdate.Broadcast();
}


