// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <vector>
#include "TargetableComponent.h"


#include "MilitaryUnit.generated.h"

//class UTargetableComponent;
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
	class UStateTreeComponent* stateTree;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* selectedIndicator;

	bool selected;
	UTargetableComponent* attackTarget = NULL;
	TArray<UTargetableComponent*> targetsInRange;

	//UTargetable

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTargetableComponent* targetableComp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class USphereComponent* attackRangeSphere;

	UPROPERTY(BlueprintReadWrite, Meta = (ExposeOnSpawn = "true"))
	int faction;
	UPROPERTY(BlueprintReadOnly)
	FVector navTarget;
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EUnitState> currentState;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float attackRange;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int attackDamage;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float attackSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float attackTimer;
	

public:
	AMilitaryUnit();
	AMilitaryUnit(int faction);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	UFUNCTION()
	void CB_OnComponentBeginOverlap(UPrimitiveComponent* overlapped, AActor* other_actor, UPrimitiveComponent* other_comp, int other_body_index, bool from_sweep, const FHitResult& sweep_result);
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
