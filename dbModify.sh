#!/bin/bash
dbModify(z, i) {
    # Now we need to add the new points to the DB
    # count the number of lines in the csv file
    file=pointsRes${z}${i}
    lines=$(wc -l file | awk '{print $1-1}')
    #file with all the points of an athlete
    touch athPoints
    # for each line, find the DB points for this competitor and print them in resultsPoints, if no entry print a new line
    # The first line is headers so we skip that
    for line in `seq -w 2 $lines`; do
        racePoints=$( awk -F ';' '{print }' file )
        (grep $(head -n $line tmp | tail -n 1 | awk -F ';' '{print $4";"$5}') pointsDB || printf "\n") | awk -F ';' '{print $3;$4;$5;$6;$7}' >> athPoints
    done
}
