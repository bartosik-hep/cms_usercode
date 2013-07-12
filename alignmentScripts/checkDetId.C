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

#include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/DetId/interface/DetId.h"
#include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/src/PXBDetId.cc"
#include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiPixelDetId/src/PXFDetId.cc"
#include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "/afs/cern.ch/user/n/nbartosi/cms/TkAl/MillePede/CMSSW_5_3_3_patch2/src/DataFormats/SiStripDetId/interface/TOBDetId.h"


int checkDetId( UInt_t detId )
{
    DetId det_ ( detId );
    printf("Checking detId: %d\n",detId);
    if ( det_.det() !=DetId::Tracker ) {
        printf("This detId is not in Tracker.\n");
        return -1;
    }


    switch(det_.subdetId()) {
        case PixelSubdetector::PixelBarrel :
            {
                PXBDetId det(detId);
                int layer = det.layer();
                int ring = det.module();
                printf("BPIX: Layer: %d Ring: %d\n",layer,ring);
                break;
            }
        case PixelSubdetector::PixelEndcap :
            {
                PXFDetId det(detId);
                int side = det.side();
                printf("FPIX: Side: %d\n",side);
                break;
            }
        case StripSubdetector::TIB :
            {
                TIBDetId det(detId);
                int layer = det.layer();
                int module = det.module();
                int side = det.side();
        	    printf("TIB: Layer: %d Module: %d Side: %d\n",layer,module,side);
                break;
            }
        case StripSubdetector::TOB :
            {
                TOBDetId det(detId);
                int layer = det.layer();
                int module = det.module();
                int side = det.side();
        	    printf("TOB: Layer: %d Module: %d Side: %d\n",layer,module,side);
                break;
            }
        } 

    return detId;
}
