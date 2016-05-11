//---------------------------------------------------------------------------------------------------------------------------------------
// This actor spawns other actors into the world over a network. This is the first in a series of simple examples of network coding	  	-
// This class is ment to show competency in basic asset replication over the network, and spawning over a network.						          - 
// It should be noted that the actor that is spawned is set to be replicated over the unreal network. Please see the ASpawning_Orb      -
// class to see which object is going to be spawned. The Classes mesh or other visible assets are ment to be implmented through the     -
// the blueprint system.																											                                                        	-
//																																	                                                                  	-
// Nicholas Mallonee																												                                                          	-
// Unreal Engine 4.10.2																												                                                         	-
//---------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------------------------------
// Libraries																															                                                              -
//---------------------------------------------------------------------------------------------------------------------------------------
#include "ReplicationPractice.h"
#include "Replicated_Orb.h"

//---------------------------------------------------------------------------------------------------------------------------------------
// Constructors																															                                                            -
//---------------------------------------------------------------------------------------------------------------------------------------
AReplicated_Orb::AReplicated_Orb()
{
	PrimaryActorTick.bCanEverTick = false;

	// -- Varaiables -- 
	this->bIsSpawning = false;					// Set The default behavior to not currently spawning 
	this->R_numToSpawn = 1;						// Set the default number of objects to spawn as 1, overriden in the blueprints 

	// -- Replication -- 
	this->SetReplicates(true);					// Allow this to replicate
}

//---------------------------------------------------------------------------------------------------------------------------------------
// Engine Events																														                                                            -
//---------------------------------------------------------------------------------------------------------------------------------------
void AReplicated_Orb::BeginPlay()
{
	Super::BeginPlay();

	// Once we have spawned, check for the world and start the spawn method tree
	UWorld* const world = GetWorld();

	if (world)
	{
		OnSpawn();
	}
}

void AReplicated_Orb::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//---------------------------------------------------------------------------------------------------------------------------------------
// Spawning Methods for the replicated orb																								                                              -
//---------------------------------------------------------------------------------------------------------------------------------------
void AReplicated_Orb::OnSpawn()																        // -- Starts the Spawning Tree -- //
{
	if (!bIsSpawning)
		ServerBeginSpawning(); 
	else 
		return;
}

bool AReplicated_Orb::ServerBeginSpawning_Validate()									// -- Validates the Action -- // 
{
	return true;
}

void AReplicated_Orb::ServerBeginSpawning_Implementation()						// -- Implements the Start of the Spawn -- // 
{
	if (Role == ROLE_Authority && !bIsSpawning)
	{
		bIsSpawning = true;
		OnRep_Spawning(); 
	}
}

void AReplicated_Orb::OnRep_Spawning()														    // -- On the Replication of the spawning -- // 
{
	if (bIsSpawning)
		ServerDoSpawning(); 
}

bool AReplicated_Orb::ServerDoSpawning_Validate()											// -- Validate the Spawning further -- // 
{
	return true; 
}

void AReplicated_Orb::ServerDoSpawning_Implementation()								// -- Set the timer and spawn the number of orbs -- // 
{
	GetWorld()->GetTimerManager().SetTimer(spawnTimerHandler, this, &AReplicated_Orb::SpawnOrbs, 1.f, false);
}

void AReplicated_Orb::SpawnOrbs()															        // -- Generates Vectors and spawns the orbs -- //
{
	// check if the world exists and then spawn the number of robs 
	UWorld* const world = GetWorld();

	if (world)
	{
		for (int32 i = 0; i < R_numToSpawn; i++)
		{

			// ** Debug ** // 
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Spawning Orb.. "));

			// Create a float to use as a random factor 
			float ran = (float)FMath::RandRange(-50, 50);

			// Create a new vector 
			FVector newVec = FVector(i + ran, i + ran, 250.f);

			// spawn the orbs on the server 
			ServerSpawnOrbs(newVec);
		}

		// Clean up the timer .. 
		GetWorld()->GetTimerManager().ClearTimer(spawnTimerHandler);
	}
}

bool AReplicated_Orb::ServerSpawnOrbs_Validate(FVector vec)									// -- Validates on the server to spawn an Orb  -- // 
{
	return true; 
}

void AReplicated_Orb::ServerSpawnOrbs_Implementation(FVector vec)						// -- Implements the Server spawning the orbs -- // 
{
	// if the role is the same as the server 
	if (Role == ROLE_Authority)
	{
		// ** debug ** // 
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Orb Spawned "));

		// set the spawn params 
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = Instigator;

		// Spawn the actor 
		GetWorld()->SpawnActor<ASpawning_Orb>(basicOrb, vec, FRotator::ZeroRotator, spawnParams);
	}
	else
	{
		// ** Debug ** // 
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Orb Not Spawned "));
	}
}

//---------------------------------------------------------------------------------------------------------------------------------------
// List of Replicated Properties.																									                                                    	-
//---------------------------------------------------------------------------------------------------------------------------------------
void AReplicated_Orb::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// NUmber of orbs to spawn -- int32 
	DOREPLIFETIME(AReplicated_Orb, R_numToSpawn);

	// That the orb is spawning  -- bool
	DOREPLIFETIME(AReplicated_Orb, bIsSpawning);

	// Timer of Spawning Objects -- FTimeHandler
	DOREPLIFETIME(AReplicated_Orb, spawnTimerHandler);
}
