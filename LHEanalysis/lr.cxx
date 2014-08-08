void lr(std::string variable, std::string variable1, int nbin, int low, int up ) ////variable selects the variable to plot variable 1 selects the cut

{
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


TCanvas *c = new TCanvas();
//h1->DrawNormalized();
//c->SaveAs("h1.pdf");



//c->Clear();
//h2->DrawNormalized();
//c->SaveAs("h2.pdf");


       TH1F *hnew = (TH1F*)h1->Clone("hnew");
h1-> Divide(h2);


c->Clear();
c->SetLogy();
hnew.SetLineColor(1); 
hnew->DrawNormalized();
h1.SetLineColor(2); 
h1->DrawNormalized("same");
h2.SetLineColor(4); 
h2->DrawNormalized("same");

c->SaveAs("ratio.pdf");

}

