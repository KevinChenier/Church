// Fill out your copyright notice in the Description page of Project Settings.


#include "LOG735HUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ALOG735HUD::ALOG735HUD()
{
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("WidgetBlueprint'/Game/ThirdPersonCPP/Blueprints/HUD/Main_HUD'"));
	HUDWidgetClass = HealthBarObj.Class;
}


void ALOG735HUD::DrawHUD()
{
	Super::DrawHUD();
}

void ALOG735HUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}
