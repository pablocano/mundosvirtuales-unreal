// Fill out your copyright notice in the Description page of Project Settings.

#include "Plant.h"
#include "PlantActor.h"
#include "AssemblyComponent.h"
#include "WidgetInfoComponent.h"

#define M_PI           3.14159265358979323846  /* pi */

UAssemblyComponent::UAssemblyComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), selected(false), Space(EWidgetSpace::World), sizeWidget(1920, 1080), widgetInfoComponent(nullptr), pose()
{
	PrimaryComponentTick.bCanEverTick = true;
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

void UAssemblyComponent::CustomOnBeginMouseClicked(UPrimitiveComponent * TouchedComponent, FKey key)
{
	// If this component is already selected or if cannot be selected, do nothing
	if (selected || !stock->getCanBeSelected())
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

				if (actor->selectedStock)
				{
					// Access to the previous selected component
					IMeshInterface* previousSelectedStockInterface = Cast<IMeshInterface>(actor->selectedStock);

					// Remove the selection status of the component
					previousSelectedStockInterface->Execute_SetSelected(actor->selectedStock, false);

					// Collapse the component
					previousSelectedStockInterface->Execute_Collapse(actor->selectedStock);
				}

				actor->selectedStock = parent;

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

bool UAssemblyComponent::IsSelected_Implementation()
{
	return selected;
}

void UAssemblyComponent::SetSelected_Implementation(bool select)
{
	selected = select;

	if(widgetInfoComponent)
	{
		if (selected)
		{
			widgetInfoComponent->EnableWidget();
			UpdateWidgetPosition();
		}
		else
			widgetInfoComponent->DisableWidget();
	}
}

void UAssemblyComponent::Hide()
{
	SetVisibility(false);
	SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void UAssemblyComponent::Collapse_Implementation()
{
	if (selected)
	{
		// Iterate over all the substocks again
		for (UMeshComponent* substock : this->subStocks)
		{
			// Access to the subcomponent
			IMeshInterface* subStockInterface = Cast<IMeshInterface>(substock);

			// Show the mesh of the subcomponent
			subStockInterface->Execute_ShowComponent(substock);
		}

		return;
	}

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

	// Disable the widget of this component
	if (widgetInfoComponent && widgetInfoComponent->IsVisible())
		widgetInfoComponent->DisableWidget();
  
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

void UAssemblyComponent::UnregisterStock_Implementation()
{
	if (widgetInfoComponent)
	{
		widgetInfoComponent->DisableWidget();
		widgetInfoComponent->UnregisterComponent();
	}

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

void UAssemblyComponent::Expand_Implementation()
{
	ExpandStock();
}

void UAssemblyComponent::RemoveFocus_Implementation()
{
	SetBorders(NOTHING);

	if (widgetInfoComponent && widgetInfoComponent->IsVisible())
		widgetInfoComponent->DisableWidget();
}

void UAssemblyComponent::RemoveFocusChild_Implementation()
{
	if (focusedChild)
	{
		IMeshInterface* focusedChildInterface = Cast<IMeshInterface>(focusedChild);

		focusedChildInterface->Execute_RemoveFocus(focusedChild);
	}

	if(widgetInfoComponent && widgetInfoComponent->IsVisible())
		widgetInfoComponent->DisableWidget();
}

void UAssemblyComponent::SetFocusChild_Implementation(UMeshComponent * child)
{
	this->focusedChild = child;
}

FTransform UAssemblyComponent::GetGlobalPosition_Implementation()
{
	if (parent)
	{
		IMeshInterface* parentInterface = Cast<IMeshInterface>(parent);

		return pose * parentInterface->Execute_GetGlobalPosition(parent);
	}
	else 
	{
		FTransform actorTransform = actor->GetActorTransform();
		return pose* actorTransform;
	}
		
}

void UAssemblyComponent::ExpandStock()
{
	// Set this component as selected
	selected = true;

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
	if (actor->selectedStock)
	{
		// Access to the previous selected component
		IMeshInterface* previousSelectedStockInterface = Cast<IMeshInterface>(actor->selectedStock);

		// Remove the selection status of the component
		previousSelectedStockInterface->Execute_SetSelected(actor->selectedStock, false);

		// Remove its focus child, if it has one
		previousSelectedStockInterface->Execute_RemoveFocusChild(actor->selectedStock);
		
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
		if (substock.isEnable())
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

			// Set the relative position of the component
			subAssembly->SetRelativeLocationAndRotation(subAssembly->pose.GetTranslationV(), subAssembly->pose.GetRotationV(), false, nullptr, ETeleportType::None);
			
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

	// Get the pose of the stock, and transform its measures to centimeter
	Vectorf3D position = stock->getPosition().m_pos * 100;

	// Get the rotation of the substokc
	Vectorf3D rotation = stock->getPosition().m_rot;

	// Transform it to degrees
	rotation = rotation * (180.f / M_PI);

	// Set the pose of this component
	pose = FTransform(FRotator(rotation.y, -rotation.z, rotation.x), FVector(position.x, -position.y, position.z));

	// Get the model name of the stock
	std::string modelname = assembly->getModel().getPathModel();

	// Erase the file type
	modelname.erase(modelname.end() - 4, modelname.end());

	// Create the model name for unreal
	modelname = "/Game/CashSorter/" + modelname + "." + modelname;

	// Transform the name into a FString
	FString meshName = FString(modelname.c_str());

	// Create the name of the material
	std::string materialStd = "/Game/Materials/ProterMaterials/BlueMetal.BlueMetal";

	// Transform the name into a Fstring
	FString materialName = FString(materialStd.c_str());

	// Load the mesh from the library
	mesh = LoadObject<UStaticMesh>(NULL, *meshName, NULL, LOAD_None, NULL);

	// Load the material from the library
	material = LoadObject<UMaterialInterface>(NULL, *materialName, NULL, LOAD_None, NULL);

	// If the mesh was loaded
	if (mesh)
	{
		// Set the collitions as query only
		this->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);

		// Block all chanenels
		this->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

		// Set the mesh of this component
		this->SetStaticMesh(mesh);

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

		ToggleConstructionMode_Implementation();
	}

	this->OnBeginCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseOver);
	this->OnEndCursorOver.AddDynamic(this, &UAssemblyComponent::CustomOnEndMouseOver);
	this->OnClicked.AddDynamic(this, &UAssemblyComponent::CustomOnBeginMouseClicked);

	this->SetCustomDepthStencilValue(254);

	if (this->stock->getCanShowInfo())
	{
		// Generate Widget Info
		std::string widgetHashName = this->stock->getstrHash() + "widget";

		FString widgetName = widgetHashName.c_str();

		widgetInfoComponent = NewObject<UWidgetInfoComponent>(this, FName(*widgetName));
		widgetInfoComponent->SetVisibility(true);
		widgetInfoComponent->SetOnlyOwnerSee(false);
		
		widgetInfoComponent->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
		widgetInfoComponent->SetDrawSize(sizeWidget);
		widgetInfoComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		widgetInfoComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		widgetInfoComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		widgetInfoComponent->SetBackgroundColor(FLinearColor(.0f, .0f, .0f, .95f));
		widgetInfoComponent->SetBlendMode(EWidgetBlendMode::Transparent);
		widgetInfoComponent->SetWidgetSpace(Space);
		widgetInfoComponent->SetRelativeScale3D(FVector(0.03f, 0.03f, 0.03f));

		if (Space == EWidgetSpace::World)
		{
			widgetInfoComponent->SetTwoSided(true);
			SetBoolUProperty(widgetInfoComponent, TEXT("bReceiveHardwareInput"), true);  // Enable click
		}

		widgetInfoComponent->RegisterComponent();
	}
	else
		widgetInfoComponent = nullptr;
}

// Called when the game starts or when spawned
void UAssemblyComponent::BeginPlay()
{
	if (widgetInfoComponent)
	{
		widgetInfo = NewObject<UMyUserWidgetInfo>(this, UMyUserWidgetInfo::StaticClass());

		widgetInfo->SetStock(this->stock);

		USensor* sensor1 = NewObject<USensor>();
		sensor1->SetNameSensor("Temperatura");
		sensor1->SetTypeSensor(ETypeSensor::Temperature);
		Sensors.Add(sensor1);

		USensor* sensor2 = NewObject<USensor>();
		sensor2->SetNameSensor("Presion");
		sensor2->SetTypeSensor(ETypeSensor::Pressure);
		Sensors.Add(sensor2);

		USensor* sensor3 = NewObject<USensor>();
		sensor3->SetNameSensor("Flujo");
		sensor3->SetTypeSensor(ETypeSensor::Flow);
		Sensors.Add(sensor3);

		widgetInfo->SetSensors(Sensors);
		widgetInfoComponent->SetWidget(widgetInfo);
		widgetInfo->buttonOk->OnClicked.AddDynamic(this, &UAssemblyComponent::OnClickButtonOk);
		widgetInfoComponent->OnClicked.AddDynamic(this, &UAssemblyComponent::OnClickWidgetComponent);
		widgetInfoComponent->DisableWidget();
	}
}

// Called every frame
void UAssemblyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{

	if (widgetInfoComponent && widgetInfoComponent->IsVisible())
	{
		FVector cameraLocation = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager->GetCameraLocation();

		FVector widgetLocation = GetGlobalPosition_Implementation().GetLocation();

		float angle = (cameraLocation - widgetLocation).Rotation().Yaw;
		
		widgetInfoComponent->SetWorldRotation(FRotator(0.f, angle , 0.f));
		widgetInfo->UpdateWidgetSensors(DeltaTime);
	}
}

void UAssemblyComponent::UpdateWidgetPosition()
{
	widgetInfoComponent->bAbsoluteLocation = 1;
	widgetInfoComponent->bAbsoluteRotation = 1;

	FTransform componentTransform = GetGlobalPosition_Implementation();

	FVector componentPosition = componentTransform.GetTranslationV();
	componentPosition.Z = 160;

	widgetInfoComponent->SetWorldLocation(componentPosition);
	widgetInfoComponent->SetWorldRotation(FRotator(0.f, 0.f, 0.f));
}

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
		actor->selectedStock = parent;

		// Disable the parent widget
		parentInterface->Execute_RemoveFocusChild(parent);

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
	}
}

void UAssemblyComponent::OnClickWidgetComponent(UPrimitiveComponent* pComponent, FKey inKey)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, TEXT("Click on WidgetComponent"));
	}
}

void UAssemblyComponent::ToggleConstructionMode_Implementation()
{
	if (actor->constructionMode)
	{
		for (int i = 0; i < DynMaterials.Num(); i++)
		{
			switch (stock->getState())
			{
			case StateStock::INSTALLED:
				DynMaterials[i]->SetVectorParameterValue("BaseColor", FLinearColor::Green);
				break;

			case StateStock::CONSTRUCTION:
				DynMaterials[i]->SetVectorParameterValue("BaseColor", FLinearColor(0.f,1.f,1.f));
				break;

			case StateStock::WAREHOUSE:
				DynMaterials[i]->SetVectorParameterValue("BaseColor", FLinearColor::Blue);
				break;

			case StateStock::PROCESS_OF_PURCHASE:
				DynMaterials[i]->SetVectorParameterValue("BaseColor", FLinearColor::Yellow);
				break;

			case StateStock::NEED_BUY:
				DynMaterials[i]->SetVectorParameterValue("BaseColor", FLinearColor::Red);
				break;

			default:
				DynMaterials[i]->SetVectorParameterValue("BaseColor", FLinearColor::Black);
				break;
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

	for (const auto& subComponent : subStocks)
	{
		// Access to the subcomponent
		IMeshInterface* subStockInterface = Cast<IMeshInterface>(subComponent);

		subStockInterface->Execute_ToggleConstructionMode(subComponent);
	}
}


