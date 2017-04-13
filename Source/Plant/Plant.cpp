// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantCore/Machine.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Plant, "Plant" );

// Utils Function

void SetBoolUProperty(UObject* Object, FName PropertyName, bool value)
{
	UProperty* Property = Object->GetClass()->FindPropertyByName(PropertyName);

	if (UBoolProperty *BoolProperty = Cast<UBoolProperty>(Property))
	{
		void* ValuePtr = BoolProperty->ContainerPtrToValuePtr<void>(Object);
		BoolProperty->SetPropertyValue(ValuePtr, value);
	}
}