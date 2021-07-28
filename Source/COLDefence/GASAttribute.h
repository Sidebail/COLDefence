// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "GASAttribute.generated.h"


// Using macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * @note VLAD: Deprecated. Trash code. Sorry...
 */
UCLASS()
class COLDEFENCE_API UGASAttribute : public UAttributeSet
{
	GENERATED_BODY()

public:
	//UGASAttribute();
	
	UPROPERTY(BlueprintReadOnly, Category="Attributes")
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UGASAttribute, Health);

	UPROPERTY(BlueprintReadOnly, Category="Attributes")
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UGASAttribute, Damage);
};
