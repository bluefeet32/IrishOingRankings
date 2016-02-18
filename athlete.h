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

    void clear() {
        firstName  = "Empty";
        surName = "Empty";
        rankPoints = 0;
        racePoints.clear();
    }

    void modifyName( string inFirstName, string inSurName ) {
        firstName  = inFirstName;
        surName = inSurName;
    }

    // Get the name with semicolons removed for searching in files
    string getSearchName() {
        return surName + firstName;
    }

    // Get the surName
    string getSurName() {
        return surName;
    }

    // Get the firstName
    string getFirstName() {
        return firstName;
    }

    void print() const {
        cout << firstName << " " << surName << " " << rankPoints << " ";
//        for ( int i = 0; i < racePoints.size(); i++ ) {
//            cout << racePoints[i] << " ";
//        }
        multiset<float>::iterator rPos = racePoints.begin();
        for ( rPos = racePoints.begin(); rPos != racePoints.end(); rPos++ ) {
            cout << *rPos << " ";
        }
        cout << endl;
    }

    void printToDbFile( ofstream &dbFile ) {
        dbFile << surName << ";" << firstName << ";" << rankPoints << ";";
        multiset<float>::iterator rPos = racePoints.begin();
        for ( rPos = racePoints.begin(); rPos != racePoints.end(); rPos++ ) {
            dbFile << *rPos << ";";
        }
        dbFile << endl;
    }

    float setRankPoints( float inPoints ) {
        rankPoints = inPoints;
    }

    float getRankPoints() const {
        return rankPoints;
    }

    void scaleAllPoints( float scale ) {
        rankPoints *= scale;
        multiset<float>::iterator rPos = racePoints.begin();
        float tmpPoints[4];
        int i = 0;
        for ( rPos = racePoints.begin(); rPos != racePoints.end(); rPos++ ) {
            tmpPoints[i] =  *rPos * scale;
            i++;
        }
        racePoints.clear();

        for ( int j = 0; j < i; j++ ) {
            racePoints.insert( tmpPoints[j] );
        }
    }

    float getPoints( int i ) const {
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
        // Would like the athlete to have the average of only the points that they do have,
        // not the average of 4
        int iter = 0;
        multiset<float>::iterator rPos = racePoints.begin();
        for ( rPos = racePoints.begin(); rPos != racePoints.end(); rPos++ ) {
            athAvg += *rPos;
            iter++;
        }
        rankPoints = athAvg / iter;
    }

    bool operator < ( const Athlete& ath) const {
        return ( rankPoints > ath.getRankPoints() );
    }

    bool operator > ( const Athlete& ath) const {
        return ( rankPoints < ath.getRankPoints() );
    }

//FIXME for now we don't also copy the race points
//    bool operator = ( const Athlete& ath ) const {
//        firstName  = ath.getFirstName();
//        surName    = ath.getSurName();
//        rankPoints = ath.getRankPoints();
//    }

};
