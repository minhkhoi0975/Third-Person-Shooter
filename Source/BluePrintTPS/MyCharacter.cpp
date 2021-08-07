// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMyCharacter::CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor) const
{
	static const FName NAME_AILineOfSight = FName(TEXT("TestPawnLineOfSight"));

	FHitResult HitResult;

	auto sockets = GetMesh()->GetAllSocketNames();

	// Check if AI can detect character's bone.
	for (int i = 0; i < sockets.Num(); i++)
	{
		FVector socketLocation = GetMesh()->GetSocketLocation(sockets[i]);

		//const bool bHitSocket = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, socketLocation
		//	, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)) // << Changed this line
		//	, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor)
		//);

		const bool bHitSocket = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, socketLocation, ECollisionChannel::ECC_Visibility, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

		NumberOfLoSChecksPerformed++;

		if (bHitSocket == false || (HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this))) {
			OutSeenLocation = socketLocation;
			OutSightStrength = 1;
			
			UE_LOG(LogTemp, Error, TEXT("Detected a bone:"));
			return true;
		}
	}

	// Check if AI can detect "middle point".
	//const bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, ObserverLocation, GetActorLocation()
	//	, FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)) // << Changed this line
	//	, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	NumberOfLoSChecksPerformed++;

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, GetActorLocation(), ECollisionChannel::ECC_Visibility, FCollisionQueryParams(NAME_AILineOfSight, true, IgnoreActor));

	if (bHit == false || (HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this)))
	{
		OutSeenLocation = GetActorLocation();
		OutSightStrength = 1;

		UE_LOG(LogTemp, Error, TEXT("Detected character"));
		return true;
	}

	OutSightStrength = 0;
	return false;
}

