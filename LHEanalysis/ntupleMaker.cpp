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
  int njet_; 
  int n1jet_;//number of jets greater than 30Gev

//bjet variables
  float bjetpt1_;
  float bjetpt2_;
  float bjetpt12_;
  float bjeteta1_;
  float bjeteta2_;
  float bjeteta12_;
  float bjetphi1_;
  float bjetphi2_;
  float bjetphi12_;    
  float bjetinmass1_;
  float bjetinmass2_;
  float bjetinmass12_; 
  int nbjet_; 


//ljet variables
  float ljetpt1_;
  float ljetpt2_;
  float ljetpt12_;
  float ljeteta1_;
  float ljeteta2_;
  float ljeteta12_;
  float ljetphi1_;
  float ljetphi2_;
  float ljetphi12_;    
  float ljetinmass1_;
  float ljetinmass2_;
  float ljetinmass12_; 
  int nljet_; 


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
  int nlep_;
  int pdgid1_;
  int pdgid2_;

//lep_quark variables
 float lep1jet1delR_;
 float lep1jet2delR_;
 float lep2jet1delR_;
 float lep2jet2delR_;
 float lep1jetinmassc_;
 float lep1jetinmassf_;
 float lep2jetinmassc_;
 float lep2jetinmassf_;
 float lepjetdelphi_; 
 float lowlepjetinmass_; 
 float biglepjetinmass_; 



//lep_bquark variables
 float lep1bjet1delR_;
 float lep1bjet2delR_;
 float lep2bjet1delR_;
 float lep2bjet2delR_;
 float lep1bjetinmassc_;
 float lep1bjetinmassf_;
float lep2bjetinmassc_;
 float lep2bjetinmassf_;
 float lepbjetdelphi_; 
float Mlep12jet12_;
 float lowlepbjetinmass_; 
 float biglepbjetinmass_; 
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
 tree->Branch("jetinmass1",&jetinmass1_,"jetinmass1/F");  
 tree->Branch("jetinmass2",&jetinmass2_,"jetinmass2/F");  
 tree->Branch("jetinmass12",&jetinmass12_,"jetinmass12/F");  
 tree->Branch("njet",&njet_,"njet/I"); 
 tree->Branch("n1jet",&n1jet_,"n1jet/I"); 

//bjet branch
 tree->Branch("bjetpt1",&bjetpt1_,"bjetpt1/F");
 tree->Branch("bjetpt2",&bjetpt2_,"bjetpt2/F"); 
 tree->Branch("bjetpt12",&bjetpt12_,"bjetpt12/F");  
 tree->Branch("bjeteta1",&bjeteta1_,"bjeteta1/F");
 tree->Branch("bjeteta2",&bjeteta2_,"bjeteta2/F"); 
 tree->Branch("bjeteta12",&bjeteta12_,"bjeteta12/F");   
 tree->Branch("bjetphi1",&bjetphi1_,"bjetphi1/F");
 tree->Branch("bjetphi2",&bjetphi2_,"bjetphi2/F"); 
 tree->Branch("bjetphi12",&bjetphi12_,"bjetphi12/F");  
 tree->Branch("bjetinmass1",&bjetinmass1_,"bjetinmass1/F");  
 tree->Branch("bjetinmass2",&bjetinmass2_,"bjetinmass2/F");  
 tree->Branch("bjetininmass12",&bjetinmass12_,"bjetinmass12/F");  
 tree->Branch("nbjet",&nbjet_,"nbjet/I"); 


//ljet branch
 tree->Branch("ljetpt1",&ljetpt1_,"ljetpt1/F");
 tree->Branch("ljetpt2",&ljetpt2_,"ljetpt2/F"); 
 tree->Branch("ljetpt12",&ljetpt12_,"ljetpt12/F");  
 tree->Branch("ljeteta1",&ljeteta1_,"ljeteta1/F");
 tree->Branch("ljeteta2",&ljeteta2_,"leteta2/F"); 
 tree->Branch("ljeteta12",&ljeteta12_,"ljeteta12/F");   
 tree->Branch("ljetphi1",&ljetphi1_,"ljetphi1/F");
 tree->Branch("ljetphi2",&ljetphi2_,"ljetphi2/F"); 
 tree->Branch("ljetphi12",&ljetphi12_,"ljetphi12/F");  
 tree->Branch("ljetininmass1",&ljetinmass1_,"ljetinmass1/F");  
 tree->Branch("ljetininmass2",&ljetinmass2_,"ljetinmass2/F");  
 tree->Branch("ljetininmass12",&ljetinmass12_,"ljetinmass12/F");  
 tree->Branch("nljet",&nljet_,"nljet/I"); 

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
 tree->Branch("lepinmass1",&lepinmass1_,"lepinmass1/F");  
 tree->Branch("lepinmass2",&lepinmass2_,"lepinmass2/F");   
 tree->Branch("lepinmass12",&lepinmass12_,"lepinmass12/F");   
 tree->Branch("lepinmass12",&lepinmass12_,"lepinmass12_/F");   
 tree->Branch("lepdelphi12",&lepdelphi12_,"lepdelphi12/F");   
 tree->Branch("lepdeleta12",&lepdeleta12_,"lepdeleta12/F"); 
 tree->Branch("nlep",&nlep_,"nlep/I"); 
 tree->Branch("pdgid1",&pdgid1_,"pdgid1_/I");  
 tree->Branch("pdgid2",&pdgid2_,"pdgid2_/I");  





// lepjet branch Mlep12jet12
tree->Branch("lep1jet1delR",&lep1jet1delR_,"lep1jet1delR/F");   
tree->Branch("lep1jet2delR",&lep1jet2delR_,"lep1jet2delR/F");   
tree->Branch("lep2jet1delR",&lep2jet1delR_,"lep2jet1delR/F");   
tree->Branch("lep2jet2delR",&lep2jet2delR_,"lep2jet2delR/F");   
tree->Branch("lep1jetinmassc",&lep1jetinmassc_,"lep1jetinmassc/F");   
tree->Branch("lep1jetinmassf",&lep1jetinmassf_,"lep1jetinmassf/F");   
tree->Branch("lep2jetinmassc",&lep2jetinmassc_,"lep2jetinmassc/F");   
tree->Branch("lep2jetinmassf",&lep2jetinmassf_,"lep2jetinmassf/F");   
tree->Branch("lepjetdelphi",&lepjetdelphi_,"lepjetdelphi/F");   

tree->Branch("Mlep12jet12",&Mlep12jet12_,"Mlep12jet12/F");   
tree->Branch("lowlepjetinmass",&lowlepjetinmass_,"lowlepjetinmass/F");   
tree->Branch("biglepjetinmass",&biglepjetinmass_,"biglepjetinmass/F");   
// lepbjet branch lowlepbjetinmass_
tree->Branch("lep1bjet1delR",&lep1bjet1delR_,"lep1bjet1delR/F");   
tree->Branch("lep1bjet2delR",&lep1bjet2delR_,"lep1bjet2delR/F");   
tree->Branch("lep2bjet1delR",&lep2bjet1delR_,"lep2bjet1delR/F");   
tree->Branch("lep2bjet2delR",&lep2bjet2delR_,"lep2bjet2delR/F");   
tree->Branch("lep1bjetinmassc",&lep1bjetinmassc_,"lep1bjetinmassc/F");   
tree->Branch("lep1bjetinmassf",&lep1bjetinmassf_,"lep1bjetinmassf/F");   
tree->Branch("lep2bjetinmassc",&lep2bjetinmassc_,"lep2bjetinmassc/F");   
tree->Branch("lep2bjetinmassf",&lep2bjetinmassf_,"lep2bjetinmassf/F");   
tree->Branch("lepbjetdelphi",&lepbjetdelphi_,"lepbjetdelphi/F");   
tree->Branch("lowlepbjetinmass",&lowlepbjetinmass_,"lowlepbjetinmass/F");   
tree->Branch("biglepbjetinmass",&biglepbjetinmass_,"biglepbjetinmass/F"); 

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
 njet_=-99;
 n1jet_=-99;

//bjet set
 bjetpt1_ = -99;
 bjetpt2_ =-99;
 bjetpt12_=-99;
 bjeteta1_=-99;
 bjeteta2_=-99;
 bjeteta12_=-99;
 bjetphi1_=-99;
 bjetphi2_=-99;
 bjetphi12_=-99;    
 bjetinmass1_=-99;
 bjetinmass2_=-99;
 bjetinmass12_=-99;
 nbjet_=-99;



//ljet set
 ljetpt1_ = -99;
 ljetpt2_ =-99;
 ljetpt12_=-99;
 ljeteta1_=-99;
 ljeteta2_=-99;
 ljeteta12_=-99;
 ljetphi1_=-99;
 ljetphi2_=-99;
 ljetphi12_=-99;    
 ljetinmass1_=-99;
 ljetinmass2_=-99;
 ljetinmass12_=-99;
 nljet_=-99;

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
 nlep_=-99;
 pdgid1_=-99;
 pdgid2_=-99;


//lepquark set
lep1jet1delR_=-99;
lep1jet2delR_=-99;
lep2jet1delR_=-99;
lep2jet2delR_=-99;
lep1jetinmassc_=-99;
lep1jetinmassf_=-99;
lep2jetinmassc_=-99;
lep2jetinmassf_=-99;
lepjetdelphi_=-99;
lowlepjetinmass_=-99;
Mlep12jet12_=-99;
biglepjetinmass_=-99;


//lepbquark set
lep1bjet1delR_=-99;
lep1bjet2delR_=-99;
lep2bjet1delR_=-99;
lep2bjet2delR_=-99;
lep1bjetinmassc_=-99;
lep1bjetinmassf_=-99;
lep2bjetinmassc_=-99;
lep2bjetinmassf_=-99;
lepbjetdelphi_=-99;
lowlepbjetinmass_=-99;
biglepbjetinmass_=-99;
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
  std::vector<TLorentzVector> v_tlv_all_bquarks ; //bquarks
  std::vector<TLorentzVector> v_tlv_all_lquarks ;  //lquarks
  std::vector<TLorentzVector> v_tlv_all_leptons ; //leptons
  std::vector<TLorentzVector> v_tlv_all_neutrinos ; //neutrinos
    
// loop over particles in the event
  // and fill the variables of leptons and quarks
  for (int iPart = 0 ; iPart < reader.hepeup.IDUP.size (); ++iPart) {

   // outgoing particles
   if (reader.hepeup.ISTUP.at (iPart) == 1) {
    // quarks
    if (abs (reader.hepeup.IDUP.at (iPart)) < 7 || abs (reader.hepeup.IDUP.at (iPart)) ==21) {
     TLorentzVector dummy (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;

     v_tlv_all_quarks.push_back (dummy) ;
    }
//bquarks
    if (abs (reader.hepeup.IDUP.at (iPart)) == 5) {
     TLorentzVector dummy00 (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;

     v_tlv_all_bquarks.push_back (dummy00) ;
    }

//lquarks
    if (abs (reader.hepeup.IDUP.at (iPart)) < 5 || abs (reader.hepeup.IDUP.at (iPart)) ==21) {
     TLorentzVector dummy22 (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
                          ) ;

     v_tlv_all_lquarks.push_back (dummy22) ;
    }

//leptons
    if (abs (reader.hepeup.IDUP.at (iPart)) == 11 || abs(reader.hepeup.IDUP.at (iPart)) == 13 ||   abs(reader.hepeup.IDUP.at (iPart)) == 15) {

           if(abs (reader.hepeup.IDUP.at (iPart)) == 11)
             {
                pdgid1_=11;
                pdgid2_=11;
             }
          if(abs (reader.hepeup.IDUP.at (iPart)) == 13)
         {
               pdgid1_=13;
               pdgid2_=13;  
          }

       if(abs (reader.hepeup.IDUP.at (iPart)) == 15)
         {
               pdgid1_=15;
               pdgid2_=15;
          }
 

     TLorentzVector dummy1 (
       reader.hepeup.PUP.at (iPart).at (0), // px
       reader.hepeup.PUP.at (iPart).at (1), // py
       reader.hepeup.PUP.at (iPart).at (2), // pz
       reader.hepeup.PUP.at (iPart).at (3) // E
           
               ) ;

     v_tlv_all_leptons.push_back (dummy1) ;
    }
//neutrinos
    if (abs (reader.hepeup.IDUP.at (iPart)) == 12 || abs(reader.hepeup.IDUP.at (iPart)) == 14|| abs(reader.hepeup.IDUP.at (iPart)) == 16) {
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
    sort (v_tlv_all_bquarks.rbegin (), v_tlv_all_bquarks.rend (), ptsort ()) ;
  sort (v_tlv_all_lquarks.rbegin (), v_tlv_all_lquarks.rend (), ptsort ()) ;
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
//bjets
  if (v_tlv_all_bquarks.size()>1) {
   bjetpt1_ = v_tlv_all_bquarks.at (0).Pt ();
   bjetpt2_ = v_tlv_all_bquarks.at (1).Pt ();  
   bjetpt12_= (v_tlv_all_bquarks.at (0)+v_tlv_all_bquarks.at (1)).Pt();
   bjeteta1_= v_tlv_all_bquarks.at (0).Eta ();
   bjeteta2_= v_tlv_all_bquarks.at (1).Eta ();
   bjeteta12_= (v_tlv_all_bquarks.at (0)+v_tlv_all_bquarks.at (1)).Eta();
   bjetphi1_= v_tlv_all_bquarks.at (0).Phi ();
   bjetphi2_= v_tlv_all_bquarks.at (1).Phi ();
   bjetphi12_= (v_tlv_all_bquarks.at (0)+v_tlv_all_bquarks.at (1)).Phi();
   bjetinmass1_=v_tlv_all_bquarks.at (0).M(); 
   bjetinmass2_=v_tlv_all_bquarks.at (1).M(); 
   bjetinmass12_= (v_tlv_all_bquarks.at (0)+v_tlv_all_bquarks.at (1)).M();
   
}


//ljets
  if (v_tlv_all_lquarks.size()>1) {
   ljetpt1_ = v_tlv_all_lquarks.at (0).Pt ();
   ljetpt2_ = v_tlv_all_lquarks.at (1).Pt ();  
   ljetpt12_= (v_tlv_all_lquarks.at (0)+v_tlv_all_lquarks.at (1)).Pt();
   ljeteta1_= v_tlv_all_lquarks.at (0).Eta ();
   ljeteta2_= v_tlv_all_lquarks.at (1).Eta ();
   ljeteta12_= (v_tlv_all_lquarks.at (0)+v_tlv_all_lquarks.at (1)).Eta();
   ljetphi1_= v_tlv_all_lquarks.at (0).Phi ();
   ljetphi2_= v_tlv_all_lquarks.at (1).Phi ();
   ljetphi12_= (v_tlv_all_lquarks.at (0)+v_tlv_all_lquarks.at (1)).Phi();
   ljetinmass1_=v_tlv_all_lquarks.at (0).M(); 
   ljetinmass2_=v_tlv_all_lquarks.at (1).M(); 
   ljetinmass12_= (v_tlv_all_lquarks.at (0)+v_tlv_all_lquarks.at (1)).M();
   
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
lep2jet1delR_= v_tlv_all_leptons.at (1).DeltaR(v_tlv_all_quarks.at (0));
lep2jet1delR_= v_tlv_all_leptons.at (1).DeltaR(v_tlv_all_quarks.at (1));
      lepjetdelphi_= (v_tlv_all_leptons.at (0) + v_tlv_all_leptons.at (1)).DeltaPhi(v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1));

 //missenergy_=(v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1)+v_tlv_all_leptons.at (0)+v_tlv_all_leptons.at (1)).Pt();
if(lep1jet1delR_ > lep1jet2delR_)
{
lep1jetinmassf_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (0)).M();
lep1jetinmassc_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (1)).M();

}
if(lep1jet1delR_ < lep1jet2delR_)
{
lep1jetinmassf_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (1)).M();
lep1jetinmassc_=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (0)).M();
}
if(lep2jet1delR_ > lep2jet2delR_)
{
lep2jetinmassf_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (0)).M();
lep2jetinmassc_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (1)).M();

}
if(lep2jet1delR_ < lep1jet2delR_)
{
lep2jetinmassf_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (1)).M();
lep2jetinmassc_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (0)).M();
}

float a=lep1jet1delR_, b=lep1jet2delR_, c=lep2jet1delR_, d=lep2jet2delR_;
float a1=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (0)).M(), b1=(v_tlv_all_leptons.at (0) + v_tlv_all_quarks.at (1)).M(), c1=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (0)).M(), d1=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (1)).M();
float x,y,p,t,f,ans;

if (a<b)
{
x=a1;
p=a;
}

if (b<a)
{
x=b1;
p=b;
}

if (c<d)
{
y=c1;
t=c;
}
if (d<c)
{
y=d1;
t=d;
}

if(p<t)
{
lowlepjetinmass_=x;
 if (a<b) {f=d1;}
  if (b<a) {f=c1;}
}
if(t<p)
{
lowlepjetinmass_=y;
if (c<d) {f=b1;}
  if (d<c) {f=a1;}
}

biglepjetinmass_=f;






}


//leptons and bjets
  if (v_tlv_all_leptons.size()>1 && v_tlv_all_bquarks.size()>1) {
      lep1bjet1delR_= v_tlv_all_leptons.at (0).DeltaR(v_tlv_all_bquarks.at (0));
      lep1bjet2delR_= v_tlv_all_leptons.at (0).DeltaR(v_tlv_all_bquarks.at (1));
      lep2bjet1delR_= v_tlv_all_leptons.at (1).DeltaR(v_tlv_all_bquarks.at (0));
      lep2bjet2delR_= v_tlv_all_leptons.at (1).DeltaR(v_tlv_all_bquarks.at (1));
      lepbjetdelphi_= (v_tlv_all_leptons.at (0) + v_tlv_all_leptons.at (1)).DeltaPhi(v_tlv_all_bquarks.at (0)+v_tlv_all_bquarks.at (1));
     
 //missenergy_=(v_tlv_all_quarks.at (0)+v_tlv_all_quarks.at (1)+v_tlv_all_leptons.at (0)+v_tlv_all_leptons.at (1)).Pt();
if(lep1bjet1delR_ > lep1bjet2delR_)
{
lep1bjetinmassc_=(v_tlv_all_leptons.at (0) + v_tlv_all_bquarks.at (0)).M();
lep1bjetinmassf_=(v_tlv_all_leptons.at (0) + v_tlv_all_bquarks.at (1)).M();

}
if(lep1bjet1delR_ < lep1bjet2delR_)
{
lep1bjetinmassc_=(v_tlv_all_leptons.at (0) + v_tlv_all_bquarks.at (1)).M();
lep1bjetinmassf_=(v_tlv_all_leptons.at (0) + v_tlv_all_bquarks.at (0)).M();
}


if(lep2bjet1delR_ > lep2bjet2delR_)
{
lep2bjetinmassf_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (0)).M();
lep2bjetinmassc_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (1)).M();

}
if(lep2bjet1delR_ < lep1bjet2delR_)
{
lep2bjetinmassf_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (1)).M();
lep2bjetinmassc_=(v_tlv_all_leptons.at (1) + v_tlv_all_quarks.at (0)).M();
}



float ab=lep1jet1delR_, bb=lep1jet2delR_, cb=lep2jet1delR_, db=lep2jet2delR_;
float a1b=(v_tlv_all_leptons.at (0) + v_tlv_all_bquarks.at (0)).M(), b1b=(v_tlv_all_leptons.at (0) + v_tlv_all_bquarks.at (1)).M(), c1b=(v_tlv_all_leptons.at (1) + v_tlv_all_bquarks.at (0)).M(), d1b=(v_tlv_all_leptons.at (1) + v_tlv_all_bquarks.at (1)).M();
float xb,yb,pb,tb,fb,ansb;

if (ab<bb)
{
xb=a1b;
pb=ab;
}

if (bb<ab)
{
xb=b1b;
pb=bb;
}

if (cb<db)
{
yb=c1b;
tb=cb;
}
if (db<cb)
{
yb=d1b;
tb=db;
}

if(pb<tb)
{
lowlepbjetinmass_=xb;
 if (ab<bb) {fb=d1b;}
  if (bb<ab) {fb=c1b;}
}
if(tb<pb)
{
lowlepbjetinmass_=yb;
if (cb<db) {fb=b1b;}
  if (db<cb) {fb=a1b;}
}

biglepbjetinmass_=fb;








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
Mlep12jet12_=missdummy.M();
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

//jet number
if (v_tlv_all_quarks.size()>1)
{
njet_= 0;
n1jet_=0;
for(int a4=0; a4 < v_tlv_all_quarks.size() ; a4++) 
{
 
njet_++;
if(v_tlv_all_quarks.at (a4).Pt ()>30)
{
n1jet_++;
}

}
}


//lepton number

if (v_tlv_all_leptons.size()>1)
{
nlep_= 0;
for(int a5=0; a5 < v_tlv_all_leptons.size() ; a5++) 
{
nlep_++;

}
}

//bjet number

if (v_tlv_all_bquarks.size()>1)
{
nbjet_= 0;
for(int a6=0; a6 < v_tlv_all_bquarks.size() ; a6++) 
{
 
nbjet_++;


}
}


//ljet number

if (v_tlv_all_lquarks.size()>1)
{
nljet_= 0;
for(int a7=0; a7 < v_tlv_all_lquarks.size() ; a7++) 
{
 
nljet_++;

}
}



  
  tree->Fill();
 }
}


void myTree::Write(TFile& out)
 {
  out.cd();
  tree->Write();
 }


int main (int argc, char **argv)
 {
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
