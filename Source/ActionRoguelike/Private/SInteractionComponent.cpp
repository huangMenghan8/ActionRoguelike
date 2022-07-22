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

//�ҵ�Ҫ�����Ķ���Ȼ��ִ�ж���Ľ�������
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

	//ɨ=ɨ�赽����Ʒ����
	TArray<FHitResult> Hits;
	//GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(Radius);

	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, CollisionShape);
	
	FColor Color = bBlockingHit == true ? FColor::Green : FColor::Red;
	
	for (auto Hit : Hits)
	{

		AActor* HitActor = Hit.GetActor();
		//����û��ײ������
		if (HitActor) {
			//ײ������֮�������Ʒ�Ľ�������
			//�ȼ��ײ����������û��ʵ�ֽ�������
			if (HitActor->Implements<USGameplayInterface>()) {

				APawn* MyPawn = Cast<APawn>(MyOwner);
				ISGameplayInterface::Execute_Interact(HitActor, MyPawn);
			}
		}

		//ײ�������廭��
		DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 16, Color, false, 2.0f);
	}

	DrawDebugLine(GetWorld(), EyeLocation, End, Color, false, 2.0, 0, 2);
}

