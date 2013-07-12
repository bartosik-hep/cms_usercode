#!/bin/sh

inputFilePath="runs.txt"
outputFilePath="lumisInIOV.C"
jsonFile=$1

echo "Starting to calculate luminosities for runs in file '$inputFilePath'"


nRangesToCalc=`grep -e "[0-9]*" $inputFilePath | wc -l`
let "nRangesToCalc = nRangesToCalc - 1"
nRangesCalc=0
run1=-1
run2=-1

while read line 
do

  run2=$line

  if [ $run1 -lt 0  ] || [ $run2 -lt 0 ]; then
    echo "First or last run not set. Skipping"
    run1=$run2
    continue
  fi

  let "nRangesCalc = nRangesCalc + 1"

  searchString="^  lumis\[$run1]\[$run2]"
  isInFile=`grep -oE "$searchString" $outputFilePath | wc -l`

  if [ $isInFile -gt 0 ]; then
    echo "Lumi for runs:  $run1 - $run2 already in the list"
#run1=$run2
    continue
  fi

  echo "Checking lumi in runs: $run1 - $run2  ($nRangesCalc/$nRangesToCalc)"
  if [ "a$jsonFile" = "a" ]; then 
    lumiCalc2.py --begin $run1 --end $run2 delivered >  ~/lumis.log
  else
    lumiCalc2.py --begin $run1 --end $run2 -i $jsonFile delivered >  ~/lumis.log
  fi

  totalLumi=0.0
  currentLumi=0.0
  runList=""
  nRuns=0

  searchString="| *[0-9]\+:[0-9]\+ *| *[0-9]\+ *| *[0-9]\+ *| *[0-9.]\+ *(/[a-z]*)"
  
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

#    echo "Lumi: $lumi /$unit Current: $currentLumi"
  
  done < ~/lumis.log
  
#  cat ~/lumis.log | tail -n 2

#  echo "   Lumi: $currentLumi"
#  if [ $nRangesCalc -eq 1 ]; then
#    echo "lumiInIOV.push_back($currentLumi);" > $outputFilePath
#  else
#    echo "lumiInIOV.push_back($currentLumi);" >> $outputFilePath
#  fi
  echo "    Lumi: $currentLumi"
  if [ "$currentLumi" == "0" ]; then
    continue
  fi
  writeString="lumis[$run1][$run2] = $currentLumi;"

  perl -pi -e "s/.*/\n  $writeString/g if $. == 7" $outputFilePath

#run1=$run2
  rm ~/lumis.log

done < $inputFilePath

# perl -pi -e "s/.*/\n  /g if $. == 7" $outputFilePath

echo "Luminosities of each run range have been stored in file: $outputFilePath"
echo
