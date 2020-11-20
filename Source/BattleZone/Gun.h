// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class BATTLEZONE_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger(USkeletalMeshComponent* Mesh);

private:
	void Shoot();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffectWorld;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;
	

	UPROPERTY(EditAnywhere)
	float MaxRange = 50000;

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);

	AController* GetOwnerController();

	
	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_MuzzleEffects(USkeletalMeshComponent* Mesh);
	void Multi_MuzzleEffects_Implementation(USkeletalMeshComponent* Mesh);
	bool Multi_MuzzleEffects_Validate(USkeletalMeshComponent* Mesh);

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_ImpactEffects(FVector Location, FRotator Direction);
	void Multi_ImpactEffects_Implementation(FVector Location, FRotator Direction);
	bool Multi_ImpactEffects_Validate(FVector Location, FRotator Direction);

};
