#!/bin/sh

startRun=$1
endRun=$2
lumiLimit=$3
jsonPath=$4

if [ "$startRun" == "" ] || [ "$endRun" == "" ] || [ "$lumiLimit" == "" ]; then
  echo "USAGE: ./runsbylumi.sh <startRun> <endRun> <lumiBlock (pb-1)> <json_file:optional>"
  exit 1
fi



echo "Evaluating the list of runs in range [$startRun-$endRun] divided by luminosity of $lumiLimit pb-1"
if [ "$jsonPath" != "" ]; then
  echo "Using JSON file: $jsonPath"
fi

searchString="| *[0-9]\+:[0-9]\+ *| *[0-9]\+ *| *[0-9]\+ *| *[0-9.]\+ *(/[a-z]*)"
if [ "$jsonPath" == "" ];then 
  lumiCalc2.py --begin $startRun --end $endRun delivered >  ~/runsbylumi.log
  else
  lumiCalc2.py -i $jsonPath --begin $startRun --end $endRun delivered >  ~/runsbylumi.log
fi
nRuns=`cat ~/runsbylumi.log | grep -e "| *[0-9]\+:[0-9]\+ *|" | wc -l`
echo "lumiCalc finished with total of $nRuns runs"
echo "Selecting run ranges by luminosity"

totalLumi=0.0
currentLumi=0.0
runList=""
nRuns=0

while read line
do
  string=`grep -o -e "$searchString" <<< $line`
  if [ "$string" == "" ]; then
    continue
  fi
#  echo $line
  run=${string#| *}
  run=${run%%:*}
#  echo "Run: $run"
  lumi=`sed "s/| *[0-9]\+:[0-9]\+ *| *[0-9]\+ *| *[0-9]\+ *| *//" <<< $string`
  unit=`sed "s:.*(/::" <<< $lumi`
  unit=`sed "s:).*::" <<< $unit`
  lumi=`sed "s/ *(.*).*//" <<< $lumi`
#  echo "Lumi: $lumi"

  if [ "$unit" == "fb" ]; then
    scale=1000
  elif [ "$unit" == "pb" ]; then
    scale=1.0
  elif [ "$unit" == "nb" ]; then
    scale=0.001
  elif [ "$unit" == "ub" ]; then
    scale=0.000001
  elif [ "$unit" == "mb" ]; then
    scale=0.000000001
  elif [ "$unit" == "b" ]; then
    scale=0.000000000001
  fi
  
  totalLumi=`echo "$totalLumi + $lumi * $scale" | bc`
  currentLumi=`echo "$currentLumi + $lumi * $scale" | bc`
  compareResult=`echo "$currentLumi >= $lumiLimit" | bc`
#  echo "Lumi: $lumi /$unit Current: $currentLumi"
  if [ $compareResult -gt 0 ]; then
    currentLumi=0.0
    runList+="$run,"
    let "nRuns = nRuns + 1"
  fi

done < ~/runsbylumi.log

let "nRuns = nRuns + 1"
runList=${runList%,}

echo
echo "Runs: [ $startRun,$runList,$endRun ]"
echo "Number of Run ranges: $nRuns"
echo

echo "Total Luminosity: $totalLumi /pb"
echo "From lumiCalc2.py:"
cat ~/runsbylumi.log | grep -e "." | tail -3

rm ~/runsbylumi.log
