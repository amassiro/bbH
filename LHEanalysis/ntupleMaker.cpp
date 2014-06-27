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
//jet variables
  float jetpt1_;
  float jetpt2_;
  float jetpt12_;
  float jeteta1_;
  float jeteta2_;
  float jeteta12_;
  float jetphi1_;
  float jetphi2_;
  float jetphi12_;    
  float jetinmass1_;
  float jetinmass2_;
  float jetinmass12_; 


//lepton variables
  float leppt1_;
  float leppt2_;
  float leppt12_;
  float lepeta1_;
  float lepeta2_;
  float lepeta12_;
  float lepphi1_;
  float lepphi2_;
  float lepphi12_;    
  float lepinmass1_;
  float lepinmass2_;
  float lepinmass12_;
  float lepdelphi12_; 
  float lepdeleta12_;  

 
//lep_quark variables
 float lep1jet1delR_;
 float lep1jet2delR_;
 float lepjetinmassc_;
 float lepjetinmassf_;
 float lepjetdelphi_; 
 

//missing energy
 float missenergy_;
 float missenergy1_; 


//higgs mass
 float mt_;

  void fillTree (std::string fileNameLHE);
  void Write(TFile& out);
  void Init();

};


myTree::myTree(){
 tree = new TTree("tree","tree");

//jet branch
 tree->Branch("jetpt1",&jetpt1_,"jetpt1/F");
 tree->Branch("jetpt2",&jetpt2_,"jetpt2/F"); 
 tree->Branch("jetpt12",&jetpt12_,"jetpt12/F");  
 tree->Branch("jeteta1",&jeteta1_,"jeteta1/F");
 tree->Branch("jeteta2",&jeteta2_,"jeteta2/F"); 
 tree->Branch("jeteta12",&jeteta12_,"jeteta12/F");   
 tree->Branch("jetphi1",&jetphi1_,"jetphi1/F");
 tree->Branch("jetphi2",&jetphi2_,"jetphi2/F"); 
 tree->Branch("jetphi12",&jetphi12_,"jetphi12/F");  
 tree->Branch("jetininmass1",&jetinmass1_,"jetinmass1/F");  
 tree->Branch("jetininmass2",&jetinmass2_,"jetinmass2/F");  
 tree->Branch("jetininmass12",&jetinmass12_,"jetinmass12/F");  



 //lepton branch

 tree->Branch("leppt1",&leppt1_,"leppt/F");
 tree->Branch("leppt2",&leppt2_,"leppt2/F"); 
 tree->Branch("leppt12",&leppt12_,"leppt12/F");  
 tree->Branch("lepeta1",&lepeta1_,"lepeta1/F");
 tree->Branch("lepeta2",&lepeta2_,"lepeta2/F"); 
 tree->Branch("lepeta12",&lepeta12_,"lepeta12/F");   
 tree->Branch("lepphi1",&lepphi1_,"lepphi1/F");
 tree->Branch("lepphi2",&lepphi2_,"lepphi2/F"); 
 tree->Branch("lepphi12",&lepphi12_,"lepphi12/F");  
 tree->Branch("lepininmass1",&lepinmass1_,"lepinmass1/F");  
 tree->Branch("lepininmass2",&lepinmass2_,"lepinmass2/F");   
 tree->Branch("lepininmass12",&lepinmass12_,"lepinmass12/F");   
 tree->Branch("lepininmass12",&lepinmass12_,"lepinmass12_/F");   
 tree->Branch("lepdelphi12",&lepdelphi12_,"lepdelphi12/F");   
 tree->Branch("lepdeleta12",&lepdeleta12_,"lepdeleta12/F"); 


// lepjet branch
tree->Branch("lep1jet1delR",&lep1jet1delR_,"lep1jet1delR/F");   
tree->Branch("lep1jet2delR",&lep1jet2delR_,"lep1jet2delR/F");   
tree->Branch("lepjetinmassc",&lepjetinmassc_,"lepjetinmassc/F");   
tree->Branch("lepjetinmassf",&lepjetinmassf_,"lepjetinmassf/F");   
tree->Branch("lepjetdelphi",&lepjetdelphi_,"lepjetdelphi/F");   


//missing energy
tree->Branch("missenergy",&missenergy_,"missenergy/F");   
tree->Branch("missenergy1",&missenergy1_,"missenergy1/F");   
tree->Branch("lepjetdelphi",&lepjetdelphi_,"lepjetdelphi/F");


//higgs mass
tree->Branch("mt",&mt_,"mt/F");

}

void myTree::Init(){
 

//jet set
 jetpt1_ = -99;
 jetpt2_ =-99;
 jetpt12_=-99;
 jeteta1_=-99;
 jeteta2_=-99;
 jeteta12_=-99;
 jetphi1_=-99;
 jetphi2_=-99;
 jetphi12_=-99;    
 jetinmass1_=-99;
 jetinmass2_=-99;
 jetinmass12_=-99;
 


//lepton set 
 leppt1_=-99;
 leppt1_ = -99;
 leppt2_ =-99;
 leppt12_=-99;
 lepeta1_=-99;
 lepeta2_=-99;
 lepeta12_=-99;
 lepphi1_=-99;
 lepphi2_=-99;
 lepphi12_=-99;    
 lepinmass1_=-99;
 lepinmass2_=-99; 
 lepinmass12_=-99;
 lepdelphi12_=-99;
 lepdeleta12_=-99; 


//lepquark set
lep1jet1delR_=-99;
lep1jet2delR_=-99;
lepjetinmassc_=-99;
lepjetinmassf_=-99;
lepjetdelphi_=-99;



//misssing energy
missenergy_=-99;
missenergy1_=-99;


//higgs mass

mt_=-99;


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
  std::vector<TLorentzVector> v_tlv_all_leptons ; 
  std::vector<TLorentzVector> v_tlv_all_neutrinos ; 
    
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

     v_tlv_all_quarks.push_back (dummy) ;
    }

//leptons
    if (abs (reader.hepeup.IDUP.at (iPart)) == 11 | abs(reader.hepeup.IDUP.at (iPart)) == 13) {
     TLorentzVector dummy1 (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;

     v_tlv_all_leptons.push_back (dummy1) ;
    }

    if (abs (reader.hepeup.IDUP.at (iPart)) == 12 | abs(reader.hepeup.IDUP.at (iPart)) == 14) {
     TLorentzVector dummy2 (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;

     v_tlv_all_neutrinos.push_back (dummy2) ;
    }


   }

  } // outgoing particles

  // sorting in pt
  sort (v_tlv_all_quarks.rbegin (), v_tlv_all_quarks.rend (), ptsort ()) ;
  sort (v_tlv_all_leptons.rbegin (), v_tlv_all_leptons.rend (), ptsort ()) ;
    sort (v_tlv_all_neutrinos.rbegin (), v_tlv_all_neutrinos.rend (), ptsort ()) ;

  //---- fill the ntuple
//jets
  if (v_tlv_all_quarks.size()>1) {
   jetpt1_ = v_tlv_all_quarks.at (0).Pt ();
   jetpt2_ = v_tlv_all_quarks.at (1).Pt ();  
   jetpt12_= (v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1)).Pt();
   jeteta1_= v_tlv_all_quarks.at (0).Eta ();
   jeteta2_= v_tlv_all_quarks.at (1).Eta ();
   jeteta12_= (v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1)).Eta();
   jetphi1_= v_tlv_all_quarks.at (0).Phi ();
   jetphi2_= v_tlv_all_quarks.at (1).Phi ();
   jetphi12_= (v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1)).Phi();
   jetinmass1_=v_tlv_all_quarks.at (0).M(); 
   jetinmass2_=v_tlv_all_quarks.at (1).M(); 
   jetinmass12_= (v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1)).M();

}
//leptons
  if (v_tlv_all_leptons.size()>1) {
   leppt1_ = v_tlv_all_leptons.at (0).Pt ();
   leppt2_ = v_tlv_all_leptons.at (1).Pt ();  
   leppt12_= (v_tlv_all_leptons.at (0)+v_tlv_all_leptons.at (1)).Pt();
   lepeta1_= v_tlv_all_leptons.at (0).Eta ();
   lepeta2_= v_tlv_all_leptons.at (1).Eta ();
   lepeta12_= (v_tlv_all_leptons.at (0)+v_tlv_all_leptons.at (1)).Eta();
   lepphi1_= v_tlv_all_leptons.at (0).Phi ();
   lepphi2_= v_tlv_all_leptons.at (1).Phi ();
   lepphi12_= (v_tlv_all_leptons.at (0)+v_tlv_all_leptons.at (1)).Phi();
   lepinmass1_=v_tlv_all_leptons.at (0).M();        
   lepinmass2_=v_tlv_all_leptons.at (1).M();       
   lepinmass12_= (v_tlv_all_leptons.at (0)+v_tlv_all_leptons.at (1)).M();
   lepdelphi12_= v_tlv_all_leptons.at (0).DeltaPhi(v_tlv_all_leptons.at (1));
   lepdeleta12_=lepeta1_- lepeta2_;

}
//leptons and jets
  if (v_tlv_all_leptons.size()>1 && v_tlv_all_quarks.size()>1) {
      lep1jet1delR_= v_tlv_all_leptons.at (0).DeltaR(v_tlv_all_quarks.at (0));
      lep1jet2delR_= v_tlv_all_leptons.at (0).DeltaR(v_tlv_all_quarks.at (1));
      lepjetdelphi_= (v_tlv_all_leptons.at (0) + v_tlv_all_leptons.at (1)).DeltaPhi(v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1));
     
 //missenergy_=(v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1)+v_tlv_all_leptons.at (0)+v_tlv_all_leptons.at (1)).Pt();
if(lep1jet1delR_ > lep1jet2delR_)
{
lepjetinmassc_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (0)).M();
lepjetinmassf_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (1)).M();

}
if(lep1jet1delR_ < lep1jet2delR_)
{
lepjetinmassc_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (1)).M();
lepjetinmassf_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (0)).M();
}


}
//MISSING ENERGY CALCULATION!!!!!!!!!!!
//way 1   
 TLorentzVector missdummy;

for(int a1=0; a1 < v_tlv_all_quarks.size() ; a1++) 
{
missdummy=v_tlv_all_quarks.at(a1) + missdummy;
}

for(int a2=0; a2 < v_tlv_all_leptons.size(); a2++)
{
missdummy=v_tlv_all_leptons.at(a2) + missdummy;

}

missenergy_=missdummy.Pt();

//way 2
 TLorentzVector missdummy1;
for(int a3=0; a3 < v_tlv_all_neutrinos.size(); a3++)
{
missdummy1=v_tlv_all_neutrinos.at(a3) + missdummy1;
}
missenergy1_=missdummy1.Pt();



//higgs mass
if (v_tlv_all_leptons.size()>1 && v_tlv_all_quarks.size()>1) {

mt_=sqrt(2*missenergy1_* leppt12_*(1-cos(lepjetdelphi_) ));




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
