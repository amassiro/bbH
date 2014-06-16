//****************************************************************************************************************************

// on slc6: compile with -----> c++ -O2 -ansi -pedantic -W -Wall -I /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc6-gcc46-opt/lib -lHepMC -lpythia8tohepmc `pythia8-config --cxxflags --libs --ldflags` -o Decay_HWW_lvlv.exe Decay_HWW_lvlv.cpp

// on slc5: compile with -----> g++ -Wall -o Decay_HWW_lvlv.exe `pythia8-config --cxxflags --libs --ldflags` -I /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc5-gcc46-opt/include/ -L /afs/cern.ch/sw/lcg/external/HepMC/2.06.08/x86_64-slc5-gcc46-opt/lib -lHepMC -lpythia8tohepmc Decay_HWW_lvlv.cpp

//****************************************************************************************************************************

// just for back-compatibility

#ifdef PYTHIA8176
#include "Pythia.h"
#include "HepMCInterface.h"
#define HepMCConverter HepMC::I_Pythia8
#else
#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#define HepMCConverter HepMC::Pythia8ToHepMC
#endif

#include "HepMC/GenEvent.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/IO_AsciiParticles.h"

using namespace Pythia8; 

int main(int argc, char **argv) 
{
    if (argc < 3) 
    {
        std::cerr << " Not enough input information!  no input/output file!" << std::endl;
        exit (0);
    }

    int startEntry = 0;
    if (argc >= 4) startEntry = atoi(argv[3]);
    int endEntry = -1;
    if (argc >= 5) endEntry   = atoi(argv[4]);
    char* SEED = "1";
    if (argc >= 6) SEED = argv[5];

    std::cout << "Start-Entry = " << startEntry << std::endl;
    std::cout << "End-Entry = " << endEntry << std::endl;

    std::string namefile_in;
    namefile_in = argv[1];
    std::string namefile_out;
    namefile_out = argv[2];
    
    // Initialize Les Houches Event File run. List initialization information.
    Pythia pythia;    
    pythia.readString("Beams:frameType = 4");
    std::string sfile = "Beams:LHEF ="+namefile_in;
    pythia.readString("HadronLevel:Hadronize = off");
    pythia.readString("HadronLevel:all = off"); // On hadronization
//     pythia.readString("HadronLevel:all = on"); // generic hadronization settings
    pythia.readString(sfile.c_str());

    int nAbort = 2000000;
    int iAbort = 0;

    std::string SeedString = "Random:seed = "+std::string(SEED);
    std::cout << "SeedString = " << SeedString << std::endl;

    pythia.readString("Random:setSeed = on");
    pythia.readString(SeedString.c_str());

    pythia.readString("25:m0 = 125");

//     pythia.readString("SLHA:useDecayTable = off");
//     pythia.readString("25:mWidth = 0.00407");
//     pythia.readString("25:doForceWidth = true");

    pythia.readString("25:onMode = off");
//     pythia.readString("25:onIfMatch = 4 4");
    pythia.readString("25:onIfMatch = 13 13"); //---> mumu ... seriously??? Otherwise 25 is considered stable ... seriously?!?!?
    pythia.readString("25:onIfMatch = 24 24");

    pythia.readString("24:onMode = off");
    pythia.readString("-24:onMode = off");
    pythia.readString("24:onIfMatch = 12 11"); // e ve
    pythia.readString("24:onIfMatch = 14 13"); // mu numu
// pythia.readString("24:onIfMatch = 15 16"); // tau nutau
    pythia.readString("-24:onIfMatch = 12 -11"); // e ve
    pythia.readString("-24:onIfMatch = 14 -13"); // mu numu
// pythia.readString("-24:onIfMatch = 15 -16"); // tau nutau



    // Initialization
    pythia.init();

    // Create an LHAup object that can access relevant information in pythia.
    Pythia8::LHAupFromPYTHIA8 myLHA(&pythia.process, &pythia.info);   

    // Open a file on which LHEF events should be stored, and write header.
    myLHA.openLHEF(namefile_out.c_str());
    // Store initialization info in the LHAup object.
    myLHA.setInit();
    // Write out this initialization info on the file.
    myLHA.initLHEF();

    // Begin event loop; generate until none left in input file.
    for (int iEvent = 0; ; ++iEvent) {

         //---- subrange of events ----
         if (endEntry!=-1 && iEvent>=endEntry) break;
         if (iEvent >= startEntry){

//               std::cout<<" ievent = " << iEvent << std::endl;
             if (!(iEvent%500)) std::cout<<" ievent = " << iEvent << std::endl;
  
             // Generate events, and check whether generation failed.
             if (!pythia.next()) {

                  // If failure because reached end of file then exit event loop.
                  if (pythia.info.atEndOfFile()) break;

                  // First few failures write off as "acceptable" errors, then quit.
                  if (++iAbort < nAbort) continue;
                  ++iAbort;
                  break;
             }


            // Store event info in the LHAup object.
            myLHA.setEvent();

            // Write out this event info on the file.
            // With optional argument (verbose =) false the file is smaller.
            myLHA.eventLHEF();

            // End of event loop.
         }

    }

    // Write endtag. Overwrite initialization info with new cross sections.
    myLHA.closeLHEF(true);

    //delete pythia;

    return 0;
}
