// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "BattleZoneGameModeBase.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();


	RotationRate = 0.8;
	Health = MaxHealth;

	if (HasAuthority())
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		Gun->SetOwner(this);
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority())
	{
		ReplicatedTransform = GetActorTransform();
		ReplicatedHealth = Health;
	}
}

void AShooterCharacter::OnRep_ReplicatedTransform()
{
	SetActorTransform(ReplicatedTransform);
}

void AShooterCharacter::OnRep_ReplicatedHealth()
{
	Health = ReplicatedHealth;
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShooterCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::LookRight);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), IE_Pressed, this, &AShooterCharacter::PullTrigger);

}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Damage = FMath::Min(Health, Damage);
	Health -= Damage;

	if (IsDead())
	{
		ABattleZoneGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ABattleZoneGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		Health = 200;
	}
	return Damage;
}

bool AShooterCharacter::IsDead() const
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}


void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterCharacter, ReplicatedTransform);
	DOREPLIFETIME(AShooterCharacter, ReplicatedHealth);
}

void AShooterCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
	Server_MoveForward_Implementation(AxisValue);
}
void AShooterCharacter::Server_MoveForward_Implementation(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}
bool AShooterCharacter::Server_MoveForward_Validate(float AxisValue)
{
	return FMath::Abs(AxisValue) <= 1;
}


void AShooterCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
	Server_MoveRight_Implementation(AxisValue);
}
void AShooterCharacter::Server_MoveRight_Implementation(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}
bool AShooterCharacter::Server_MoveRight_Validate(float AxisValue)
{
	return FMath::Abs(AxisValue) <= 1;
}


void AShooterCharacter::LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate);
	Server_LookUp_Implementation(AxisValue);
}
void AShooterCharacter::Server_LookUp_Implementation(float AxisValue)
{
	AddControllerPitchInput(AxisValue * RotationRate);
}
bool AShooterCharacter::Server_LookUp_Validate(float AxisValue)
{
	return true;
}


void AShooterCharacter::LookRight(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate);
	Server_LookRight_Implementation(AxisValue);
}
void AShooterCharacter::Server_LookRight_Implementation(float AxisValue)
{
	AddControllerYawInput(AxisValue * RotationRate);
}
bool AShooterCharacter::Server_LookRight_Validate(float AxisValue)
{
	return true;
}


void AShooterCharacter::PullTrigger()
{
	if (HasAuthority())
	{
		Gun->PullTrigger(GetMesh());
	}
	else
	{
		Server_PullTrigger(GetMesh());
	}
}

void AShooterCharacter::Server_PullTrigger_Implementation(USkeletalMeshComponent* NewMesh)
{
	Gun->PullTrigger(NewMesh);
}

bool AShooterCharacter::Server_PullTrigger_Validate(USkeletalMeshComponent* NewMesh)
{
	return true;
}