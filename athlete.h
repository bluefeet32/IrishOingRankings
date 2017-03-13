#ifndef ATHLETE_INCL_GUARD
#define ATHLETE_INCL_GUARD

#include <iostream>
#include <vector>
#include <set>
#include <string>

#define N 4

class Athlete {

private:
    int id;
    std::string firstName;
    std::string surName;
    float rankPoints;
    // This could be better as a std::multiset
    //vector<float> racePoints;
    std::multiset<float> racePoints;

public:
    Athlete( std::string inFirstName, std::string inSurName );

    ~Athlete() {};

    void clear();

    void modifyName( std::string inFirstName, std::string inSurName );

    // Get the name with semicolons removed for searching in files
    std::string getSearchName() {
        return surName + firstName;
    }

    // Get the surName
    std::string getSurName() {
        return surName;
    }

    // Get the firstName
    std::string getFirstName() {
        return firstName;
    }

    void print() const;

    void printToDbFile( std::ofstream &dbFile );

    float setRankPoints( float inPoints ) {
        rankPoints = inPoints;
    }

    float getRankPoints() const {
        return rankPoints;
    }

    void scaleAllPoints( float scale );

    float getPoints( int i ) const;

    void givePoints( float inPoints );

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

#endif
