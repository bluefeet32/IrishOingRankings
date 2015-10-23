#! /bin/bash

sed 's/"//g' 2503.csv > tmp
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
    cat ${course}Points
    paste ${course}Res ${course}Points >> fullRes
done
