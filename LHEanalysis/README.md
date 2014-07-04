LHE analysis
====

Parton level analysis:

    compare distributions for signal and background
    setup a first ME method to distinguish bbH from tt

Where:

    /home/amassiro/Generation/CMSSW_5_3_14_patch2/src/bbH/LHEanalysis/


Location of LHE events:
====

Signal LO:

    /afs/cern.ch/work/b/bmarzocc/public/xMassiro/bbH_mg5_aMC_LO/bbH_mg5_aMC_LO_100k.lhe

    ww decay:
    /afs/cern.ch/work/a/amassiro/public/bbH/bbH_mg5_aMC_LO_100k_ww_lvlv.lhe


Background:

    /TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM
        /eos/cms/store/lhe/6721/

    /TTTo2L2Nu2B_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM
        /eos/cms/store/lhe/6723/

    /TT_8TeV-mcatnlo/Summer12_DR53X-PU_S10_START53_V7A-v1/AODSIM
        /eos/cms/store/lhe/7424/ttb_8TeV_cteq6m.lhe


ntupleMaker
====

Compile:

signal: c++ -o turza.exe `root-config --glibs --cflags` -lm ntupleMaker.cpp
background: c++ -o hello.exe `root-config --glibs --cflags` -lm ntupleMaker.cpp

to use:

signal:   ./turza.exe     /afs/cern.ch/work/a/amassiro/public/bbH/bbH_mg5_aMC_LO_100k_ww_lvlv.lhe    my.root 

background:   ./hello.exe     /afs/cern.ch/work/a/amassiro/public/bbH/ttj_pow/TTTo2L2Nu2B_CT10_8TeV-powheg_329639198.lhe    my1.root

    /afs/cern.ch/work/a/amassiro/public/bbH/ttj_pow/*.lhe
    eos ls -alrth  /eos/cms/store/lhe/6723/
    eos ls -alrth  /eos/cms/store/lhe/6723/ | awk '{print "eos cp /eos/cms/store/lhe/6723/"$9" /tmp/"}'
    eos cp /eos/cms/store/lhe/6723/TTTo2L2Nu2B_CT10_8TeV-powheg_329639197.lhe




