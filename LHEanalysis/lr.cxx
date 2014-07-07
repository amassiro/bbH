void lr(std::string variable, int nbin, int low, int up ) //plotting the function
{
TFile file("my.root","READ");
TFile file1("my1.root","READ");

TTree* tree=(TTree*) file.Get("tree");
TTree* tree1=(TTree*) file1.Get("tree");

TH1F *h1 = new TH1F("h1", "h1 title", nbin,low, up);
TH1F *h2 = new TH1F("h2", "h2 title", nbin, low, up);


TString drawStr1(variable + " >> h1");
TString drawStr2(variable + " >> h2");

tree->Draw(drawStr1);
tree1->Draw(drawStr2);

TCanvas *c = new TCanvas();
h1->Draw();
c->SaveAs("h1.pdf");



c->Clear();
h2->Draw();
c->SaveAs("h2.pdf");


       TH1F *hnew = (TH1F*)h1->Clone("hnew");
h1-> Divide(h2);


c->Clear();
c->SetLogy();
hnew->Draw();
hnew.SetLineColor(1); \\1->black
h1->Draw("same");
h1.SetLineColor(2); \\2->red
h2->Draw("same");
h2.SetLineColor(4); \\4-> blue

c->SaveAs("ratio.pdf");

}

