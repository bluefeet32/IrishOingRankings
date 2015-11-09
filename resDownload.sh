#! /bin/bash

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
    for i in `seq -w 03 03`; do
        echo $i
        # pull the results
        echo "Downloading results"
#        curl -s "http://www.orienteering.ie/results/files/$z/${z}${i}.csv" > ${z}${i}.csv
#        grep Brown ${z}${i}.csv
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
            for line in `seq -w 2 $lines`; do
                (grep $(head -n $line tmp | tail -n 1 | awk -F ';' '{print $4";"$5}') pointsDB || printf "\n") | awk -F ';' '{print $3}' >> resultsPoints
            done
            echo "pasting"
            # Paste the DB points into the csv file
            paste -d ";" resultsPoints tmp > newTmp #${z}${i}.csv
            mv newTmp tmp
            touch fullRes
            for course in Brown Blue Green; do 
                echo course $course
                #must add last column of runnings rankings at this points
                awk -F ';' '{print $1";"$5";"$6";"$13";"$17";"$41}' tmp | grep $course > ${course}Res
                #awk '{sum += $3; sumsq=$1*$1} END { if (NR > 0) print sqrt(sumsq/NR - (sum/NR)**2)}' ${course}Res #| awk -F ':' '{print $1+$2/60}' | head -n 1
                awk -F ';' '{print $4}' ${course}Res | awk -F ':' '{print $1+$2/60}' > ${course}Times
                mean=$(awk '{sum+=$1} END { print (sum/NR) }' ${course}Times)
                echo mean $mean
                stdDev=$(awk -v sum=$mean '{array[NR]=$1} END {for(x=1;x<=NR;x++) {susq+=((array[x]-sum)**2);}print sqrt(susq/NR)}' ${course}Times)
                echo stdDev $stdDev
                awk -v sDev=$stdDev -v mean=$mean '{if ( $1 <= mean ) { print 1000+(100*(sqrt(($1-mean)**2))/sDev) } else { print 1000-(100*sqrt(($1-mean)**2)/sDev) } }' ${course}Times > ${course}Points
#    #            cat ${course}Points
#                paste ${course}Res ${course}Points >> pointsRes${z}${i}
            done
        fi
    done
done

#find 

#for line in 1..$(wc -l ${z}${c}); do

