import FWCore.ParameterSet.Config as cms

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
lumiSecs = cms.untracked.VLuminosityBlockRange()
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles, lumisToProcess = lumiSecs)
readFiles.extend( [
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/228/42CF11AD-5005-E211-B042-BCAEC518FF54.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/227/56907B2A-4E05-E211-89FA-003048D2BEAA.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/225/4C11A141-6805-E211-961B-001D09F24FBA.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/224/60288D76-0D05-E211-BC85-001D09F2447F.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/222/EA28E174-0D05-E211-82EC-001D09F29321.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/218/923E1869-0605-E211-B46B-001D09F25479.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/216/927B2C19-0005-E211-B52E-001D09F2424A.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/215/B4547719-0005-E211-824E-001D09F26C5C.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/211/5AC179A0-F004-E211-A345-001D09F25479.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/209/70F683E7-EF04-E211-8998-001D09F25460.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/206/E6C42E81-EE04-E211-BAFE-001D09F29533.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/204/D2487BB3-EB04-E211-B3C8-001D09F290CE.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/203/B6FDA1FE-EA04-E211-9A74-001D09F252E9.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/200/E8CD69DE-E604-E211-B85B-001D09F252E9.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/193/08D251AC-E404-E211-854C-001D09F2441B.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/190/38351043-E304-E211-902D-001D09F23C73.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/186/4CF5EB42-E304-E211-BFFC-001D09F2B2CF.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/184/A423C96E-E004-E211-950E-001D09F2924F.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/176/3EAB51A1-DB04-E211-9F1C-E0CB4E55365D.root",
	"/store/data/Run2012C/Cosmics/ALCARECO/TkAlCosmics0T-v2/000/203/174/5CC46633-D504-E211-9DBD-0019B9F72CE5.root"
])

lumiSecs.extend([
	'203174:1-203174:max',
	'203176:1-203176:max',
	'203184:1-203184:max',
	'203186:1-203186:max',
	'203190:1-203190:max',
	'203193:1-203193:max',
	'203200:1-203200:max',
	'203203:1-203203:max',
	'203204:1-203204:max',
	'203206:1-203206:max',
	'203209:1-203209:max',
	'203211:1-203211:max',
	'203215:1-203215:max',
	'203216:1-203216:max',
	'203218:1-203218:max',
	'203222:1-203222:max',
	'203224:1-203224:max',
	'203225:1-203225:max',
	'203227:1-203227:max',
	'203228:1-203228:max',
	])