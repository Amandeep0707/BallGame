#include "World/BallPlayerState.h"

ABallPlayerState::ABallPlayerState()
{
	BankedScore = 0;
	SessionScore = 1000;
}

void ABallPlayerState::AddToSessionScore(int32 ScoreAmount)
{
	SessionScore += ScoreAmount;
	BroadcastScoreChange();
}

void ABallPlayerState::DecrementSessionScore(int32 ScoreAmount)
{
	// Prevent the session score from going massively negative from the ticker.
	SessionScore = FMath::Max(0, SessionScore - ScoreAmount);
	BroadcastScoreChange();
}

void ABallPlayerState::BankSessionScore()
{
	BankedScore += SessionScore;
	// SessionScore = 0;
	BroadcastScoreChange();
}

void ABallPlayerState::ResetSessionScore()
{
	// When you fall, you lose your progress since the last checkpoint.
	SessionScore = 0;
	BroadcastScoreChange();
}

int32 ABallPlayerState::GetTotalScore() const
{
	return SessionScore;
}

void ABallPlayerState::BroadcastScoreChange()
{
	OnScoreChanged.Broadcast(GetTotalScore());
}