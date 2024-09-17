// Fill out your copyright notice in the Description page of Project Settings.

#include "MilitaryUnit.h"
#include "TargetableComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AMilitaryUnit::AMilitaryUnit()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	selected = false;
	
	targetableComp = CreateDefaultSubobject<UTargetableComponent>(TEXT("CPP Targetable Component"));
	targetableComp->SetupAttachment(RootComponent);
	targetableComp->SetFaction(faction);

	attackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
	attackRangeSphere->SetupAttachment(RootComponent);
	attackRangeSphere->SetSphereRadius(attackRange);

	UE_LOG(LogTemp, Display, TEXT("Setup completed"));
}

AMilitaryUnit::AMilitaryUnit(int faction)
{
	AMilitaryUnit();
	this->faction = faction;

}

// Called when the game starts or when spawned
void AMilitaryUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMilitaryUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMilitaryUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool AMilitaryUnit::IsOwnedBy(int player) {
	return player == faction;
}

float Get2DDistanceSquared(FVector a, FVector b) {
	return pow(b.X - a.X, 2) + pow(b.Y - a.Y, 2);
}

// TODO: Test
bool AMilitaryUnit::CheckIfValidTarget(AActor* potentialTarget) {
	// Attempt to cast PT to Targetable
	UTargetableComponent* pt = Cast<UTargetableComponent>(potentialTarget);
	
	// Can't target something that's not targetable
	if (pt == NULL)
		return false;

	// Can't target allies
	if (pt->GetFaction() == faction)
		return false;

	// Get the (2D) distance from us to target
	float distSquared = Get2DDistanceSquared(GetActorLocation(), pt->GetAttachParentActor()->GetActorLocation());
	// Can't target out of range
	if (distSquared > pow(attackRange, 2))
		return false;

	return true;
}

std::vector<UTargetableComponent*> AMilitaryUnit::GetTargetsInRange() {
	return targetsInRange;
}

void AMilitaryUnit::BeginAttack() {
	currentState = UnitState::ATTACKING;
	attackTimer = attackSpeed;
}

void AMilitaryUnit::DoAttack(float DeltaTime) {
	attackTimer -= DeltaTime;

	if (attackTimer > 0)
		return;

	attackTimer += attackSpeed;
	attackTarget->ReceiveDamage(attackDamage);
}