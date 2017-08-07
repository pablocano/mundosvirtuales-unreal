// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"
#include "AssemblyComponent.h"

#define M_PI           3.14159265358979323846  /* pi */

UAssemblyComponent::UAssemblyComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), selected(false)
{
}

void UAssemblyComponent::CustomOnBeginMouseOver(UPrimitiveComponent * TouchedComponent)
{
	//If this component is focused, do nothing
	if (borderStatus == FOCUS)
		return;

	// If this component has a parent
	if (parent)
	{
		// Access to the parent
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
		
		// And if its parent is selected
		if (parentInterface->Execute_IsSelected(parent))
		{
			// If this componet is not already selected
			if (!selected /*&& stock->getCanBeSelected()*/)
			{
				// Set borders as hover
				SetBorders(HOVER);
				return;
			}
		}
	}

	// If this component is not selected, set hover
	if (!selected /*&& stock->getCanBeSelected()*/)
		SetHover();
}

void UAssemblyComponent::CustomOnEndMouseOver(UPrimitiveComponent * TouchedComponent)
{
	// If this component is focused, do nothing
	if (borderStatus == FOCUS)
		return;

	// If this component has a parent
	if (parent)
	{
		// Access to the parent
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
    
		//If the parent is selected
		if (parentInterface->Execute_IsSelected(parent))
		{
			// If this component is not already selected
			if (!selected /*&& stock->getCanBeSelected()*/)
			{
				// Set borders to nothing
				SetBorders(NOTHING);
				return;
			}
		}
	}

	// If this component is not selected, remove hover
	if (!selected /*&& stock->getCanBeSelected()*/)
		SetHover(false);
}

void UAssemblyComponent::SetHover(bool hover)
{
	// If the material can be accessed
	if (DynMaterial)
	{
		// Get the color from the material
		FLinearColor color;
		DynMaterial->GetVectorParameterValue("BaseColor", color);

		// Toogle the emissivity accoding to the hover state
		float emissive = hover ? 0.3f : 0.f;

		// Set the emissive color
		color = color * emissive;

		// Set the emissivity of the material
		DynMaterial->SetVectorParameterValue("Emissive", color);
	}
	
}

void UAssemblyComponent::CustomOnBeginMouseClicked(UPrimitiveComponent * TouchedComponent, FKey key)
{
	// If this component is already selected or if cannot be selected, do nothing
	if (selected /*|| !stock->getCanBeSelected()*/)
		return;

	// If this stock has substocks, expand this component
	if (!stock->getSubStock().empty())
	{
		// Expand this component
		ExpandStock();
	}
	// Or, if this component is not focused
	else if(borderStatus != FOCUS)
	{
		// If this component has a parent
		if (parent)
		{
			// Access to the parent
			IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

			// Remove the focused child of the parent of this component
			parentInterface->Execute_RemoveFocusChild(parent);

			// Set this component as focused
			parentInterface->Execute_SetFocusChild(parent, this);
		}

		// Set the borders as focused
		SetBorders(FOCUS);
	}
}

bool UAssemblyComponent::IsSelected_Implementation()
{
	return selected;
}

void UAssemblyComponent::SetSelected_Implementation(bool select)
{
	selected = select;
}

void UAssemblyComponent::Hide()
{
	SetVisibility(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UAssemblyComponent::Collapse_Implementation()
{
	// If one of the subcomponent of this component is selected, stop the collapse
	for (UMeshComponent* substock : this->subStocks)
	{
		// Access to the subcomponent
		IMeshInterface* subStockInterface = Cast<IMeshInterface>(substock);

		// If the subcomponent is selected, it means that this layer must be shown
		if (subStockInterface->Execute_IsSelected(substock))
		{
			// Iterate over all the substocks again
			for (UMeshComponent* substock2 : this->subStocks)
			{
				// Access to the subcomponent
				IMeshInterface* subStockInterface2 = Cast<IMeshInterface>(substock2);

				// If the subcomponent is not selected
				if (!subStockInterface2->Execute_IsSelected(substock2))
				{
					// Show the mesh of the subcomponent
					subStockInterface2->Execute_ShowComponent(substock2);
				}
			}

			// End the collapse call
			return;
		}
	}
  
	//If this is not the layer where the selected component is, remove all childs
	for (UMeshComponent* substock : this->subStocks)
	{
		// Access to the subcomponent
		IMeshInterface* subStockInterface = Cast<IMeshInterface>(substock);

		// Remove it from the tree
		subStockInterface->Execute_UnregisterStock(substock);
	}
  
	// Delete all the substocks from the list
	this->subStocks.Empty();
  
	// Access to the parent
	IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
  
	// Collapse the parent
	parentInterface->Execute_Collapse(parent);
}

void UAssemblyComponent::ShowComponent_Implementation()
{
	SetVisibility(true);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetBorders(NOTHING);
}

void UAssemblyComponent::UnregisterStock_Implementation()
{
	this->UnregisterComponent();
}

void UAssemblyComponent::SetBorders(FocusStatus status)
{
	// Set the border status
	borderStatus = status;

	//According to the status, set the borders
	switch (status)
	{
	case NOTHING:
		this->SetRenderCustomDepth(false);
		break;

	case HOVER:
		this->SetCustomDepthStencilValue(252);
		this->SetRenderCustomDepth(true);
		break;

	case FOCUS:
		this->SetCustomDepthStencilValue(254);
		this->SetRenderCustomDepth(true);
		break;

	default:
		break;
	}
}

void UAssemblyComponent::Expand_Implementation()
{
	ExpandStock();
}

void UAssemblyComponent::RemoveFocus_Implementation()
{
	SetBorders(NOTHING);
}

void UAssemblyComponent::RemoveFocusChild_Implementation()
{
	if (focusedChild)
	{
		IMeshInterface* focusedChildInterface = Cast<IMeshInterface>(focusedChild);

		focusedChildInterface->Execute_RemoveFocus(focusedChild);
	}
}

void UAssemblyComponent::SetFocusChild_Implementation(UMeshComponent * child)
{
	this->focusedChild = child;
}

void UAssemblyComponent::ExpandStock()
{
	// Set this component as selected
	selected = true;

	// If this component has a parent, remove the selection state of the parent
	if (parent)
	{
		// Access to the parent
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

		// Remove the selection status
		parentInterface->Execute_SetSelected(parent, false);
	}

	// If there was a previous selected component, collapse the tree
	if (actor->selectedStock)
	{
		// Access to the previous selected component
		IMeshInterface* previousSelectedStockInterface = Cast<IMeshInterface>(actor->selectedStock);

		// Remove the selection status of the component
		previousSelectedStockInterface->Execute_SetSelected(actor->selectedStock, false);
		
		// Collapse the component
		previousSelectedStockInterface->Execute_Collapse(actor->selectedStock);

	}

	// Set the selected component as this
	actor->selectedStock = this;

	// Hide the mesh of this componet
	Hide();

	// Expand this component in all its childs
	for (StockPlant const& substock : stock->getSubStock())
	{
		// Expand the substock only if is enable
		//if (substock.isEnable())
		{
			// Unique name for this component
			FString name(substock.getstrHash().c_str());

			// Create the subcomponent
			UAssemblyComponent* subAssembly = NewObject<UAssemblyComponent>(this, FName(*name)); // text("") can be just about anything.
			
			// Init the subcomponent
			subAssembly->init(actor, this, &substock);

			// Register the subcomponent into the tree
			subAssembly->RegisterComponent();

			// Attach the subcomponent to the this component
			subAssembly->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

			// Get the pose of the substock, and transform its measures to centimeter
			Vectorf3D pose = substock.getPosition().m_pos * 100;

			// Get the rotation of the substokc
			Vectorf3D rotation = substock.getPosition().m_rot;

			// Transform it to degrees
			rotation = rotation * (180.f / M_PI);

			// Set the relative position of the component
			subAssembly->SetRelativeLocationAndRotation(FVector(pose.x, -pose.y, pose.z), FRotator(rotation.y, -rotation.z, rotation.x), false, nullptr, ETeleportType::None);
			
			// Add the substock into the substocks list
			subStocks.Add(subAssembly);
		}
	}
}

void UAssemblyComponent::init(APlantActor* actorPointer, UMeshComponent* parentComponent, StockPlant const *stockEntry)
{
	// Initialization of the pointers
	this->actor = actorPointer;
	this->parent = parentComponent;
	this->stock = stockEntry;
	this->assembly = &this->stock->getAssembly();

	// Get the model name of the stock
	std::string modelname = assembly->getModel().getPathModel();

	// Erase the file type
	modelname.erase(modelname.end() - 4, modelname.end());

	// Create the model name for unreal
	modelname = "/Game/CashSorter/" + modelname + "." + modelname;

	// Transform the name into a FString
	FString meshName = FString(modelname.c_str());

	// Create the name of the material
	std::string materialStd = assembly->getModel().getColor() + assembly->getModel().getMaterial();
	materialStd = "/Game/Materials/ProterMaterials/" + materialStd + "." + materialStd;

	// Transform the name into a Fstring
	FString materialName = FString(materialStd.c_str());

	// Load the mesh from the library
	mesh = LoadObject<UStaticMesh>(NULL, *meshName, NULL, LOAD_None, NULL);

	// Load the material from the library
	material = LoadObject<UMaterialInterface>(NULL, *materialName, NULL, LOAD_None, NULL);

	if (mesh)
	{
		this->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		this->SetStaticMesh(mesh);
	}

	if (material)
	{
		DynMaterial = UMaterialInstanceDynamic::Create(material, this);
		DynMaterial->SetScalarParameterValue("Emissive", 0.f);
		this->SetMaterial(0, DynMaterial);
	}

	this->OnBeginCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseClicked);

	this->SetCustomDepthStencilValue(254);
}


