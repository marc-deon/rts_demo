// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UnitState.h"
#include <vector>
//#include "TargetableComponent.h"

#include "MilitaryUnit.generated.h"

class UTargetableComponent;
//class USphereCollision;

enum RTS_DEMO_API UnitState : uint8 {
	IDLE = 0 UMETA(DisplayName = "IDLE"),
	MOVING = 1 UMETA(DisplayName = "MOVING"),
	ATTACKING = 2 UMETA(DisplayName = "ATTACKING"),
	DYING = 3 UMETA(DisplayName = "DYING")
};


UCLASS()
class RTS_DEMO_API AMilitaryUnit : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	UTargetableComponent* targetableComp;
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* attackRangeSphere;
	

	FVector navTarget;
	int faction;
	bool selected;
	UTargetableComponent* attackTarget;
	std::vector<UTargetableComponent*> targetsInRange;
	UnitState currentState;

	//UTargetable

public:
	UPROPERTY(EditAnywhere)
	float attackRange;
	UPROPERTY(EditAnywhere)
	int attackDamage;
	UPROPERTY(EditAnywhere)
	float attackSpeed;
	UPROPERTY(EditAnywhere)
	float attackTimer;


public:
	AMilitaryUnit();
	AMilitaryUnit(int faction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	bool IsOwnedBy(int player);
	bool CheckIfValidTarget(AActor* potentionalTarget);
	std::vector<UTargetableComponent*> GetTargetsInRange();
	void BeginAttack();
	// previously: Returns true if target died
	// Should: do nothing; we will respond to target death via some kind of event
	void DoAttack(float DeltaTime);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
