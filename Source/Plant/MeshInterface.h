// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "MeshInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UMeshInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

/**
 * 
 */
class PLANT_API IMeshInterface
{
	GENERATED_IINTERFACE_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void Collapse();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	bool IsSelected();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void SetSelected(bool select);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void UnregisterStock();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void Expand();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void RemoveFocus();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void RemoveFocusChild();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "MyCategory")
	void SetFocusChild(UMeshComponent* child);
};
