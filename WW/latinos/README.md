Latinos instructions
====

Create list of files

    eos ls /eos/cms/store/user/amassiro/Latinos/gg2vv/Hw25_SigOnPeak_8TeV/ | awk '{print "   @/eos/cms/store/user/amassiro/Latinos/gg2vv/Hw25_SigOnPeak_8TeV/"$1"@,"}' | tr "@" "'"

hardcoded in skim.py.

Then Step1

    cd /home/amassiro/Latinos/CMSSW_5_3_11_patch6/src/WWAnalysis/SkimStep/test/
    cmsRun latinosYieldSkim.py.bbHww.py   print isMC=True globalTag=GR_R_52_V7  correctMetPhi=False    doNoFilter=True  outputFile=bbH_step1.root


Step2+3

    cd /home/amassiro/Latinos/CMSSW_5_3_11_patch6/src/WWAnalysis/AnalysisStep/test/step3/
    cmsRun step3.py print inputFiles=file:/home/amassiro/Latinos/CMSSW_5_3_11_patch6/src/WWAnalysis/SkimStep/test/bbH_step1.root  label=bbHww id=3125  scale=1 outputFile=step3_bbHww.root doNoFilter=True



Add variables

triggW

    ./gardener.py efftfiller \
      /data/amassiro/LatinosTrees/nominals_all/latinostep3_latinosYieldSkim_MC_bHWW_100k_new.root  \
      /data/amassiro/LatinosTrees/nominals_all/latinostep3_latinosYieldSkim_MC_bHWW_100k_new_weight.root  \
      -f ../data/fit_results.txt

puW

    ./gardener.py  puadder \
            /data/amassiro/LatinosTrees/nominals_all/latinostep3_latinosYieldSkim_MC_bHWW_100k_new_weight.root  \
            /data/amassiro/LatinosTrees/nominals_all/latinostep3_latinosYieldSkim_MC_bHWW_100k_new_weight_puW.root \
           --mc=../data/PileupMC_60bin_S10.root    \
           --data=../data/PUdata2012Final.root   \
           --HistName=pileup   \
           --branch=puW  \
           --kind=trpu

effW

    ./gardener.py effwfiller \
        /data/amassiro/LatinosTrees/nominals_all/latinostep3_latinosYieldSkim_MC_bHWW_100k_new_weight_puW.root  \
        /data/amassiro/LatinosTrees/nominals_all/latinostep3_latinosYieldSkim_MC_bHWW_100k_new_weight_puW_effW.root \
        --mufile=../data/muons_scale_factors.root \
        --elfile=../data/electrons_scale_factors.root \
        --muname=muonsDATAMCratio_all \
        --elname=electronsDATAMCratio_All_selec

