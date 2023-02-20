#include "GymMember.h"

GymMember::GymMember(std::string name, std::string accountnumber, Kind kind)
{
	mName = name;
	mAccountNumber = accountnumber;
	mKind = kind;
	mWorkoutCount = 0;
}

int GymMember::workoutsThisMonth() const
{
	return(mWorkoutCount);
}
void GymMember::startNewMonth()
{
	mWorkoutCount = 0;
}
void GymMember::newWorkout()
{
	mWorkoutCount++;
}

Kind GymMember::getKind() const
{
	return(mKind);
}
std::string GymMember::getKindAsString() const
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
std::string GymMember::getName() const
{
	return(mName);
}
std::string GymMember::getAccountNumber() const
{
	return(mAccountNumber);
}