void plotsignalwc(std::string variable,std:: string variable1) //plotting the function
{
TFile file("my.root","READ");
TTree* tree=(TTree*)file .Get("tree");

double x = tree->Draw(variable.c_str(),variable1.c_str() );
cout << "X:: "<< x  << endl; 

}

