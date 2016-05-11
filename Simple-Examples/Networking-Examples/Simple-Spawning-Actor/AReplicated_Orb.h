//---------------------------------------------------------------------------------------------------------------------------------------
// This actor spawns other actors into the world over a network. This is the first in a series of simple examples of network coding		  -
// This class is ment to show competency in basic asset replication over the network, and spawning over a network.						          - 
// It should be noted that the actor that is spawned is set to be replicated over the unreal network. Please see the ASpawning_Orb      -
// class to see which object is going to be spawned. The Classes mesh or other visible assets are ment to be implmented through the     -
// the blueprint system.																											                                                        	-
//																																		                                                                  -
// Note: The timer in this class is not needed, but allows the user to get closer to the origin of the world before the objects to	    -
// spawn. Futhermore, the time delay in the timer is hard coded. Normally, this would be a float UPROPERTY to allow the artist or		    -
// designers to change the value without a recompile of the c++ source.																	                                -
//																																		                                                                  -
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
#include "GameFramework/Actor.h"												                // Include Actor Framework 
#include "Engine.h"																                      // Include Engine For Debug Messages 
#include "UnrealNetwork.h"														                  // Include Unreal Network for Replications
#include "Runtime/Engine/Public/TimerManager.h"									        // Include the timer manager for time handler 
#include "ReplicationPractice/Orbs/Spawning_Orb.h"								      // Include the Spawning orbs as the class to spawn 
#include "Replicated_Orb.generated.h"											              // Include auto generated header file for this class 


//---------------------------------------------------------------------------------------------------------------------------------------
// Class Information 																													                                                          -
//---------------------------------------------------------------------------------------------------------------------------------------
UCLASS()
class REPLICATIONPRACTICE_API AReplicated_Orb : public AActor
{
	GENERATED_BODY()

	// -- Public Information -- Constructor and Engine Events -- // 
public:
	AReplicated_Orb();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	// -- Private Information -- Replication Values and Variables -- // 
private:
	UPROPERTY(EditDefaultsOnly, Replicated)
	int32 R_numToSpawn; 

	UPROPERTY(EditDefaultsOnly, Replicated, ReplicatedUsing = OnRep_Spawning)
	bool bIsSpawning; 

	UPROPERTY(Replicated)
	FTimerHandle spawnTimerHandler;

  // -- Private Information --  Spawning Methods, both replicated and non-replicated -- // 
private:
	UFUNCTION() 
	void OnSpawn(); 

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerBeginSpawning(); 

	UFUNCTION()
	void OnRep_Spawning(); 

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDoSpawning();

	UFUNCTION()
	void SpawnOrbs(); 

	UFUNCTION(Reliable, Server, WithValidation)
	void ServerSpawnOrbs(FVector vec);

	// -- Private information -- Object to Spawn -- // 
private:
	UPROPERTY(EditDefaultsOnly, Category = "Object To Spawn")
	TSubclassOf<ASpawning_Orb> basicOrb; 

	// -- Public Information -- List of replicated properties-- // 
public:
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;
};
