// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"
#include "AssemblyComponent.h"
#include "AnimatedAssemblyComponent.h"

FLinearColor APlantActor::StateColorArray[6] = { FLinearColor::Green, FLinearColor(0.f,1.f,1.f), FLinearColor::Blue, FLinearColor::Yellow, FLinearColor::Red, FLinearColor::Black };

// Sets default values
APlantActor::APlantActor() : constructionMode(false), highlightState(StateStock::NONE_STATE), procedureMode(false), stepPerformed(true)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlantActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlantActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlantActor::Init(const StockPlant* stock)
{
	// Set the root component of this actor
	USceneComponent* root = NewObject<USceneComponent>(this, TEXT("RootComponent"));
	this->SetRootComponent(root);

	// Create the root assembly component
	FString name(stock->getstrHash().c_str());
	UAssemblyComponent* InitRootComponent = NewObject<UAssemblyComponent>(this, FName(*name));

	//TODO
	// Procedure init
	FString path = FPaths::ProjectConfigDir();// +FString("Config/");
	p = new Procedure(std::string(TCHAR_TO_UTF8(*path)) + "filterchange.xml");

	// Init the root assembly component
	InitRootComponent->Init(this, nullptr, stock);
	InitRootComponent->RegisterComponent();
	InitRootComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	// Set the Assembly root component
	AssemblyRootComponent = InitRootComponent;

	// Expand the firts layer of the tree
	IMeshInterface* AssemblyRootComponentInterface = Cast<IMeshInterface>(AssemblyRootComponent);
	AssemblyRootComponentInterface->Execute_Expand(AssemblyRootComponent);

	InitProcedure();
}

void APlantActor::HandleClickOnComponent(UMeshComponent* clickedComponent)
{
	if (procedureMode)
		return;

	// Access to the clicked component
	IMeshInterface* ClickedComponentInterface = Cast<IMeshInterface>(clickedComponent);

	if (!ClickedComponentInterface)
	{
		return;
	}
	
	// Select the clicked component
	ClickedComponentInterface->Execute_SetSelected(clickedComponent, true);

	// Expand the current component
	ClickedComponentInterface->Execute_Expand(clickedComponent);

	// Obtain the parent of the clicked component
	UMeshComponent* ClickedParent = ClickedComponentInterface->Execute_GetParent(clickedComponent);

	if (SelectedComponent)
	{
		// Access to the previous selected component
		IMeshInterface* SelectedComponentInterface = Cast<IMeshInterface>(SelectedComponent);

		// Collapse iteratively, until reaching the parent of the clicked component
		SelectedComponentInterface->Execute_Collapse(SelectedComponent, ClickedParent);

		SelectedComponentInterface->Execute_SetSelected(SelectedComponent, false);
	}

	// Set the new selected component
	SelectedComponent = clickedComponent;
}

void APlantActor::PerformStepForward()
{
	// Check if the root component is expanded
	IMeshInterface* ProcedureComponentRootInterface = Cast<IMeshInterface>(procedureComponentRoot);
	if (!ProcedureComponentRootInterface->Execute_IsExpanded(procedureComponentRoot))
		ProcedureComponentRootInterface->Execute_Expand(procedureComponentRoot);

	bool performNextStep;
	if (stepPerformed) 
	{
		performNextStep = p->NextStep(s);
	}
	else
	{
		performNextStep = true;
	}

	// Ask for the next step
	if (performNextStep)
	{
		//
		stepPerformed = true;

		// Remove the focus of all the components of the previous step
		for (UMeshComponent* lastComponent : LastUsedComponents)
		{
			IMeshInterface* LastComponentInterface = Cast<IMeshInterface>(lastComponent);
			LastComponentInterface->Execute_RemoveFocus(lastComponent);
		}

		// Reset the list of the last used components
		LastUsedComponents.Empty();

		// Iterate over all the instructions of this step
		for (Instruction instruction : s.getInstructions())
		{
			// Restart the root of the search
			UMeshComponent* currentComponent = procedureComponentRoot;

			// Search the component using the path
			for (std::pair<int, int> path : instruction.m_path.GetPath())
			{
				IMeshInterface* CurrentComponentInterface = Cast<IMeshInterface>(currentComponent);
				UMeshComponent* currentSubComponent = CurrentComponentInterface->Execute_GetSubComponent(currentComponent, path.first, path.second);

				// It should always find a component
				if (currentSubComponent) {
					// If the component is not expanded, expand it
					IMeshInterface* CurrentSubComponentInterface = Cast<IMeshInterface>(currentSubComponent);
					if(!CurrentSubComponentInterface->Execute_IsExpanded(currentSubComponent))
						CurrentSubComponentInterface->Execute_Expand(currentSubComponent);

					// Update the current component
					currentComponent = currentSubComponent;
				}
				else
				{
					// This should never happen
					std::cerr << "Component not found" << std::endl;
					break;
				}
			}

			// Add this component to the list of  used components
			LastUsedComponents.Add(currentComponent);

			//  Access to the interface
			IMeshInterface* CurrentComponentInterface = Cast<IMeshInterface>(currentComponent);

			// Set this component as focused
			CurrentComponentInterface->Execute_SetFocus(currentComponent);

			// Execute the instruction accordingly
			switch (instruction.m_type)
			{
			case Instruction::INS_CREATE:
				CurrentComponentInterface->Execute_ShowComponent(currentComponent);
				break;
			case Instruction::INS_DELETE:
				CurrentComponentInterface->Execute_HideComponent(currentComponent);
				break;
			case Instruction::INS_INSERT:
			{
				UAnimatedAssemblyComponent* animatedComponent;
				animatedComponent = Cast<UAnimatedAssemblyComponent>(currentComponent);
				if (animatedComponent) {
					animatedComponent->SetPlayRate(-1);
					animatedComponent->Play(false);
				}
			}
				break;
			case Instruction::INS_REMOVE:
			{
				UAnimatedAssemblyComponent* animatedComponent;
				animatedComponent = Cast<UAnimatedAssemblyComponent>(currentComponent);
				if (animatedComponent) {
					animatedComponent->SetPlayRate(1);
					animatedComponent->Play(false);
				}
			}
				break;
			default:
				break;
			}
		}
	}
	else
	{
		procedureMode = false;
	}
}

void APlantActor::PerformStepBackward()
{
	// Check if the root component is expanded
	IMeshInterface* ProcedureComponentRootInterface = Cast<IMeshInterface>(procedureComponentRoot);
	if (!ProcedureComponentRootInterface->Execute_IsExpanded(procedureComponentRoot))
		ProcedureComponentRootInterface->Execute_Expand(procedureComponentRoot);

	// Remove the focus of all the components of the previous step
	for (UMeshComponent* lastComponent : LastUsedComponents)
	{
		IMeshInterface* LastComponentInterface = Cast<IMeshInterface>(lastComponent);
		LastComponentInterface->Execute_RemoveFocus(lastComponent);
	}

	// Reset the list of the last used components
	LastUsedComponents.Empty();

	// Iterate over all the instructions of this step
	for (Instruction instruction : s.getInstructions())
	{
		// Restart the root of the search
		UMeshComponent* currentComponent = procedureComponentRoot;

		// Search the component using the path
		for (std::pair<int, int> path : instruction.m_path.GetPath())
		{
			IMeshInterface* CurrentComponentInterface = Cast<IMeshInterface>(currentComponent);
			UMeshComponent* currentSubComponent = CurrentComponentInterface->Execute_GetSubComponent(currentComponent, path.first, path.second);

			// It should always find a component
			if (currentSubComponent) {
				// If the component is not expanded, expand it
				IMeshInterface* CurrentSubComponentInterface = Cast<IMeshInterface>(currentSubComponent);
				if (!CurrentSubComponentInterface->Execute_IsExpanded(currentSubComponent))
					CurrentSubComponentInterface->Execute_Expand(currentSubComponent);

				// Update the current component
				currentComponent = currentSubComponent;
			}
			else
			{
				// This should never happen
				std::cerr << "Component not found" << std::endl;
				break;
			}
		}

		// Add this component to the list of  used components
		LastUsedComponents.Add(currentComponent);

		//  Access to the interface
		IMeshInterface* CurrentComponentInterface = Cast<IMeshInterface>(currentComponent);

		// Set this component as focused
		CurrentComponentInterface->Execute_SetFocus(currentComponent);

		// Execute the instruction accordingly
		switch (instruction.m_type)
		{
		case Instruction::INS_CREATE:
			CurrentComponentInterface->Execute_ShowComponent(currentComponent);
			break;
		case Instruction::INS_DELETE:
			CurrentComponentInterface->Execute_HideComponent(currentComponent);
			break;
		case Instruction::INS_INSERT:
		{
			UAnimatedAssemblyComponent* animatedComponent;
			animatedComponent = Cast<UAnimatedAssemblyComponent>(currentComponent);
			if (animatedComponent) {
				animatedComponent->SetPlayRate(1);
				animatedComponent->Play(false);
			}
		}
		break;
		case Instruction::INS_REMOVE:
		{
			UAnimatedAssemblyComponent* animatedComponent;
			animatedComponent = Cast<UAnimatedAssemblyComponent>(currentComponent);
			if (animatedComponent) {
				animatedComponent->SetPlayRate(-1);
				animatedComponent->Play(false);
			}
		}
		break;
		default:
			break;
		}
	}

	// Ask for the previous step
	if (p->PreviousStep(s))
	{
		//
		stepPerformed = false;

		// Remove the focus of all the components of the previous step
		for (UMeshComponent* lastComponent : LastUsedComponents)
		{
			IMeshInterface* LastComponentInterface = Cast<IMeshInterface>(lastComponent);
			LastComponentInterface->Execute_RemoveFocus(lastComponent);
		}

		// Reset the list of the last used components
		LastUsedComponents.Empty();

		// Iterate over all the instructions of this step
		for (Instruction instruction : s.getInstructions())
		{
			// Restart the root of the search
			UMeshComponent* currentComponent = procedureComponentRoot;

			// Search the component using the path
			for (std::pair<int, int> path : instruction.m_path.GetPath())
			{
				IMeshInterface* CurrentComponentInterface = Cast<IMeshInterface>(currentComponent);
				UMeshComponent* currentSubComponent = CurrentComponentInterface->Execute_GetSubComponent(currentComponent, path.first, path.second);

				// It should always find a component
				if (currentSubComponent) {
					// If the component is not expanded, expand it
					IMeshInterface* CurrentSubComponentInterface = Cast<IMeshInterface>(currentSubComponent);
					if (!CurrentSubComponentInterface->Execute_IsExpanded(currentSubComponent))
						CurrentSubComponentInterface->Execute_Expand(currentSubComponent);

					// Update the current component
					currentComponent = currentSubComponent;
				}
				else
				{
					// This should never happen
					std::cerr << "Component not found" << std::endl;
					break;
				}
			}

			// Add this component to the list of  used components
			LastUsedComponents.Add(currentComponent);

			//  Access to the interface
			IMeshInterface* CurrentComponentInterface = Cast<IMeshInterface>(currentComponent);

			// Set this component as focused
			CurrentComponentInterface->Execute_SetFocus(currentComponent);

			// Execute the instruction accordingly
			switch (instruction.m_type)
			{
			case Instruction::INS_CREATE:
				CurrentComponentInterface->Execute_ShowComponent(currentComponent);
				break;
			case Instruction::INS_DELETE:
				CurrentComponentInterface->Execute_HideComponent(currentComponent);
				break;
			case Instruction::INS_INSERT:
			{
				UAnimatedAssemblyComponent* animatedComponent;
				animatedComponent = Cast<UAnimatedAssemblyComponent>(currentComponent);
				if (animatedComponent) {
					animatedComponent->SetPlayRate(1);
					animatedComponent->Play(false);
				}
			}
			break;
			case Instruction::INS_REMOVE:
			{
				UAnimatedAssemblyComponent* animatedComponent;
				animatedComponent = Cast<UAnimatedAssemblyComponent>(currentComponent);
				if (animatedComponent) {
					animatedComponent->SetPlayRate(-1);
					animatedComponent->Play(false);
				}
			}
			break;
			default:
				break;
			}
		}
	}
	else
	{
		procedureMode = false;
	}
}

void APlantActor::ToggleConstructionMode()
{
	// Toggle the construction mode
	constructionMode = !constructionMode;

	// Process the visualization mode in all the tree
	IMeshInterface* AssemblyRootComponentInterface = Cast<IMeshInterface>(AssemblyRootComponent);
	AssemblyRootComponentInterface->Execute_ProcessVisualizationMode(AssemblyRootComponent);
}

void APlantActor::SetHighlightState(StateStock state)
{
	// Set the highlight state
	highlightState = state;

	// Process the visualization mode in all the tree
	IMeshInterface* AssemblyRootComponentInterface = Cast<IMeshInterface>(AssemblyRootComponent);
	AssemblyRootComponentInterface->Execute_ProcessVisualizationMode(AssemblyRootComponent);
}

void APlantActor::InitProcedure()
{
	procedureMode = true;

	IMeshInterface* AssemblyRootComponentInterface = Cast<IMeshInterface>(AssemblyRootComponent);
	procedureComponentRoot = AssemblyRootComponentInterface->Execute_GetSubComponent(AssemblyRootComponent, p->GetAssemblyId(), 1);
}

void APlantActor::FinishProcedure()
{
	procedureMode = false;
}

