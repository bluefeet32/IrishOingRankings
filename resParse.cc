// compile with  g++ -o resParse resParse.cc -std=c++11
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "athlete.h"

using namespace std;

int main( void ) {
    ifstream dbFile ("pointsDB");
    string line;
    vector<string> name;
    int i, noEntries;
    // count the number of lines, i.e. number of athletes in the DB
    for( i = 0; getline( dbFile, line ); i++ ) {;}
    cout << "number of lines " << i << endl;
    noEntries = i;
    vector<int> id ( noEntries );

    // We create a vector that contains lists of points
    vector<vector <float> > points;

    // Create an list of Athletes
    vector<Athlete> athList;
    Athlete tmpAth ( "tmp", "tmp" );

    //reset dbFile to the start
    dbFile.clear();
    dbFile.seekg(0, ios::beg);

    // Read the Athletes and their points from the DB
    // the DB file is ; delimted
    // This has the format:
    // surname;forename;rankPoints;race1Points;race2Points;race3Points;race4Points;
    int lineNo = 0;
    while ( dbFile ) {
        if ( !getline( dbFile, line ) ) break;
//        cout << "lineNo " << lineNo << ": " << line << endl;

        istringstream ss( line );
        int col = 0;
        string name;
        while ( ss ) {
            string s;
            // Read a string with ; as the delimiter between strings
            if ( !getline( ss, s, ';' ) ) break;
            // Use a switch to do the appropriate column based action
            switch ( col ) {
                // column 0 is the surname
                case 0:
                    name = s;
                    break;
                // column 1 is the forename
                case 1:
                    tmpAth.modifyName( s, name );
                    name += s;
//                    cout << "name " << name << endl; //points[i];
                    break;
                // column 2 is the athletes current ranking score
                // Note that there is no break as we want give the athlete these points
                case 2:
                    tmpAth.setRankPoints( stof ( s ) );
                case 3:
                case 4:
                case 5:
                case 6:
                    if ( col != 2 ) {
                        tmpAth.givePoints( stof ( s ) );
                    }
                    break;
                default:
                    cout << "Too many columns!" << endl;
            }
            col++;
        }
        // clean up the temporary data structures
        athList.push_back( tmpAth );
        tmpAth.clear();
        lineNo++;
    }

    dbFile.close();

    // Sort the athlete list
    sort( athList.begin(), athList.end() );
    for ( int i = 0; i < athList.size(); i++ ) {
        athList[i].print();
    }
    cout << "\n";
//    cout << "points " << points[0][0] << " " << points[0][1] << endl; //points[i];

//FIXME need to pass an argument to set what this file is
    // Parse the new results file
    // This has the format:
    // points;surname;forename
    ifstream resFile( "25/pointsRes2503" );
    lineNo = 0;
    vector<string> raceName; // Vector containing the names of Athletes for the race
    vector<float> racePoints; // Vector of the points of the athletes for a race
    while ( resFile ) {
        if ( !getline(resFile, line) ) break;
//        cout << "lineNo " << lineNo << ": " << line << endl;

        istringstream ss( line );
        int col = 0;
        while ( ss ) {
            string s;
            if ( !getline( ss, s, ';' ) ) break;
            switch ( col ) {
                case 0:
                    racePoints.push_back( stof( s ) );
                    break;
                case 2:
                    raceName.push_back( s );
                    break;
                case 3:
                    raceName[lineNo] += s;
//                    cout << "racename " << raceName[lineNo] << endl; //points[i];
                    break;
                default:
                    break;
            }
            col++;
        }
        lineNo++;
    }

    resFile.close();

    // Find each raceName in the DBnames list
    long raceSize = raceName.size();

    // Find each racename in the athList
    long athSize = athList.size();
    cout << raceSize << " " << athSize << endl;
    for ( long i = 0; i < raceSize; i++ ) {
        long j;
        for ( j = 0; j < athSize; j++ ) {
            if ( raceName[i] == athList[j].getSearchName() ) {
//                cout << "raceName " << raceName[i] << " athName " << athList[j].getSearchName() << endl;
                athList[j].givePoints( racePoints[i] );
                break;
            }
        }
    }

    // Average all the rows into the first points column
    // Also compute the scaling factor necessary to make the average DB score to be 1000
    for ( int i = 0; i < athList.size(); i++ ) {
        athList[i].print();
    }
    cout << "\n";
    float athSum = 0;
    for ( long i = 0; i < athList.size(); i++ ) {
        athSum += athList[i].getRankPoints();
    }
    // Factor to scale the average to 1000
    float athScale = 1000 / ( athSum / athSize );
    cout << "athScale " << athScale << " is it 1000? " <<  ( athSum / athSize ) * athScale << endl;

//    assert( ( athSum / athSize ) * athScale - 1000 < 1e-6 );

    for ( long j = 0; j < athSize; j++ ) {
        athList[j].scaleAllPoints( athScale );
    }

    // Sort the DB after scaling
    std::sort( athList.begin(), athList.end() );

    cout << "finalDB ";
    for ( int i = 0; i < athList.size(); i++ ) {
        athList[i].print();
    }
    cout << "\n";

    // Create the new DB.
    ofstream newAthDBFile ("newAthPointsDB");
    for ( long j = 0; j < athSize; j++ ) {
        athList[j].printToDbFile( newAthDBFile );
    }
    newAthDBFile.close();


    return 0;
}
