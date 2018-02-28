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

Where:

    /home/amassiro/Cern/Code/bbH/bbH/LHEanalysis
    
Compile:

    c++ -o ntupleMaker.exe `root-config --glibs --cflags`  -lm ntupleMaker.cpp   `root-config --libs`
    
Run:

    ./ntupleMaker.exe     bbH_mg5_aMC_LO_100k.lhe     bbH_mg5_aMC_LO_100k.lhe.root

    ./ntupleMaker.exe     ../Generation/MG5_aMC_v2_6_1/MY_BBH_DIR_LO/Events/run_01/unweighted_events.lhe      bbH_mg5_aMC_LO_13TeV_10k.lhe.root
    ./ntupleMaker.exe     ../Generation/MG5_aMC_v2_6_1/MY_BBH_DIR_LO/Events/run_02/unweighted_events.lhe      bbH_mg5_aMC_LO_13TeV_100k.lhe.root


Draw:

    r00t  bbH_mg5_aMC_LO_100k.lhe.root
    tree = (TTree*) _file0 -> Get ("tree")
    tree->Draw ("bjeteta1 >> h1")
    tree->Draw ("bjeteta2 >> h1")
    h1->Draw("")
    
    






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



code description[Mir Mehedi Faruk]



lr.cxx -> This code runs to create plot of signal and background variable along with likelihood ratio. (does not use THnSparse)

plotsbnormalized.cxx -> This code runs to create plot of signal and background normalized to 1

plotsbnormalizedxsec.cxx -> This code runs to create plot of signal and background normalized to cross-section

spurselikelihood.cxx -> This code runs to create a spurse object, create signal and background, and alos create Likelihood ratio from that.

ntupleMaker.cpp -> To compile/run signal and background use the above commands. The data for signal and background are saved in a ROOT file named my.root and my1.root respectively
