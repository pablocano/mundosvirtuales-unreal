// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"
#include "AssemblyComponent.h"
#include "WidgetInfoComponent.h"
#include "AnimatedAssemblyComponent.h"

#define M_PI           3.14159265358979323846  /* pi */

UAssemblyComponent::UAssemblyComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), selected(false), Space(EWidgetSpace::World), sizeWidget(1920, 1080), widgetInfoComponent(nullptr), pose()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Initialize the component
void UAssemblyComponent::Init(APlantActor* actorPointer, UMeshComponent* parentComponent, StockPlant const *stockEntry)
{
	// Initialization of the pointers
	this->actor = actorPointer;
	this->parent = parentComponent;
	this->stock = stockEntry;
	this->assembly = &this->stock->getAssembly();

	// Set the pose of this component using the position in the stock and transforming it in centimeters and degrees
	Vectorf3D position = stock->getPosition().m_pos * 100;
	Vectorf3D rotation = stock->getPosition().m_rot;
	rotation = rotation * (180.f / M_PI);
	pose = FTransform(FRotator(rotation.y, -rotation.z, rotation.x), FVector(position.x, -position.y, position.z));

	// Get the mesh of this component using the model of the stock
	std::string modelname = assembly->getModel().getPathModel();
	modelname.erase(modelname.end() - 4, modelname.end());
	modelname = "/Game/CashSorter/" + modelname + "." + modelname;
	FString meshName = FString(modelname.c_str());
	mesh = LoadObject<UStaticMesh>(NULL, *meshName, NULL, LOAD_None, NULL);

	// If the mesh wasnt loaded, stop the initialization
	if (!mesh)
		return;

	// Set the mesh of this component
	this->SetStaticMesh(mesh);

	// Set the collition enabled to respond to click
	this->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// Set all the materials of this component as a dynamic material
	for (int i = 0; i < this->GetNumMaterials(); i++)
	{
		// Create a dynamic material using the base material of this slpt
		UMaterialInstanceDynamic *dynMaterial = CreateAndSetMaterialInstanceDynamic(i);

		// Add a pointer of this material in a list
		DynMaterials.Add(dynMaterial);

		// Save the original color
		FLinearColor color;
		dynMaterial->GetVectorParameterValue("BaseColor", color);
		originalColors.Add(color);

		// Set the material of this slot with the dynamic material
		this->SetMaterial(i, dynMaterial);
	}

	// Process the visualization mode
	ProcessVisualizationMode_Implementation();

	// Set the trigger for the events of this component
	this->OnBeginCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseClicked);

	// Set the initial state of the borders
	SetBorders(NOTHING);

	// Set the widget if is necessary
	if (this->stock->getCanShowInfo())
	{
		// Generate Widget component
		std::string widgetHashName = this->stock->getstrHash() + "widget";
		FString widgetName = widgetHashName.c_str();
		widgetInfoComponent = NewObject<UWidgetInfoComponent>(this, FName(*widgetName));

		// Initialize widget component
		widgetInfoComponent->SetVisibility(true);
		widgetInfoComponent->SetOnlyOwnerSee(false);
		widgetInfoComponent->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		widgetInfoComponent->SetDrawSize(sizeWidget);

		// Set collision to respond to clicks
		widgetInfoComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		widgetInfoComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		widgetInfoComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

		// Set the style of the widget
		widgetInfoComponent->SetBackgroundColor(FLinearColor(.0f, .0f, .0f, .95f));
		widgetInfoComponent->SetBlendMode(EWidgetBlendMode::Transparent);
		widgetInfoComponent->SetWidgetSpace(Space);

		// Set the scale of the widget
		widgetInfoComponent->SetRelativeScale3D(FVector(0.03f, 0.03f, 0.03f));


		if (Space == EWidgetSpace::World)
		{
			widgetInfoComponent->SetTwoSided(true);
			SetBoolUProperty(widgetInfoComponent, TEXT("bReceiveHardwareInput"), true);  // Enable click
		}

		// Register the component
		widgetInfoComponent->RegisterComponent();
	}
	else
		widgetInfoComponent = nullptr;
}

// Called when the game starts or when spawned
void UAssemblyComponent::BeginPlay()
{
	// Initialize the widget component if needed
	if (widgetInfoComponent)
	{
		// Create the actual widget
		widgetInfo = NewObject<UMyUserWidgetInfo>(this, UMyUserWidgetInfo::StaticClass());
		widgetInfo->SetStock(this->stock);

		// Set the sensor corresponding to each machine
		if (assembly->getInfo().getName() == "Glory Sorter")
		{
			USensor* sensor1 = NewObject<USensor>();
			sensor1->SetNameSensor("Banknotes processed");
			sensor1->SetTypeSensor(ETypeSensor::Default);
			Sensors.Add(sensor1);

			USensor* sensor2 = NewObject<USensor>();
			sensor2->SetNameSensor("Banknotes rejected");
			sensor2->SetTypeSensor(ETypeSensor::Pressure);
			Sensors.Add(sensor2);

			widgetInfo->SetSensors(Sensors);
		}
		else if (assembly->getInfo().getName() == "Banding Machine")
		{
			USensor* sensor1 = NewObject<USensor>();
			sensor1->SetNameSensor("Banknotes boundled");
			sensor1->SetTypeSensor(ETypeSensor::Flow);
			Sensors.Add(sensor1);

			widgetInfo->SetSensors(Sensors);
		}

		// Set the widget into the widget component
		widgetInfoComponent->SetWidget(widgetInfo);

		// Set the events triggered by the clicks
		widgetInfo->buttonOk->OnClicked.AddDynamic(this, &UAssemblyComponent::OnClickButtonOk);
		widgetInfoComponent->OnClicked.AddDynamic(this, &UAssemblyComponent::OnClickWidgetComponent);

		// Start with the widget hidden
		widgetInfoComponent->DisableWidget();
	}
}

// Called every frame
void UAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	// If this component has a widget component
	if (widgetInfoComponent)
	{
		// Set the widget to autowrap the text
		widgetInfo->textInfo->SetAutoWrapText(true);

		// If the widget is visible
		if (widgetInfoComponent->IsVisible())
		{
			// Set the angle of the widget to point always to the camera
			FVector cameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();
			FVector widgetLocation = GetGlobalPosition_Implementation().GetLocation();
			float angle = (cameraLocation - widgetLocation).Rotation().Yaw;
			widgetInfoComponent->SetWorldRotation(FRotator(0.f, angle, 0.f));
		}

		// Update the sensors associated to the widgets
		widgetInfo->UpdateWidgetSensors(DeltaTime);
	}
}

// Get the selection status of this component
bool UAssemblyComponent::IsSelected_Implementation()
{
	return selected;
}

// Set the selection status of this component
void UAssemblyComponent::SetSelected_Implementation(bool select)
{
	// Set the selection
	selected = select;

	// If this component has a widget component
	if (widgetInfoComponent)
	{
		// If this component is selected, show the widget
		if (selected)
		{
			widgetInfoComponent->EnableWidget();
			UpdateWidgetPosition();
		}
		else
			widgetInfoComponent->DisableWidget();
	}
}

// Collapse this component and all its parents until reach the selected layer of the tree
void UAssemblyComponent::Collapse_Implementation()
{
	if (selected)
	{
		// Iterate over all the substocks again
		for (UMeshComponent* subComponent : this->subComponents)
		{
			// Access to the subcomponent
			IMeshInterface* subComponentInterface = Cast<IMeshInterface>(subComponent);

			// Show the mesh of the subcomponent
			subComponentInterface->Execute_ShowComponent(subComponent);
		}

		return;
	}

	// If one of the subcomponent of this component is selected, stop the collapse
	for (UMeshComponent* subComponent : this->subComponents)
	{
		// Access to the subcomponent
		IMeshInterface* subComponentInterface = Cast<IMeshInterface>(subComponent);

		// If the subcomponent is selected, it means that this layer must be shown
		if (subComponentInterface->Execute_IsSelected(subComponent))
		{
			// Iterate over all the subComponents again
			for (UMeshComponent* subComponent2 : this->subComponents)
			{
				// Access to the subcomponent
				IMeshInterface* subComponentInterface2 = Cast<IMeshInterface>(subComponent2);

				// If the subcomponent is not selected
				if (!subComponentInterface2->Execute_IsSelected(subComponent2))
				{
					// Show the mesh of the subcomponent
					subComponentInterface2->Execute_ShowComponent(subComponent2);
				}
			}

			// End the collapse call
			return;
		}
	}

	//If this is not the layer where the selected component is, remove all childs
	for (UMeshComponent* subComponent : this->subComponents)
	{
		// Access to the subcomponent
		IMeshInterface* subComponentInterface = Cast<IMeshInterface>(subComponent);

		// Remove it from the tree
		subComponentInterface->Execute_UnregisterStock(subComponent);
	}

	// Disable the widget of this component
	if (widgetInfoComponent && widgetInfoComponent->IsVisible())
		widgetInfoComponent->DisableWidget();

	// Delete all the subComponents from the list
	this->subComponents.Empty();

	// Access to the parent
	IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

	// Collapse the parent
	parentInterface->Execute_Collapse(parent);
}

// Show the mesh of this component
void UAssemblyComponent::ShowComponent_Implementation()
{
	// Initialize this component as it was new
	SetVisibility(true);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	SetBorders(NOTHING);

	// If the material can be accessed
	for (int i = 0; i < DynMaterials.Num(); i++)
	{
		// Get the color from the material
		FLinearColor color;
		DynMaterials[i]->GetVectorParameterValue("BaseColor", color);

		// Toogle the emissivity accoding to the hover state
		float emissive = 0.f;

		// Set the emissive color
		color = color * emissive;

		// Set the emissivity of the material
		DynMaterials[i]->SetVectorParameterValue("Emissive", color);
	}
}

// Unregister this component from the tree
void UAssemblyComponent::UnregisterStock_Implementation()
{
	// Remove the widget if exist
	if (widgetInfoComponent)
	{
		widgetInfoComponent->DisableWidget();
		widgetInfoComponent->UnregisterComponent();
	}

	// Unregister this components
	this->UnregisterComponent();
}

// Expand this component
void UAssemblyComponent::Expand_Implementation()
{
	ExpandComponent();
}

// Remove the focus of this component
void UAssemblyComponent::RemoveFocus_Implementation()
{
	// Set the borders to nothing
	SetBorders(NOTHING);

	// Disable the widget
	if (widgetInfoComponent && widgetInfoComponent->IsVisible())
		widgetInfoComponent->DisableWidget();
}

// Remove the focus of the child
void UAssemblyComponent::RemoveFocusChild_Implementation()
{
	// If there is a focused child
	if (focusedChild)
	{
		// Remove its focus
		IMeshInterface* focusedChildInterface = Cast<IMeshInterface>(focusedChild);
		focusedChildInterface->Execute_RemoveFocus(focusedChild);
	}

	// Disable the widget
	if (widgetInfoComponent && widgetInfoComponent->IsVisible())
		widgetInfoComponent->DisableWidget();
}

// Set the component as the focused child
void UAssemblyComponent::SetFocusChild_Implementation(UMeshComponent * child)
{
	this->focusedChild = child;
}

// Get the global pose of this component by using the parent position
FTransform UAssemblyComponent::GetGlobalPosition_Implementation()
{
	// If there is a parent
	if (parent)
	{
		// Get the parent position
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);
		return pose * parentInterface->Execute_GetGlobalPosition(parent);
	}
	else
	{
		// Use the actor position, because this is the root component
		FTransform actorTransform = actor->GetActorTransform();
		return pose* actorTransform;
	}

}

// Process the visualization mode
void UAssemblyComponent::ProcessVisualizationMode_Implementation()
{
	// If the visualization is in construction mode
	if (actor->constructionMode)
	{
		// If some of the states is higlighted
		if (actor->highlightState != StateStock::NONE_STATE)
		{
			// Iterate over all materials of the mesh of this component
			for (int i = 0; i < DynMaterials.Num(); i++)
			{
				// If the state of this component is highlighted
				if (stock->getState() == actor->highlightState)
				{
					// Show the state of this component
					DynMaterials[i]->SetVectorParameterValue("BaseColor", APlantActor::StateColorArray[actor->highlightState]);
				}
				else
				{
					// Hide the state of this component
					DynMaterials[i]->SetVectorParameterValue("BaseColor", FLinearColor::Gray);
				}
			}
		}
		else
		{
			// Show the state of the component
			for (int i = 0; i < DynMaterials.Num(); i++)
			{
				DynMaterials[i]->SetVectorParameterValue("BaseColor", APlantActor::StateColorArray[stock->getState()]);
			}
		}
	}
	else
	{
		for (int i = 0; i < DynMaterials.Num(); i++)
		{
			// Set the color of the material with its original color
			DynMaterials[i]->SetVectorParameterValue("BaseColor", originalColors[i]);
		}
	}

	// Recursive call to process the visualization mode
	for (const auto& subComponent : subComponents)
	{
		// Access to the subcomponent
		IMeshInterface* subComponentInterface = Cast<IMeshInterface>(subComponent);

		// Process the visualization mode
		subComponentInterface->Execute_ProcessVisualizationMode(subComponent);
	}
}

// Called when mouse is over this component
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
			if (!selected && stock->getCanBeSelected())
			{
				// Set borders as hover
				SetBorders(HOVER);
				return;
			}
		}
	}

	// If this component is not selected, set hover
	if (!selected && stock->getCanBeSelected())
		SetHover();
}

// Called when mouse is stop been over this component
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
			if (!selected && stock->getCanBeSelected())
			{
				// Set borders to nothing
				SetBorders(NOTHING);
				return;
			}
		}
	}

	// If this component is not selected, remove hover
	if (!selected && stock->getCanBeSelected())
		SetHover(false);
}

// Set the hover status of this component
void UAssemblyComponent::SetHover(bool hover)
{
	// If the material can be accessed
	for(int i = 0; i < DynMaterials.Num(); i++)
	{
		// Get the color from the material
		FLinearColor color;
		DynMaterials[i]->GetVectorParameterValue("BaseColor", color);

		// Toogle the emissivity accoding to the hover state
		float emissive = hover ? 0.5f : 0.f;

		// Set the emissive color
		color = color * emissive;

		// Set the emissivity of the material
		DynMaterials[i]->SetVectorParameterValue("Emissive", color);
	}
}

// Event triggered when the component is clicked
void UAssemblyComponent::CustomOnBeginMouseClicked(UPrimitiveComponent * TouchedComponent, FKey key)
{
	// If this component is already selected or if cannot be selected, do nothing
	if (selected || !stock->getCanBeSelected())
		return;

	// If this stock has substocks, expand this component
	if (!stock->getSubStock().empty())
	{
		// Expand this component
		ExpandComponent();
	}
	// Or, if this component is not focused
	else if(borderStatus != FOCUS)
	{
		// If this component has a parent
		if (parent)
		{
			// Access to the parent
			IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

			if (parentInterface->Execute_IsSelected(parent))
			{
				// Remove the focused child of the parent of this component
				parentInterface->Execute_RemoveFocusChild(parent);

				// Set this component as focused
				parentInterface->Execute_SetFocusChild(parent, this);
			}
			else
			{
				parentInterface->Execute_SetSelected(parent, true);

				if (actor->SelectedComponent)
				{
					// Access to the previous selected component
					IMeshInterface* previousSelectedComponentInterface = Cast<IMeshInterface>(actor->SelectedComponent);

					// Remove the selection status of the component
					previousSelectedComponentInterface->Execute_SetSelected(actor->SelectedComponent, false);

					// Collapse the component
					previousSelectedComponentInterface->Execute_Collapse(actor->SelectedComponent);
				}

				actor->SelectedComponent = parent;

				// Remove the focused child of the parent of this component
				parentInterface->Execute_RemoveFocusChild(parent);

				// Set this component as focused
				parentInterface->Execute_SetFocusChild(parent, this);
			}
		}

		// Set the borders as focused
		SetBorders(FOCUS);
	}
}

// Hide this component
void UAssemblyComponent::Hide()
{
	SetVisibility(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Set the borders accordingly to the status of thie component
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
		if (widgetInfoComponent)
		{
			widgetInfoComponent->EnableWidget();
			UpdateWidgetPosition();
		}
		this->SetCustomDepthStencilValue(254);
		this->SetRenderCustomDepth(true);
		break;

	default:
		break;
	}
}

// Expand this component
void UAssemblyComponent::ExpandComponent()
{
	// Set this component as selected
	selected = true;

	// If this component has a widget, show it
	if (widgetInfoComponent)
	{
		widgetInfoComponent->EnableWidget();
		UpdateWidgetPosition();
	}

	// If this component has a parent, remove the selection state of the parent
	if (parent)
	{
		// Access to the parent
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

		// Remove the selection status
		parentInterface->Execute_SetSelected(parent, false);
	}

	// If there was a previous selected component, collapse the tree
	if (actor->SelectedComponent)
	{
		// Access to the previous selected component
		IMeshInterface* previousSelectedComponentInterface = Cast<IMeshInterface>(actor->SelectedComponent);

		// Remove the selection status of the component
		previousSelectedComponentInterface->Execute_SetSelected(actor->SelectedComponent, false);

		// Remove its focus child, if it has one
		previousSelectedComponentInterface->Execute_RemoveFocusChild(actor->SelectedComponent);
		
		// Collapse the component
		previousSelectedComponentInterface->Execute_Collapse(actor->SelectedComponent);

	}

	// Set the selected component as this
	actor->SelectedComponent = this;

	// Hide the mesh of this componet
	Hide();

	// Expand this component in all its childs
	for (StockPlant const& substock : stock->getSubStock())
	{
		// Expand the substock only if is enable
		if (substock.isEnable())
		{
			if (substock.getAssembly().getModel().isAnimated())
			{
				// Create the subcomponent with a unique name using the hash
				FString name(substock.getstrHash().c_str());
				UAnimatedAssemblyComponent* subAssembly = NewObject<UAnimatedAssemblyComponent>(this, FName(*name)); // text("") can be just about anything.

																									 // Init the subcomponent
				subAssembly->Init(actor, this, &substock);

				// Register and attach the subcomponent into the tree
				subAssembly->RegisterComponent();
				subAssembly->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

				// Set the relative position of the component
				subAssembly->SetRelativeLocationAndRotation(subAssembly->GetPose().GetTranslation(), subAssembly->GetPose().GetRotation(), false, nullptr, ETeleportType::None);

				// Add the subComponent into the subComponents list
				subComponents.Add(subAssembly);
			}
			else
			{
				// Create the subcomponent with a unique name using the hash
				FString name(substock.getstrHash().c_str());
				UAssemblyComponent* subAssembly = NewObject<UAssemblyComponent>(this, FName(*name)); // text("") can be just about anything.

																									 // Init the subcomponent
				subAssembly->Init(actor, this, &substock);

				// Register and attach the subcomponent into the tree
				subAssembly->RegisterComponent();
				subAssembly->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform);

				// Set the relative position of the component
				subAssembly->SetRelativeLocationAndRotation(subAssembly->GetPose().GetTranslation(), subAssembly->GetPose().GetRotation(), false, nullptr, ETeleportType::None);

				// Add the subComponent into the subComponents list
				subComponents.Add(subAssembly);
			}
		}
	}
}

// Updates the position of the widget
void UAssemblyComponent::UpdateWidgetPosition()
{
	// Set the widget position as absolute
	widgetInfoComponent->bAbsoluteLocation = 1;
	widgetInfoComponent->bAbsoluteRotation = 1;

	// Get the global pose of thie component
	FTransform componentTransform = GetGlobalPosition_Implementation();

	// Set the location and rotation of the widget
	FVector componentPosition = componentTransform.GetTranslation();
	componentPosition.Z = 160;
	widgetInfoComponent->SetWorldLocation(componentPosition);
	widgetInfoComponent->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
}

// Event triggered with the ok button of the widget
void UAssemblyComponent::OnClickButtonOk()
{
	if (selected && parent)
	{
		// Remove the selection
		selected = false;

		// Access to the parent
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

		// Select the parent
		parentInterface->Execute_SetSelected(parent, true);

		// Remove this component as selected, and set the parent
		actor->SelectedComponent = parent;

		// Collapse this component
		this->Collapse_Implementation();

		return;
	}
	else if(parent)
	{
		// Access to the parent
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

		// Disable the parent widget
		parentInterface->Execute_RemoveFocusChild(parent);

		// Show the widget of the parent
		parentInterface->Execute_SetSelected(parent, true);
	}
}

// Event triggered when the widget is clicked
void UAssemblyComponent::OnClickWidgetComponent(UPrimitiveComponent* pComponent, FKey inKey)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Click on WidgetComponent"));
	}
}


