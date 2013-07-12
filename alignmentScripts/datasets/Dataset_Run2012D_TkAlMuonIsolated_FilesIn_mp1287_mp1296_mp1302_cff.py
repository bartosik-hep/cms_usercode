import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

goodLumiSecs = LumiList.LumiList(filename = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON_MuonPhys.txt').getCMSSWString().split(',')

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
lumiSecs = cms.untracked.VLuminosityBlockRange()
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles, lumisToProcess = lumiSecs)
readFiles.extend( [
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/208/686/60BCAE03-2442-E211-9422-001D09F2437B.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/208/553/40B56C82-F140-E211-91F2-001D09F24EE3.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/208/551/C4400F57-1E42-E211-87D7-0030486780B4.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/207/922/BEA4E4C7-C23A-E211-AE5C-001D09F24FEC.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/207/921/E49731B6-183B-E211-BF02-003048D3756A.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/207/920/F817FA40-DC3A-E211-8355-5404A63886BD.root',
])
readFiles.extend( [
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/206/512/7050471E-3728-E211-B238-5404A63886B1.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/204/545/761BBEB1-5911-E211-AE3E-003048F11DE2.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/204/544/A4FB4F2F-7311-E211-B5FA-001D09F2841C.root',
'/store/data/Run2012D/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/204/541/4EC8FF6E-3E12-E211-963F-003048D2BC30.root',
])
readFiles.extend( [
])

lumiSecs.extend(goodLumiSecs)
