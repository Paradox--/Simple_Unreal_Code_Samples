//---------------------------------------------------------------------------------------------------------------------------------------
// This actor is spawned by another actor. The Mesh and visible objects are set through the blueprint system.							              -
//																																	                                                                  	-
// Nicholas Mallonee																													                                                          -
// Unreal Engine 4.10.2																													                                                        -
//---------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------
// Pragmas and Defines																													                                                        -
//---------------------------------------------------------------------------------------------------------------------------------------
#pragma once

//---------------------------------------------------------------------------------------------------------------------------------------
// Libraries																															                                                              -
//---------------------------------------------------------------------------------------------------------------------------------------
#include "GameFramework/Actor.h"												                                // Include the Actor Framework
#include "UnrealNetwork.h"													                                   	// Include Unreal Network for Replications
#include "Spawning_Orb.generated.h"											                              	// Include the Auto Generated header file. 

//---------------------------------------------------------------------------------------------------------------------------------------
// Class Information 																												                                                          	-
//---------------------------------------------------------------------------------------------------------------------------------------
UCLASS()
class REPLICATIONPRACTICE_API ASpawning_Orb : public AActor
{
	GENERATED_BODY()
	
	// -- Public Information -- Constructors and Engine Events -- // 
public:	
	ASpawning_Orb();

	virtual void BeginPlay() override;
	
	virtual void Tick( float DeltaSeconds ) override;

	// -- Private Information -- Replication Lists -- // 
private:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;	
};
