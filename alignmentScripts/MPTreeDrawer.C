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
int markerStyles[8]= {20,24,21,25,22,26,23,32};
int lineStyles[3]= {1,7,3};
int lineColors[8]= {13,kPink-9,kAzure+7,kSpring-6,kOrange+1,kPink-1,kAzure+10,kSpring+9};
int fillStyles[2]= {1001,3008};
TString inputFileName = "/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/mp1296/jobData/jobm/treeFile_merge.root";
TString outputPath = "/afs/cern.ch/cms/CAF/CMSALCA/ALCA_TRACKERALIGN/MP/MPproduction/mp1296/LA_evol_test";
TFile *inputFile;       // ROOT file (treeFile_merge.root) with the calibration result

std::vector<TString> DetName;

int nPointsToSkip=1;	// Number of points that should be skipped from the end
int maxIOVs = -1;       // Maximum number of IOVs to analyze
//float lumiScale=0.826;
float lumiScale=1.0;
bool drawLegend=true;
float minY=-1.f;
float maxY=1.f;
bool autoScaleY = true;
bool drawInput = true;      // Whether to draw lines for input LA
FILE *logFile;
FILE *runsFile;

bool canvasSquare=false;
float canvasW = 1000;
float canvasH = 700;


float iovWidth = -1.f;	  // Width of one IOV in fb-1 for labels of X axis

int firstRun = 190000;
int lastRun = 209091;


std::vector<int> iov1stRuns(0);         // Vector of first run numbers of each iov in the file (in the tree names)
std::vector<int> iov1stRuns_(0);        // Vector of first run numbers of each iov in the file (to be plotted)

struct POINT2                            // Point represented by 2 values
{
    POINT2(float v1_=9999.9, float v2_=-9999.9) : v1(v1_), v2(v2_) {};
    float v1;
    float v2;
};

// Struct that contains ranges that determine the position of the group of the modules
struct GRANUL
{
    int subDetId;
    POINT2 r;
    POINT2 z;
    POINT2 phi;
};

// Struct that contains all distinct calibration values
// Also contains the ranges of module positions in group of each value
struct GRANVAL
{
    std::vector<double> val;
    std::vector<double> val_e;
    std::vector<GRANUL> detPos;
};

std::vector<GRANVAL> calibOutValues;        // List of distinct output values for each granularity direction
GRANVAL calibInValues;                      // List of values for each granularity from calibOutValues
GRANVAL calibInValues_simple;               // List of only distinct input values
std::vector<GRANUL> allGranularities;       // List of granularities present in the root file

// ROOT file that contains all information about tracker geometry layout
TString geometryFileName = "/afs/cern.ch/user/n/nbartosi/cms/cms_usercode/alignmentScripts/TrackerTree.root";
TString geometryTreeName = "TrackerTreeGenerator/TrackerTree/TrackerTree";
struct geomStruct
{
    unsigned short int subdet;
    float r;
    float z;
    float phi;
};
std::map<unsigned int, geomStruct> GEOMETRY;     // Maps detid to its position in the ideal geometry

TCanvas* canvas;        // Canvas to which the result will be drawn

bool log_ = false;

//////////////////////////
// FUNCTIONS DEFINITION //
//////////////////////////

void setGraphStyle ( TGraph *graph, int id, int file );
TH1* drawEmptyHisto ( float Xmin=0, float Xmax=20, TString xTitle="", float Ymin=0, float Ymax=1, TString yTitle="", TString name="empty" );
void setLegendStyle ( TLegend *leg, const int ncols = 1, const int nDetParts = 6);
void setTDRStyle();
// void tdrGrid(bool gridOn);
// void fixOverlay();
void getCalibrationsOfType(TString calibrationType = "LorentzAngle", TString moduleType = "Pixel", TString stripReadoutMode = "");
int nIOVsInTrees(TString treeBaseName, std::vector<int>& iovs, std::vector<int>& iovs_);
GRANVAL distValuesFromTrees(TString treeName);
int plotCalibration( const TString granulId,
    const TString granulParameters = "", 
    const float scale = 1.f, 
    const int nCols = 1,
    const std::string yTitle="result",
    const std::string histoTitle="",
    const std::string fileName = "");
void analyzeGranularity();
int granulInVector(std::vector<GRANUL>& vector, GRANUL& checkItem );
bool granulsAreSame(GRANUL& g1, GRANUL& g2, float minD = 0.005);
bool valsAreClose(float val1, float val2, float minD = 0.005);
float iovLumiWidth(int run0, int run1, int run2);
int decodeGranularityList( TString granulId_, std::vector<int>& granulId, 
    TString granulParameters_, std::vector<std::string>& granulNames, std::vector<int>& granulColours, std::vector<int>& granulStyles );
int analyzeGeometry();


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
int MPTreeDrawer()
{
    setTDRStyle();

    DetName.push_back("");
    DetName.push_back("BPIX");      // 1
    DetName.push_back("FPIX");      // 2
    DetName.push_back("TIB");       // 3
    DetName.push_back("TID");       // 4
    DetName.push_back("TOB");       // 5
    DetName.push_back("TEC");       // 6


    // Opening the input ROOT file
    inputFile = new TFile( inputFileName );
    if(inputFile->IsZombie()) {
        printf("Couldn't open the input file: %s\n", inputFileName.Data());
        printf("Set correct path of the input file to <inputFileName> variable and rerun MPTreeDrawer().\n");
    }

    // Analyzing the geometry file
    int nModulesInGeometry = analyzeGeometry();
    if(log_) printf("Added %d modules to the geometry used for the granularity detection.\n", nModulesInGeometry);

    // Creating a canvas
    if(!canvas) canvas = new TCanvas("c1", "Calibration result", canvasW, canvasH);


    return 0;
}

void getCalibrationsOfType(TString calibrationType, TString moduleType, TString stripReadoutMode)
{
    // Opening files for logging
    logFile = fopen( "log.txt","w" );          // Debugging logs
    runsFile = fopen( "runs.txt","w" );        // Run ranges that are used (for calculation of the luminosity)


    if(stripReadoutMode == "deco") stripReadoutMode = "deconvolution";
    if(stripReadoutMode != "") stripReadoutMode = "_"+stripReadoutMode;

    // The base of the input tree name
    TString TreeBaseDir("Si"+moduleType+calibrationType+"Calibration"+stripReadoutMode+"_result_");
    int nIOVs = nIOVsInTrees(TreeBaseDir, iov1stRuns, iov1stRuns_);
    printf("Found %d IOVs with name: %s ...\n",nIOVs, TreeBaseDir.Data());

    // Cleaning the calibration values
    calibOutValues.clear();
    allGranularities.clear();

    // Getting the calibration result values for each IOV
    for(int iovId = 0; iovId < nIOVs; iovId++) {
        if(maxIOVs>=0 && iovId >= maxIOVs) break;

        TString TreeBaseNameOut = TreeBaseDir;
        TreeBaseNameOut += iov1stRuns.at(iovId);
        printf("IOV %d/%d \tTree: %s \t", iovId+1,nIOVs,TreeBaseNameOut.Data());
        // Filling the Granularity with calibration output values/errors
        GRANVAL distOutValues = distValuesFromTrees(TreeBaseNameOut);
        printf("nValues: %d\n", (int)distOutValues.val.size());

        calibOutValues.push_back(distOutValues); // Adding it to the vector for the current IOV

    }

    // Getting the calibration input values
    TString TreeBaseNameIn = TreeBaseDir.ReplaceAll("_result_","_input");
    printf("Input Tree: %s \t",TreeBaseNameIn.Data());
    calibInValues_simple = distValuesFromTrees(TreeBaseNameIn);
    printf("nValues: %d\n", (int)calibInValues_simple.val.size());

    analyzeGranularity();

    fclose ( logFile );
    fclose ( runsFile );
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
    if(nIOVs>3) {
        if(firstRun > 0) iovs_.at(0) = firstRun; 
        else iovs_.at(0) = iovs.at(1) - std::abs( iovs.at(2) - iovs.at(1) );
    }
    // Adding one more run to the end as the last run of the last IOV
    if(nIOVs>2) {
        if(lastRun > 0) iovs_.push_back(lastRun);
         else iovs_.push_back( iovs.at(nIOVs-1) + std::abs( iovs.at(nIOVs-1) - iovs.at(nIOVs-2) ) );
    }

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
    distValues.detPos = std::vector<GRANUL>();

    // Parameters to be read from the tree of calibrations
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
    if(log_) printf ( "  Got Tree with name: %s \t with nEntries: %lld\n",treeName.Data(),nEntries ); 

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
        bool isInVector = valIndex < distValues.val.size() && distValues.val.size() > 0;
        if(!isInVector) {
            distValues.val.push_back(value);
            valIndex = distValues.val.size() - 1;
            distValues.val_e.push_back(error);
            GRANUL modules;
            modules.subDetId = 0;
            modules.r = POINT2();
            modules.z = POINT2();
            modules.phi = POINT2();
            distValues.detPos.push_back(modules);
        } 

        // Getting the geometry information for the current module
        geomStruct geomInfo = GEOMETRY[detId];

        int subDetId = geomInfo.subdet;
        float posR = geomInfo.r;
        float posZ = geomInfo.z;
        float posPhi = geomInfo.phi;

        distValues.detPos.at(valIndex).subDetId = subDetId;
        POINT2& R = distValues.detPos.at(valIndex).r;
        POINT2& Z = distValues.detPos.at(valIndex).z;
        POINT2& Phi = distValues.detPos.at(valIndex).phi;
        // Updating the range values for each position component
        if(posR<R.v1) R.v1 = posR;      // Radius
        if(posR>R.v2) R.v2 = posR;
        if(posZ<Z.v1) Z.v1 = posZ;      // Z
        if(posZ>Z.v2) Z.v2 = posZ;
        if(posPhi<Phi.v1) Phi.v1 = posPhi;  // Phi
        if(posPhi>Phi.v2) Phi.v2 = posPhi;

    }      // End of loop over entries

    int nDistVals = distValues.val.size();
    if(log_) printf("      Number of distinct values: %d\n", nDistVals);

    if(log_) {
        for(int iVal = 0; iVal<nDistVals; iVal++) {
            printf("        %d: Val: %.3f += %.2f (subdet: %d)\n",iVal, distValues.val.at(iVal), distValues.val_e.at(iVal), distValues.detPos.at(iVal).subDetId);
            printf("             R: %.3f - %.3f\n", distValues.detPos.at(iVal).r.v1, distValues.detPos.at(iVal).r.v2);
            printf("             Z: %.3f - %.3f\n", distValues.detPos.at(iVal).z.v1, distValues.detPos.at(iVal).z.v2);
            printf("           Phi: %.3f - %.3f\n", distValues.detPos.at(iVal).phi.v1, distValues.detPos.at(iVal).phi.v2);
        }
    }

    return distValues;
}


/**
 * Plots calibration result and input values for the specified group(s) of the modules
 * @method plotCalibration
 * @param  granulId        [description]
 * #param  granulNames     [description]
 * @param  toSave          [description]
 * @return                 [description]
 */
int plotCalibration( const TString granulId_, const TString granulParameters_, const float scale, const int nCols, 
                    const std::string yTitle, const std::string histoTitle, const std::string fileName)
{
    std::vector<int> granulId(0);
    std::vector<int> granulColours(0);
    std::vector<int> granulStyles(0);
    std::vector<std::string> granulNames(0);
    const int nCurves = decodeGranularityList(granulId_, granulId, granulParameters_, granulNames, granulColours, granulStyles);

    if(log_) {
        printf("  Plotting results for %d granularities:\n", nCurves);
        for(int iG = 0; iG < nCurves; iG++) {
            printf("    %d. Id: %d\tName: %s\tColour: %d\tStyle: %d\n", 
                iG, granulId.at(iG), granulNames.at(iG).c_str(), granulColours.at(iG), granulStyles.at(iG) );
        }
        printf("\n");
    }

    // Initializing vector of TGraphs
    std::vector<TGraphErrors*> graphsOut(nCurves);
    std::vector<TGraphErrors*> graphsIn(nCurves);

    // Creating the TGraphs in the vector
    for(int iG=0; iG < (int)graphsOut.size(); iG++) {
        graphsOut.at(iG) = new TGraphErrors();
        graphsIn.at(iG) = new TGraphErrors();
    }

    float totLumi = 0.f;

    // Determining the range of the Y axis
    float minY_ = minY;
    float maxY_ = maxY;
    if(autoScaleY) {
        minY_=999.9;
        maxY_=-999.9;
    }

    // Looping through the IOVs of the output calibration values
    for(int iovId = 0; iovId < (int)calibOutValues.size(); iovId++) {
        // Getting the granularity and values for the current IOV
        GRANVAL& distValues = calibOutValues.at(iovId);

        float lumiWidth = iovLumiWidth(iov1stRuns_.at(0), iov1stRuns_.at(iovId), iov1stRuns_.at(iovId+1));
        if(lumiWidth < 0.f) continue;       // Skipping IOV if its luminosity couldn't be determined

        float lumiPoint = totLumi + 0.5*lumiWidth;
        totLumi += lumiWidth;
        
        // Looping through the values in each IOV to determine number of subDetIds
        for(int valId = 0; valId < (int)distValues.val.size(); valId++) {
            GRANUL& theModules = distValues.detPos.at(valId);
            // Getting the id of the granularity in list of all granularities
            int theGranulId = granulInVector(allGranularities, theModules);
            // Getting index of this granularity id in the list of granularities to be plotted
            int theGranulIndex = std::find(granulId.begin(), granulId.end(), theGranulId) - granulId.begin();
            if(theGranulIndex>=nCurves) continue;   // Skipping values that are not for granularity from the plotting list
            
            // printf("  %d. det: %d\tR: [%.3f|%.3f]\tZ: [%.3f|%.3f]\tPhi: [%.3f|%.3f]\tVal: %.2f +- %.2f\n", 
                // theGranulIndex, theModules.subDetId, theModules.r.v1, theModules.r.v2, theModules.z.v1, theModules.z.v2, theModules.phi.v1, theModules.phi.v2,
                // distValues.val.at(valId), distValues.val_e.at(valId));

            TGraphErrors* graph = graphsOut.at(theGranulIndex);
            if((int)iov1stRuns_.size() < iovId) {
                printf("Requested run number of non-existing IOV: %d. Stopping...\n", iovId);
                break;
            }


            // Setting the values to the additional point in the TGraph
            int nPoints = graph->GetN();
            float val = scale*distValues.val.at(theGranulId);
            float val_e = scale*distValues.val_e.at(theGranulId);
            graph->SetPoint(nPoints, lumiPoint, val);
            graph->SetPointError(nPoints, 0.f, val_e);
            // printf("      Set point %d to values: x: %.3f y: %.3f\n", nPoints, lumiPoint, val);

            // Updating the Y axis range
            if ( val - val_e < minY_ && autoScaleY && val != 0.f) {
                minY_ = val - val_e;
            }
            if ( val + val_e > maxY_ && autoScaleY && val != 0.0) {
                maxY_ = val + val_e;
            }

        }       // End of loop over distinct values in the IOV
    }       // End of loop over IOVs

    // Autoscaling the Y axis range
    if(autoScaleY) {
        minY_= ( minY_>0 ) ?minY_*0.98:minY_*1.02;
        maxY_= ( maxY_>0 ) ?maxY_*1.05:maxY_*0.95;
    }

    // Drawing the axis histogram
    TH1* axisHisto = drawEmptyHisto ( 0.0,totLumi,"2012 Integrated Luminosity [fb^{-1}]",minY_,maxY_,yTitle.c_str(),"empty1" );

    // Creating a legend pane
    TLegend *leg = new TLegend ( 0.7, 0.77, 0.98, 0.95, NULL, "brNDC" );

    // Drawing each calibration output value
    int nPlotted = 0;
    for(int iC = 0; iC < nCurves; iC++) {
        TGraphErrors* graph = graphsOut.at(iC);
        if(graph->GetN() < 1) continue;
        nPlotted++;

        setGraphStyle(graph, granulColours.at(iC), granulStyles.at(iC));
        graph->Draw("Psame");
        leg->AddEntry(graph, granulNames.at(iC).c_str(), "p");
    }
        
    setLegendStyle(leg, nCols, nPlotted);

    if (drawLegend) leg->Draw();

    if(log_) printf("Plotted %d curves out of %d output calibration curves.\n", nPlotted, nCurves);


    // // Drawing CMS Preliminary label
    // TLatex *TextCMS = new TLatex(0.2,0.89,"CMS Preliminary 2012");
    // TextCMS->SetTextColor(1);
    // TextCMS->SetNDC();
    // TextCMS->SetTextFont(62);
    // TextCMS->Draw();

    TLatex *TextData = new TLatex(0.2,0.89,histoTitle.c_str());
    TextData->SetTextColor(1);
    TextData->SetNDC();
    TextData->SetTextFont(62);
    TextData->Draw();


    if(fileName == "") return nPlotted;

    // Creating the output folder if it doesn't exist
    gROOT->ProcessLine(".mkdir -p "+outputPath);
    // Saving the canvas to file
    canvas->Print(outputPath+"/"+fileName+".eps");

    // Cleaning
    canvas->Clear();
    if(axisHisto) delete axisHisto;


    return 0;
}


/**
 * Analyzes groups of modules in each dimension (R, Z, Phi) that has distinct caibration value
 * @method analyzeGRANVALity
 */
void analyzeGranularity()
{
    std::vector<GRANUL> selModules (0);

    // Looping through the IOVs
    for(int iovId = 0; iovId < (int)calibOutValues.size(); iovId++) {
        // Looping through the values in each IOV to determine number of subDetIds
        GRANVAL& distValues = calibOutValues.at(iovId);
        for(int valId = 0; valId < (int)distValues.val.size(); valId++) {
            GRANUL& theModules = distValues.detPos.at(valId);
            if(granulInVector(selModules, theModules) >= 0) continue;       // If this group is already selected

            selModules.push_back(theModules);
            // printf("  %d. det: %d\tR: [%.3f|%.3f]\tZ: [%.3f|%.3f]\tPhi: [%.3f|%.3f]\n", valId+1, subDetId, R.v1, R.v2, Z.v1, Z.v2, Phi.v1, Phi.v2);
        }
    }

    allGranularities = selModules;

    // Checking all granularities in the calibration result
    printf("\nAll granularities (%d) in the file:\n", (int)allGranularities.size());
    int nSameR = 0;
    int nSameZ = 0;
    int nSamePhi = 0;
    // Additional formats for different layers, rings and phi sections
    std::vector<std::string> addStr;
    addStr.push_back("");
    addStr.push_back(" ");
    int iR = 0, iZ = 0, iPhi = 0;       // Index of format string added before R,Z Z, Phi range
    for(int iG = 0; iG < (int)allGranularities.size(); iG++){
        GRANUL gr = allGranularities.at(iG);

        // Deciding on format to add before layer, ring, phi section
        if(iG>0) {
            if(gr.subDetId != allGranularities.at(iG-1).subDetId) { nSameR=0; nSameZ=0; nSamePhi=0; printf("\n");}
            if(valsAreClose(gr.r.v1, allGranularities.at(iG-1).r.v1) ) nSameR++; 
            else if(nSameR>0) {nSameR=0; nSameZ=0; nSamePhi=0; iR = 1 - iR;}
            if(valsAreClose(gr.z.v1, allGranularities.at(iG-1).z.v1) ) nSameZ++; 
            else if(nSameZ>0) {nSameR=0; nSameZ=0; nSamePhi=0; iZ = 1 - iZ;};
            if(valsAreClose(gr.phi.v1, allGranularities.at(iG-1).phi.v1) ) nSamePhi++;
            else if(nSamePhi>0) {nSameR=0; nSameZ=0; nSamePhi=0; iPhi = 1 - iPhi;};
        }
        printf("  %d.\tdetId: %d\t%sR: [%.2f|%.2f]   \t%sZ: [%.2f|%.2f]   \t%sPhi: [%.2f|%.2f]\n", iG, gr.subDetId, addStr.at(iR).c_str(), 
            gr.r.v1, gr.r.v2, addStr.at(iZ).c_str(),
            gr.z.v1, gr.z.v2, addStr.at(iZ).c_str(), 
            gr.phi.v1, gr.phi.v2);
    }
    printf("\nPlot calibration values for the groups specifying its ids and names:\n");
    printf("  plotCalibration(\"0,1,2\",\"Ring 1|0|1, Ring 2|1|1, Ring 3|2|1\",\"test1\")  [EXAMPLE (No title, No saving)]\n\n");

}


/**
 * Calculates the luminosity for a given run range
 * @method iovLumiWidth
 * @param  run0         First run of the first IOV
 * @param  run1         First run of the given IOV
 * @param  run2         Last run of the given IOV
 * @return              Lumonisity of the given run range. -1 if luminosity couldn't be determined
 */
float iovLumiWidth(int run0, int run1, int run2) {
    float lumiWidth = -1.f;

    float lumi1 = lumisInIOV(run0, run1);
    if(run0 == run1) lumi1 = 0.f;
    float lumi2 = lumisInIOV(run0, run2);
    lumiWidth = std::fabs(lumi2 - lumi1);
    if(lumi1 < 0.f || lumi2 < 0.f) {
        printf("Couldn't get precise luminosity for the run range: [%d - %d] with starting run: %d\nWill try to use less precise estimation...\n", run1, run2, run0);
        lumiWidth = lumisInIOV(run1, run2);
        if(lumiWidth < 0.f) {
            printf("Couldn't get luminosity for the run range: [%d - %d]\nWill skip this IOV...\n", run1, run2);
        }
    }

    lumiWidth/=1000.f;      // Converting 1/fb

    return lumiWidth;
}


/**
 * Checks whether the granularity struct is in the vector of granularities
 * @method granulInVector
 * @param  vector       Vector of granularities to be checked
 * @param  checkItem    Granularity struct to be checked
 * @return      Index of checkItem in vector. If -1, not in vector.
 */
int granulInVector(std::vector<GRANUL>& vector, GRANUL& checkItem )
{
    int index = -1;

    // Looping through the items of the vector
    for(int iG = 0; iG < (int)vector.size(); iG++) {
        GRANUL item = vector.at(iG);

        if(!granulsAreSame(item, checkItem)) continue;

        index = iG;
        break;
    }

    return index;
}

/**
 * Checks whether the two granularity structs are same
 * @method granulsAreSame
 * @param  g1   First granularity struct
 * @param  g2   Second granularity struct
 * @param  minD Minimal difference between float parameters of the granularities to be treated as different
 * @return      True if both are same
 */
bool granulsAreSame(GRANUL& g1, GRANUL& g2, float minD)
{
    if(g1.subDetId != g2.subDetId) return false;

    if(!valsAreClose( g1.r.v1, g2.r.v1, minD )) return false;
    if(!valsAreClose( g1.r.v2, g2.r.v2, minD )) return false;

    if(!valsAreClose( g1.z.v1, g2.z.v1, minD )) return false;
    if(!valsAreClose( g1.z.v2, g2.z.v2, minD )) return false;

    if(!valsAreClose( g1.phi.v1, g2.phi.v1, minD )) return false;
    if(!valsAreClose( g1.phi.v2, g2.phi.v2, minD )) return false;

    return true;
}


/**
 * Checks whether two values are close
 * @method valsAreClose
 * @param  val1     First value
 * @param  val2     Second value
 * @param  minD     Minimal distance betwwen values to be treated as different
 * @return  True if values are close
 */
bool valsAreClose(float val1, float val2, float minD)
{
    if(std::fabs((val1 - val2)/val1) > minD) return false;
    return true;
}

void setGraphStyle ( TGraph *graph, int colourId, int styleId )
{
    graph->SetMarkerColor(lineColors[colourId]);
    graph->SetLineColor(lineColors[colourId]);
    graph->SetMarkerStyle(markerStyles[styleId]);
    graph->SetLineStyle(lineStyles[styleId]);
    graph->SetLineWidth(2.f);

    return;
}

TH1* drawEmptyHisto ( float Xmin, float Xmax, TString xTitle, float Ymin, float Ymax, TString yTitle, TString name )
{
    if(log_) printf ( "Setting empty histo: X: %.2f-%.2f\tY: %.2f-%.2f\n",Xmin,Xmax,Ymin,Ymax );
    if(log_) printf("Total luminosity to be plotted: %.3f\n",Xmax);
    if(lumiScale>1.0) printf("Luminosity scale used: %.2f\n",lumiScale);

    Xmax*=1.02;	  // Increasing max lumi to have free space
    gStyle->SetOptStat ( 0 );
    //    TGaxis::SetMaxDigits ( 4 );

    TH1F *histo = new TH1F ( name, xTitle, 1, Xmin, Xmax );
    TAxis *xAx = histo->GetXaxis();
    TAxis *yAx = histo->GetYaxis();
    xAx->SetTitle ( xTitle );
    //    xAx->SetTitleOffset ( 1.1 );
    //    xAx->SetNdivisions ( 510 );
    yAx->SetTitle ( yTitle );
    //    yAx->SetTitleOffset ( 1.15 );
    //    yAx->SetNdivisions ( 510 );
    histo->SetMinimum ( Ymin );
    histo->SetMaximum ( Ymax );
    histo->Draw ( "AXIS" );

    return histo;
}

void setLegendStyle ( TLegend *leg, const int ncols, const int nDetParts )
{
    leg->SetTextFont ( 42 );
//    leg->SetTextSize ( 0.04 );
    leg->SetFillStyle ( 1001 );
    leg->SetBorderSize ( 1 );
//    leg->SetLineColor ( 1 );
    leg->SetFillColor ( 0 );
    leg->SetNColumns ( ncols );
    if(ncols==1) {
        leg->SetX1(leg->GetX1() + 0.5*(leg->GetX2() - leg->GetX1() ));
    }
    leg->SetY1(leg->GetY2() - nDetParts/ncols*(0.05));
}

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


/**
 * Extract from the input strings the list of granularity ids to be plotted and its parameters
 * @method decodeGranularityList
 * @param  granulId_             String with comma separated list of granularity ids
 * @param  granulId              Reference to vector that will be filled by the granularity ids
 * @param  granulParameters_     String with comma separated list of parameters
 * @param  granulNames           Reference to vector that will be filled by the names of each granularity
 * @param  granulColours         Reference to vector that will be filled by the colour of each granularity
 * @param  granulStyles          Reference to vector that will be filled by the style of each granularity
 * @return                       Number of granularities in the list [granulId]
 */
int decodeGranularityList( TString granulId_, std::vector<int>& granulId, 
    TString granulParameters_, std::vector<std::string>& granulNames, std::vector<int>& granulColours, std::vector<int>& granulStyles ) {

    TObjArray* granulId_list = granulId_.ReplaceAll(" ","").Tokenize(",");
    for(int iG = 0; iG < granulId_list->GetEntries(); iG++) {
        granulId.push_back( ((TObjString*)granulId_list->At(iG))->String().Atoi() );
    }


    TObjArray* granulParameters_list = granulParameters_.ReplaceAll(", ",",").Tokenize(",");
    for(int iG = 0; iG < granulId_list->GetEntries(); iG++) {
        TObjArray* granulParameters = ((TObjString*)granulParameters_list->At(iG))->String().Tokenize("|");
        int nPars = granulParameters->GetEntries();
        if(nPars>0) {
            granulNames.push_back( ((TObjString*)granulParameters->At(0))->String().Data() ); 
        }
        else {
            granulNames.push_back("");
            printf("ATTENTION! No name found for granularity with index: %d\n", iG);
        }
        if(nPars>1) {
            granulColours.push_back( ((TObjString*)granulParameters->At(1))->String().Atoi() );
        }
        else {
            granulColours.push_back(-1);
            printf("ATTENTION! No colour found for granularity with index: %d\n", iG);
        }
        if(nPars>2) {
            granulStyles.push_back( ((TObjString*)granulParameters->At(2))->String().Atoi() );
        }
        else {
            granulStyles.push_back(-1);
            printf("ATTENTION! No style found for granularity with index: %d\n", iG);
        }
    }


    return granulId.size();

}


/**
 * Fills the map GEOMETRY with appropriate values for each module in the geometry (subdet, r, z, phi)
 * @method buildGeometry
 * @return Number of entries added to the map
 */
int analyzeGeometry() {
    int nModules = 0;

    TFile* file = new TFile(geometryFileName);
    if(file->IsZombie()) {
        printf("ERROR! No geometry file found with name: %s\n", geometryFileName.Data());
        printf("Set correct path to file with geometry to <geometryFileName> variable and rerun MPTreeDrawer().\n");

        return 0;
    }


    TTree* tree = (TTree*)file->Get(geometryTreeName);
    if(!tree) {
        printf("ERROR! No geometry tree found with name: %s\n", geometryTreeName.Data());
        printf("Set correct name of the geometry tree to <geometryTreeName> variable and rerun MPTreeDrawer().\n");

        return 0;
    }

    // Parameters to be read from the geometry tree
    UInt_t RawId;
    UInt_t SubdetId;
    Float_t PosR;
    Float_t PosZ;
    Float_t PosPhi;
    // Activating corresponding branches and pinting them to variables
    tree->SetBranchStatus("*", 0);
    tree->SetBranchStatus("RawId", 1);
    tree->SetBranchAddress("RawId", &RawId);
    tree->SetBranchStatus("SubdetId", 1);
    tree->SetBranchAddress("SubdetId", &SubdetId);
    tree->SetBranchStatus("PosR", 1);
    tree->SetBranchAddress("PosR", &PosR);
    tree->SetBranchStatus("PosZ", 1);
    tree->SetBranchAddress("PosZ", &PosZ);
    tree->SetBranchStatus("PosPhi", 1);
    tree->SetBranchAddress("PosPhi", &PosPhi);

    int nEntries = tree->GetEntries();
    if(log_) printf("Analyzing geometry file with %d entries:\n", nEntries);

    for(int it = 0; it < nEntries; it++) {
        tree->GetEntry(it);

        geomStruct values;
        values.subdet = SubdetId;
        values.r = PosR;
        values.z = PosZ;
        values.phi = PosPhi;

        GEOMETRY[RawId] = values;

        nModules++;
    }

    return nModules;

}