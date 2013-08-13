#! /bin/sh
# Nazar Bartosik

# This file is used as both a shell script and as a Python script.

""":"
# This part is run by the shell.  It looks for an appropriate Python
# interpreter then uses it to re-exec this script.

PYTHON=$(which python)
if [ "a$PYTHON" = "a" ]; then
  echo 1>&2 "No usable Python interpreter was found!"
  exit 1
fi

exec $PYTHON "$0" "$@"
" """

# The rest of the file is run by the Python interpreter.
__doc__ = """This string is treated as the module docstring."""

from DataFormats.FWLite import Handle, Events, Lumis, Runs
from subprocess import Popen, PIPE
from optparse import OptionParser
from sys import exit
import os.path
import re

###################
# PARSING OPTIONS #
###################
parser = OptionParser(usage="usage: ./%prog [options] -f <PATH> -e \"<run>:<lumi>:<event>\"")
parser.add_option("-f", "--files", dest="fileListString",
                  help="List of root files or paths to datasets,lists of root files. (Separated by ',') '~' will not work.",
                  type=str, metavar="<PATH>")
parser.add_option("-e", "--events", dest="eventsString",
                  help="List of events to be found. (Separated by ',') \ne.g: \"run:lumi:event,lumi:event\" Run is optional. Default Run: 1.",
                  type=str, metavar="\"<STRING>\"")
parser.add_option("-r", "--run", dest="defaultRun", default=1,
                  help="Default run number to be used for events that don't have run number specified, e.g: lumi:event",
                  type=int, metavar="<runNr>")
parser.add_option("-s", "--skipBegin", dest="filesToSkipBegin", default=0,
                  help="Number of files that should be skipped from the beginning of the list",
                  type=int, metavar="<number>")
parser.add_option("-S", "--skipEnd", dest="filesToSkipEnd", default=0,
                  help="Number of files that should be skipped from the end of the list",
                  type=int, metavar="<number>")
parser.add_option("-m", "--maxFiles", dest="nFilesMax", default=0,
                  help="Maximum number of files that should be checked from the list",
                  type=int, metavar="<number>")
parser.add_option("-p", "--filePattern", dest="filePathPattern", default="/store/.*?\.root",
                  help="Search pattern that should be used to find the paths of root files in the specified list. Default: /store/.*?\.root",
                  type=str, metavar="\"<RegExPattern>\"")
parser.add_option("-n", "--nFilesStat", dest="nFilesStat", default="0",
                  help="Print statistics of checked files every <nFilesStat> files. Default: 0 - No printout",
                  type=int, metavar="<number>")
parser.add_option("-a", "--all",
                  action="store_true", dest="findAll", default=False,
                  help="Search in all files. Allow to find the same event in multiple files.")
parser.add_option("-v", "--verbose",
                  action="store_true", dest="verbose", default=False,
                  help="Print progress of the script execution (list of checked root files, runsToFind, lumisToFind, events)")

(options, args) = parser.parse_args()



####################
# CUSTOM FUNCTIONS #
####################

# Returns the list of all indices of item <value> in  list <qlist>
def indexMult(value, qlist):
    indices = []
    idx = -1
    while True:
        try:
            idx = qlist.index(value, idx+1)
            indices.append(idx)
        except ValueError:
            break
    return indices


# Returns the sublist of <qlist> containing only items of indices <ilist>
def sublist(ilist, qlist):
	list = []
	for i in ilist:
		list.append(qlist[i])
	return list

# Returns a list that is the intersection of 2 lists
def intersect(a, b):
    result=[]
    for i in b:
        if isinstance(i,list):
            result.append(intersect(a,i))
        else:
            if i in a:
                 result.append(i)
    return result



#####################
# MAIN PROGRAM CODE #
#####################

# Getting option values
defaultRun = options.defaultRun
fileListString = options.fileListString
eventsString = options.eventsString
filesToSkipBegin = options.filesToSkipBegin
filesToSkipEnd = options.filesToSkipEnd
nFilesMax = options.nFilesMax
filePathPattern = options.filePathPattern
nFilesStat = options.nFilesStat
findAll = options.findAll

# Checking the specified list of files
if fileListString == None:
	print "Error: No list of input root files (-f) specified"
	print
	parser.print_help()
	exit()

# Checking the specified list of events
if eventsString == None:
	print "Error: No list of events (-e) specified"
	print
	parser.print_help()
	exit()


###########################################
# Processing the list of events to be found
runsToFind = []
lumisToFind = []
eventsToFind = []
eventFound = []

eventsString = ''.join(eventsString.split())			# Removing whitespaces
eventStrings = eventsString.split(",")					# Splitting into list by ','
for eventString in eventStrings:
	eventComponents = eventString.split(":")			# Splitting into components by ':'
	eventComponents = filter(None,eventComponents)		# Filter out empty components
	# Validating the specified event
	nComponents = len(eventComponents)
	if nComponents>3 or nComponents<2:
		print "Wrong event syntax: ", eventString
		print "Must be:             run:lumi:event or lumi:event"
	# Setting each component of current event
	event = eventComponents[nComponents-1]
	lumi = eventComponents[nComponents-2]
	run = defaultRun
	if len(eventComponents)>2:
		run = eventComponents[nComponents-3]
	# Ensuring the string type
	event = str(event)
	lumi = str(lumi)
	run = str(run)
	# Adding event components to the corresponding lists of events
	runsToFind.append(run)
	lumisToFind.append(lumi)
	eventsToFind.append(event)
	eventFound.append(False)

# Printing the list of events that will be searched
nEvents = len(eventsToFind)
print "Searching for", nEvents, " events:"
for i in range(0,nEvents):
	print i+1,". ",":".join( [ runsToFind[i], lumisToFind[i], eventsToFind[i] ] )


###########################################
# Setting the list of root files to examine
rootFileNames = []

fileListString = ''.join(fileListString.split())		# Removing whitespaces
fileNames = fileListString.split(",")
# Processing each string and extracting root files if it represents the list
for fileName in fileNames:
	length = len(fileName)
	if fileName[length-5:length] == ".root":
		rootFileNames.append(fileName)
		continue
	# This is not a root file, but a list. Searching the paths of root files
	rootFileRE = re.compile(filePathPattern)
	if not os.path.isfile(fileName):
		print "This file can't be opened:"
		print "    ",fileName
	with open(fileName) as f:
		for line in f:
			# Finding all occurences of the root file path "/store/ ... .root"
			rootFilesString = rootFileRE.findall(line)
			if len(rootFilesString) < 1:
				continue
			# Inserting each found root file path to the rootFileNames list
			for filePath in rootFilesString:
				rootFileNames.append(filePath)
nFileNames = len(rootFileNames)
print "Total number of root files to be checked: ", nFileNames
if nFilesMax > 0:
	print "Limited to ", nFilesMax

#########################################
# Looping through the files from the list
nFilesProcessed = 0
for fileName in rootFileNames:
	fileIsPrinted = False
	# Stopping if all events have been found
	if False not in eventFound:
		break
	# Limiting the number of processed files if needed
	nFilesProcessed = nFilesProcessed + 1
	if nFilesProcessed > nFileNames - filesToSkipEnd:		# Skipping files from the end
		break
	if nFilesProcessed < filesToSkipBegin:					# Skipping files from the beginning
		continue
	if nFilesMax > 0:										# Skipping files over the maximum limit
		if nFilesProcessed > nFilesMax:
			break
	if nFilesStat > 0:
		if nFilesProcessed%nFilesStat==0:					# Printing statistics of processed files
			print "Processed ", nFilesProcessed, " files out of ", nFileNames

	#  Setting LFN of the file
	fileLFN = fileName
	# Setting PFN of the file (Needed to open the root file locally)
	filePFN = Popen(["edmFileUtil", "-d", "-f", fileName], stdout=PIPE)
	filePFN = filePFN.communicate()[0]
	filePFN = ''.join(filePFN.split())
	if options.verbose:
		print "Checking file: ", fileLFN		
	# Checking presence of RUN NUMBER
	RUNS = Runs(filePFN)
	for entry in RUNS:
		# Stopping if all events have been found
		if False not in eventFound:
			break
		# Getting current RUN NUMBER
		theRun = entry.runAuxiliary().run()
		theRun = str(theRun)
		if options.verbose: 
			print "Run: ", theRun
		if theRun not in runsToFind:
			continue
		selectedEventRunIds = indexMult(theRun, runsToFind)	# Selecting only events with this RUN NUMBER
		# Checking presence of LUMINOSITY BLOCK
		LUMIS = Lumis(filePFN)
		for entry in LUMIS:
			# Stopping if all events have been found
			if False not in eventFound:
				break
			# Getting current LUMINOSITY BLOCK
			theLumi = entry.luminosityBlockAuxiliary().luminosityBlock()
			theLumi = str(theLumi)
			if options.verbose:
				print " Lumi: ", theLumi
			selectedLumis = sublist(selectedEventRunIds,lumisToFind)	# Selecting only lumisToFind whith this RUN NUMBER
			if theLumi not in selectedLumis:
				continue
			# Getting list of indices of events with this RUN NUMBER and LUMINOSITY BLOCK
			selectedEventRunLumiIds = indexMult(theLumi, lumisToFind)
			selectedEventRunLumiIds = intersect(selectedEventRunIds, selectedEventRunLumiIds)
			# Checking presence of EVENT  NUMBER
			EVENTS = Events(filePFN)
			for entry in EVENTS:
				# Stopping if all events have been found
				if False not in eventFound:
					break
				# Getting current EVENT NUMBER
				theEvent = entry.eventAuxiliary().event()
				theEvent = str(theEvent)
				if options.verbose:
					print "    Event: ", theEvent
				selectedEvents = sublist(selectedEventRunLumiIds, eventsToFind)	# Selecting only events with this RUN NUMBER and LUMINOSITY BLOCK
				if theEvent not in selectedEvents:
					continue
				# Identifiing the id of this event in the input list of events
				runIds = indexMult(theRun,runsToFind)
				lumiIds = indexMult(theLumi,lumisToFind)
				eventIds = indexMult(theEvent, eventsToFind)
				eventId = intersect(intersect(runIds, lumiIds), eventIds)
				eventId = eventId[0]
				# Printing the file names and list of all searched events that are in this file
				if not fileIsPrinted:
					print
					print "Found events in file ", nFilesProcessed,":"
					print "  File PFN:", filePFN
					print "  File LFN:", fileLFN
					fileIsPrinted = True
				print eventId+1,".    Run: ", theRun, " Lumi: ", theLumi, "Event: ", theEvent
				# Updating the status of the found event if allowed
				if not findAll:
					eventFound[eventId]=True

