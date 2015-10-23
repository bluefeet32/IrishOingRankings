#! /bin/bash

#2503 is http://www.orienteering.ie/result2?oaction=fullSIResult&id=2503
#League table at http://orienteering.ie/results/leagues/show_table.php?league=Family+Day&date_from=04%2F15&date_to=06%2F15&best_of[Black]=1&best_of[Brown]=7&best_of[Blue]=8&best_of[Green]=8&best_of[Red]=8&best_of[Orange]=8&best_of[Yellow]=7&best_of[Brow+n]=1
#FIXME have to watch out for people's names matching courses
for z in 25; do
    mkdir -p $z
    cd $z
#    for i in `seq -w 01 99`; do
    for i in `seq -w 01 10`; do
        echo $i
        curl -s "http://www.orienteering.ie/results/files/$z/${z}${i}.csv" > ${z}${i}.csv
        grep Brown ${z}${i}.csv
        if [[ $? == 0 ]]; then
            sed 's/"//g' ${z}${i}.csv > tmp
            #awk -F ';' '{print $40}' tmp
            touch fullRes
            for course in Brown Blue Green; do 
                echo course $course
                awk -F ';' '{print $4,"\t",$5,"\t",$12"\t",$16,"\t",$40}' tmp | grep $course > ${course}Res
                #awk '{sum += $3; sumsq=$1*$1} END { if (NR > 0) print sqrt(sumsq/NR - (sum/NR)**2)}' ${course}Res #| awk -F ':' '{print $1+$2/60}' | head -n 1
                awk '{print $3}' ${course}Res | awk -F ':' '{print $1+$2/60}' > ${course}Times
                mean=$(awk '{sum+=$1} END { print (sum/NR)}' ${course}Times)
                echo mean $mean
                stdDev=$(awk -v sum=$mean '{array[NR]=$1} END {for(x=1;x<=NR;x++){susq+=((array[x]-sum)**2);}print sqrt(susq/NR)}' ${course}Times)
                echo stdDev $stdDev
                awk -v sDev=$stdDev -v mean=$mean '{if ( $1 <= mean ) { print 1000+(100*(sqrt(($1-mean)**2))/sDev) } else { print 1000-(100*sqrt(($1-mean)**2)/sDev) } }' ${course}Times > ${course}Points
    #            cat ${course}Points
                paste ${course}Res ${course}Points >> pointsRes${z}${i}
            done
        fi
    done
done

#find 

#for line in 1..$(wc -l ${z}${c}); do
touch resultsPoints
lines=$(wc -l 2504tmp.csv | awk '{print $1-1}'
for i in `seq 1 $lines`; do
    (grep $(cat pointsRes2504 | head -n $i | tail -n 1 | awk '{print $1";"$2}') pointsDB || printf "\n") | awk -F ';' '{print $3}'
done

