#include "../race.h"

int main() {

    Race race( "../25/2501.csv", "../pointsDB", true);
//    Race race( "../25/2501.csv", false);

    race.computePoints();

    return 0;
}
