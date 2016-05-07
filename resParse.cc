// compile with  g++ -o resParse resParse.cc -std=c++11
// Reads from the files: pointsDB and 25/pointsRes2503
// Creates an updated DB with all points rescaled to have an average of 1000
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <assert.h>

#include "athlete.h"

using namespace std;

int main( int argc, char **argv ) {

    if ( argc != 3 ) {
        cout <<  "Usage:\n\t ./resParse Database_file race_file" << endl;
        return 1;
    }

    ifstream dbFile ( argv[1] );
    string line;
//    vector<string> name;
    int i, noEntries;
    // count the number of lines, i.e. number of athletes in the DB
    for( i = 0; getline( dbFile, line ); i++ ) {;}

//    if ( i == 0 ) {
//        cout << "Error: Database file is empty." << endl;
//        return 2;
//    }

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

        // In the case that the athlete had a ranking, but no race scores
        // add their ranking as a race score
        // Check against 3 since after while(ss) won't happen when col == 3 in this case
        if ( col == 3 ) {
            tmpAth.givePoints( tmpAth.getRankPoints() );
        }
        // clean up the temporary data structures
        athList.push_back( tmpAth );
        tmpAth.clear();
        lineNo++;
    }

    dbFile.close();

    // Sort the athlete list
    sort( athList.begin(), athList.end() );
//    for ( int i = 0; i < athList.size(); i++ ) {
//        athList[i].print();
//    }
    cout << "\n";
//    cout << "points " << points[0][0] << " " << points[0][1] << endl; //points[i];

    // Parse the new results file
    // This has the format:
    // points;surname;forename
    ifstream resFile( argv[2] );
    lineNo = 0;
    vector<string> raceName; // Vector containing the names of Athletes for the race
    vector<float> racePoints; // Vector of the points of the athletes for a race
    vector<Athlete> raceAthList; //We use the athlete class to store athletes in. This makes it easy to compare names and create a new Athlete in the DB
    tmpAth.clear();
    while ( resFile ) {
        string name;
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
                    tmpAth.setRankPoints( stof( s ) );
                    break;
                case 2:
                    raceName.push_back( s );
                    name = s;
                    break;
                case 3:
                    raceName[lineNo] += s;
                    tmpAth.modifyName( s, name );
//                    cout << "racename " << raceName[lineNo] << endl; //points[i];
                    break;
                default:
                    break;

            }
            col++;
        }
        // clean up the temporary data structures
        raceAthList.push_back( tmpAth );
        tmpAth.clear();
        lineNo++;
    }

    if ( lineNo == 0 ) {
        cout << "Error: race_file is empty." << endl;
        return 3;
    }

    resFile.close();


    // Find each racename in the athList
    long raceSize = raceName.size();
    bool newAth = true;
//    cout << raceName.size() << " " << athList.size() << endl;
    for ( long i = 0; i < raceSize; i++ ) {
        long j;
        // athSize can change before each time we do this loop
        // need this, e.g. same new athlete runs twice in one day
        long athSize = athList.size();
        newAth = true;
        for ( j = 0; j < athSize; j++ ) {
            if ( raceName[i] == athList[j].getSearchName() ) {
//                cout << "raceName " << raceName[i] << " athName " << athList[j].getSearchName() << endl;
                athList[j].givePoints( racePoints[i] );
                newAth = false;
                break;
            }
        }
        // This athlete wasn't in the database so create them
        if ( newAth == true ) {
            // We only populated their rank points earlier, so this is their only race score
            raceAthList[i].givePoints( raceAthList[i].getRankPoints() );
            raceAthList[i].givePoints( 1000 );
            raceAthList[i].givePoints( 1000 );
            raceAthList[i].givePoints( 1000 );

            athList.push_back( raceAthList[i] );
        }
    }
    long athSize = athList.size();

    // Average all the rows into the first points column
    // Also compute the scaling factor necessary to make the average DB score to be 1000
//    for ( int i = 0; i < athList.size(); i++ ) {
//        athList[i].print();
//    }
    cout << "\n";
    float athSum = 0;
    for ( long i = 0; i < athList.size(); i++ ) {
        athSum += athList[i].getRankPoints();
    }
    // Factor to scale the average to 1000
    float athScale = 1000 / ( athSum / athSize );
    cout << "athScale " << athScale << " is it 1000? " <<  ( athSum / athSize ) * athScale << endl;

//    assert( ( athSum / athSize ) * athScale - 1000 < 1e-6 );

//    for ( long j = 0; j < athSize; j++ ) {
//        athList[j].scaleAllPoints( athScale );
//    }

    // Sort the DB after scaling
    std::sort( athList.begin(), athList.end() );

    cout << "finalDB ";
//    for ( int i = 0; i < athList.size(); i++ ) {
    for ( int i = 0; i < 1; i++ ) {
        athList[i].print();
    }
    cout << "\n";

    // Create the new DB.
//    ofstream newAthDBFile ("newAthPointsDB");
    ofstream newAthDBFile ( argv[1]);
    for ( long j = 0; j < athSize; j++ ) {
        athList[j].printToDbFile( newAthDBFile );
    }
    newAthDBFile.close();


    return 0;
}
