#include <iostream>
#include <string>
#include <cassert>

#include "Kind.h"
#include "Gym.h"
#include "GymMember.h"

using namespace std;

int main()
{
    GymMember me("Zaid", "12345", PREMIER);
    GymMember notme("Gina", "54321", EXECUTIVE);
    GymMember definitelynotme("Janet", "123qwerty", OTHER);

    Gym mine("LA Fitness", "123 Gayley", OTHER);
    Gym notmine("Planet Fitness", "321 Wilshire", EXECUTIVE);
    Gym definitelynotmine("BFit", "456 Le Conte", REGULAR);

    assert(me.workoutsThisMonth() == 0);
    me.newWorkout();
    assert(me.workoutsThisMonth() == 1);
    me.startNewMonth();
    assert(me.workoutsThisMonth() == 0);
    assert(me.getKind() == PREMIER);
    assert(me.getKindAsString() == "PREMIER");
    assert(me.getName() == "Zaid");
    assert(me.getAccountNumber() == "12345");

    assert(mine.hasCardioFeatures() == false);
    assert(mine.hasWeightsFeatures() == false);
    assert(mine.hasPoolFeatures() == false);
    assert(mine.hasTrackFeatures() == false);

    mine.setFeatures(true, true, false, false);
    assert(mine.hasCardioFeatures() == true);
    assert(mine.hasWeightsFeatures() == true);
    assert(mine.hasPoolFeatures() == false);
    assert(mine.hasTrackFeatures() == false);
    assert(mine.getKind() == OTHER);
    assert(mine.getKindAsString() == "OTHER");
    assert(mine.getLocation() == "123 Gayley");
    assert(mine.getName() == "LA Fitness");

    assert(mine.canWorkoutHere(me) == false);
    assert(mine.canWorkoutHere(notme) == false);
    assert(mine.canWorkoutHere(definitelynotme) == true);
    assert(mine.checkin(me, true, false, false, false) == false);
    assert(me.workoutsThisMonth() == 0);
    assert(mine.checkin(notme, true, false, false, false) == false);
    assert(notme.workoutsThisMonth() == 0);
    assert(mine.checkin(definitelynotme, true, false, false, false) == true);
    assert(definitelynotme.workoutsThisMonth() == 1);

    notmine.setFeatures(true, true, false, false);
    assert(notmine.canWorkoutHere(me) == false);
    assert(notmine.canWorkoutHere(notme) == true);
    assert(notmine.canWorkoutHere(definitelynotme) == false);
    assert(notmine.checkin(me, true, false, false, false) == false);
    assert(me.workoutsThisMonth() == 0);
    assert(notmine.checkin(notme, true, true, true, false) == false);
    assert(notme.workoutsThisMonth() == 0);
    assert(notmine.checkin(definitelynotme, true, false, false, false) == false);
    assert(definitelynotme.workoutsThisMonth() == 1);

    definitelynotmine.setFeatures(true, true, false, false);
    assert(definitelynotmine.canWorkoutHere(me) == true);
    assert(definitelynotmine.canWorkoutHere(notme) == true);
    assert(definitelynotmine.canWorkoutHere(definitelynotme) == false);
    assert(definitelynotmine.checkin(me, false, false, false, true) == false);
    assert(me.workoutsThisMonth() == 0);
    assert(definitelynotmine.checkin(notme, false, true, false, false) == true);
    assert(notme.workoutsThisMonth() == 1);
    assert(definitelynotmine.checkin(definitelynotme, true, false, false, false) == false);
    assert(definitelynotme.workoutsThisMonth() == 1);

    cout << "Passed! :)" << endl;

    return(0);
}
