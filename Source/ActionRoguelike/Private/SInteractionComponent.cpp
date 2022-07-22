// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"
#include "SGameplayInterface.h"
#include <DrawDebugHelpers.h>

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//找到要互动的对象，然后执行对象的交互函数
void USInteractionComponent::PrimaryInteract()
{

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	AActor* MyOwner = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector End = EyeLocation + (EyeRotation.Vector() * 1000);

	float Radius = 30.0;

	//扫=扫描到的物品集合
	TArray<FHitResult> Hits;
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
	
	FColor Color = bBlockingHit == true ? FColor::Green : FColor::Red;
	
	for (auto Hit : Hits)
	{

		AActor* HitActor = Hit.GetActor();
		//可能没有撞到物体
		if (HitActor) {
			//撞到物体之后调用物品的交互函数
			//先检测撞到的物体有没有实现交互函数
			if (HitActor->Implements<USGameplayInterface>()) {

				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
			}
		}

		//撞到的物体画球
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, Color, false, 2.0f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, Color, false, 2.0, 0, 2);
}

