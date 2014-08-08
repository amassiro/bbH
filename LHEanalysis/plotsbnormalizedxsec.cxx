#include <iostream> #include <string> 
#include <sstream> using namespace std;
using namespace std;


void plotsbnormalizedxsec(std::string variable,std:: string variable_cut, int nbin, int low, int up ) //variable selects the variable to plot variable 1 selects the cut

{

  TFile *root_file = new TFile("plotsbnormalizedxsec.root","RECREATE");
TCanvas *A = new TCanvas("canvas","canvas");
TH1F *hsig; 
TH1F *hback; 

TFile filesig("signal.root","READ");
TFile fileback("background.root","READ");



TTree* treesig=(TTree*) filesig.Get("tree");
TTree* treeback=(TTree*) fileback.Get("tree");



//Getting entries
   int n_entries_sig = treesig->GetEntries();
      int n_entries_back = treeback->GetEntries();
      string a22 = "(";     
      string a23 = ")";
      string a24 = "*"; 
   


float sigxsec = 0.004593*100;// mutiplied 100 to set y-axis
float a = sigxsec/n_entries_sig;
stringstream ss;
ss << a;
string str_sigweight = ss.str();



float backxsec= 225;
float a1 = backxsec/n_entries_back;
stringstream ss1;
ss1 << a1;
string str_backweight = ss1.str();




//final string
str_sigweight=  a22 + variable_cut.c_str() + a23 + a24 + a22+ str_sigweight + a23;
str_backweight= a22 +variable_cut.c_str() + a23 + a24 +a22 + str_backweight + a23;







cout << " signal entries= " << n_entries_sig <<"\n";
cout << " background entries= " << n_entries_back<<"\n" ;

cout << " signal input = " << str_sigweight <<"\n";	
cout << " background input = " << str_backweight <<"\n";	






hsig = new TH1F("hsig", "hsig title", nbin,low, up);
hback = new TH1F("hback", "h2 title", nbin, low, up);


TString drawStr1(variable + " >> hsig");
TString drawStr2(variable + " >> hback");

treesig->Draw(drawStr1,  str_sigweight.c_str(),"goff" );
treeback->Draw(drawStr2, str_backweight.c_str(),"goff" );

leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("");
   leg->AddEntry(hsig,"SIGNAL*100","l");
   leg->AddEntry(hback,"BACKGROUND","l");

//A->Clear();
A->SetLogy();
//TLegend(plot);
hback->SetLineColor(7); 
hback->GetXaxis()->SetTitle(variable.c_str());
hback->GetYaxis()->SetTitle("Pb");
hback->DrawClone("hist");
hsig->SetLineColor(1); //1 is black
hsig->DrawClone("samehist");
leg->DrawClone("same");
A->Update();
A->SaveAs("plotsb.pdf");
root_file -> cd();
A->Write();
}

