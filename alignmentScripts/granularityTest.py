#datasetconfigname 0T_Coll_runc_v1
# last update on $Date: 2008/08/11 19:12:41 $ by $Author: flucke $
#data name: minbias

import FWCore.ParameterSet.Config as cms

process = cms.Process("Alignment")

#-- MessageLogger
# process.load("FWCore.MessageLogger.MessageLogger_cfi")
# This whole mess does not really work - I do not get rid of FwkReport and TrackProducer info...
process.MessageLogger = cms.Service("MessageLogger",
#    statistics = cms.untracked.vstring('alignment'), ##, 'cout')

    categories = cms.untracked.vstring('Alignment', 
	),
    granularityTest = cms.untracked.PSet(
        INFO = cms.untracked.PSet(
            limit = cms.untracked.int32(1)
        ),
        DEBUG = cms.untracked.PSet(
            limit = cms.untracked.int32(-1)
        ),
        WARNING = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        ERROR = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        Alignment = cms.untracked.PSet(
            limit = cms.untracked.int32(1)
        )
    ),
    destinations = cms.untracked.vstring('granularityTest') ## (, 'cou')

)


#-- Ideal geometry and interface

process.load("Configuration.Geometry.GeometryIdeal_cff")

## if alignment constants not from global tag, add this
# from CondCore.DBCommon.CondDBSetup_cfi import *
#####################################################################
## Global Tag
######################################################################

process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.connect = "frontier://FrontierProd/CMS_COND_31X_GLOBALTAG"
process.GlobalTag.globaltag = "FT_R_53_V21::All"


import Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi
process.HighPuritySelector = Alignment.CommonAlignmentProducer.AlignmentTrackSelector_cfi.AlignmentTrackSelector.clone(
    applyBasicCuts = True,
    #filter = True,
    src = 'ALCARECOTkAlMinBias',
    trackQualities = ["highPurity"],
    pMin = 4.9,
    pMax = 5.1
    )

####################################################
#-- Alignment producer
####################################################
process.load("Alignment.CommonAlignmentProducer.AlignmentProducer_cff")




process.AlignmentProducer.tjTkAssociationMapTag = "HighPuritySelector"

process.AlignmentProducer.algoConfig = cms.PSet(
    process.MillePedeAlignmentAlgorithm
)


process.AlignmentProducer.algoConfig.mode = 'mille'
process.AlignmentProducer.algoConfig.mergeBinaryFiles = cms.vstring()
process.AlignmentProducer.algoConfig.monitorFile = ''
process.AlignmentProducer.algoConfig.binaryFile = ''
process.AlignmentProducer.algoConfig.TrajectoryFactory = cms.PSet(
    process.BrokenLinesBzeroTrajectoryFactory
)

import Alignment.CommonAlignmentAlgorithm.SiStripLorentzAngleCalibration_cff as siStripLA
# assign by reference to finally get same values as AlignmentProducer
siStripLA.SiStripLorentzAngleCalibration_deco.treeFile       = process.AlignmentProducer.algoConfig.treeFile
siStripLA.SiStripLorentzAngleCalibration_deco.mergeTreeFiles = process.AlignmentProducer.algoConfig.mergeTreeFiles
siStripLA.SiStripLorentzAngleCalibration_deco.saveToDB       = process.AlignmentProducer.saveToDB

######################################################################## IOVS
RunRange = cms.vuint32(1)


######################################################################## LORENTZ ANGLE
siStripLA.SiStripLorentzAngleCalibration_deco.LorentzAngleModuleGroups.RunRange = RunRange
siStripLA.SiStripLorentzAngleCalibration_deco.LorentzAngleModuleGroups.Granularity = cms.VPSet()


############### Granularities to be tested
iSide = 1
iLayer = 3
iRod = 2
iModule=1
rMin=0.0
rMax=9999.9
r = 34.0
PI = 3.1415
siStripLA.SiStripLorentzAngleCalibration_deco.LorentzAngleModuleGroups.Granularity.append(
                cms.PSet(
                    levels = cms.PSet(
                        alignParams = cms.vstring(
                            'TrackerTOBModuleUnit,,theModules'
                            ),
                        theModules = cms.PSet(
                            tobDetId  = cms.PSet(
                                layerRanges = cms.vint32(iLayer,iLayer),
                                # moduleRanges = cms.vint32(iModule, iModule),
                                sideRanges = cms.vint32(iSide,iSide),
                                # stringRanges = cms.vint32(iRod,iRod)
                                ),
                            phiRanges = cms.vdouble(-1.57,1.57)
                            )
                        )
                    )
                )
siStripLA.SiStripLorentzAngleCalibration_deco.LorentzAngleModuleGroups.Granularity.append(
                cms.PSet(
                    levels = cms.PSet(
                        alignParams = cms.vstring(
                            'TrackerTOBModuleUnit,,theModules',
                            'TrackerTOBModuleUnit,,theModules2'
                            ),
                        theModules = cms.PSet(
                            tobDetId  = cms.PSet(
                                layerRanges = cms.vint32(iLayer,iLayer),
                                # moduleRanges = cms.vint32(iModule, iModule),
                                sideRanges = cms.vint32(iSide,iSide),
                                # stringRanges = cms.vint32(iRod,iRod)
                                ),
                            phiRanges = cms.vdouble(1.57,PI)
                            ),
                        theModules2 = cms.PSet(
                            tobDetId  = cms.PSet(
                                layerRanges = cms.vint32(iLayer,iLayer),
                                # moduleRanges = cms.vint32(iModule, iModule),
                                sideRanges = cms.vint32(iSide,iSide),
                                # stringRanges = cms.vint32(iRod,iRod)
                                ),
                            phiRanges = cms.vdouble(-PI,-1.57)
                            )
                        )
                    )
                )
# siStripLA.SiStripLorentzAngleCalibration_deco.LorentzAngleModuleGroups.Granularity.append(
#                 cms.PSet(
#                     levels = cms.PSet(
#                         alignParams = cms.vstring(
#                             'TrackerTIBModuleUnitStereo,,theModules'
#                             ),
#                         theModules = cms.PSet(
#                             tibDetId  = cms.PSet(
#                                 layerRanges = cms.vint32(iLayer,iLayer),
#                                 moduleRanges = cms.vint32(iModule, iModule),
#                                 sideRanges = cms.vint32(iSide,iSide),
#                                 stringRanges = cms.vint32(iRod,iRod)
#                                 ),
#                             rRanges = cms.vdouble(r, rMax)
#                             )
#                         )
#                     )
#                 )



# for iSide in range(1,3): # [1,2] Sides
#     for iModule in range(1,7): # [1,6] Modules
#         for iLayer in range(1,7): # [1,6] Layers
#             siStripLA.SiStripLorentzAngleCalibration_deco.LorentzAngleModuleGroups.Granularity.append(
#                 cms.PSet(
#                     levels = cms.PSet(
#                         alignParams = cms.vstring(
#                             'TrackerTOBModuleUnit,,layerModule'
#                             ),
#                         layerModule = cms.PSet(
#                             tobDetId  = cms.PSet(
#                                 layerRanges = cms.vint32(iLayer,iLayer),
#                                 moduleRanges = cms.vint32(iModule, iModule),
#                                 sideRanges = cms.vint32(iSide,iSide)
#                                 )
#                             )
#                         )
#                     )
#                 )



process.AlignmentProducer.calibrations = [
    siStripLA.SiStripLorentzAngleCalibration_deco
    ] 

readFiles = cms.untracked.vstring()

 
readFiles.extend(('/store/data/Run2012C/ZeroBias1/ALCARECO/TkAlMinBias-v2/000/201/475/E28C90EF-7EEF-E111-BD7D-003048F024F6.root',))

process.source = cms.Source("PoolSource",
    fileNames = readFiles
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
