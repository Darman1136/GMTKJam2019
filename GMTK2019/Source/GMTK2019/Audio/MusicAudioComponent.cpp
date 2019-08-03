// Fill out your copyright notice in the Description page of Project Settings.


#include "MusicAudioComponent.h"

UMusicAudioComponent::UMusicAudioComponent() : Super() {
	bIgnoreForFlushing = true;
	bAutoActivate = false;
	bStopWhenOwnerDestroyed = false;
}
