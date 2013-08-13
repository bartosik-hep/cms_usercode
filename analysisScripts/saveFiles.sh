#!/bin/sh

# USAGE: ./saveFiles.sh <short_word> <clean_switch>

# Copies contents of the working directory: scripts, plots, rootfiles to the newly created folder with name of current date and short word describing main topic of current results being copied.
# If second argument is "clean", then the copied files from plots and rootfiles folder will be removed after copying
# Also a new entry is added to the list.txt file where more detailed can be added about the results in today's folder.

workdir="/afs/desy.de/user/b/bartosik/cms/5X/CMSSW_5_3_11/src/TopAnalysis/Configuration/analysis/ttH"

resultdir="/afs/desy.de/user/b/bartosik/space/cms/5X/ttH_result"

year=`eval date +%Y`
month=`eval date +%m`
day=`eval date +%d`

addition=""
if [ -n "$1" ]; then
 addition="_$1"
fi

clean=$2
if [ "$clean" != "clean" ] && [ "$clean" != "" ]; then
  echo "ERROR! Wrong second argument for <clean>! Should be "clean" or nothing!"
  exit 0
fi

echo " Starting saving results of today: $day.$month.$year"

dirname="$resultdir/$year"_"$month"_"$day"$addition

if [ -d "$dirname" ]; then
 echo "ERROR! Today's directory ($dirname) already exists"
 exit 0
fi
echo " Copying files to the folder: $dirname"

echo "${dirname##$resultdir/} - " >> $resultdir/list.txt

mkdir $dirname

cp -r $workdir/* $dirname/

nitems_orig=`ls -1 $workdir/** | wc -l`
nitems_copy=`ls -1 $dirname/** | wc -l`

echo "Copied $nitems_copy files out of $nitems_orig"

# if [ "$clean" == "clean" ]; then
#   echo "Cleaning .${plotsdir##$workdir}/* and .${rootfilesdir##$workdir}/*" 
#   if [ $nplots_orig -ne $nplots_copy ] || [ $nrootfiles_orig -ne $nrootfiles_copy ]; then
#     echo "ERROR: Not all plots and rootfiles have been copied!"
#     echo "Clean of the original folders haven't been performed!"
#     exit 0
#   fi
#   rm -r $plotsdir/*
#   rm -r $rootfilesdir/*
# fi

if [ $nitems_orig -eq $nitems_copy ]; then 
  echo "done!"
fi

