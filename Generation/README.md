Generation with MG of the signal
=======

Instructions from:

    https://launchpad.net/madgraph5
    (check that gfortran is installed, otherwise mg5_aMC doesn't work: sudo apt-get install gfortran)
    
Process:

    generate p p > b b~ h [QCD]
    generate p p > b b~ h

[QCD] to have NLO

    output MY_BBH_DIR_NLO
    output MY_BBH_DIR_LO

It compiles the code!

Then modify the card to have the correct center of mass energy

    MY_BBH_DIR_NLO/Cards/run_card.dat
    MY_BBH_DIR_LO/Cards/run_card.dat

    MY_BBH_DIR_NLO/Cards/param_card.dat
    MY_BBH_DIR_LO/Cards/param_card.dat

    MY_BBH_DIR_NLO/Cards/proc_card_mg5.dat
    MY_BBH_DIR_LO/Cards/proc_card_mg5.dat


And generate:

    launch

    
LHADF
=======

Install pdf before:

    wget http://www.hepforge.org/archive/lhapdf/lhapdf-5.9.1.tar.gz
    sudo lhapdf-getdata MSTW2008nlo68cl
    
if you are in an /afs/ area instead do:
    
     export LHAPDFSYS=/afs/cern.ch/sw/lcg/external/MCGenerators/lhapdf/5.8.8/x86_64-slc5-gcc46-opt/
     export PATH=${PATH}:${LHAPDFSYS}/bin 
     export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${LHAPDFSYS}/lib
     LHAPATH=/afs/cern.ch/cms/slc5_amd64_gcc462/external/lhapdf/5.8.5-cms2/share/lhapdf/PDFsets/
    

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


Decay & hadronize: lxbatch
=======

see directory

    lxbatch


Backup
=======


    cmsDriver.py MCDBtoEDM --conditions START53_V19::All -s NONE --eventcontent RAWSIM --datatier GEN --filein file:/tmp/amassiro/unweighted_events.lhe      --fileout /tmp/amassiro/prova_bbH_unweighted.root -n -1
    

    
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




