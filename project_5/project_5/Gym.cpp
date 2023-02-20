#include "Gym.h"

Gym::Gym(std::string name, std::string location, Kind kind)
{
	mName = name;
	mLocation = location;
	mKind = kind;
	hasCardio = false;
	hasWeights = false;
	hasPool = false;
	hasTrack = false;
}
void Gym::setFeatures(bool cardio, bool weights, bool pool, bool track)
{
	hasCardio = cardio;
	hasWeights = weights;
	hasPool = pool;
	hasTrack = track;
}

bool Gym::canWorkoutHere(GymMember m)
{
	if (m.getKind() == OTHER || mKind == OTHER)
	{
		return(m.getKind() == mKind);
	}
	else
	{
		return(m.getKind() >= mKind);
	}
}
bool Gym::checkin(GymMember& m, bool wantsCardio, bool wantsWeights, bool wantsPool, bool wantsTrack)
{
	if (canWorkoutHere(m) && wantsCardio <= hasCardio && wantsWeights <= hasWeights && wantsPool <= hasPool && wantsTrack <= hasTrack)
	{
		m.newWorkout();
		return(true);
	}
	else
	{
		return(false);
	}
}

bool Gym::hasCardioFeatures() const
{
	return(hasCardio);
}
bool Gym::hasWeightsFeatures() const
{
	return(hasWeights);
}
bool Gym::hasPoolFeatures() const
{
	return(hasPool);
}
bool Gym::hasTrackFeatures() const
{
	return(hasTrack);
}

Kind Gym::getKind() const
{
	return(mKind);
}
std::string Gym::getKindAsString() const
{
	std::string kind;
	switch (mKind)
	{
	case 0:
		kind = "OTHER";
		break;
	case 1:
		kind = "REGULAR";
		break;
	case 2:
		kind = "PREMIER";
		break;
	case 3:
		kind = "EXECUTIVE";
		break;
	default:
		break;
	}
	return(kind);
}
std::string Gym::getLocation() const
{
	return(mLocation);
}
std::string Gym::getName() const
{
	return(mName);
}