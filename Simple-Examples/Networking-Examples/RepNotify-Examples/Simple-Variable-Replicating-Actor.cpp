/---------------------------------------------------------------------------------------------------
// This is a simple replication actor example. It will replicate a float variable using the rep		  -
// notify system to replicate the varaible myVaraible.											                        -
// Nicholas Mallonee																			                                        	-
// Unreal Engine 4.7.6																			                                      	-
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Libraries and Includes																		                                      	-
//---------------------------------------------------------------------------------------------------
#include "AI_Playground.h"
#include "Engine.h"
#include "UnrealNetwork.h"
#include "MyRepNotifyExample.h"

//---------------------------------------------------------------------------------------------------
// Constructor																					                                          	-
//---------------------------------------------------------------------------------------------------
AMyRepNotifyExample::AMyRepNotifyExample()
{
	PrimaryActorTick.bCanEverTick = false;

	R_myVaraible = 0.f; 
}

//---------------------------------------------------------------------------------------------------
// Engine Events																				                                          	-
//---------------------------------------------------------------------------------------------------
void AMyRepNotifyExample::BeginPlay()
{
	Super::BeginPlay();


	if (Role == ROLE_Authority)
	{
		FString temp = FString::SanitizeFloat(R_myVaraible);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("The spawned object has a value of: ") + temp);
	}	
}

void AMyRepNotifyExample::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//---------------------------------------------------------------------------------------------------
// Replication Methods 																				                                      -
//---------------------------------------------------------------------------------------------------
bool AMyRepNotifyExample::SERVER_SetMyVaraible_Validate(float myVar)
{
	if (myVar > 0.f)
		return true;
	else
		return false; 
}

void AMyRepNotifyExample::SERVER_SetMyVaraible_Implementation(float myVar)
{
	// Replicate the varaible 
	R_myVaraible = myVar; 

	// ** debug ** // 
	if (Role == ROLE_Authority)
	{
		FString temp = FString::SanitizeFloat(R_myVaraible);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("The spawned object has a vale of: ") + temp);
	}

	// Run the server method 
	OnRep_SetMyVaraible();
}

void AMyRepNotifyExample::OnRep_SetMyVaraible()
{
  R_myVaraible = 200.f; 
}

void AMyRepNotifyExample::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Properties to replicate 
	DOREPLIFETIME(AMyRepNotifyExample, R_myVaraible);
	DOREPLIFETIME(AMyRepNotifyExample, DoRep_VaraibleSet);
}
