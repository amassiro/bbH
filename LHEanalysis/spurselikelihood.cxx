void spurselikelihood()
{

TFile *root_file = new TFile("spurselikelihood.root","RECREATE");






TFile file("signal.root","READ");
TTree* tree_signal_1=(TTree*) file.Get("tree");


tree_signal_1->SetEntryList(0);
 tree_signal_1->Draw(">> myList","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","entrylist");
 TEntryList *myList = (TEntryList*)gDirectory->Get("myList");
 
 tree_signal_1->SetEntryList(myList);

tree_signal_1->Draw("lep1bjetinmassc:bjetpt2:bjetpt12","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","para goff");

int nEntries = myList->GetN();
 Double_t *vTemp1 = tree_signal_1->GetV1();
 Double_t *vDEta1 = new Double_t[nEntries];
 for (int iEntry = 0; iEntry<nEntries; iEntry++){
  vDEta1[iEntry] = vTemp1[iEntry];
//cout<<vDEta[iEntry]<<endl;
}


 Double_t *vTemp2 = tree_signal_1->GetV2();
 Double_t *vDEta2 = new Double_t[nEntries];
 for ( iEntry = 0; iEntry<nEntries; iEntry++){
  vDEta2[iEntry] = vTemp2[iEntry];
//cout<<vDEta[iEntry]<<endl;
}
 
Double_t *vTemp3 = tree_signal_1->GetV3();
 Double_t *vDEta3 = new Double_t[nEntries];
 for ( iEntry = 0; iEntry<nEntries; iEntry++){
  vDEta3[iEntry] = vTemp3[iEntry];
//cout<<vDEta[iEntry]<<endl;
}


TTree* tree_signal_2=(TTree*) file.Get("tree");


tree_signal_2->SetEntryList(0);
 tree_signal_2->Draw(">> myList1","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","entrylist");
 TEntryList *myList1 = (TEntryList*)gDirectory->Get("myList1");
 
 tree_signal_2->SetEntryList(myList1);

tree_signal_2->Draw("leppt1:leppt2","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","para goff");


 Double_t *vTemp4 = tree_signal_2->GetV1();
 Double_t *vDEta4 = new Double_t[nEntries];
 for (int iEntry = 0; iEntry<nEntries; iEntry++){
  vDEta4[iEntry] = vTemp4[iEntry];
}

Double_t *vTemp5 = tree_signal_2->GetV2();
 Double_t *vDEta5 = new Double_t[nEntries];
 for (int iEntry = 0; iEntry<nEntries; iEntry++){
  vDEta5[iEntry] = vTemp5[iEntry];
}


TTree* tree_signal_3=(TTree*) file.Get("tree");


tree_signal_3->SetEntryList(0);
 tree_signal_3->Draw(">> myList2","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","entrylist");
 TEntryList *myList2 = (TEntryList*)gDirectory->Get("myList2");
 
 tree_signal_3->SetEntryList(myList2);

tree_signal_3->Draw("lepinmass12","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","para goff");


 Double_t *vTemp6 = tree_signal_3->GetV1();
 Double_t *vDEta6 = new Double_t[nEntries];
 for (int iEntry = 0; iEntry<nEntries; iEntry++){
  vDEta6[iEntry] = vTemp6[iEntry];
}




  // create a THnSparse and draw it.

#ifdef __CINT__
   printf("For performance reasons we advise to run \".x drawsparse.C+\"\n");
#endif


 
 








TFile file("background.root","READ");
TTree* tree_back_1=(TTree*) file.Get("tree");


tree_back_1->SetEntryList(0);
 tree_back_1->Draw(">> myList","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<90 && bjetpt1<170  && bjetpt12<200 && missenergy<150","entrylist");
 TEntryList *myList = (TEntryList*)gDirectory->Get("myList");
 
 tree_back_1->SetEntryList(myList);

tree_back_1->Draw("lep1bjetinmassc:bjetpt2:bjetpt12","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<90  && bjetpt1<170  && bjetpt12<200 && missenergy<150","para goff");

int nEntriesb = myList->GetN();
 Double_t *vTemp1b = tree_back_1->GetV1();
 Double_t *vDEta1b = new Double_t[nEntriesb];
 for (int iEntryb = 0; iEntryb<nEntriesb; iEntryb++){
  vDEta1b[iEntryb] = vTemp1b[iEntryb];
//cout<<vDEta[iEntry]<<endl;
}


 Double_t *vTemp2b = tree_back_1->GetV2();
 Double_t *vDEta2b = new Double_t[nEntriesb];
 for ( iEntryb = 0; iEntryb<nEntriesb; iEntryb++){
  vDEta2b[iEntryb] = vTemp2b[iEntryb];
//cout<<vDEta[iEntry]<<endl;
}
 
Double_t *vTemp3b = tree_back_1->GetV3();
 Double_t *vDEta3b = new Double_t[nEntriesb];
 for ( iEntryb = 0; iEntryb<nEntriesb; iEntryb++){
  vDEta3b[iEntryb] = vTemp3b[iEntryb];
//cout<<vDEta[iEntry]<<endl;
}

TTree* tree_back_2=(TTree*) file.Get("tree");


tree_back_2->SetEntryList(0);
 tree_back_2->Draw(">> myList1","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","entrylist");
 TEntryList *myList1 = (TEntryList*)gDirectory->Get("myList1");
 
 tree_back_2->SetEntryList(myList1);

tree_back_2->Draw("leppt1:leppt2","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","para goff");


 Double_t *vTemp4b = tree_back_2->GetV1();
 Double_t *vDEta4b = new Double_t[nEntriesb];
 for (int iEntryb = 0; iEntryb<nEntriesb; iEntryb++){
  vDEta4b[iEntryb] = vTemp4b[iEntryb];
}

Double_t *vTemp5b = tree_back_2->GetV2();
 Double_t *vDEta5b = new Double_t[nEntriesb];
 for (int iEntryb = 0; iEntryb<nEntriesb; iEntryb++){
  vDEta5b[iEntryb] = vTemp5b[iEntryb];
}


TTree* tree_back_3=(TTree*) file.Get("tree");


tree_back_3->SetEntryList(0);
 tree_back_3->Draw(">> myList2","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","entrylist");
 TEntryList *myList2 = (TEntryList*)gDirectory->Get("myList2");
 
 tree_back_3->SetEntryList(myList2);

tree_back_3->Draw("lep1bjetinmassc","bjetpt1>20 && bjetpt2>20 && leppt1>20 && leppt2>10 && lepinmass12!=125 && lepinmass12<200  && bjetpt1<170 && bjetpt2<80 && bjetpt12<200 && leppt1<100 && leppt2<60 && missenergy<150","para goff");


 Double_t *vTemp6b = tree_back_3->GetV1();
 Double_t *vDEta6b = new Double_t[nEntriesb];
 for (int iEntryb = 0; iEntryb<nEntriesb; iEntryb++){
  vDEta6b[iEntryb] = vTemp6b[iEntryb];
}

   const Int_t ndims = 6;
   Int_t bins[ndims] = {30, , 10, 10, 10, 10};
   Double_t xmin[ndims] = {20., 20., 0., 20., 10.,   0.};
   Double_t xmax[ndims] = {500, 80., 200.,   100.,   60,  150 };
   THnSparse* hs = new THnSparseD("hs", "Sparse Histogram", ndims, bins, xmin, xmax);
 const Int_t ndimsb = 10;
   Int_t binsb[ndimsb] = {30, 10, 10, 10, 10, 10, };
   Double_t xminb[ndimsb] = {20., 20., 0., 20., 10.,   0.};
   Double_t xmaxb[ndimsb] = {500, 80., 200.,   100.,   60,  150 , };
   THnSparse* hb = new THnSparseD("hs", "Sparse Histogram", ndimsb, binsb, xminb, xmaxb);
  


  // create a THnSparse and draw it.

#ifdef __CINT__
   printf("For performance reasons we advise to run \".x drawsparse.C+\"\n");
#endif
   // fill it
   Double_t x[ndims];
   for (Long_t i = 0; i < nEntries; ++i) {
      for (Int_t d = 0; d < ndims; ++d) {
         switch (d) {
         case 0: x[d] = vDEta1[i]; break;
               case 1: x[d] = vDEta2[i]; break;
                case 2: x[d] = vDEta3[i]; break;
                case 3: x[d] = vDEta4[i]; break;
               case 4: x[d] = vDEta5[i]; break;
              case 5: x[d] = vDEta6[i]; break;
}
      }
      hs->Fill(x);
   }



     
   // fill it
   Double_t x[ndimsb];
   for (Long_t j = 0; j < nEntriesb; ++j) {
      for (Int_t b = 0; b < ndimsb; ++b) {
         switch (b) {
         case 0: x[b] = vDEta1b[j]; break;
        case 1:x[b] = vDEta2b[j]; break;
        case 2:x[b] = vDEta3b[j]; break;
        case 3:x[b] = vDEta4b[j]; break;
       case 4:x[b] = vDEta5b[j]; break;
      case 5:x[b] = vDEta6b[j]; break;

}
      }
      hb->Fill(x);
   }




//THnSparse *hnew = (THnSparse*)hs->Clone("hnew");
//hs-> Divide(hb);



   TFile* f = new TFile("drawsparse.root","RECREATE");

   TCanvas* canv = new TCanvas("hDrawSparse", "Drawing a sparse hist");

//canv->SetLogy();
   // draw it

   // project it

   TH1D* h3proj = hs->Projection(0);

   h3proj->SetLineColor(kRed);
   h3proj->SetDirectory(0);
   h3proj->DrawNormalized("");

   // save everything to a file
   canv->Write();
root_file -> cd();
   hs->Write();
  // h3proj->Write();



TH1D* h3projb = hb->Projection(0);

   h3projb->SetLineColor(kBlack);
   h3projb->SetDirectory(0);
  h3projb->DrawNormalized("");
   h3proj->DrawNormalized("same");
   // save everything to a file
   canv->Write();
   //hb->Write();
   //h3projb->Write();





leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("");
   leg->AddEntry(h3proj,"SIGNAL","l");
   leg->AddEntry(h3projb,"BACKGROUND","l");
   leg->Draw();








   delete f;
}
