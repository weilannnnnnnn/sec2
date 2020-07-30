#include "secRandGenFromFile.hh"
#include "globals.hh"
#include "G4ios.hh"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>

secRandGenFromFile::secRandGenFromFile() : 
    secVRandGen()
{

}

secRandGenFromFile::~secRandGenFromFile()
{

}

secRandGenFromFile* secRandGenFromFile::GetInstance()
{
    static secRandGenFromFile Instance;
    std::cout << "In GetInstance()" << std::endl;
    return &Instance;
}

void secRandGenFromFile::LoadFile(const std::string& Name)
{  
    std::ifstream ifstrm;
    std::istringstream istrstrm;
    std::vector<G4double> Xval;
    std::vector<G4double> Yval;

    ifstrm.open(Name, std::ifstream::in | std::ifstream::binary);

    if( !ifstrm.is_open() )
    {
        //error msg!!
        std::cerr << "===========================================================\n"
               << "                   Warning From sec2\n"
               << "In function secRandGenFromFile::LoadFile() FileName called \n" 
               << Name << " Not found!, probably wrong name.\n"
               << std::endl;
	return;
    }
    else
    {
        std::string line;
        G4double Max = 0;

        while( getline(ifstrm, line) )
        {
	    istrstrm.clear();
            istrstrm.str(line);
            
            G4double X = NAN, Y = NAN;
            istrstrm >> X >> Y;
            
	    if( std::isnan(X) || std::isnan(Y) )
	    {
                break;
	    } 

	    std::cout << "X = " << X << " " << "Y = " << Y << std::endl;
	    Xval.push_back(X);
            Yval.push_back(Y);
            if( Y > Max )
            {
                Max = Y;
            }
        }
        YmaxVect.push_back(Max);
    }
    
    G4PhysicsFreeVector XYpairs( Xval.size() );//set the x - y data pairs

    for(size_t i = 0; i != Xval.size(); ++i)
    {
        XYpairs.PutValue(i, Xval[i], Yval[i]);
    }
    
    XminVect.push_back( XYpairs.Energy(0) );
    XmaxVect.push_back( XYpairs.GetMaxEnergy() );
    XYvectors.push_back( XYpairs );
}

G4double secRandGenFromFile::PDF(G4double X, size_t i)
{
    if( XYvectors.empty() || XYvectors.size() - 1 < i )
    {
        std::cerr << "===========================================================\n"
               << "                   Warning From sec2\n"
               << "In function secRandGenFromFile::PDF(), Index OverFlow!\n"
               << std::endl;
          
        return 1. ;
    }

    return ( XYvectors[i] ).Value(X);
}
