#pragma once

#include "Engine.h"

#define Print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, text)

static void LogInfo(const FString& text)
{
	UE_LOG(LogBerserk, Log, TEXT("[LogBerserk] %s"), *(FString)text);
}

static void LogWarning(const FString& text)
{
	UE_LOG(LogBerserk, Warning, TEXT("[LogBerserk] %s"), *(FString)text);
}
