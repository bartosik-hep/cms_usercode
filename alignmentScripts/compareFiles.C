
void compareFiles()
{
 const int nFiles=2;
 int entries=15;
 TString output_base="/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/data/commonValidation/results/nbartosi/";
 TString output_folder="Cosmics_0T_Feb2012_2010_comp/"; 

 TString filename[nFiles]={"AlignmentValidation_GR10_v6_offline.root","AlignmentValidation_GR10_v4_offline_nodeform.root"};
 TString title[nFiles]={"2012","2010"};
// int style[nFiles]={1,1,1,1};
// int color[nFiles]={2,4,1,3};

 // load framework lite just to find the CMSSW libs...
 gSystem->Load("libFWCoreFWLite");
 AutoLibraryLoader::enable();
 //compile the macro
  gROOT->ProcessLine(".L /afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/data/commonValidation/alignmentObjects/nbartosi/custom_scripts/compareFiles_Draw.C++");

  addFile(output_base+output_folder+filename[0],title[0],1,1);
  addFile(output_base+output_folder+filename[1],title[1],2,1);
  setDir("/TrackerOfflineValidationStandalone/GlobalTrackVariables/");
  setSavePath("/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/data/commonValidation/results/nbartosi/Cosmics_0T_Feb2012_2010_comp/test/");
  drawHist("h_trackphi","phi","TrackPhi",1);
  drawHist("h_tracketa","eta","TrackEta",1);
  drawHist("h_trackNumberOfValidHits","validHits","",1);
  drawHist("h_trackNumberOfLostHits","lostHits","",1);

  setDir("/TrackerOfflineValidationStandalone/");
  setTreeName("TkOffVal");
  drawTree("entries","subDetId==6&&entries>0","nHits",200,0,400,"tracksTEC",1);
  drawTree("entries","subDetId==5&&entries>0","nHits",200,0,400,"tracksTOB",1);
  drawTree("entries","subDetId==4&&entries>0","nHits",200,0,400,"tracksTID",1);
  drawTree("entries","subDetId==3&&entries>0","nHits",200,0,400,"tracksTIB",1);
  drawTree("entries","subDetId==2&&entries>0","nHits",200,0,400,"tracksPXF",1);
  drawTree("entries","subDetId==1&&entries>0","nHits",200,0,400,"tracksPXB",1);
  finish();

}
