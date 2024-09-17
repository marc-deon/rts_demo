// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetableComponent.h"

// Sets default values for this component's properties
UTargetableComponent::UTargetableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	currentHealth = maxHealth;
}

bool UTargetableComponent::ReceiveDamage(int dmg) {
	currentHealth -= dmg;
	if (currentHealth <= 0) {
		Die();
		return true;
	}

	float percentage = (float)currentHealth / maxHealth;
	//HealthBar->UpdateHealthBar(percentage);
	return false;

}

// TODO: Test
void UTargetableComponent::Die() {
	GetAttachParentActor()->Destroy();
}

void UTargetableComponent::SetFaction(int f) {
	this->faction = f;
}

int UTargetableComponent::GetFaction() {
	return faction;
}

// Called when the game starts
void UTargetableComponent::BeginPlay()
{
	Super::BeginPlay();

	//HealthBar->UpdateHealthBar(1.0f);
	//HealthBar->SetNewfaction(faction);
	
}


// Called every frame
void UTargetableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Display, TEXT("Component Tick"));
}

