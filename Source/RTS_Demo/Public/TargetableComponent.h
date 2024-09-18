// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TargetableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_DEMO_API UTargetableComponent : public USceneComponent
{
	GENERATED_BODY()

private:
	int faction;
	int currentHealth;
	int maxHealth;
	void* HealthBar;
	//UPROPERTY(VisibleAnywhere)
	//WB_HealthBar* healthBar;
	//UUserWidget* healthBarWidget;

	void Die();

public:	
	// Sets default values for this component's properties
	UTargetableComponent();
	bool ReceiveDamage(int dmg);
	void SetFaction(int faction);
	int GetFaction();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
