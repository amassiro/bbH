void C(std::string variable, std:: string variable1) // Draw both signal and generaor at the same canvas
{
// It seems the scaling for the my1.root tree histogram is broken.have to check again to be sure.

	TFile file("my.root","READ");
	TFile file1("my1.root","READ");
	TTree* tree = (TTree* )file.Get("tree");
	TTree* tree1 = (TTree* )file1.Get("tree");

double x = tree->Draw(variable.c_str(),variable1.c_str() );


	gPad->Update();

double y = tree1->Draw(variable.c_str(),variable1.c_str(),"same");
cout << "X:: "<< x << ", Y:: " << y << endl; 

// Next Task::  Findout a way to Colorize each graph line for easy viewing.


}
