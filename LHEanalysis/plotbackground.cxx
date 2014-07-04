void plotbackground(std::string variable) //plotting the function
{
TFile file("my1.root","READ");
TTree* t1=(TTree*)file .Get("tree");
t1-> Draw(variable.c_str()); 

}

