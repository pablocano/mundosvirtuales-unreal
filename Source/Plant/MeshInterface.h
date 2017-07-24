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
	
  void Collapse();

	virtual bool IsSelected() = 0;

	virtual bool SetSelected(bool select) = 0;
};
