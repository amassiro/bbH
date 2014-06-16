Decay
====

for WW:

Compile:

    source  setup_slc5.sh 
    g++ -Wall -o Decay_HWW_lvlv.exe `pythia8-config --cxxflags --libs --ldflags` -I /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc5-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc5-gcc46-opt/lib -lHepMC -lpythia8tohepmc Decay_HWW_lvlv.cpp

Exec:

    ./Decay_HWW_lvlv.exe /afs/cern.ch/work/b/bmarzocc/public/xMassiro/bbH_mg5_aMC_LO/bbH_mg5_aMC_LO_100k.lhe /tmp/amassiro/bbH_mg5_aMC_LO_100k_ww_lvlv.lhe   0 100



for gg:

Compile:

    source  setup_slc5.sh 
    g++ -Wall -o Decay_Hgg.exe `pythia8-config --cxxflags --libs --ldflags` -I /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc5-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc5-gcc46-opt/lib -lHepMC -lpythia8tohepmc Decay_Hgg.cpp

Exec:

    ./Decay_Hgg.exe /afs/cern.ch/work/b/bmarzocc/public/xMassiro/bbH_mg5_aMC_LO/bbH_mg5_aMC_LO_100k.lhe /tmp/amassiro/bbH_mg5_aMC_LO_100k_gg.lhe    0    100

