import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

import FWCore.Utilities.FileUtils as FileUtils
<<<<<<< HEAD
mylist = FileUtils.loadListFromFile('130.txt')
=======
mylist = FileUtils.loadListFromFile('90.txt')
>>>>>>> 2908c8155495873c1df20881babf7e78b9fd93ad
process.source = cms.Source("PoolSource",fileNames =
cms.untracked.vstring( *mylist ))

process.demo = cms.EDAnalyzer('HiggsGenHltRecoAnalyzer',
    GenParticleLabel = cms.InputTag("genParticles"),
    hltInputTag      = cms.InputTag( "TriggerResults::HLT" )
)


process.p = cms.Path(process.demo)
