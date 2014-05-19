#!/bin/sh

eosPath="/store/caf/user/nbartosi/MPproduction"
geom=$1
file=$2

if [ "$geom" = "" ]; then
  echo "No geometry specified"
  exit 0
fi

echo "Will get tree files from: $eosPath/$geom"

if [ "$file" = "" ]; then
  list=$(cmsLs $eosPath/$geom | grep -oE "treeFile.*")
  
  for i in $list; do
    echo "Getting: $i"
    cmsStage $eosPath/$geom/$i ./
  done
else
  fileName=$(cmsLs $eosPath/$geom | grep -oE "treeFile$file.*")
  echo "Getting file: $fileName"
  cmsStage $eosPath/$geom/$fileName ./
fi
