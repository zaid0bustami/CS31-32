#ifndef HEADER_M
#define HEADER_M

#include <string>
#include "Kind.h"

class GymMember
{
private:
	std::string mName;
	std::string mAccountNumber;
	Kind mKind;
	int mWorkoutCount;

public:
	GymMember(std::string name, std::string accountnumber, Kind kind);

	int workoutsThisMonth() const;
	void startNewMonth();
	void newWorkout();

	Kind getKind() const;
	std::string getKindAsString() const;
	std::string getName() const;
	std::string getAccountNumber() const;
};

#endif // !HEADER_M
