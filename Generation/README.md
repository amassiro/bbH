Generation with MG of the signal
=======

Instructions from:

    https://launchpad.net/madgraph5

Process:

    generate p p > b b~ h [QCD]
    generate p p > b b~ h

[QCD] to have NLO

    output MY_BBH_DIR
    output MY_BBH_DIR_LO

It compiles the code!

Then modify the card to have the correct center of mass energy

    MY_BBH_DIR/Cards/run_card.dat
    MY_BBH_DIR_LO/Cards/run_card.dat

And generate:

    launch
    

Decay & hadronize
=======

Where:

    cmsneu
    /home/amassiro/Generation/CMSSW_5_3_9_patch3/src/LHE
    /home/amassiro/Generation/CMSSW_5_3_9_patch3/src/

    git cms-addpkg Configuration/Generator

    new for GENSIM
    /home/amassiro/Generation/CMSSW_5_3_14_patch2/src/

Prepare LHE file:

    cmsDriver.py MCDBtoEDM --conditions START53_V19::All -s NONE --eventcontent RAWSIM --datatier GEN --filein file:LHE/bbH_LO.lhe --fileout /tmp/amassiro/prova.root -n -1


Run (from CMSSW folder):

    cmsRun GENSIM_bbh_H_WW_lnulnu_withTau_8TeV.py   inputFiles=file:in.root       outputFile=out.root
    cmsRun GENSIM_bbh_H_GG_8TeV.py                  inputFiles=file:in.root       outputFile=out.root


Decay & hadronize: GEN-SIM step lxbatch
=======


1) Prepare the jobs to launch on lxbatch:

   perl launchJobs_lxbatch_GEN-SIM.pl params_lxbatch_GEN-SIM.CFG
   
   params_lxbatch_GEN-SIM.CFG has the following input parameters:
   
   - BASEDir: complete path of this lxbatch directory, eg:   
     
     /afs/cern.ch/work/b/bmarzocc/GenerationMC/CMSSW_5_3_14_patch2/src/bbH/Generation/lxbatch/
   
   - JOBCfgTemplate: cfg file to run with cmsRun, USE THE TEMPLATE:
                
     GENSIM_bbh_H_GG_8TeV_template.py

   - HEPMinput: hepmc root files complete path, PUT IT IN A public OR /tmp/, eg:
     
     /afs/cern.ch/work/b/bmarzocc/public/bbH_GG_HEPMC.root

   - OUTPUTSAVEPath: directory where to save the output files (also a eos directpry), eg:
     
     /store/caf/user/bmarzocc/bbHtoGG_GEN-SIM/

   - OUTPUTFILEName: name of a single job output root file, eg:
     
     bbHGG_GEN-SIM

   - EVENTSNumber: total number of events to analyze
     
   - EVENTSPerjob: number of events per job
     
   - EXEName: name of the executable in the JOB directory.
   
   - QUEUE: name of the queue where launch the jobs to, eg: 1nd
   
2) Launch the jobs:

   sh lancia.sh
   
   
Decay & hadronize: DIGI step lxbatch
=======

1) Prepare the jobs to launch on lxbatch:

   perl launchJobs_lxbatch_DIGI.pl params_lxbatch_DIGI.CFG
   
   params_lxbatch_GEN-SIM.CFG has the following input parameters:
   
   - BASEDir: complete path of this lxbatch directory, eg:   
     
     /afs/cern.ch/work/b/bmarzocc/GenerationMC/CMSSW_5_3_14_patch2/src/bbH/Generation/lxbatch/
   
   - JOBCfgTemplate: path of the cfg file to run with cmsRun, USE THE TEMPLATE:
                
     DIGI_template_cfg.py

   - LISTOFSamples: txt file of the list of directories that contain the GEN-SIM root files, eg of path into the txt:
     
     /store/caf/user/bmarzocc bbHtoGG_GEN-SIM

     where the directory path and the directpry have to be separated by a spacetab
     
   - OUTPUTSAVEPath: directory where to save the output files (also a eos directpry), eg:
     
     /store/caf/user/bmarzocc/bbHtoGG_GEN-SIM/

   - OUTPUTFILEName: name of a single job output root file, eg:
     
     bbHGG_DIGI
   
   - EXEName: name of the executable in the JOB directory.

   - JOBModulo: numeber of split lhe read per job. 
     
   
2) Launch the jobs:

   sh lancia.sh
   

Decay & hadronize: AODSIM step lxbatch
=======

1) Prepare the jobs to launch on lxbatch:

   perl launchJobs_lxbatch_AODSIM.pl params_lxbatch_AODSIM.CFG
   
   params_lxbatch_AODSIM.CFG has the following input parameters:
   
   - BASEDir: complete path of this lxbatch directory, eg:   
     
     /afs/cern.ch/work/b/bmarzocc/GenerationMC/CMSSW_5_3_14_patch2/src/bbH/Generation/lxbatch/
   
   - JOBCfgTemplate: path of the cfg file to run with cmsRun, USE THE TEMPLATE:
                
     AODSIM_template_cfg.py

   - LISTOFSamples: txt file of the list of directories that contain the DIGI root files, eg of path into the txt:
     
     /store/caf/user/bmarzocc/bbHtoGG_DIGI bbHtoGG_GEN-SIM

     where the directory path and the directpry have to be separated by a spacetab
     
   - OUTPUTSAVEPath: directory where to save the output files (also a eos directpry), eg:
     
     /store/caf/user/bmarzocc/bbHtoGG_AODSIM/

   - OUTPUTFILEName: name of a single job output root file, eg:
     
     bbHGG_AODSIM
   
   - EXEName: name of the executable in the JOB directory.

   - JOBModulo: numeber of split lhe read per job. 
     
   
2) Launch the jobs:

   sh lancia.sh

Backup
=======


Run GEN-SIM:
NB: the decay python config file should be in a dedicated folder, otherwise errors!

    scram b

    cmsDriver.py Configuration/GenProduction/python/EightTeV/POWHEG_PYTHIA6_Tauola_H_WW_lnulnu_withTau_8TeV_cff.py   --step GEN,SIM   \
           --beamspot Realistic8TeVCollision    \
           --pileup NoPileUp         --datamix NODATAMIXER        --eventcontent RAWSIM    \
           --datatier GEN-SIM         --conditions START53_V19::All     \
           --filein file:/tmp/amassiro/prova.root     \
           --fileout /tmp/amassiro/provaGENSIM.root \
           -n -1 --no_exec

    cmsDriver.py Configuration/GenProduction/python/EightTeV/POWHEG_PYTHIA6_Tauola_H_WW_lnulnu_withTau_8TeV_cff.py \
        --filein file:/tmp/amassiro/prova.root     \
        --fileout /tmp/amassiro/provaGENSIM.root \
        --mc --eventcontent RAWSIM --datatier GEN-SIM \
        --conditions START53_V7C::All \
        --beamspot Realistic8TeVCollision --step GEN,SIM \              
        --python_filename HIG-Summer12-01937_1_cfg.py 
        --no_exec

For some reasons it's not copying the fragment where it should!
Hardcoded python for generation.


Old instructions:


    cmsDriver.py POWHEG_PYTHIA6_Tauola_H_WW_lnulnu_withTau_8TeV_cff.py  \
               --filein file:/tmp/amassiro/prova.root     \
               --fileout file:HIG-Summer12-01308.root \
               --mc --eventcontent RAWSIM --pileup NoPileUp --datatier GEN-SIM --conditions START53_V7C::All  \
               --beamspot Realistic8TeVCollision --step GEN,SIM --datamix NODATAMIXER  \
               --python_filename HIG-Summer12-01308_1_cfg.py --no_exec
    
    cmsRun HIG-Summer12-01308_1_cfg.py
    
    
    cmsDriver.py POWHEG_PYTHIA6_Tauola_H_WW_lnulnu_withTau_8TeV_cff.py --step GEN,SIM --beamspot Realistic8TeVCollision \
        --pileup NoPileUp --datamix NODATAMIXER --eventcontent RAWSIM --datatier GEN-SIM --conditions auto:mc \
        --filein file:/tmp/amassiro/prova.root     \
        --fileout file:HIG-Summer12-01308.root
    
    cmsDriver.py step1  --filein "dbs:/GluGluToHToWWTo2LAndTau2Nu_M-95_8TeV-minloHJJ-pythia6-tauola/Summer12-START53_V7C-v3/GEN-SIM" --fileout file:step1.root --pileup_input "dbs:/MinBias_TuneZ2star_8TeV-pythia6/Summer12-START50_V13-v3/GEN-SIM" --mc --eventcontent RAWSIM --pileup 2012_Summer_50ns_PoissonOOTPU --datatier GEN-SIM-RAW --conditions START53_V19::All --step DIGI,L1,DIGI2RAW,HLT:7E33v2  --python_filename HIG-Summer12DR53X-01423_1_cfg.py --no_exec --customise Configuration/DataProcessing/Utils.addMonitoring -n 169 || exit $? ; 

    
    
    
    cmsRun PYTHIA6_Tauola_SM_H_gluglu_wh_zh_tth_mH125_8TeV_cff_py_GEN_SIM.py

    cmsRun ~/public/CMSSW/DrawTree.py inputFiles=file:PYTHIA6_Tauola_SM_H_gluglu_wh_zh_tth_mH125_8TeV_cff_py_GEN_SIM.root




    cmsDriver.py Configuration/Generator/python/PYTHIA6_Tauola_SM_H_gluglu_wh_zh_tth_mH125_8TeV_cff.py \
       --step GEN,SIM --beamspot Realistic8TeVCollision \
       --conditions START53_V7C::All \
       --pileup NoPileUp \
       --datamix NODATAMIXER \
       --eventcontent RAWSIM \
       --datatier GEN-SIM \
       -n 200000 \
       --no_exec

    cmsRun PYTHIA6_Tauola_SM_H_gluglu_wh_zh_tth_mH125_8TeV_cff_py_GEN_SIM.py

    cmsRun ~/public/CMSSW/DrawTree.py inputFiles=file:PYTHIA6_Tauola_SM_H_gluglu_wh_zh_tth_mH125_8TeV_cff_py_GEN_SIM.root




