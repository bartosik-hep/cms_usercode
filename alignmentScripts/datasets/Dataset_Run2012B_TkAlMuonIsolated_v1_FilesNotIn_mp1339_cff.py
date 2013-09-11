import FWCore.ParameterSet.Config as cms
import FWCore.PythonUtilities.LumiList as LumiList

goodLumiSecs = LumiList.LumiList(filename = '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions12/8TeV/Prompt/Cert_190456-200601_8TeV_PromptReco_Collisions12_JSON_MuonPhys_v2.txt').getCMSSWString().split(',')

maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
readFiles = cms.untracked.vstring()
secFiles = cms.untracked.vstring()
lumiSecs = cms.untracked.VLuminosityBlockRange()
source = cms.Source ("PoolSource",fileNames = readFiles, secondaryFileNames = secFiles, lumisToProcess = lumiSecs)
readFiles.extend( [
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/452/D823C2D0-B9BA-E111-9275-001D09F23A20.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/438/96804576-CCB9-E111-A45C-002481E0D958.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/437/C481C417-4BB9-E111-AD07-003048D37580.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/433/24B89A75-35B9-E111-BD55-BCAEC5364C93.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/432/7E99E538-19B9-E111-ACDF-001D09F25479.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/431/E2141109-3EB9-E111-B0DA-001D09F253D4.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/363/D22452C7-8AB8-E111-A002-002481E0E56C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/250/AAE264B2-7EB8-E111-A302-0025901D627C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/249/F0402CE0-35B8-E111-94C8-001D09F24024.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/239/2E8E066B-09B8-E111-81CD-BCAEC518FF44.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/202/98C0D08D-E5B6-E111-9FC3-003048F1182E.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/197/F6007A9E-1DB8-E111-9E18-0030486780B4.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/135/B02A8B75-A8B6-E111-B63F-5404A6388697.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/131/4407EAB5-C0B6-E111-BD5A-BCAEC5364CFB.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/122/A8791515-5BB6-E111-866C-002481E0D90C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/107/AEAAB793-DAB6-E111-BF50-BCAEC5329709.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/196/103/12BFF6F4-44B6-E111-9942-BCAEC518FF30.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/948/9CF5A775-58B5-E111-ADC2-BCAEC518FF50.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/947/9E974B8D-EAB4-E111-AF3A-0025901D5D80.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/937/8C3D8999-BCB4-E111-B504-003048D2BBF0.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/868/8861BA80-F3B3-E111-A8A2-001D09F2B30B.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/841/502B0984-F1B3-E111-A521-001D09F2AD84.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/776/B21EAFD0-D1B3-E111-AA02-0025901D5DB2.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/775/F650D9C0-1EB3-E111-9EC0-001D09F248F8.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/664/64A15494-58B2-E111-A2A7-BCAEC518FF8F.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/658/CA3F367A-96B2-E111-BF09-001D09F2AD4D.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/656/B6FEDACA-B9B2-E111-8F11-5404A63886C1.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/655/6EB1D3F8-A5B2-E111-B7B0-001D09F29114.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/634/A698ADA4-24B2-E111-98FC-BCAEC5364C4C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/633/88DC4CEF-00B2-E111-890A-001D09F290CE.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/552/B4DB9493-F3B1-E111-AE5D-001D09F2924F.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/529/E4B528DC-C7B0-E111-AE2A-BCAEC53296F7.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/399/4AA2C084-81AF-E111-BEFB-001D09F2424A.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/398/66640844-D3AF-E111-8331-BCAEC5364C93.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/397/C891085F-CEAF-E111-8F10-0019B9F4A1D7.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/396/38369863-30AF-E111-924C-BCAEC518FF7C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/390/EE7DE3F2-EDAF-E111-BE51-001D09F24399.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/390/C2ABEE38-82AF-E111-8C98-001D09F2424A.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/390/8C0CA8A2-01B0-E111-8E71-001D09F2B30B.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/303/2AC610C5-53AD-E111-8834-003048D2BC62.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/266/1227549A-45AF-E111-8567-0025901D629C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/265/16A606DC-51AE-E111-8CA4-0025B320384C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/251/56B92605-DAAC-E111-8332-003048F1C424.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/165/12B95F4C-6CAC-E111-890F-001D09F2A49C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/164/C6320CF3-AFAB-E111-8CCD-BCAEC518FF54.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/163/420C2593-AAAC-E111-B671-001D09F28EA3.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/130/783DB84A-6EAA-E111-930E-BCAEC5329708.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/016/1E0AFB3A-B0A9-E111-99C0-001D09F26509.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/015/32AD9684-19A9-E111-9970-003048F024C2.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/014/483A2A47-43A9-E111-A8D1-003048F11112.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/195/013/AE2C9EDC-1CAA-E111-83BA-003048F117B6.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/915/B8D075C1-E0A8-E111-9A01-5404A63886CC.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/912/9EF9F50E-AEA9-E111-AAF6-001D09F241F0.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/897/9496A9AE-1EA8-E111-ADD5-003048D373F6.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/896/C87BFDBF-36A8-E111-A013-5404A63886CB.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/825/569CA4A9-A7A7-E111-B3DE-001D09F24682.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/790/027E0193-15A7-E111-92BD-001D09F2841C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/789/920C285B-A8A7-E111-BD69-001D09F253D4.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/711/98E7E9F4-94A6-E111-99A8-0025901D623C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/704/CE4D19F5-AAA6-E111-9138-BCAEC518FF30.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/702/6CEBAA6A-1DA6-E111-987A-001D09F25041.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/631/68B5F998-8FA5-E111-B814-001D09F24D67.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/627/7AF5B792-EAA4-E111-B718-0025901D5D7E.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/619/28C7DF2A-87A5-E111-AD62-003048D3756A.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/535/987C0DA9-ACA4-E111-B767-E0CB4E553651.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/533/FA49AFC3-ABA5-E111-AD6A-001D09F29597.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/480/5CC98465-C7A4-E111-AA3E-003048D2BC62.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/464/D8CFC187-03A5-E111-B72F-001D09F24D8A.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/455/FC7D2C4C-B7A4-E111-9F59-5404A63886BD.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/439/2EDDDBA7-39A4-E111-ABC2-BCAEC518FF41.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/429/56A4A9F6-9CA3-E111-A224-5404A63886C3.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/314/FC335B8C-53A2-E111-BB2C-003048D2C1C4.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/305/D25636EF-F9A1-E111-A988-BCAEC53296FB.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/304/C8734BAF-E4A1-E111-B956-001D09F24EE3.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/303/78B8231D-EBA1-E111-8714-001D09F2841C.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/270/98D64769-97A1-E111-901E-0025901D5D7E.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/251/0E000ECE-F0A0-E111-BBD7-0025901D6288.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/223/FCA7D80D-28A1-E111-8780-002481E94C7E.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/150/78A70675-8BA0-E111-B9E9-003048D37666.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/115/F06CE574-FC9F-E111-8B3B-BCAEC5329709.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/115/2A57A3CA-EC9F-E111-87D9-0025901D624A.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/108/82F6D1CE-BD9F-E111-8F2F-003048D2BD66.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/076/888523BE-ED9E-E111-A67C-003048D37560.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/075/D22F9657-B19E-E111-85C8-BCAEC518FF54.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/194/027/12F3D627-BC9D-E111-A499-003048D3756A.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/193/999/AE18910D-BA9D-E111-8AF4-001D09F2527B.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/193/998/8A666632-C39D-E111-BFDE-003048F11112.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/193/928/76C4B0C7-0D9D-E111-9CB8-BCAEC518FF6B.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/193/925/F29F974F-E39C-E111-BAC1-003048D3C980.root",
"/store/data/Run2012B/SingleMu/ALCARECO/TkAlMuonIsolated-v1/000/193/752/40F1AED3-7C9B-E111-B908-001D09F24664.root"
] )

lumiSecs.extend(goodLumiSecs)

