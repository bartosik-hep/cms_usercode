cms_usercode
============

My user code in CMS


## Alignment Scripts

**MPTreeDrawer.C**
Draws calibration result from the pede output: mp<number>/jobData/jobm/treeFile_merge.root

============

## Analysis scripts
**fileWithEvent.py**

Finds the file that contains an event that needs to be found.
* Takes as input the path to the file containing the #list of files# (including CMS dataset files), and list of #event ids# (run:lumi:event).
* Returns pairs of filenames and events id's that have been matched.
