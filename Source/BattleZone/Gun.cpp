// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "ShooterCharacter.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGun::PullTrigger(USkeletalMeshComponent* Mesh)
{
	Shoot();
	
	Multi_MuzzleEffects(Mesh);
}

void AGun::Shoot()
{
	FHitResult Hit;
	FVector ShotDirection;
	bool bSuccess = GunTrace(Hit, ShotDirection);
	if (bSuccess)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, Hit, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
			Multi_ImpactEffects(Hit.Location, ShotDirection.Rotation());
		}
	}
}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection)
{
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector ViewpointLocation;
	FRotator ViewpointRotation;
	OwnerController->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);
	FVector End = ViewpointLocation + ViewpointRotation.Vector() * MaxRange;
	ShotDirection = -ViewpointRotation.Vector();

	return GetWorld()->LineTraceSingleByChannel(Hit, ViewpointLocation, End, ECollisionChannel::ECC_GameTraceChannel1);
}

AController* AGun::GetOwnerController()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) return nullptr;

	return OwnerPawn->GetController();
}


void AGun::Multi_MuzzleEffects_Implementation(USkeletalMeshComponent* Mesh)
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("Muzzle_01"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("Muzzle_01"));
}

bool AGun::Multi_MuzzleEffects_Validate(USkeletalMeshComponent* Mesh)
{
	return true;
}

void AGun::Multi_ImpactEffects_Implementation(FVector Location, FRotator Direction)
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffectWorld, Location, Direction);
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, Location, Direction);
}

bool AGun::Multi_ImpactEffects_Validate(FVector Location, FRotator Direction)
{
	return true;
}


