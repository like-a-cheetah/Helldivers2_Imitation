// Fill out your copyright notice in the Description page of Project Settings.


#include "B.h"

AB::AB()
{
	SkinnedMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	SkelMeshComp = Cast<USkeletalMeshComponent>(SkinnedMeshComp);
}
