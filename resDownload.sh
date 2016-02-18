#! /bin/bash

#source dbModify.sh

#2503 is http://www.orienteering.ie/result2?oaction=fullSIResult&id=2503
#League table at http://orienteering.ie/results/leagues/show_table.php?league=Family+Day&date_from=04%2F15&date_to=06%2F15&best_of[Black]=1&best_of[Brown]=7&best_of[Blue]=8&best_of[Green]=8&best_of[Red]=8&best_of[Orange]=8&best_of[Yellow]=7&best_of[Brow+n]=1
#FIXME have to watch out for people's names matching courses
# mimick o.ie folder structure
for z in 25; do
    #make a new folder with this number name
    mkdir -p $z
    cd $z
#    for i in `seq -w 01 99`; do
    # iterate over the results in this folder on o.ie
#    for i in `seq -w 03 05`; do
    for i in `seq -w 03 05`; do
        echo $i
        # pull the results
        echo "Downloading results"
        curl -s "http://www.orienteering.ie/results/files/$z/${z}${i}.csv" > ${z}${i}.csv
        if [ -f ${z}${i}.csv ]; then
    #FIXME for now we only do results on Brown
            grep Brown ${z}${i}.csv
            if [[ $? == 0 ]]; then
                # We remove any quotation marks
                sed 's/"//g' ${z}${i}.csv > tmp
                #awk -F ';' '{print $40}' tmp
                touch resultsPoints
                printf "\n" > resultsPoints
                # count the number of lines in the csv file
                lines=$(wc -l ${z}${i}.csv | awk '{print $1-1}')
                # for each line, find the DB points for this competitor and print them in resultsPoints, if no entry print a new line
                # The first line is headers so we skip that
                echo "Finding points from DB"
                dbFile="../pointsDB"
                if [ -f $dbFile ]; then
                    for line in `seq -w 2 $lines`; do
                        (grep $(head -n $line tmp | tail -n 1 | awk -F ';' '{print $4";"$5}') $dbFile || printf "\n") | awk -F ';' '{print $3}' >> resultsPoints
                    done
                else
                    # There is no database file so we will initialise the system by giving all of these competitors 1000 points
                    for line in `seq -w 2 $lines`; do
                        echo "1000" >> resultsPoints
                    done
                fi
#FIXME what about when none of the compatitors were in the DB? How will the points be calculated for this race?
                echo "pasting"
                # Paste the DB points into the csv file
                paste -d ";" resultsPoints tmp > newTmp #${z}${i}.csv
                mv newTmp tmp
                touch fullRes
                rm pointsRes${z}${i}
                touch pointsRes${z}${i}
                for course in Brown; do # Blue Green; do
                    echo course $course
                    #must add last column of runnings rankings at this points
                    #columns we select are:
                    #   ranking points; surname; forename; time; club; course
                    awk -F ';' '{print $1";"$5";"$6";"$13";"$17";"$41}' tmp | grep $course > ${course}Res
                    #awk '{sum += $3; sumsq=$1*$1} END { if (NR > 0) print sqrt(sumsq/NR - (sum/NR)**2)}' ${course}Res #| awk -F ':' '{print $1+$2/60}' | head -n 1
                    awk -F ';' '{print $4}' ${course}Res | awk -F ':' '{print $1+$2/60}' > ${course}Times
                    meanPoints=$(awk '{sum+=$1} END { print (sum/NR) }' ${course}Res)
                    stdDevPoints=$(awk -v sum=$meanPoints '{array[NR]=$1} END {for(x=1;x<=NR;x++) {susq+=((array[x]-sum)**2);}print sqrt(susq/NR)}' ${course}Res)
                    echo meanPoints $meanPoints
                    echo stdDevPoints $stdDevPoints
                    mean=$(awk '{sum+=$1} END { print (sum/NR) }' ${course}Times)
                    echo mean $mean
                    stdDev=$(awk -v sum=$mean '{array[NR]=$1} END {for(x=1;x<=NR;x++) {susq+=((array[x]-sum)**2);}print sqrt(susq/NR)}' ${course}Times)
                    echo stdDev $stdDev
                    awk -v sDev=$stdDev -v mean=$mean -v mP=$meanPoints sP=$stdDevPoints'{ print 1000+(100*(mean - $1)/sDev) }' ${course}Times > ${course}Points
                    #Points = mP + ( sP * ( - time + meanTime ) ) / sDev
                    awk -v sDev=$stdDev -v mean=$mean -v mP=$meanPoints sP=$stdDevPoints'{ print mP + ( sP * ( mean - $1 ) / sDev ) }' ${course}Times > ${course}RankPoints
                    # Ensure 0 is the lowest possible score
                    awk '{if ( $1 < 0 ) { print 0 } else { print $1 } }' ${course}RankPoints > ${course}tmp
                    mv ${course}tmp ${course}RankPoints

                    # Paste the points into the result list
                    paste -d ";" ${course}RankPoints ${course}Res >> pointsRes${z}${i}

                done

                cd ..
                ./resParse ./newAthPointsDB ${z}/pointsRes${z}${i}
                cd $z
            fi
        fi
    done
done

#find 

#for line in 1..$(wc -l ${z}${c}); do

#old points calculation
#                awk -v sDev=$stdDev -v mean=$mean -v mP=$meanPoints sP=$stdDevPoints'{if ( $1 <= mean ) { print 1000+(100*(sqrt(($1-mean)**2))/sDev) } else { print 1000-(100*sqrt(($1-mean)**2)/sDev) } }' ${course}Times > ${course}Points
#                awk -v sDev=$stdDev -v mean=$mean -v mP=$meanPoints sP=$stdDevPoints'{if ( $1 <= mean ) { print mP+(sP*(sqrt(($1-mean)**2))/sDev) } else { print mP-(sP*sqrt(($1-mean)**2)/sDev) } }' ${course}Times > ${course}RankPoints
