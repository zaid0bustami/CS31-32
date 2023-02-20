#ifndef HEADER_G
#define HEADER_G

#include "GymMember.h"
#include "Kind.h"

class Gym
{
private:
	std::string mName;
	std::string mLocation;
	Kind mKind;

	bool hasCardio;
	bool hasWeights;
	bool hasPool;
	bool hasTrack;

public:
	Gym(std::string name, std::string location, Kind kind);
	void setFeatures(bool cardio, bool weights, bool pool, bool track);

	bool canWorkoutHere(GymMember m);
	bool checkin(GymMember& m, bool wantsCardio, bool wantsWeights, bool wantsPool, bool wantsTrack);

	bool hasCardioFeatures() const;
	bool hasWeightsFeatures() const;
	bool hasPoolFeatures() const;
	bool hasTrackFeatures() const;

	Kind getKind() const;
	std::string getKindAsString() const;
	std::string getLocation() const;
	std::string getName() const;
};

#endif // !HEADER_G
