
#include "../../Flare.h"
#include "FlareSpacecraftInfo.h"
#include "FlareCargoInfo.h"
#include "../../Game/FlareTradeRoute.h"
#include "../../Economy/FlareCargoBay.h"
#include "../../Player/FlarePlayerController.h"

#define LOCTEXT_NAMESPACE "FlareSpacecraftInfo"


/*----------------------------------------------------
	Construct
----------------------------------------------------*/

void SFlareSpacecraftInfo::Construct(const FArguments& InArgs)
{
	// Data
	PC = InArgs._Player;
	OwnerWidget = InArgs._OwnerWidget->AsShared();
	NoInspect = InArgs._NoInspect;
	Minimized = InArgs._Minimized;
	OnRemoved = InArgs._OnRemoved;
	AFlareGame* Game = InArgs._Player->GetGame();
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();
	
	// Create the layout
	ChildSlot
	.VAlign(VAlign_Top)
	.HAlign(HAlign_Left)
	[
		SNew(SVerticalBox)

		// Header container
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)
			.WidthOverride(Theme.ContentWidth)
			[
				SNew(SHorizontalBox)
			
				// Data block
				+ SHorizontalBox::Slot()
				[
					SNew(SVerticalBox)

					// Main line
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)

						// Class icon
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.VAlign(VAlign_Center)
						[
							SNew(SImage).Image(this, &SFlareSpacecraftInfo::GetClassIcon)
						]

						// Ship name
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(FMargin(8))
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(this, &SFlareSpacecraftInfo::GetName)
							.TextStyle(&Theme.NameFont)
							.ColorAndOpacity(this, &SFlareSpacecraftInfo::GetTextColor)
						]

						// Ship class
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(FMargin(8))
						.VAlign(VAlign_Center)
						[
							SNew(STextBlock)
							.Text(this, &SFlareSpacecraftInfo::GetDescription)
							.TextStyle(&Theme.TextFont)
						]

						// Status
						+ SHorizontalBox::Slot()
						.HAlign(HAlign_Right)
						[
							SAssignNew(ShipStatus, SFlareShipStatus)
						]
					]

					// Company line
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SNew(SHorizontalBox)

						// Company flag
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(FMargin(0, 8))
						[
							SAssignNew(CompanyFlag, SFlareCompanyFlag)
							.Player(InArgs._Player)
							.Visibility(this, &SFlareSpacecraftInfo::GetCompanyFlagVisibility)
						]

						// Company info
						+ SHorizontalBox::Slot()
						.AutoWidth()
						.Padding(FMargin(8))
						[
							SNew(STextBlock)
							.Text(this, &SFlareSpacecraftInfo::GetSpacecraftInfo)
							.TextStyle(&Theme.TextFont)
							.Visibility(this, &SFlareSpacecraftInfo::GetSpacecraftInfoVisibility)
						]
					]

					// Captures
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						SAssignNew(CaptureBox, SVerticalBox)
					]

					// Cargo bay block
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(FMargin(6))
					.HAlign(HAlign_Left)
					[
						SAssignNew(CargoBay, SHorizontalBox)
					]

					// Buttons : Generic
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(FMargin(8, 8, 8, 0))
					[
						SNew(SHorizontalBox)

						// Inspect
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(InspectButton, SFlareButton)
							.Text(LOCTEXT("Inspect", "DETAILS"))
							.HelpText(LOCTEXT("InspectInfo", "Take a closer look at this spacecraft"))
							.OnClicked(this, &SFlareSpacecraftInfo::OnInspect)
							.Width(4)
						]

						// Upgrade
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(UpgradeButton, SFlareButton)
							.Text(LOCTEXT("Upgrade", "UPGRADE"))
							.OnClicked(this, &SFlareSpacecraftInfo::OnUpgrade)
							.Width(4)
						]

						// Fly this ship
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(FlyButton, SFlareButton)
							.Text(LOCTEXT("ShipFly", "FLY"))
							.OnClicked(this, &SFlareSpacecraftInfo::OnFly)
							.Width(4)
						]
					]

					// Buttons : advanced
					+ SVerticalBox::Slot()
					.AutoHeight()
					.Padding(FMargin(8, 0, 8, 0))
					[
						SNew(SHorizontalBox)

						// Trade
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(TradeButton, SFlareButton)
							.Text(LOCTEXT("Trade", "TRADE"))
							.OnClicked(this, &SFlareSpacecraftInfo::OnTrade)
							.Width(4)
						]
			
						// Dock here
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(DockButton, SFlareButton)
							.Text(LOCTEXT("Dock", "DOCK HERE"))
							.HelpText(LOCTEXT("DockInfo", "Try to dock at this spacecraft"))
							.OnClicked(this, &SFlareSpacecraftInfo::OnDockAt)
							.Width(4)
						]

						// Undock
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(UndockButton, SFlareButton)
							.Text(LOCTEXT("Undock", "UNDOCK"))
							.HelpText(LOCTEXT("UndockInfo", "Undock from this spacecraft and go back to flying the ship"))
							.OnClicked(this, &SFlareSpacecraftInfo::OnUndock)
							.Width(4)
						]

						// Scrap
						+ SHorizontalBox::Slot()
						.AutoWidth()
						[
							SAssignNew(ScrapButton, SFlareButton)
							.Text(LOCTEXT("Scrap", "SCRAP"))
							.OnClicked(this, &SFlareSpacecraftInfo::OnScrap)
							.Width(4)
						]
					]
				]

				// Icon
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Top)
				[
					SNew(SImage).Image(this, &SFlareSpacecraftInfo::GetIcon)
					.Visibility(InArgs._NoInspect ? EVisibility::Hidden : EVisibility::Visible)
				]
			]
		]
	];

	// Setup
	if (InArgs._Spacecraft)
	{
		SetSpacecraft(InArgs._Spacecraft);
	}
	if (InArgs._Visible && TargetSpacecraft)
	{
		Show();
	}
	else
	{
		Hide();
	}
}


/*----------------------------------------------------
	Interaction
----------------------------------------------------*/

void SFlareSpacecraftInfo::SetSpacecraft(UFlareSimulatedSpacecraft* Target)
{
	TargetSpacecraft = Target;
	ShipStatus->SetTargetShip(Target);

	// Get the save data info to retrieve the class data
	if (TargetSpacecraft && PC)
	{
		// Setup basic info
		CompanyFlag->SetCompany(TargetSpacecraft->GetCompany());
		TargetName = FText::FromName(TargetSpacecraft->GetImmatriculation());
		FFlareSpacecraftSave* SaveData = TargetSpacecraft->Save();
		if (SaveData)
		{
			TargetSpacecraftDesc = PC->GetGame()->GetSpacecraftCatalog()->Get(SaveData->Identifier);
		}

		// Fill the cargo bay
		CargoBay->ClearChildren();
		UFlareCompany* Company = TargetSpacecraft->GetCompany();
		if (Company->GetPlayerWarState() != EFlareHostility::Hostile)
		{
			for (uint32 CargoIndex = 0; CargoIndex < TargetSpacecraft->GetCargoBay()->GetSlotCount() ; CargoIndex++)
			{
				CargoBay->AddSlot()
				[
					SNew(SFlareCargoInfo)
					.Spacecraft(TargetSpacecraft)
					.CargoIndex(CargoIndex)
				];
			}
		}
	}
}

void SFlareSpacecraftInfo::SetNoInspect(bool NewState)
{
	NoInspect = NewState;
}

void SFlareSpacecraftInfo::SetMinimized(bool NewState)
{
	Minimized = NewState;

	if (GetVisibility() == EVisibility::Visible)
	{
		Show();
	}
}

void SFlareSpacecraftInfo::Show()
{
	SetVisibility(EVisibility::Visible);

	if (Minimized)
	{
		CargoBay->SetVisibility(EVisibility::Collapsed);

		InspectButton->SetVisibility(EVisibility::Collapsed);
		UpgradeButton->SetVisibility(EVisibility::Collapsed);
		TradeButton->SetVisibility(EVisibility::Collapsed);
		FlyButton->SetVisibility(EVisibility::Collapsed);
		DockButton->SetVisibility(EVisibility::Collapsed);
		UndockButton->SetVisibility(EVisibility::Collapsed);
		ScrapButton->SetVisibility(EVisibility::Collapsed);
	}
	else if (TargetSpacecraft && TargetSpacecraft->IsValidLowLevel())
	{
		// Useful data
		UFlareSimulatedSpacecraft* PlayerShip = PC->GetPlayerShip();
		AFlareSpacecraft* DockedStation = NULL;
		AFlareSpacecraft* ActiveTargetSpacecraft = NULL;
		if (TargetSpacecraft->IsActive())
		{
			ActiveTargetSpacecraft = TargetSpacecraft->GetActive();
			DockedStation = ActiveTargetSpacecraft->GetNavigationSystem()->GetDockStation();
		}

		// Helpers
		bool Owned = TargetSpacecraft->GetCompany()->GetPlayerHostility() == EFlareHostility::Owned;
		bool OwnedAndNotSelf = Owned && TargetSpacecraft != PlayerShip;
		bool IsFriendly = TargetSpacecraft->GetCompany()->GetPlayerWarState() >= EFlareHostility::Neutral;
		bool IsOutsidePlayerFleet = (TargetSpacecraft->GetCurrentFleet() != PlayerShip->GetCurrentFleet()) || !ActiveTargetSpacecraft;
		bool IsDocked = ActiveTargetSpacecraft && (DockedStation || ActiveTargetSpacecraft->GetDockingSystem()->IsDockedShip(PlayerShip->GetActive()));
		bool IsStation = TargetSpacecraft->IsStation();
		bool IsCargo = (TargetSpacecraft->GetDescription()->CargoBayCount > 0) && !IsStation;
		
		// Permissions
		bool CanDock =     !IsDocked && IsFriendly && ActiveTargetSpacecraft && ActiveTargetSpacecraft->GetDockingSystem()->HasCompatibleDock(PlayerShip->GetActive());
		bool CanUpgradeDistant = IsOutsidePlayerFleet && TargetSpacecraft->GetCurrentSector()->CanUpgrade(TargetSpacecraft->GetCompany());
		bool CanUpgradeDocked = ActiveTargetSpacecraft && DockedStation && DockedStation->GetParent()->HasCapability(EFlareSpacecraftCapability::Upgrade);
		bool CanUpgrade =  CanUpgradeDistant || CanUpgradeDocked;
		bool CanTrade =    IsCargo && (IsDocked || IsOutsidePlayerFleet);
		bool CanScrap =    CanUpgrade && OwnedAndNotSelf;
		
		// Is a battle in progress ?
		if (TargetSpacecraft->GetCurrentSector())
		{
			if (TargetSpacecraft->GetCurrentSector()->IsPlayerBattleInProgress())
			{
				CanTrade = false;
				CanUpgrade = false;
				CanScrap = false;
			}
			if (TargetSpacecraft->GetDamageSystem()->IsUncontrollable())
			{
				CanTrade = false;
				CanUpgrade = false;
			}
		}
		FLOGV("SFlareSpacecraftInfo::Show : CanDock = %d CanUpgrade = %d CanTrade = %d CanScrap = %d", CanDock, CanUpgrade, CanTrade, CanScrap);

		// Button states : hide stuff that can never make sense (flying stations etc), disable other states after that
		CargoBay->SetVisibility(CargoBay->NumSlots() > 0 ? EVisibility::Visible : EVisibility::Collapsed);
		
		// Upper line
		InspectButton->SetVisibility(NoInspect ?           EVisibility::Collapsed : EVisibility::Visible);
		UpgradeButton->SetVisibility(Owned && !IsStation ? EVisibility::Visible : EVisibility::Collapsed);
		FlyButton->SetVisibility(!Owned || IsStation ?     EVisibility::Collapsed : EVisibility::Visible);

		// Second line
		TradeButton->SetVisibility(Owned && IsCargo ?                            EVisibility::Visible : EVisibility::Collapsed);
		DockButton->SetVisibility(CanDock ?                                      EVisibility::Visible : EVisibility::Collapsed);
		UndockButton->SetVisibility(Owned && IsDocked && !IsOutsidePlayerFleet ? EVisibility::Visible : EVisibility::Collapsed);
		ScrapButton->SetVisibility(Owned && !IsStation ?                         EVisibility::Visible : EVisibility::Collapsed);

		// Flyable ships : disable when not flyable
		FText Reason;
		if (!TargetSpacecraft->CanBeFlown(Reason))
		{
			FlyButton->SetHelpText(Reason);
			FlyButton->SetDisabled(true);
		}
		else if (TargetSpacecraft == PlayerShip)
		{
			FlyButton->SetHelpText(LOCTEXT("CantFlySelfInfo", "You are already flying this ship"));
			FlyButton->SetDisabled(true);
		}
		else
		{
			FlyButton->SetHelpText(LOCTEXT("ShipFlyInfo", "Take command of this spacecraft"));
			FlyButton->SetDisabled(false);
		}

		// Can undock
		if (TargetSpacecraft->IsTrading())
		{
			UndockButton->SetHelpText(LOCTEXT("ShipTradingUndockInfo", "Trading operations are blocking this ship for a day"));
			UndockButton->SetDisabled(true);
		}
		else
		{
			UndockButton->SetHelpText(LOCTEXT("ShipUndockInfo", "Undock the ship and leave the station"));
			UndockButton->SetDisabled(false);
		}

		// Disable trade while flying unless docked
		if (CanTrade && !TargetSpacecraft->IsTrading())
		{
			TradeButton->SetHelpText(LOCTEXT("TradeInfo", "Trade with this spacecraft"));
			TradeButton->SetDisabled(false);
		}
		else if (CanTrade && TargetSpacecraft->IsTrading())
		{
			TradeButton->SetHelpText(LOCTEXT("CantTradeInfo", "Trading in progress"));
			TradeButton->SetDisabled(true);
		}
		else
		{
			TradeButton->SetHelpText(LOCTEXT("CantTradeInfo", "Trading requires to be docked in a peaceful sector (ships outside the player fleet can trade within their sector)"));
			TradeButton->SetDisabled(true);
		}

		// Disable upgrades
		if (CanUpgrade)
		{
			UpgradeButton->SetHelpText(LOCTEXT("UpgradeInfo", "Upgrade this spacecraft"));
			UpgradeButton->SetDisabled(false);
		}
		else
		{
			UpgradeButton->SetHelpText(LOCTEXT("CantUpgradeInfo", "Upgrading requires to be docked at a supply outpost in a peaceful sector"));
			UpgradeButton->SetDisabled(true);
		}

		// Disable scrapping
		if (CanScrap)
		{
			ScrapButton->SetHelpText(LOCTEXT("ScrapInfo", "Permanently destroy this ship and get some resources back"));
			ScrapButton->SetDisabled(false);
		}
		else
		{
			ScrapButton->SetHelpText(LOCTEXT("CantScrapInfo", "Scrapping requires to be docked in a peaceful sector (ships outside the player fleet can be scrapped when a station is present)"));
			ScrapButton->SetDisabled(true);
		}
	}

	UpdateCaptureList();

	if (PC->GetMenuManager()->GetCurrentMenu() == EFlareMenu::MENU_Trade)
	{
		CargoBay->SetVisibility(EVisibility::Visible);
	}
}

void SFlareSpacecraftInfo::Hide()
{
	CaptureBox->ClearChildren();
	TargetSpacecraft = NULL;
	TargetSpacecraftDesc = NULL;
	SetVisibility(EVisibility::Collapsed);
}


/*----------------------------------------------------
	Callbacks
----------------------------------------------------*/

void SFlareSpacecraftInfo::UpdateCaptureList()
{
	CaptureBox->ClearChildren();
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();

	// Initial checks
	FCHECK(PC);
	if (TargetSpacecraft == NULL || !TargetSpacecraft->IsValidLowLevel() || !TargetSpacecraft->IsStation())
	{
		return;
	}

	// Find all companies that could capture this
	TArray<UFlareCompany*> Companies = PC->GetGame()->GetGameWorld()->GetCompanies();
	for (int32 CompanyIndex = 0; CompanyIndex < Companies.Num(); CompanyIndex++)
	{
		UFlareCompany* Company = Companies[CompanyIndex];
		if (TargetSpacecraft->GetCapturePoint(Company) > 0)
		{
			FText CaptureInfo = FText::Format(LOCTEXT("CaptureFormat", "Capture in progress by {0} ({1})"),
				Company->GetCompanyName(),
				Company->GetPlayerHostilityText());

			CaptureBox->AddSlot()
			.AutoHeight()
			.Padding(Theme.SmallContentPadding)
			[
				SNew(SBorder)
				.BorderImage(&Theme.BackgroundBrush)
				.Padding(Theme.SmallContentPadding)
				[
					SNew(SHorizontalBox)

					// Flag
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(Theme.SmallContentPadding)
					[
						SNew(SFlareCompanyFlag)
						.Company(Company)
						.Player(PC)
					]

					// Textual warning
					+ SHorizontalBox::Slot()
					.Padding(Theme.SmallContentPadding)
					[
						SNew(STextBlock)
						.TextStyle(&Theme.TextFont)
						.Text(CaptureInfo)
					]

					// Progress bar
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(Theme.SmallContentPadding)
					[
						SNew(SBox)
						.WidthOverride(100)
						.VAlign(VAlign_Center)
						[
							SNew(SProgressBar)
							.Percent(static_cast<float>(TargetSpacecraft->GetCapturePoint(Company)) / static_cast<float>(TargetSpacecraft->GetCapturePointThreshold()))
							.BorderPadding(FVector2D(0, 0))
							.Style(&Theme.ProgressBarStyle)
						]
					]
				]
			];
		}
	}
}

void SFlareSpacecraftInfo::OnInspect()
{
	if (PC && TargetSpacecraft)
	{
		FLOGV("SFlareSpacecraftInfo::OnInspect : TargetSpacecraft=%p", TargetSpacecraft);
		FFlareMenuParameterData Data;
		Data.Spacecraft = TargetSpacecraft;
		PC->GetMenuManager()->OpenMenu(EFlareMenu::MENU_Ship, Data);
	}
}

void SFlareSpacecraftInfo::OnUpgrade()
{
	if (PC && TargetSpacecraft)
	{
		FLOGV("SFlareSpacecraftInfo::OnUpgrade : TargetSpacecraft=%p", TargetSpacecraft);
		FFlareMenuParameterData Data;
		Data.Spacecraft = TargetSpacecraft;
		PC->GetMenuManager()->OpenMenu(EFlareMenu::MENU_ShipConfig, Data);
	}
}

void SFlareSpacecraftInfo::OnTrade()
{
	if (PC && TargetSpacecraft)
	{
		FFlareMenuParameterData Data;
		Data.Spacecraft = TargetSpacecraft;
		PC->GetMenuManager()->OpenMenu(EFlareMenu::MENU_Trade, Data);
	}
}

void SFlareSpacecraftInfo::OnFly()
{
	FText Unused;
	if (PC && TargetSpacecraft && TargetSpacecraft->IsActive() && TargetSpacecraft->CanBeFlown(Unused))
	{
		FLOGV("SFlareSpacecraftInfo::OnFly : flying active spacecraft '%s'", *TargetSpacecraft->GetImmatriculation().ToString());
		FFlareMenuParameterData Data;
		Data.Spacecraft = TargetSpacecraft;
		PC->GetMenuManager()->OpenMenu(EFlareMenu::MENU_FlyShip, Data);
	}
}

void SFlareSpacecraftInfo::OnDockAt()
{
	if (PC && TargetSpacecraft && TargetSpacecraft->IsActive() && TargetSpacecraft->GetActive()->GetDockingSystem()->GetDockCount() > 0)
	{
		bool DockingConfirmed = PC->GetShipPawn()->GetNavigationSystem()->DockAt(TargetSpacecraft->GetActive());
		PC->NotifyDockingResult(DockingConfirmed, TargetSpacecraft);
		if (DockingConfirmed)
		{
			PC->GetMenuManager()->CloseMenu();
		}
	}
}

void SFlareSpacecraftInfo::OnUndock()
{
	if (PC && TargetSpacecraft)
	{
		if (TargetSpacecraft->IsActive() && TargetSpacecraft->GetActive()->GetNavigationSystem()->IsDocked())
		{
			TargetSpacecraft->GetActive()->GetNavigationSystem()->Undock();
			PC->GetMenuManager()->CloseMenu();
		}
		else if(TargetSpacecraft->IsActive() && TargetSpacecraft->GetActive()->GetDockingSystem()->GetDockCount() > 0)
		{
			PC->GetShipPawn()->GetNavigationSystem()->Undock();
			PC->GetMenuManager()->CloseMenu();
		}
	}
}

void SFlareSpacecraftInfo::OnScrap()
{
	if (PC && TargetSpacecraft && TargetSpacecraft->IsValidLowLevel())
	{
		FLOGV("SFlareSpacecraftInfo::OnScrap : scrapping '%s'", *TargetSpacecraft->GetImmatriculation().ToString());
		UFlareSimulatedSpacecraft* TargetStation = NULL;
		TArray<UFlareSimulatedSpacecraft*> SectorStations = TargetSpacecraft->GetCurrentSector()->GetSectorStations();

		// Find a suitable station
		for (int Index = 0; Index < SectorStations.Num(); Index++)
		{
			if (SectorStations[Index]->GetCompany() == PC->GetCompany())
			{
				TargetStation = SectorStations[Index];
				FLOGV("SFlareSpacecraftInfo::OnScrap : found player station '%s'", *TargetStation->GetImmatriculation().ToString());
				break;
			}
			else if (TargetStation == NULL)
			{
				TargetStation = SectorStations[Index];
			}
		}

		// Scrap
		if (TargetStation)
		{
			FLOGV("SFlareSpacecraftInfo::OnScrap : scrapping at '%s'", *TargetStation->GetImmatriculation().ToString());

			OnRemoved.ExecuteIfBound(TargetSpacecraft);
			PC->GetGame()->Scrap(TargetSpacecraft->GetImmatriculation(), TargetStation->GetImmatriculation());
			PC->GetMenuManager()->Back();
		}
		else
		{
			FLOG("SFlareSpacecraftInfo::OnScrap : couldn't find a valid station here !");
		}
	}
}


/*----------------------------------------------------
	Content
----------------------------------------------------*/

FText SFlareSpacecraftInfo::GetName() const
{
	return TargetName;
}

FSlateColor SFlareSpacecraftInfo::GetTextColor() const
{
	FLinearColor Result;
	const FFlareStyleCatalog& Theme = FFlareStyleSet::GetDefaultTheme();

	if (TargetSpacecraft && TargetSpacecraft->IsValidLowLevel())
	{
		if (TargetSpacecraft == PC->GetPlayerShip())
		{
			return Theme.ObjectiveColor;
		}
		else if (TargetSpacecraft->GetCurrentFleet() == PC->GetPlayerFleet())
		{
			return Theme.FriendlyColor;
		}
		else if (TargetSpacecraft->GetCompany()->GetWarState(PC->GetCompany()) == EFlareHostility::Hostile)
		{
			return Theme.EnemyColor;
		}
		else
		{
			return Theme.NeutralColor;
		}
	}

	return Result;
}

FText SFlareSpacecraftInfo::GetDescription() const
{
	// Common text
	FText DefaultText = LOCTEXT("Default", "UNKNOWN OBJECT");

	// Description builder
	if (TargetSpacecraftDesc && TargetSpacecraft->IsValidLowLevel())
	{
		// TODO exclude shipyard
		if(TargetSpacecraft && TargetSpacecraft->IsStation())
		{
			return FText::Format(LOCTEXT("DescriptionStationFormat", "(Lv {0} {1})"),
				FText::AsNumber(TargetSpacecraft->GetLevel()),
				TargetSpacecraftDesc->Name);
		}
		else
		{
			return FText::Format(LOCTEXT("DescriptionFormat", "({0})"), TargetSpacecraftDesc->Name);
		}
	}

	return DefaultText;
}

const FSlateBrush* SFlareSpacecraftInfo::GetIcon() const
{
	if (TargetSpacecraftDesc)
	{
		return &TargetSpacecraftDesc->MeshPreviewBrush;
	}
	return NULL;
}

const FSlateBrush* SFlareSpacecraftInfo::GetClassIcon() const
{
	if (TargetSpacecraftDesc)
	{
		return FFlareSpacecraftDescription::GetIcon(TargetSpacecraftDesc);
	}
	return NULL;
}

EVisibility SFlareSpacecraftInfo::GetCompanyFlagVisibility() const
{
	// Crash mitigation - If parent is hidden, so are we, don't try to use the target (#178)
	if ((OwnerWidget.IsValid() && OwnerWidget->GetVisibility() != EVisibility::Visible)
		|| (PC && !PC->GetMenuManager()->IsUIOpen()))
	{
		return EVisibility::Collapsed;
	}

	// Check the target
	if (TargetSpacecraft && TargetSpacecraft->IsValidLowLevel())
	{
		UFlareCompany* TargetCompany = TargetSpacecraft->GetCompany();

		if (TargetCompany && PC && TargetCompany == PC->GetCompany())
		{
			// Not interesting after all
			return EVisibility::Collapsed;
		}
	}

	return EVisibility::Visible;
}

EVisibility SFlareSpacecraftInfo::GetSpacecraftInfoVisibility() const
{
	return (GetSpacecraftInfo().ToString().Len() > 0) ? EVisibility::Visible : EVisibility::Collapsed;
}

FText SFlareSpacecraftInfo::GetSpacecraftInfo() const
{
	// Crash mitigation - If parent is hidden, so are we, don't try to use the target (#178)
	if ((OwnerWidget.IsValid() && OwnerWidget->GetVisibility() != EVisibility::Visible)
		|| (PC && !PC->GetMenuManager()->IsUIOpen()))
	{
		return FText();
	}

	if (TargetSpacecraft && TargetSpacecraft->IsValidLowLevel())
	{
		// Get the object's distance
		FText DistanceText;
		if (PC->GetPlayerShip() && PC->GetPlayerShip() != TargetSpacecraft)
		{
			AFlareSpacecraft* PlayerShipPawn = PC->GetPlayerShip()->GetActive();
			AFlareSpacecraft* TargetSpacecraftPawn = TargetSpacecraft->GetActive();
			if (PlayerShipPawn && TargetSpacecraftPawn)
			{
				if (PlayerShipPawn->GetNavigationSystem()->GetDockStation() == TargetSpacecraftPawn)
				{
					DistanceText = LOCTEXT("DockedHereStation", "Docked - ");
				}
				else
				{
					float Distance = (PlayerShipPawn->GetActorLocation() - TargetSpacecraftPawn->GetActorLocation()).Size();
					DistanceText = FText::FromString(AFlareHUD::FormatDistance(Distance / 100) + " - ");
				}
			}
		}
		else
		{
			DistanceText = LOCTEXT("PlayerShipText", "Player ship - ");
		}
		
		// Our company
		UFlareCompany* TargetCompany = TargetSpacecraft->GetCompany();
		if (TargetCompany && PC && TargetCompany == PC->GetCompany())
		{
			// Station : show production, if simulated
			if (TargetSpacecraft->IsStation())
			{
				FText ProductionStatusText = FText();
				TArray<UFlareFactory*>& Factories = TargetSpacecraft->GetFactories();

				if (Factories.Num() > 0)
				{
					for (int FactoryIndex = 0; FactoryIndex < Factories.Num(); FactoryIndex++)
					{
						FText NewLineText = (FactoryIndex > 0) ? FText::FromString("\n") : FText();
						UFlareFactory* Factory = Factories[FactoryIndex];

						ProductionStatusText = FText::Format(LOCTEXT("ProductionStatusFormat", "{0}{1}{2} : {3}"),
							ProductionStatusText,
							NewLineText,
							Factory->GetDescription()->Name,
							Factory->GetFactoryStatus());
					}

					return FText::Format(LOCTEXT("StationInfoFormat", "{0}{1}"),
						DistanceText,
						ProductionStatusText);
				}
				else
				{
					return FText::Format(LOCTEXT("StationInfoFormatNoFactories", "{0}No factories"), DistanceText);
				}
			}

			// Ship : show fleet info
			else
			{
				UFlareFleet* Fleet = TargetSpacecraft->GetCurrentFleet();
				if (Fleet)
				{
					FText FleetAssignedText;

					if (Fleet->GetCurrentTradeRoute())
					{
						FleetAssignedText = FText::Format(LOCTEXT("FleetAssignedFormat", " - {0}"),
							Fleet->GetCurrentTradeRoute()->GetTradeRouteName());
					}

					FText SpacecraftDescriptionText = FText::Format(LOCTEXT("FleetFormat", "{0} ({1} / {2}){3}"),
						Fleet->GetFleetName(),
						FText::AsNumber(Fleet->GetShipCount()),
						FText::AsNumber(Fleet->GetMaxShipCount()),
						FleetAssignedText);

					return FText::Format(LOCTEXT("SpacecraftInfoFormat", "{0}{1} - {2}"),
						DistanceText,
						Fleet->GetStatusInfo(),
						SpacecraftDescriptionText);
				}
				return FText();
			}
		}

		// Other company
		else if (TargetCompany)
		{
			return FText::Format(LOCTEXT("OwnedByFormat", "{0}Owned by {1} ({2})"),
				DistanceText,
				TargetCompany->GetCompanyName(),
				TargetCompany->GetPlayerHostilityText());
		}
	}

	return FText();
}


#undef LOCTEXT_NAMESPACE
