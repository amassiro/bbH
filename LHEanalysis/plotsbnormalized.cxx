void plotsbnormalized(std::string variable,std:: string variable1, int nbin, int low, int up ) //variable selects the variable to plot variable 1 selects the cut

{


TFile *root_file = new TFile("plotsbnormalized.root","RECREATE");
TCanvas *A = new TCanvas("canvas","canvas");


TFile file("signal.root","READ");
TFile file1("background.root","READ");



TTree* tree=(TTree*) file.Get("tree");
TTree* tree1=(TTree*) file1.Get("tree");





TH1F *h1 = new TH1F("h1", "h1 title", nbin,low, up);
TH1F *h2 = new TH1F("h2", "h2 title", nbin, low, up);


TString drawStr1(variable + " >> h1");
TString drawStr2(variable + " >> h2");

tree->Draw(drawStr1, variable1.c_str());
tree1->Draw(drawStr2, variable1.c_str());

A->Clear();
//A->SetLogy();
//TLegend(plot);
h1->SetLineColor(2); 
h1->GetXaxis()->SetTitle(variable.c_str());
h1->DrawNormalized("");
h2->SetLineColor(1); //1 is black
h2->DrawNormalized("same");
leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("");
   leg->AddEntry(h1,"SIGNAL","l");
   leg->AddEntry(h2,"BACKGROUND","l");
   leg->Draw();

A->Update();
A->SaveAs("plotsb.pdf");
root_file -> cd();
A->Write();

A->SaveAs("plotsb.pdf");

}

