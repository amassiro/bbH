void plotsignal(std::string variable) //plotting the function
{
TFile file("my.root","READ");
TTree* t=(TTree*)file .Get("tree");
t-> Draw(variable.c_str()); 

}

