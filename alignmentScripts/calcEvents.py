import os
import sys
import subprocess
import re

if len(sys.argv) < 2:
  print "USAGE: calcEvents <file with fileList> <doLogging>"
  exit(1)

inputFileName = sys.argv[1]

#Reading the list files
files=[]
fileList = open(inputFileName);
for line in fileList:
  searchResult = re.search('/store.*\.root',line)
  if searchResult is None:
    continue
  fileName = searchResult.group(0)
  if fileName.__len__() > 5:
    files.append(fileName)

print "### Calculating number of events in ", len(files), "files..."


from DataFormats.FWLite import Events
nEvents = 0
for file in files:
  tmp = open('/afs/cern.ch/user/n/nbartosi/tmp_file', 'w+')
  #Getting the absolute path of the file
  process = subprocess.call(["edmFileUtil","-d","-f",file],stdout=tmp)
  tmp.seek(0)
  fileName = tmp.readline()
  fileName = re.search('.*\.root',fileName)
  fileName = fileName.group(0)
  print "Checking events in file: ", fileName
  #Calculating total number of entries in the files
  events = Events(fileName)
  nThisEvents = events.size()
  print "     Events in the file: ", nThisEvents
  nEvents += nThisEvents
  if len(sys.argv) > 2:
    print "### ", file, " : ", events.size()

  tmp.close()
os.remove('/afs/cern.ch/user/n/nbartosi/tmp_file')

print "### Total number of events in (",len(files),") is : ", nEvents

