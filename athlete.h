#include <string>
#include <iostream>
#include <vector>
#include <set>

#define N 4

using namespace std;

static int idCount = 0;

class Athlete {

private:
    int id;
    string firstName;
    string surName;
    float rankPoints;
    // This could be better as a multiset
    //vector<float> racePoints;
    multiset<float> racePoints;

public:
    Athlete( string inFirstName, string inSurName ) {
        id = idCount++;
        firstName  = inFirstName;
        surName = inSurName;
        rankPoints = 0;
    };

    ~Athlete() {
    };


    void modifyName( string inFirstName, string inSurName ) {
        firstName  = inFirstName;
        surName = inSurName;
    }

    void print() const {
        cout << firstName << " " << surName << " " << rankPoints << endl;
        cout << "\t";
//        for ( int i = 0; i < racePoints.size(); i++ ) {
//            cout << racePoints[i] << " ";
//        }
        multiset<float>::iterator rPos = racePoints.begin();
        for ( rPos = racePoints.begin(); rPos != racePoints.end(); rPos++ ) {
            cout << *rPos << " ";
        }
        cout << endl;
    }

    float getRankPoints() const {
        return rankPoints;
    }

    float getPoints( int i ) {
        float output;
        if ( i < 0 || i > N - 1 ) {
            output = -1;
        }
        else {
            multiset<float>::iterator rPos = racePoints.begin();
            for ( int j = 0 ; j < i; j++ ) {
                rPos++;
            }
            output = *rPos;
        }

        return output;
    }

    void givePoints( float inPoints ) {
        racePoints.insert( inPoints );
        if ( racePoints.size() > N  ) {
            multiset<float>::iterator rPos = racePoints.begin();
            racePoints.erase( rPos );
        }

        float athAvg = 0;
        multiset<float>::iterator rPos = racePoints.begin();
        for ( rPos = racePoints.begin(); rPos != racePoints.end(); rPos++ ) {
            athAvg += *rPos;
        }
        rankPoints = athAvg / N;
    }

    bool operator < ( const Athlete& ath) const {
        return ( rankPoints > ath.getRankPoints());
    }

    bool operator > ( const Athlete& ath) const {
        return ( rankPoints < ath.getRankPoints());
    }


};
