#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TTree.h"
#include "TList.h"
#include "TKey.h"
#include "TH1F.h"
#include "TGaxis.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TCanvas.h"
#include "TLatex.h"

#include <exception>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>

// #include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/DetId/interface/DetId.h"
// #include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/interface/PXBDetId.h"
// #include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/src/PXBDetId.cc"
// #include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/interface/PXFDetId.h"
// #include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/src/PXFDetId.cc"
// #include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiStripDetId/interface/TIBDetId.h"
// #include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiStripDetId/interface/TOBDetId.h"


#include "lumisInIOV.C"   // Luminosity of each run range of each IOV

//////////////////////
// GLOBAL VARIABLES //
//////////////////////

TStyle *tdrStyle;
int markerColors[2]= {1,2};
int markerStyles[6][2]= {{20,24},{21,25},{22,26},{23,32}};
int lineStyles[3]= {1,7,3};
int lineColors[8]= {13,kPink-9,kAzure+7,kSpring-6,kOrange+1,kPink-1,kAzure+10,kSpring+9};
int fillStyles[2]= {1001,3008};
TString inputFileName = "/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/mp1296/jobData/jobm/treeFile_merge.root";
TString outputPath = "/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/mp1296/LA_evol_test";
TFile *inputFile;       // ROOT file (treeFile_merge.root) with the calibration result

std::vector<TString> DetName;

int nPhiParts = 0;      // Number of parts with different calibration values in Phi
int nZParts = 0;        // Number of parts with different calibration values in Z (rings)
int nLayers = 0;        // Number of parts with different calibration values in R (layers)

int DetIndex = -1;	// 0-BPIX 1-FPIX 2-TIB 3-TOB
int LayerId = 1;	// [BPIX] Layers: 1-3 Rings: 1-8 [FPIX] Side: 0-1 [TIB] Layers: 1-4 Modules: 1-6 [TOB] Layers: 1-6 Modules: 1-4
int ZId = 0; // Meaning of VLayer: 0-Layer; 1-Ring (Module);
int PhiId = 0;  // Strip readout mode: 0-peak 1-deco
int nPointsToSkip=1;	// Number of points that should be skipped from the end
//float lumiScale=0.826;
float lumiScale=1.0;
bool drawLegend=true;
float minY=0.f;
float maxY=0.f;
bool autoScaleY = false;
bool drawInput = true;      // Whether to draw lines for input LA
FILE *logFile;
FILE *runsFile;

bool canvasSquare=false;

//float iovWidth = 0.33;	  // Width of one IOV in fb-1 for labels of X axis
float fixedIOVwidth[4] = {0.35, 0.35, 1.27, 1.27};      // Width of one IOV in fb-1 for each DetId
bool iovWidthIsFixed = false;                           // Whether to compute real width of which IOV based on its luminosity
float minY_det[4] = {0.35,-0.095,0.06,0.065};           // BPIX, FPIX, TIB, TOB
float maxY_det[4] = {0.46,-0.075,0.082,0.11};           // BPIX, FPIX, TIB, TOB


std::vector<int> iov1stRuns(0);         // Vector of first run numbers of each iov in the file (in the tree names)
std::vector<int> iov1stRuns_(0);        // Vector of first run numbers of each iov in the file (to be plotted)

struct POINT2                            // Point represented by 2 values
{
    POINT2(float v1_=9999.9, float v2_=-9999.9) : v1(v1_), v2(v2_) {};
    float v1;
    float v2;
};

// Struct that contains all distinct calibration values
// Also contains the ranges of module positions in group of each value
struct GRANVAL
{
    std::vector<double> val;
    std::vector<double> val_e;
    std::vector<POINT2> r;
    std::vector<POINT2> z;
    std::vector<POINT2> phi;
    std::vector<int> subDetId;
};

std::vector<GRANVAL> calibOutValues;       // List of distinct values for each GRANVALity direction
GRANVAL calibInValues;                     // List of distinct values for each GRANVALity direction


TString geometryFileName = "TrackerTree.root";  // ROOT file that contains all information about tracker geometry layout
TTree* geomTree;

TCanvas* canvas;        // Canvas to which the result will be drawn

bool log_ = false;

//////////////////////////
// FUNCTIONS DEFINITION //
//////////////////////////

// int histoIdx ( UInt_t detId );
// void setGraphStyle ( TGraph *graph, int id, int file );
// void drawEmptyHisto ( float Xmin=0, float Xmax=20, TString xTitle="", float Ymin=0, float Ymax=1, TString yTitle="", TString name="empty" );
// void setLegendStyle ( TLegend *leg,int ncols, int nDetParts );
void setTDRStyle();
// void tdrGrid(bool gridOn);
// void fixOverlay();
int nIOVsInTrees(TString treeBaseName, std::vector<int>& iovs, std::vector<int>& iovs_);
GRANVAL distValuesFromTrees(TString treeName);
int plotCalibration( const int subDetId = 1, const int rId = 1, const int zId = 0, const int phiId = 0, const bool toSave = false);
int analyzeGRANVALity( const int subDetId, const int rId, const int zId, const int phiId, const bool toPrint = false );



//////////////////////////////
// FUNCTIONS IMPLEMENTATION //
//////////////////////////////

/**
 * Finds the list of IOVs and analyzed GRANVALity for the selected calibration type, module type and readout mode
 * @method MPTreeDrawer
 * @param  calibrationType  LorentzAngle | Backplane
 * @param  moduleType   Pixel | Strip
 * @param  stripReadoutMode deco | peak
 */
int MPTreeDrawer(TString calibrationType = "LorentzAngle", TString moduleType = "Pixel", TString stripReadoutMode = "")
{
    setTDRStyle();

    DetName.push_back("");
    DetName.push_back("BPIX");      // 1
    DetName.push_back("FPIX");      // 2
    DetName.push_back("TIB");       // 3
    DetName.push_back("TID");       // 4
    DetName.push_back("TOB");       // 5
    DetName.push_back("TEC");       // 6

    TFile* geomFile = new TFile(geometryFileName);
    if(geomFile->IsZombie()) {
        printf("ERROR! No geometry file found with name: %s\n", geometryFileName.Data());
        printf("Rerun MPTreeDrawer(<fileName>) with correct path to the geometry file.\n");

        return 1;
    }

    TString geometryTreeName = "TrackerTreeGenerator/TrackerTree/TrackerTree";
    geomTree = (TTree*)geomFile->Get(geometryTreeName);
    if(!geomTree) {
        printf("ERROR! No geometry tree found with name: %s\n", geometryTreeName.Data());

        return 1;
    }

    if(stripReadoutMode == "deco") stripReadoutMode = "deconvolution";

    // Opening files for loggins
    logFile = fopen( "log.txt","w" );          // Debugging logs
    runsFile = fopen( "runs.txt","w" );        // Run ranges that are used (for calculation of the luminosity)

    // Opening the input ROOT file
    inputFile = new TFile( inputFileName );

    // Creating a canvas
    if(!canvas) canvas = new TCanvas("c1", calibrationType+" calibration", 1000, 800);

    // The base of the input tree name
    TString TreeBaseDir("Si"+moduleType+calibrationType+"Calibration"+stripReadoutMode+"_result_");
    int nIOVs = nIOVsInTrees(TreeBaseDir, iov1stRuns, iov1stRuns_);
    printf("Found %d IOVs with name: %s ...\n",nIOVs, TreeBaseDir.Data());
    // Total luminosity of all IOVs (to be updated)
    // double totLumi=0.0;



    // Getting the calibration result values for each IOV
    for(int iovId = 0; iovId < nIOVs; iovId++) {
        if(iovId > 4) break;

        printf("Checking IOV: %d\n", iovId);
        TString TreeBaseNameOut = TreeBaseDir;
        TreeBaseNameOut += iov1stRuns.at(iovId);
        // Filling the GRANVALity with calibration output values/errors
        printf("Checking entries from the tree: %s\n",TreeBaseNameOut.Data());
        GRANVAL distOutValues = distValuesFromTrees(TreeBaseNameOut);

        
        calibOutValues.push_back(distOutValues); // Adding it to the vector for the current IOV

    }

    TString TreeBaseNameIn = TreeBaseDir.ReplaceAll("_result_","_input");
    // Getting the calibration input values
    printf("Checking entries from the tree: %s\n",TreeBaseNameIn.Data());
    calibInValues = distValuesFromTrees(TreeBaseNameIn);



    fclose ( logFile );

    fclose ( runsFile );

    return 0;
}


/**
 * Finds all iovs stored in the file by the given name template
 * @method nIOVsInTrees
 * @param  TreeBaseName     Template name of the trees ot find
 * @param  iovs             Reference to the vector of IOVs in the tree names
 * @param  iovs_            Reference to the vector of IOVs to be shown on the plot
 * @return
 */
int nIOVsInTrees(TString treeBaseName, std::vector<int>& iovs, std::vector<int>& iovs_)
{
    iovs.clear();       // Cleaning the vector if not empty
    iovs_.clear();      // Cleaning the vector if not empty
    if(!inputFile) {
        printf("No input file opened.\n Exiting...\n");
        return 0;
    }
    // Getting the list of all stored keys in the file
    TList *list = inputFile->GetListOfKeys();
    int nKeys = list->GetSize();
    for( int keyNum=0; keyNum<nKeys; keyNum++ ) {
        TKey* key = ( TKey* ) list->At ( keyNum );
        TString str ( key->GetName() );
        if ( !str.BeginsWith ( treeBaseName ) ) {
            continue;
        }
        str.ReplaceAll ( treeBaseName,"" );
        int run = str.Atoi();
        iovs.push_back ( run );     // Adding run to the list
        iovs_.push_back ( run );     // Adding run to the list
        fprintf(runsFile,"%d\n",run);     // Putting run to the file (for luminosity calculation)
    }
    int nIOVs = iovs.size();

    // Updating the the first run number of the first IOV to more meaningful value
    if(nIOVs>3) iovs_.at(0) = iovs.at(1) - std::abs( iovs.at(2) - iovs.at(1) );
    // Adding on more run to the end as the last run of the last IOV
    if(nIOVs>2) iovs_.push_back( iovs.at(nIOVs-1) + std::abs( iovs.at(nIOVs-1) - iovs.at(nIOVs-2) ) );

    return nIOVs;
}


/**
 * Fills the vector of calibration values for each GRANVALity type
 * @method nDistValuesFromTrees
 * @param  treeName     Name of the TTree that contains the values
 * @param  calibValues  Struct that will be filled with the distinct values from the TTree
 * @param  isOldFormat  Swith for the old format (parameter error was stored directly in the tree, instead of a struct)
 * @return  Number of distinct values found in the tree
 */
GRANVAL distValuesFromTrees(TString treeName)
{
    GRANVAL distValues;
    // Setting the initial values
    distValues.val = std::vector<double>();
    distValues.val_e = std::vector<double>();
    distValues.r = std::vector<POINT2>();
    distValues.z = std::vector<POINT2>();
    distValues.phi = std::vector<POINT2>();
    distValues.subDetId = std::vector<int>();

    // Parameters to be read from the tree
    UInt_t detId;
    Float_t value;
    Float_t error;
    // Struct of additional values from the tree
    struct treeStr
    {
        Float_t delta;
        Float_t error;
        UInt_t paramIndex;
    };
    treeStr treeStruct;

    bool isOldFormat = false;   // Parameter error was stored directly in the tree, instead of a struct

    // Reading a tree of specified name from the TFile
    TTree *tree = 0;
    tree = ( TTree* ) ( TDirectoryFile* ) inputFile->Get ( treeName );
    if(!tree) {
        printf( "  Couldn't find the tree with name: %s\n", treeName.Data() );
        return distValues;
    }

    Long64_t nEntries = tree->GetEntries();
    printf ( "  Got Tree with name: %s \t with nEntries: %lld\n",treeName.Data(),nEntries ); 

    tree->SetBranchAddress ( "detId",&detId );
    tree->SetBranchAddress ( "value",&value );
    if(tree->GetBranch("error")) isOldFormat = true;
    if(isOldFormat) tree->SetBranchAddress ( "error",&error ); else
    tree->SetBranchAddress ( "treeStruct",&treeStruct );

    for ( Long64_t entry=0; entry<nEntries; entry++ ) {
        tree->GetEntry ( entry );
        if(!isOldFormat) error = treeStruct.error;
        fprintf ( logFile, "    entry: %lld\tvalue: %.3f\terror: %.3f\tdetId: %d\n",entry,value,error,detId );

        // Adding to the vector of distinct values if needed
        unsigned int valIndex = std::find(distValues.val.begin(), distValues.val.end(), value) - distValues.val.begin();
        bool isInVector = (valIndex < distValues.val.size()) && distValues.val.size() > 0;
        if(!isInVector) {
            distValues.val.push_back(value);
            valIndex = distValues.val.size() - 1;
            distValues.val_e.push_back(error);
            distValues.r.push_back(POINT2());
            distValues.z.push_back(POINT2());
            distValues.phi.push_back(POINT2());
            distValues.subDetId.push_back(0);
        } 

        // Reading the position and subDetId of the module with current calibration value from the geometry Tree
        TString cutString = "RawId == ";
        cutString += detId;
        geomTree->Draw("SubdetId", cutString);
        int nSelEntries = geomTree->GetSelectedRows();
        if(nSelEntries!=1) {
            printf("ERROR! No entry with position information found for module with detId: %d\n", detId);
            continue;
        }
        int subDetId = geomTree->GetV1()[0];
        distValues.subDetId.at(valIndex) = subDetId;

        geomTree->Draw("PosR:PosZ:PosPhi", cutString);
        float posR = geomTree->GetV1()[0];
        float posZ = geomTree->GetV2()[0];
        float posPhi = geomTree->GetV3()[0];
        POINT2& R = distValues.r.at(valIndex);
        POINT2& Z = distValues.z.at(valIndex);
        POINT2& Phi = distValues.phi.at(valIndex);
        // Updating the range values for each position component
        if(posR<R.v1) R.v1 = posR;      // Radius
        if(posR>R.v2) R.v2 = posR;
        if(posZ<Z.v1) Z.v1 = posZ;      // Z
        if(posZ>Z.v2) Z.v2 = posZ;
        if(posPhi<Phi.v1) Phi.v1 = posPhi;  // Phi
        if(posPhi>Phi.v2) Phi.v2 = posPhi;

    }      // End of loop over entries

    int nDistVals = distValues.val.size();
    printf("      Number of distinct values: %d\n", nDistVals);

    if(log_) {
        for(int iVal = 0; iVal<nDistVals; iVal++) {
            printf("        %d: Val: %.3f += %.2f (subdet: %d)\n",iVal, distValues.val.at(iVal), distValues.val_e.at(iVal), distValues.subDetId.at(iVal));
            printf("             R: %.3f - %.3f\n", distValues.r.at(iVal).v1, distValues.r.at(iVal).v2);
            printf("             Z: %.3f - %.3f\n", distValues.z.at(iVal).v1, distValues.z.at(iVal).v2);
            printf("           Phi: %.3f - %.3f\n", distValues.phi.at(iVal).v1, distValues.phi.at(iVal).v2);
        }
    }

    return distValues;
}


/**
 * Plots calibration result and input values for the specified group(s) of the modules
 * @method plotCalibration
 * @param  subDetId     DetId of the subdtector (0-6) 0 - all subDet curves will be drawn
 * @param  rId          Index of the group by the R position
 * @param  zId          Index of the group by the Z position
 * @param  phiId        Index of the group by the Phi position
 * @param  toSave       Whether the plot should be saved automatically
 * @return  Number of result curves that have been drawn
 */
int plotCalibration( const int subDetId, const int rId, const int zId, const int phiId, const bool toSave)
{
    // Validating the detId
    if(subDetId > (int)DetName.size()-1) {
        printf("Wrong detector id. Should be one of the following:\n");
        for(unsigned int i=1; i<DetName.size(); i++) printf("  %d : %s\n", i, DetName.at(i).Data());
        printf("\nExecute plotCalibration() with proper parameters...\n");
        return 1;
    }

    // analyzeGRANVALity(false);


    return 0;
}


/**
 * Analyzes maximal number of groups in each dimension (R, Z, Phi) for each subdetector
 * @method analyzeGRANVALity
 * @param  toPrint  Whether number of groups should be printed for each subdetector
 * @return  Total number of 
 */
int analyzeGRANVALity( const int subDetId, const int rId, const int zId, const int phiId, const bool toPrint )
{
    // // Determining subDetIds
    // std::vector<int> subDetIds;
    // // Looping through the IOVs
    // for(int iovId = 0; iovId < (int)calibOutValues.size(); iovId++) {
    //     // Looping through the values in each IOV to determine number of subDetIds
    //     GRANVAL& distValues = calibOutValues.at(iovId);
    //     for(int valId = 0; valId < (int)distValues.val.size(); valId++) {
    //         int subDetId = distValues.subDetId.at(valId);
    //         if(std::find(subDetIds.begin(), subDetIds.end(), subDetId) != subDetIds.end()) continue;
    //         subDetIds.push_back(subDetId);
    //         // POINT2 R = distValues.r.at(valId);
    //         // POINT2 Z = distValues.z.at(valId);
    //         // POINT2 Phi = distValues.phi.at(valId);
    //         // printf("  %d. det: %d\tR: [%.3f|%.3f]\tZ: [%.3f|%.3f]\tPhi: [%.3f|%.3f]\n", valId+1, subDetId, R.v1, R.v2, Z.v1, Z.v2, Phi.v1, Phi.v2);
    //     }
    // }

    // // Determining bumber of layers in each subDet
    // for(int detId = 0; detId < (int)subDetIds.size(); detId++) {
    //     int subDetId = subDetIds.at(detId);
    //     std::vector<POINT2> Rs;
    //     // Looping through the IOVs
    //     for(int iovId = 0; iovId < (int)calibOutValues.size(); iovId++) {
    //         // Looping through the values in each IOV to determine number of subDetIds
    //         GRANVAL& distValues = calibOutValues.at(iovId);
    //         for(int valId = 0; valId < (int)distValues.val.size(); valId++) {
    //             if(subDetId!=distValues.subDetId) continue;
    //             POINT2 R = distValues.r.at(valId);
    //             if(std::find(Rs.begin(), Rs.end(), R) != Rs.end()) continue;
    //             Rs.push_back(R);
    //             // POINT2 R = distValues.r.at(valId);
    //             // POINT2 Z = distValues.z.at(valId);
    //             // POINT2 Phi = distValues.phi.at(valId);
    //             // printf("  %d. det: %d\tR: [%.3f|%.3f]\tZ: [%.3f|%.3f]\tPhi: [%.3f|%.3f]\n", valId+1, subDetId, R.v1, R.v2, Z.v1, Z.v2, Phi.v1, Phi.v2);
    //         }
    //     }
    // }


    return 0;
}

// /**
//  * Index of the histogram that should contain the value of the module
//  * @method histoIdx
//  * @param  detId
//  * @return
//  */
// int histoIdx ( UInt_t detId )
// {
//     DetId det_ ( detId );
// //    fprintf(logFile,"  detId: %d\tdet: %d\tsubDetId: %d\t", detId, det_.det(), det_.subdetId());
//     if ( det_.det() !=DetId::Tracker ) {
// //        fprintf(logFile,"\n");
//         return -1;
//     }

//     int id=-1;
//     int layer=-1;
//     int ring=-1;

//     switch(DetIndex) {
//         case 0: if(det_.subdetId() == PixelSubdetector::PixelBarrel) {
//             PXBDetId det(detId);
//             layer = det.layer();
//             ring = det.module();
//             if(LayerRing==0 && layer==VLayer) id = ring-1;
//             if(LayerRing==1 && ring==VLayer) id = layer-1;
//         } break;
//         case 1: if(det_.subdetId() == PixelSubdetector::PixelEndcap) {
//             PXFDetId det(detId);
//             id = det.side()-1;
//         } break;
//         case 2: if(det_.subdetId() == StripSubdetector::TIB) {
//             TIBDetId det(detId);
//             layer = det.layer();
//             ring = det.module();
//             int side = det.side();
// //            fprintf(logFile,"layer: %d\tring: %d\tside: %d",layer,ring,side);
//             int modulesInStruct = 6/nRings[DetIndex];

//             if(side==1) ring = 3-ring; else
//             if(side==2) ring = 3+ring-1;
//             ring = ring/modulesInStruct;

//             if(LayerRing==0 && layer==VLayer) id = ring;
//             if(LayerRing==1 && ring+1==VLayer) id = layer-1;
//         } break;
//         case 3: if(det_.subdetId() == StripSubdetector::TOB) {
//             TOBDetId det(detId);
//             layer = det.layer();
//             ring = det.module();
//             int side = det.side();
// //            fprintf(logFile,"layer: %d\tring: %d\tside: %d\t",layer,ring,side);
//             int modulesInStruct = 12/nRings[DetIndex];

//             if(side==1) ring = 6-ring; else
//             if(side==2) ring = 6+ring-1;
//             ring = ring/modulesInStruct;

//             if(LayerRing==0 && layer==VLayer) id = ring;
//             if(LayerRing==1 && ring+1==VLayer) id = layer-1;
//         } break;
//     }
// //    fprintf(logFile,"\n");

//     return id;
// }

// void setGraphStyle ( TGraph *graph, int id, int file )
// {
//     int structId = -1;
//     if(LayerRing==0) structId = ( id<nRings[DetIndex]/2 )?id:nRings[DetIndex]-1-id; else structId = id/2;

//     int Zpart = ( id<nRings[DetIndex]/2 )?0:1;
//     if(LayerRing==1) Zpart = id%2;
//     if(LayerRing==0 && nRings[DetIndex]%2!=0) {
//         Zpart=0;
//         structId = id;
//     }

//     graph->SetMarkerStyle ( markerStyles[structId][Zpart] );
// //    graph->SetMarkerSize ( 1 );
//     graph->SetMarkerColor ( lineColors[structId] );
// //    graph->SetFillStyle ( fillStyles[Zpart] );
//     graph->SetLineColor ( lineColors[structId] );
//     graph->SetLineWidth ( 2 );
//     graph->SetLineStyle ( lineStyles[0] );
//     if(file>0) graph->SetLineStyle ( lineStyles[Zpart] );
// //    graph->SetFillColor ( lineColors[structId] );
// //
// //    graph->SetLineWidth ( file+1 );
// }

// void drawEmptyHisto ( float Xmin, float Xmax, TString xTitle, float Ymin, float Ymax, TString yTitle, TString name )
// {
//     printf ( "Setting empty histo: X: %.2f-%.2f\tY: %.2f-%.2f\n",Xmin,Xmax,Ymin,Ymax );
//     printf("Total luminosity to be plotted: %.3f\n",Xmax);
//     printf("Luminosity scale used: %.2f\n",lumiScale);

//     Xmax*=1.02;	  // Increasing max lumi to have free space
//     gStyle->SetOptStat ( 0 );
//     //    TGaxis::SetMaxDigits ( 4 );

//     TH1F *histo = new TH1F ( name, xTitle, 1, Xmin, Xmax );
//     TAxis *xAx = histo->GetXaxis();
//     TAxis *yAx = histo->GetYaxis();
//     xAx->SetTitle ( xTitle );
//     //    xAx->SetTitleOffset ( 1.1 );
//     //    xAx->SetNdivisions ( 510 );
//     yAx->SetTitle ( yTitle );
//     //    yAx->SetTitleOffset ( 1.15 );
//     //    yAx->SetNdivisions ( 510 );
//     histo->SetMinimum ( Ymin );
//     histo->SetMaximum ( Ymax );
//     histo->Draw ( "AXIS" );
// }

// void setLegendStyle ( TLegend *leg,int ncols, int nDetParts )
// {
//     leg->SetTextFont ( 42 );
// //    leg->SetTextSize ( 0.04 );
//     leg->SetFillStyle ( 1001 );
//     leg->SetBorderSize ( 1 );
// //    leg->SetLineColor ( 1 );
//     leg->SetFillColor ( 0 );
//     leg->SetNColumns ( ncols );
//     if(ncols==1) {
//         leg->SetX1(leg->GetX1() + 0.5*(leg->GetX2() - leg->GetX1() ));
//     }
//     leg->SetY1(leg->GetY2() - nDetParts/ncols*(0.05));
// }

// // Public CMS style (Detector Performance)

// // tdrGrid: Turns the grid lines on (true) or off (false)

// void tdrGrid(bool gridOn) {
//     tdrStyle->SetPadGridX(gridOn);
//     tdrStyle->SetPadGridY(gridOn);
// }

// // fixOverlay: Redraws the axis

// void fixOverlay() {
//     gPad->RedrawAxis();
// }

void setTDRStyle() {
    tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
    tdrStyle->SetCanvasBorderMode(0);
    tdrStyle->SetCanvasColor(kWhite);
    if(canvasSquare) {
        tdrStyle->SetCanvasDefH(600); //Height of canvas
        tdrStyle->SetCanvasDefW(600); //Width of canvas
    } else {
        tdrStyle->SetCanvasDefH(800); //Height of canvas
        tdrStyle->SetCanvasDefW(1200); //Width of canvas
    }
    tdrStyle->SetCanvasDefX(0);   //POsition on screen
    tdrStyle->SetCanvasDefY(0);

    // For the Pad:
    tdrStyle->SetPadBorderMode(0);
    // tdrStyle->SetPadBorderSize(Width_t size = 1);
    tdrStyle->SetPadColor(kWhite);
    tdrStyle->SetPadGridX(false);
    tdrStyle->SetPadGridY(false);
    tdrStyle->SetGridColor(0);
    tdrStyle->SetGridStyle(3);
    tdrStyle->SetGridWidth(1);

    // For the frame:
    tdrStyle->SetFrameBorderMode(0);
    tdrStyle->SetFrameBorderSize(1);
    tdrStyle->SetFrameFillColor(0);
    tdrStyle->SetFrameFillStyle(0);
    tdrStyle->SetFrameLineColor(1);
    tdrStyle->SetFrameLineStyle(1);
    tdrStyle->SetFrameLineWidth(1);

    // For the histo:
    // tdrStyle->SetHistFillColor(1);
    // tdrStyle->SetHistFillStyle(0);
    tdrStyle->SetHistLineColor(1);
    tdrStyle->SetHistLineStyle(0);
    tdrStyle->SetHistLineWidth(1);
    // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
    // tdrStyle->SetNumberContours(Int_t number = 20);

    tdrStyle->SetEndErrorSize(2);
    //  tdrStyle->SetErrorMarker(20);
    tdrStyle->SetErrorX(0.);

    tdrStyle->SetMarkerStyle(20);
    if(!canvasSquare) tdrStyle->SetMarkerSize(2);

    //For the fit/function:
    tdrStyle->SetOptFit(1);
    tdrStyle->SetFitFormat("5.4g");
    tdrStyle->SetFuncColor(2);
    tdrStyle->SetFuncStyle(1);
    tdrStyle->SetFuncWidth(1);

    //For the date:
    tdrStyle->SetOptDate(0);
    // tdrStyle->SetDateX(Float_t x = 0.01);
    // tdrStyle->SetDateY(Float_t y = 0.01);

    // For the statistics box:
    tdrStyle->SetOptFile(0);
    tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
    tdrStyle->SetStatColor(kWhite);
    tdrStyle->SetStatFont(42);
    tdrStyle->SetStatFontSize(0.025);
    tdrStyle->SetStatTextColor(1);
    tdrStyle->SetStatFormat("6.4g");
    tdrStyle->SetStatBorderSize(1);
    tdrStyle->SetStatH(0.1);
    tdrStyle->SetStatW(0.15);
    // tdrStyle->SetStatStyle(Style_t style = 1001);
    // tdrStyle->SetStatX(Float_t x = 0);
    // tdrStyle->SetStatY(Float_t y = 0);

    // Margins:
    tdrStyle->SetPadTopMargin(0.05);
    tdrStyle->SetPadBottomMargin(0.13);
    tdrStyle->SetPadLeftMargin(0.16);
    tdrStyle->SetPadRightMargin(0.02);

    // For the Global title:

    tdrStyle->SetOptTitle(0);
    tdrStyle->SetTitleFont(42);
    tdrStyle->SetTitleColor(1);
    tdrStyle->SetTitleTextColor(1);
    tdrStyle->SetTitleFillColor(10);
    tdrStyle->SetTitleFontSize(0.05);
    // tdrStyle->SetTitleH(0); // Set the height of the title box
    // tdrStyle->SetTitleW(0); // Set the width of the title box
    // tdrStyle->SetTitleX(0); // Set the position of the title box
    // tdrStyle->SetTitleY(0.985); // Set the position of the title box
    // tdrStyle->SetTitleStyle(Style_t style = 1001);
    // tdrStyle->SetTitleBorderSize(2);

    // For the axis titles:

    tdrStyle->SetTitleColor(1, "XYZ");
    tdrStyle->SetTitleFont(42, "XYZ");
    tdrStyle->SetTitleSize(0.06, "XYZ");
    // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
    // tdrStyle->SetTitleYSize(Float_t size = 0.02);
    tdrStyle->SetTitleXOffset(0.9);
    tdrStyle->SetTitleYOffset(1.25);
    // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

    // For the axis labels:

    tdrStyle->SetLabelColor(1, "XYZ");
    tdrStyle->SetLabelFont(42, "XYZ");
    tdrStyle->SetLabelOffset(0.007, "XYZ");
    tdrStyle->SetLabelOffset(0.0075, "Y");
    tdrStyle->SetLabelSize(0.05, "XYZ");

    // For the axis:

    tdrStyle->SetAxisColor(1, "XYZ");
    tdrStyle->SetStripDecimals(kTRUE);
    tdrStyle->SetTickLength(0.03, "XYZ");
    tdrStyle->SetNdivisions(510, "XYZ");
    tdrStyle->SetPadTickX(0);  // To get tick marks on the opposite side of the frame
    tdrStyle->SetPadTickY(0);

    // Change for log plots:
    tdrStyle->SetOptLogx(0);
    tdrStyle->SetOptLogy(0);
    tdrStyle->SetOptLogz(0);

    // Postscript options:
    tdrStyle->SetPaperSize(20.,20.);
    // tdrStyle->SetLineScalePS(Float_t scale = 3);
    // tdrStyle->SetLineStyleString(Int_t i, const char* text);
    // tdrStyle->SetHeaderPS(const char* header);
    // tdrStyle->SetTitlePS(const char* pstitle);

    // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
    // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
    // tdrStyle->SetPaintTextFormat(const char* format = "g");
    // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
    // tdrStyle->SetTimeOffset(Double_t toffset);
    // tdrStyle->SetHistMinimumZero(kTRUE);

    tdrStyle->cd();

}
