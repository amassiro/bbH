void Amt(std::string variable) //plotting the function
{
TFile file("my.root","READ");
TTree* t=(TTree*)file .Get("tree");
t-> Draw(variable.c_str(),"missenergy<0.00001"); 

}

