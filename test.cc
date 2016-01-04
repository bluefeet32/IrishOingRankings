#include "athlete.h"

#include <iostream>
#include <vector>
#include <algorithm>

bool athComp( Athlete &ath1, Athlete &ath2 ) {
    return ( ath1.getRankPoints() < ath2.getRankPoints() );
};

int main( void ) {

    vector<Athlete> athleteList;

    Athlete ath( "Ruairi", "Short" );
    athleteList.push_back( ath );

    ath.modifyName( "Colm", "Hill" );
    athleteList.push_back( ath );

    for ( int i = 0; i < athleteList.size(); i++ ) {
        athleteList[i].givePoints( 2*i + 1230 );
        athleteList[i].givePoints( 2*i + 1110 );
        athleteList[i].givePoints( 2*i + 1000 );
        athleteList[i].givePoints( 2*i + 901 );
        athleteList[i].print();

        athleteList[i].givePoints( 2*i + 1130 );
        athleteList[i].print();

    }
    cout << "pre sort" << endl;
    athleteList[0].print();
    athleteList[1].print();

    sort( athleteList.begin(), athleteList.end() );

    cout << "post sort" << endl;
    athleteList[0].print();
    athleteList[1].print();

    cout << "roar's 2 points " << athleteList[1].getPoints(2) << endl;

}
