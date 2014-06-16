// to compile:
// c++ -o ntupleMaker.exe `root-config --glibs --cflags` -lm ntupleMaker.cpp
//
// to use:
// ./ntupleMaker.exe fileLHE outputFileRoot
//

#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <assert.h>


#include "LHEF.h"



#include "TTree.h"
#include "TFile.h"
#include "TLorentzVector.h"
#include "Math/Vector3D.h"
#include "Math/Vector4D.h"

#include "TRandom.h"

struct ptsort: public std::binary_function<TLorentzVector, TLorentzVector, bool> {
 bool operator () (const TLorentzVector & x, const TLorentzVector & y) {
  return (x.Perp () < y.Perp () ) ;
 }
} ;

TLorentzVector buildP (const LHEF::HEPEUP & event, int iPart) {
 TLorentzVector dummy ;
 dummy.SetPxPyPzE (
   event.PUP.at (iPart).at (0), // px
 event.PUP.at (iPart).at (1), // py
 event.PUP.at (iPart).at (2), // pz
 event.PUP.at (iPart).at (3) // E
                  ) ;
 return dummy ;
}


float SmearMET = 0.10; //---- 10% flat
float SmearJES = 0.10; //---- 1% flat


void smearVectorKeepMass (TLorentzVector& part, float smear){
 //---- smear = 0.10 ---> 10%
 float scale = -1;
 while (scale <= 0) { //---- no negative values!
  scale = gRandom->Gaus(1,smear);
 }
 part.SetXYZM(scale * part.X(), scale * part.Y(), scale * part.Z(), part.M() );
}


void smearVector (TLorentzVector& part, float smear){
 //---- smear = 0.10 ---> 10%
 float scale = -1;
 while (scale <= 0) { //---- no negative values!
  scale = gRandom->Gaus(1,smear);
 }
 part.SetPxPyPzE(scale * part.X(), scale * part.Y(), scale * part.Z(), part.E() );
}


void setMassLess (TLorentzVector& part){
 part.SetXYZM(part.X(), part.Y(), part.Z(), 0 );
}



class myParticle: public TLorentzVector {

 public:

  myParticle(): TLorentzVector() {};
  myParticle( const myParticle& other ) : TLorentzVector( other ) {
   pdgId_ = other.GetPdgId();
  };

  void SetPdgId(int pdgId) {pdgId_ = pdgId;}
  int GetPdgId() const {return pdgId_;}

 private :
  int pdgId_;
};


class myTree {

 public:

  myTree();
  ~myTree() {};

  TTree* tree;

  float jetpt1_;

  void fillTree (std::string fileNameLHE);
  void Write(TFile& out);
  void Init();

};


myTree::myTree(){
 tree = new TTree("tree","tree");


 tree->Branch("jetpt1",&jetpt1_,"jetpt1/F");

}

void myTree::Init(){

 jetpt1_ = -99;

}


void myTree::fillTree(std::string fileNameLHE){
 std::ifstream ifs (fileNameLHE.c_str ()) ;
 LHEF::Reader reader (ifs) ;

 long ieve = 0 ;
 long selected = 0 ;

 std::cout << " reading " << fileNameLHE << std::endl;

 // loop over events
 while ( reader.readEvent () ) {
  ieve++;
  if (ieve % 10000 == 0) std::cout << "event " << ieve << "\n" ;

  //---- initialize variables in tree
  Init();


  std::vector<TLorentzVector> v_tlv_all_quarks ; //---- tlv = TLorentzVector

  // loop over particles in the event
  // and fill the variables of leptons and quarks
  for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) {

   // outgoing particles
   if (reader.hepeup.ISTUP.at (iPart) == 1) {
    // quarks
    if (abs (reader.hepeup.IDUP.at (iPart)) < 7) {
     TLorentzVector dummy (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;

     smearVectorKeepMass(dummy,SmearJES); //---- smear jet energy scale

     if (abs (reader.hepeup.IDUP.at (iPart)) != 5) {
      //---- set massless quarks!
      setMassLess(dummy);
     }

     v_tlv_all_quarks.push_back (dummy) ;
    }
   }

  } // outgoing particles

  // sorting in pt
  sort (v_tlv_all_quarks.rbegin (), v_tlv_all_quarks.rend (), ptsort ()) ;


  //---- fill the ntuple

  if (v_tlv_all_quarks.size()>0) {
   jetpt1_ = v_tlv_all_quarks.at (0).Pt ();
  }

  tree->Fill();
 }
}


void myTree::Write(TFile& out){
 out.cd();
 tree->Write();
}


int main (int argc, char **argv) {
 // Open a stream connected to an event file:
 if (argc < 3) exit (1) ;

 std::cout << " Input LHE = " << argv[1] << std::endl;
 std::cout << " Output ROOT = " << argv[2] << std::endl;

 myTree myT;
 myT.fillTree (std::string(argv[1])) ;

 TFile output (argv[2], "recreate") ;
 output.cd() ;
 myT.Write(output);
 output.Close();

}
