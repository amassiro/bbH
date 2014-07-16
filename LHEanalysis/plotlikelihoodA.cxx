void plotlikelihoodA(std::string variable,std:: string variable1, int dim , int nbin, double low, int up) //variable selects the variable to plot and variable 1 selects the cut
      
{


TFile *root_file = new TFile("plotlikelihood.root","RECREATE");
TCanvas *A = new TCanvas("canvas","canvas");


TFile file("signal.root","READ");
TFile file1("background.root","READ");



TTree* tree=(TTree*) file.Get("tree");
TTree* tree1=(TTree*) file1.Get("tree");


//THnSparseF fSparse1 = new THnSparseF("hs", "hs", fDim, bins, xmin, xmax);

Double_t *Low = new Double_t[dim];
Double_t *Up = new Double_t[dim];
Int_t *NBins = new Int_t[dim];
for(int d = 0; d < dim; d++)
{
	Low[d] = low;
	Up[d] = up;
	NBins[d] = nbin;
}
THnSparseF *h1 = new THnSparseF("h1", "h1 title", dim, NBins, Low, Up );
THnSparseF *h2 = new THnSparseF("h2", "h2 title", dim, NBins, Low, Up);


TString drawStr1(variable + " >> h1");
TString drawStr2(variable + " >> h2");

tree->Draw(drawStr1, variable1.c_str());
tree1->Draw(drawStr2, variable1.c_str());




       THnSparseF *hnew = (THnSparseF*)h1->Clone("hnew");
h1-> Divide(h2);



A->Clear();

//A->SetLogy();

//A->SetLogy();
//hnew->SetLineColor(1); 
h2->Draw();
/*
h2->SetLineColor(2); //2 is red
h2->DrawNormalized("same");
h1->SetLineColor(4); // 4 is blue
h1->Draw("same");

*/



leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("");
   leg->AddEntry(hnew,"SIGNAL","l");
   leg->AddEntry(h2,"BACKGROUND","l");
   leg->AddEntry(h1,"LR","l");
   leg->Draw();

A->Update();

root_file -> cd();
A->Write();

A->SaveAs("plotlikelihoodAAAAAAAAAA.pdf");

}

