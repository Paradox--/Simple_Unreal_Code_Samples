//---------------------------------------------------------------------------------------------------
// This is a simple replication actor example. It will replicate a float variable using the rep	   	-
// notify system to replicate the varaible myVaraible.											                        -
// Nicholas Mallonee																				                                        -
// Unreal Engine 4.7.6																			                                      	-
//---------------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------------
// Pragmas and Defines																			                                      	-
//---------------------------------------------------------------------------------------------------
#pragma once

//---------------------------------------------------------------------------------------------------
// Includes and Libraries																		                                       	-
//---------------------------------------------------------------------------------------------------
#include "GameFramework/Actor.h"
#include "MyRepNotifyExample.generated.h"

//---------------------------------------------------------------------------------------------------
// Class Information																			                                        	-
//---------------------------------------------------------------------------------------------------
UCLASS()
class Code_Example_API AMyRepNotifyExample : public AActor
{
	GENERATED_BODY()
	
public:	
	AMyRepNotifyExample();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;


	// -- Public Information -- Server Replication calls - // 
public:
	UFUNCTION(reliable, server, WithValidation)
	void SERVER_SetMyVaraible(float myVar);					// With validation we can set the varaible 

	UPROPERTY(ReplicatedUsing = OnRep_SetMyVaraible)
	bool DoRep_VaraibleSet;									// Boolean to hold if the server can change the value 

	UFUNCTION()
	virtual void OnRep_SetMyVaraible();						// Actually Sets the varaible 

	// -- Private Information -- Varaible to Replicate -- // 
private:
	UPROPERTY(Replicated)
	float R_myVaraible;										// The Float Varaible we are going to replicate	
};
