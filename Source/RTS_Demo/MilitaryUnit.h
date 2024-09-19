// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <vector>
//#include "TargetableComponent.h"


#include "MilitaryUnit.generated.h"

class UTargetableComponent;
//class USphereCollision;

UENUM()
//enum RTS_DEMO_API EUnitState : uint8 {
enum EUnitState {
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
	//UPROPERTY(VisibleAnywhere)
	//class UStaticMeshComponent* basicMesh;

	UPROPERTY(VisibleAnywhere)
	class UStateTreeComponent* stateTree;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* selectedIndicator;


	bool selected;
	UTargetableComponent* attackTarget = NULL;
	TArray<UTargetableComponent*> targetsInRange;

	//UTargetable

public:

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	int faction;
	UPROPERTY(BlueprintReadOnly)
	FVector navTarget;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EUnitState> currentState;
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
	
public:	
	UFUNCTION(BlueprintCallable)
	void OnSelect();
	UFUNCTION(BlueprintCallable)
	void OnDeselect();
	UFUNCTION(BlueprintCallable)
	bool IsOwnedBy(int player);
	UFUNCTION(BlueprintCallable)
	bool CheckIfValidTarget(AActor* potentionalTarget);
	UFUNCTION(BlueprintCallable)
	TArray<UTargetableComponent*> GetTargetsInRange();
	UFUNCTION(BlueprintCallable)
	void BeginAttack();
	// previously: Returns true if target died
	// Should: do nothing; we will respond to target death via some kind of event
	UFUNCTION(BlueprintCallable)
	bool DoAttack(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void SetNavTarget(FVector location);
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
