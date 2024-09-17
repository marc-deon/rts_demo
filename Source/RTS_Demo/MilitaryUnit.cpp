// Fill out your copyright notice in the Description page of Project Settings.


#include "MilitaryUnit.h"

// Sets default values
AMilitaryUnit::AMilitaryUnit()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	selected = false;
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

// TODO
bool AMilitaryUnit::CheckIfValidTarget(AActor potentionalTarget) {
	return false;
}

// TODO
std::vector<UTargetableComponent*> AMilitaryUnit::GetTargetsInRange() {
	return std::vector<UTargetableComponent*>();
}

// TODO
void AMilitaryUnit::BeginAttack() {

}

// TODO
// previously: Returns true if target died
// Should: do nothing; we will respond to target death via some kind of event
void AMilitaryUnit::DoAttack(float DeltaTime) {

}