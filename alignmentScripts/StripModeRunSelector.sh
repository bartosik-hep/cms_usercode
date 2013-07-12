#!/bin/sh

searchMode=$1 #Target mode (deco/peak)
firstRun=$2
lastRun=$3

if [ "$1" = "" ] || [ "$2" = "" ] || [ "$3" = "" ]; then
  echo "  USAGE: $0 <mode> <startRun> <endRun>"
  echo "  <mode>: peak/deco"
  exit
fi

startRun=$firstRun
while [ $startRun -lt $lastRun ]; do
  line=$(python /afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_cand1_jbehr/src/CondFormats/SiStripObjects/test/SiStripLatencyInspector.py $startRun)
  startRun=$(echo $line | grep -o -e "since = [0-9]*" | sed -e "s/since = //")
  endRun=$(echo $line | grep -o -e "till = [0-9]*" | sed -e "s/till = //")
  mode=$(echo $line | grep -o -e "$searchMode")
  if [ "$mode" = "$searchMode" ]; then 
    begin=$startRun
    end=$endRun
    if [ $begin -lt $firstRun ]; then 
      begin=$firstRun
    fi
    if [ $end -gt $lastRun ]; then
      end=$lastRun
    fi
    echo "'$begin:1-$end:max', #$searchMode"
  fi
  startRun=$[$endRun+1]
done
