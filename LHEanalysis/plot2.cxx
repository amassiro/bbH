void B(std::string variable,std:: string variable1) //plotting the function
{
TFile file1("my1.root","READ");
TTree* tree1=(TTree*)file1 .Get("tree");
//TString test(variable);
TString cut = "1";//test + " < 450";
double y = tree1->Draw(variable.c_str(),variable1.c_str());
cout << "Y:: "<< y  << endl; 

}

