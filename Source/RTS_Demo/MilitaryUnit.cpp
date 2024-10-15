// Fill out your copyright notice in the Description page of Project Settings.

#include "MilitaryUnit.h"
#include "TargetableComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StateTreeComponent.h"
#include "GameplayTagsManager.h"
#include "StateTreeEvents.h"


//FScriptDelegate Foo2;

void AMilitaryUnit::CB_OnComponentBeginOverlap(UPrimitiveComponent* overlapped, AActor* other_actor, UPrimitiveComponent* other_comp, int other_body_index, bool from_sweep, const FHitResult& sweep_result) {
	//UE_LOG(LogTemp, Display, TEXT("Ping"));

	if (!other_actor) {
		UE_LOG(LogTemp, Display, TEXT("other actor null"));
		return;
	}

	if (other_actor == this) {
		UE_LOG(LogTemp, Display, TEXT("same hat"));
		return;
	}

	auto target = other_actor->GetComponentByClass<UTargetableComponent>();

	// Need to change this function to take a UTargetableComponent
	if (!CheckIfValidTarget(target)) {
		UE_LOG(LogTemp, Display, TEXT("other actor invalid target"));
		return;
	}

	if (!target) {
		UE_LOG(LogTemp, Error, TEXT("Attempting to target actor without a TargetableComponent"));
		return;
	}
	targetsInRange.Add(target);
	UE_LOG(LogTemp, Display, TEXT("Added to targetsInRange"));
}

// Sets default values
AMilitaryUnit::AMilitaryUnit()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	selected = false;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	//UE_LOG(LogTemp, Display, TEXT("A:TC-loc %p"), targetableComp);
	targetableComp = CreateDefaultSubobject<UTargetableComponent>(TEXT("Cpp Targetable Component"));
	//UE_LOG(LogTemp, Display, TEXT("B: TC-loc %p"), targetableComp);
	targetableComp->SetupAttachment(RootComponent);
	//UE_LOG(LogTemp, Display, TEXT("C: TC-loc %p"), targetableComp);
	targetableComp->SetFaction(faction);

	attackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
	attackRangeSphere->SetupAttachment(RootComponent);
	
	stateTree = CreateDefaultSubobject<UStateTreeComponent>(TEXT("state tree"));
	stateTree->SetStartLogicAutomatically(true);

	selectedIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Select Indicator"));
	selectedIndicator->SetupAttachment(RootComponent);

	UE_LOG(LogTemp, Display, TEXT("Setup completed"));
}

// Called when the game starts or when spawned
void AMilitaryUnit::BeginPlay()
{
	Super::BeginPlay();
	/*Foo2.BindUFunction(this, FName("CB_OnComponentBeginOverlap"));
	attackRangeSphere->OnComponentBeginOverlap.Add(Foo2);*/
	attackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AMilitaryUnit::CB_OnComponentBeginOverlap);
	attackRangeSphere->bHiddenInGame = false;
	attackRangeSphere->SetSphereRadius(attackRange);
	attackRangeSphere->SetGenerateOverlapEvents(true);
	targetableComp->bHiddenInGame = false;
	targetableComp->SetSphereRadius(1);
	targetableComp->SetGenerateOverlapEvents(true);
	/*auto fooa = attackRangeSphere->GetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1);
	auto foob = targetableComp->GetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1);*/
	//UE_LOG(LogTemp, Display, TEXT("D: ARSCR %d TCCR %d"), fooa, foob);
	//UE_LOG(LogTemp, Display, TEXT("Z: TC-loc %p"), targetableComp);
	//UE_LOG(LogTemp, Display, TEXT("ARS-Gen %d TC-Gen %d"), attackRangeSphere->GetGenerateOverlapEvents(), targetableComp->GetGenerateOverlapEvents());
	//UE_LOG(LogTemp, Display, TEXT("attack radius %f"), attackRangeSphere->GetUnscaledSphereRadius());
	
}

AMilitaryUnit::AMilitaryUnit(int faction)
{
	AMilitaryUnit();
	this->faction = faction;

}

// TODO NEXT
void AMilitaryUnit::SetNavTarget(FVector location) {
	navTarget = location;
	//UE_LOG(LogTemp, Display, TEXT("Setting nav target"));
	//FName name = FName("Found New Target");
	const FGameplayTag gpt = UGameplayTagsManager::Get().RequestGameplayTag(FName("New Nav Target"), true);
	//FStateTreeEvent()
	stateTree->SendStateTreeEvent(FStateTreeEvent(gpt));
}

void AMilitaryUnit::OnSelect() {
	selected = true;
	//UE_LOG(LogTemp, Display, TEXT("Selected"));
	selectedIndicator->SetVisibility(true);
}

void AMilitaryUnit::OnDeselect() {
	selected = false;
	//UE_LOG(LogTemp, Display, TEXT("Deselected"));
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

	//UE_LOG(LogTemp, Display, TEXT("Status %d"), stateTree->IsRunning());

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
	//return false;
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