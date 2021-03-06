# Auto generated configuration file
# using: 
# Revision: 1.381.2.20 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: Configuration/GenProduction/python/EightTeV/POWHEG_PYTHIA6_Tauola_H_GG_8TeV_cff.py --filein file:/tmp/amassiro/prova.root --fileout /tmp/amassiro/provaGENSIM.root --mc --eventcontent RAWSIM --datatier GEN-SIM --conditions START53_V7C::All --beamspot Realistic8TeVCollision --step GEN,SIM
import FWCore.ParameterSet.Config as cms

from FWCore.ParameterSet.VarParsing import VarParsing
options = VarParsing ('analysis')
# add a list of strings for events to process
options.parseArguments()


process = cms.Process('SIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('SimGeneral.MixingModule.mixNoPU_cfi')
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.GeometrySimDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.Generator_cff')
process.load('IOMC.EventVertexGenerators.VtxSmearedRealistic8TeVCollision_cfi')
process.load('GeneratorInterface.Core.genFilterSummary_cff')
process.load('Configuration.StandardSequences.SimIdeal_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')



process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(NUMBEREVENTS)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    firstEvent = cms.untracked.uint32(FIRSTEVENT),
    fileNames = cms.untracked.vstring (options.inputFiles),
)


process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.381.2.20 $'),
    annotation = cms.untracked.string('Configuration/GenProduction/python/EightTeV/POWHEG_PYTHIA6_Tauola_H_GG_8TeV_cff.py nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)


# Output definition

process.RAWSIMoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
    outputCommands = process.RAWSIMEventContent.outputCommands,
    fileName = cms.untracked.string (options.outputFile),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('GEN-SIM')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('generation_step')
    )
)

# Additional output definition

# Other statements
process.genstepfilter.triggerConditions=cms.vstring("generation_step")
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, 'START53_V7C::All', '')

process.generator = cms.EDFilter("Herwig6HadronizerFilter",
        comEnergy = cms.double(8000.0),
        doMPInteraction = cms.bool(False),
        emulatePythiaStatusCodes = cms.untracked.bool(True),
        filterEfficiency = cms.untracked.double(1.0),
        herwigHepMCVerbosity = cms.untracked.bool(False),
        herwigVerbosity = cms.untracked.int32(0),
        lhapdfSetPath = cms.untracked.string(''),
        maxEventsToPrint = cms.untracked.int32(0),
        printCards = cms.untracked.bool(False),
        useJimmy = cms.bool(False),
        doMatching = cms.untracked.bool(False),
        nMatch = cms.untracked.int32(0),
        inclusiveMatching = cms.untracked.bool(True),
        matchingScale = cms.untracked.double(0.0),
        ExternalDecays = cms.PSet(
            Photos = cms.untracked.PSet(),
            parameterSets = cms.vstring( "Photos" )
        ),
        HerwigParameters = cms.PSet(
                herwigUEsettings = cms.vstring(
                       'JMUEO = 1 ! multiparton interaction model',
                       'PTJIM = 4.189 ! 2.8x(sqrt(s)/1.8TeV)^0.27 @ 8 TeV',
                       'JMRAD(73) = 1.8 ! inverse proton radius squared',
                       'PRSOF = 0.0 ! prob. of a soft underlying event',
                       'MAXER = 1000000 ! max error'
                ),
                herwigMcatnlo = cms.vstring(
                        'PTMIN = 0.5 ! minimum pt in hadronic jet',
                        'IPROC = -18000 ! proc should be -ve'
                ),
                parameterSets = cms.vstring('herwigUEsettings',
                                            'herwigMcatnlo')
        )
)
    
    

# Path and EndPath definitions
process.generation_step = cms.Path(process.pgen)
process.simulation_step = cms.Path(process.psim)
process.genfiltersummary_step = cms.EndPath(process.genFilterSummary)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RAWSIMoutput_step = cms.EndPath(process.RAWSIMoutput)

# Schedule definition
process.schedule = cms.Schedule(process.generation_step,process.genfiltersummary_step,process.simulation_step,process.endjob_step,process.RAWSIMoutput_step)
# filter all path with the production filter sequence
for path in process.paths:
    getattr(process,path)._seq = process.generator * getattr(process,path)._seq 

