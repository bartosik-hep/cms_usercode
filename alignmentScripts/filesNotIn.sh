#!/bin/sh

doReverse=false

# Identifiying the options
set -- $(getopt r "$@")
while [ $# -gt 0 ]
do
    case "$1" in
    (-r) doReverse=true;;
    (--) shift; break;;
    (-*) echo "$0: error - unrecognized option $1" 1>&2; exit 1;;
    (*)  break;;
    esac
    shift
done

geometryBase="/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction"
outputFile=dataset

if [ "x$1" == "x" ] || [ "x$2" == "x" ]; then
  echo "Makes a copy of the input dataset file, removing from it all files that are in any of the specified geometries"
  echo
  echo "USAGE: ./${0##/*/} [-r] <dataset.py> <mpxxxx>"
  echo "   <dataset>  : path to the input dataset file"
  echo "   <mpxxxx>   : name of the geometry in $geometryBase"
  echo "   -r	      : reverse mode - leaving only files that are in all of the specified geometries "
  echo
  exit 0
fi


# Getting the initial array of arguments - geometries
geoms_=( $@ )
nGeoms=${#geoms_[@]}
inputFile=$1
# Removing the first argument that is the dataset file
pos=0
geoms=(${geoms_[@]:0:$pos} ${geoms_[@]:$(($pos + 1))})
nGeoms=${#geoms[@]}


echo "  Checking files in the dataset: $inputFile"
if $doReverse; then
  echo "  Looking for files that are in each of the following geometries ($nGeoms):"
else
  echo "  Looking for files that are not in any of the following geometries ($nGeoms):"
fi

fileName="./"${inputFile##/*/}
fileName=${fileName%_cff.py}
if $doReverse ; then
  fileName=$fileName"_FilesIn"
else 
  fileName=$fileName"_FilesNotIn"
fi

for geom in "${geoms[@]}"; do
  echo $geom
  fileName=$fileName"_$geom"
done
fileName=$fileName"_cff.py"


dtime="$(date +%Y_%m_%d_%H_%M)"

if [ -e $fileName ]; then
  newFileName=$fileName"_$dtime"
  echo "Renaming $fileName to $newFileName"
  mv $fileName $newFileName
fi

#for i in $(cat $inputFile); do
while read i; do
    file=$(echo $i | grep -Eo "/store.*.root")
    if [ ${#file} -lt 11 ]; then
      echo $i >> $fileName
      continue
    fi

    echo $file

    foundInGeoms=0
    for geom in "${geoms[@]}"; do
      found=$(zgrep $file $geometryBase/$geom/jobData/job*/alignment.log.gz | wc -l)
      if [ $found -gt 0 ]; then
	let "foundInGeoms=foundInGeoms+1"
        output=${i/\'*\',/}
        output=${output/\"*\",/}
        if [ ${#output} -gt 0 ] && ! $doReverse; then
  	  echo $output >> $fileName
        fi
        if ! $doReverse; then
	  break
	fi
      fi
      if [ $found -le 0 ] && $doReverse; then
	break
      fi
    done
    echo "    Found in $foundInGeoms geometries"
    if [ $foundInGeoms -eq 0 ] && ! $doReverse; then
      echo "  Selected: unique"
      echo $i >> $fileName
    elif [ $foundInGeoms -eq $nGeoms ] && $doReverse; then
      echo "  Selected: in all"
      echo "$i" >> $fileName
    fi
done  < $inputFile

exit 0
