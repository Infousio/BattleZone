// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;

UCLASS()
class BATTLEZONE_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;
	
	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;


private:

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveForward(float AxisValue);
	void MoveForward(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_MoveRight(float AxisValue);
	void MoveRight(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LookUp(float AxisValue);
	void LookUp(float AxisValue);
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LookRight(float AxisValue);
	void LookRight(float AxisValue);

	void PullTrigger();
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_PullTrigger(USkeletalMeshComponent* NewMesh);
	void Server_PullTrigger_Implementation(USkeletalMeshComponent* NewMesh);
	bool Server_PullTrigger_Validate(USkeletalMeshComponent* NewMesh);


	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTransform)
	FTransform ReplicatedTransform;
	UFUNCTION()
	void OnRep_ReplicatedTransform();

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedHealth)
	float ReplicatedHealth;
	UFUNCTION()
	void OnRep_ReplicatedHealth();

	UPROPERTY(EditAnywhere)
	float RotationRate;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY()
	AGun* Gun;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 200;

	UPROPERTY(VisibleAnywhere)
	float Health;
};
