// Fill out your copyright notice in the Description page of Project Settings.

#include "MilitaryUnit.h"
#include "TargetableComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StateTreeComponent.h"
//#include "NativeGameplayTags.h"
#include "GameplayTagsManager.h"
#include "StateTreeEvents.h"

// Sets default values
AMilitaryUnit::AMilitaryUnit()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	selected = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	targetableComp = CreateDefaultSubobject<UTargetableComponent>(TEXT("CPP Targetable Component"));
	targetableComp->SetupAttachment(RootComponent);
	targetableComp->SetFaction(faction);

	attackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
	attackRangeSphere->SetupAttachment(RootComponent);
	attackRangeSphere->SetSphereRadius(attackRange);

	/*basicMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("cube"));
	basicMesh->SetupAttachment(RootComponent);*/

	stateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("state tree"));
	stateTree->SetStartLogicAutomatically(true);
	//stateTree->
	//stateTree->SetupAttachment(RootComponent);

	selectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Select Indicator"));
	selectedIndicator->SetupAttachment(RootComponent);

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

// TODO NEXT
void AMilitaryUnit::SetNavTarget(FVector location) {
	navTarget = location;
	UE_LOG(LogTemp, Display, TEXT("Setting nav target"));
	//FName name = FName("Found New Target");
	const FGameplayTag gpt = UGameplayTagsManager::Get().RequestGameplayTag(FName("New Nav Target"), true);
	//FStateTreeEvent()
	stateTree->SendStateTreeEvent(FStateTreeEvent(gpt));
}

void AMilitaryUnit::OnSelect() {
	selected = true;
	UE_LOG(LogTemp, Display, TEXT("Selected"));
	selectedIndicator->SetVisibility(true);
}

void AMilitaryUnit::OnDeselect() {
	selected = false;
	UE_LOG(LogTemp, Display, TEXT("Selected"));
	selectedIndicator->SetVisibility(false);
}

int foo = 0;
// Called every frame
void AMilitaryUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (foo == 0 && !stateTree->IsRunning()) {
	//	foo = 20;
	//	stateTree->StartLogic();
	//}
	//else {
	//	foo--;
	//}

	UE_LOG(LogTemp, Display, TEXT("Status %d"), stateTree->IsRunning());

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
	return false;
//	// Attempt to cast PT to Targetable
//	UTargetableComponent* pt = Cast<UTargetableComponent>(potentialTarget);
//	
//	// Can't target something that's not targetable
//	if (pt == NULL)
//		return false;
//
//	// Can't target allies
//	if (pt->GetFaction() == faction)
//		return false;
//
//	// Get the (2D) distance from us to target
//float distSquared = Get2DDistanceSquared(GetActorLocation(), pt->GetAttachParentActor()->GetActorLocation());
//	// Can't target out of range
//	if (distSquared > pow(attackRange, 2))
//		return false;
//
//	return true;
//	//return false;
}

TArray<UTargetableComponent*> AMilitaryUnit::GetTargetsInRange() {
	return targetsInRange;
}

void AMilitaryUnit::BeginAttack() {
	currentState = EUnitState::ATTACKING;
	attackTimer = attackSpeed;
}

bool AMilitaryUnit::DoAttack(float DeltaTime) {
	attackTimer -= DeltaTime;

	if (attackTimer > 0)
		return false;

	attackTimer += attackSpeed;
	if (!attackTarget) {
		UE_LOG(LogTemp, Error, TEXT("invalid target"));
		return false;
	}
	return attackTarget->ReceiveDamage(attackDamage);
}