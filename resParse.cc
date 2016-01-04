// compile with  g++ -o resParse resParse.cc -std=c++11
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "athlete.h"

using namespace std;

int main( void ) {
    ifstream dbFile ("pointsDB");
    string line;
    vector<string> fullName;
    vector<string> name;
    int i, noEntries;
    for( i = 0; getline(dbFile, line); i++ ) {;}
    cout << "number of lines " << i << endl;
    noEntries = i;
    vector<int> id (noEntries);

    vector<vector <float> > points;
//    float points[5];

    vector<Athlete> athList;
    Athlete tmpAth ( "tmp", "tmp" );

    dbFile.clear();
    dbFile.seekg(0, ios::beg);
    //reset dbFile to the start
    int lineNo = 0;
    while ( dbFile ) {
        if ( !getline(dbFile, line) ) break;
        cout << "lineNo " << lineNo << ": " << line << endl;

        istringstream ss( line );
        int col = 0;
        vector<float> tmpPoints;
        while ( ss ) {
            string s;
            if ( !getline( ss, s, ';' ) ) break;
            switch ( col ) {
                case 0:
                    name.push_back( s );
                    fullName.push_back( s );
                    break;
                case 1:
                    tmpAth.modifyName( s, name[lineNo] );
                    fullName[lineNo] += ';' + s;
                    name[lineNo] += s;
                    cout << "name " << name[lineNo] << endl; //points[i];
                    break;
                case 2:
                    tmpAth.setRankPoints( stof ( s ) );
                case 3:
                case 4:
                case 5:
                case 6:
//                    s >> points[i-2];
                    if ( col != 2 ) {
                        tmpAth.givePoints( stof ( s ) );
                    }
                    tmpPoints.push_back( stof( s ) );
                    cout << "points " << tmpPoints[col-2] << endl; //points[i];
                    break;
                default:
                    cout << "Too many columns!" << endl;
            }
            col++;
        }
        athList.push_back( tmpAth );
        tmpAth.clear();
        points.push_back( tmpPoints );
        lineNo++;
    }

    dbFile.close();

    // Sort the athlete list
    sort( athList.begin(), athList.end() );
    for ( int i = 0; i < athList.size(); i++ ) {
        athList[i].print();
    }
//    cout << "points " << points[0][0] << " " << points[0][1] << endl; //points[i];

    //parse the new results file
    ifstream resFile("25/pointsRes2503");
    lineNo = 0;
    vector<string> raceName;
    vector<float> racePoints;
    while ( resFile ) {
        if ( !getline(resFile, line) ) break;
        cout << "lineNo " << lineNo << ": " << line << endl;

        istringstream ss( line );
        int col = 0;
        vector<float> tmpPoints;
        while ( ss ) {
            string s;
            if ( !getline( ss, s, ';' ) ) break;
            switch ( col ) {
                case 0:
                    racePoints.push_back( stof( s ) );
                    break;
                case 2:
                    raceName.push_back(s);
                    break;
                case 3:
                    raceName[lineNo] += s;
                    cout << "racename " << raceName[lineNo] << endl; //points[i];
                    break;
                default:
                    break;
            }
            col++;
        }
        lineNo++;
    }

    resFile.close();

    //find raceName in the DBnames list
    long raceSize = raceName.size();
    long dbSize = name.size();
    cout << raceSize << " " << dbSize << endl;

    for ( long i = 0; i < raceSize; i++ ) {
        long j;
        for ( j = 0; j < dbSize; j++ ) {
            if ( raceName[i] == name[j] ) {
                cout << "raceName " << raceName[i] << " dbName " << name[j] << endl;
                break;
            }
        }

        vector<float>::iterator dbPos = points[j].begin() + 1;
        for ( int col = 1; col < points[j].size(); col++ ) {
            if ( racePoints[i] > points[j][col] ) {
                //insert the race points into the vector here
                points[j].insert( dbPos, racePoints[i] );
                if ( points[j].size() > 5 ) {
                    points[j].pop_back();
                }
                break;
            }
            dbPos++;
        }
    }

    cout << "\n";

    long athSize = athList.size();
    for ( long i = 0; i < raceSize; i++ ) {
        long j;
        for ( j = 0; j < athSize; j++ ) {
            if ( raceName[i] == athList[j].getSearchName() ) {
                cout << "raceName " << raceName[i] << " athName " << athList[j].getSearchName() << endl;
                break;
            }
        }
        athList[j].givePoints( racePoints[i] );
    }

    // Average all the rows into the first points column
    // Also compute the scaling factor necessary to make the average DB score to be 1000
    float dbSum = 0;
    dbSize = name.size();
    for ( long j = 0; j < dbSize; j++ ) {
        float sum = 0;
        for ( int i = 0; i < points[j].size(); i++ ) {
            sum += points[j][i];
        }
        points[j][0] = sum / points[j].size();
        dbSum += points[j][0];
    }

    for ( int i = 0; i < athList.size(); i++ ) {
        athList[i].print();
    }
    float athSum = 0;
    for ( long i = 0; i < athList.size(); i++ ) {
        athSum += athList[i].getRankPoints();
    }
    // Factor to scale the average to 1000
    float dbScale = 1000 / ( dbSum / dbSize );
    float athScale = 1000 / ( athSum / athSize );
    cout << "dbScale " << dbScale << " is it 1000? " <<  ( dbSum / dbSize ) * dbScale << endl;
    cout << "athScale " << athScale << " is it 1000? " <<  ( athSum / athSize ) * athScale << endl;

    for ( long j = 0; j < dbSize; j++ ) {
        cout << "finalDB " << name[j];
        for ( int i = 0; i < points[j].size(); i++ ) {
            points[j][i] *= dbScale;
            cout << " " << points[j][i];
        }
        cout << endl;
    }

    for ( long j = 0; j < athSize; j++ ) {
        athList[j].scaleAllPoints( athScale );
        athList[j].print();
    }

    // Sort the DB after scaling

    // Overwrite the old DB with the new one.
    ofstream newDBFile ("newPointsDB");

    for ( long j = 0; j < dbSize; j++ ) {
        newDBFile << fullName[j] << ";";
        for ( int i = 0; i < points[j].size(); i++ ) {
            newDBFile << points[j][i] << ";";
        }
        newDBFile << "\n";
    }

    newDBFile.close();

    return 0;
}
