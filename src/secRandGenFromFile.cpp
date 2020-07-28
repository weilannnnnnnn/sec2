#include "secRandGenFromFile.hh"
#include "globals.hh"
#include <fstream>
#include <sstream>
#include <iostream>

secRandGenFromFile::secRandGenFromFile()
    secVRandGen()
{

}

secRandGenFromFile::~secRandGenFromFile()
{

}

static secRandGenFromFile* secRandGenFromFile::GetInstance()
{
    static secRandGenFromFile Instance;
    return &Instance;
}

void secRandGenFromFile::LoadFile(const std::string& Name)
{   
    G4PhysicsFreeVector XYparis; //save the x-y data pairs
    std::ifstream ifstrm;
    std::istringstream istrstrm;
    std::vector<G4double> Xval;
    std::vector<G4double> Yval;

    ifstrm.open(Name, std::ifstream::in | std::ifstream::binary);

    if( !ifstrm.is_open() )
    {
        //error msg!!
        G4cerr << "===========================================================\n"
               << "                   Warning From sec2\n"
               << "In function secRandGenFromFile::LoadFile() FileName called \n" 
               << Name << "Not found!, probably wrong name.\n"
               << G4endl;
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
            
            G4double X = 0, Y = 0;
            istrstrm >> X >> Y;
            Xval.push_back(X);
            Yval.push_back(Y);
            if( Y > Max )
            {
                Max = Y;
            }
        }
        YmaxVect.push_back(Max);
    }

    for(size_t i = 0; i != Xval.size(); ++i)
    {
        XYpairs.PutValue(i, Xval[i], Yval[i]);
    }
    
    XminVect.push_back( XYpairs.GetEnergy(0) );
    XmaxVect.push_back( XYparis.GetMaxEnergy() );
    XYvectors.push_back( XYparis );
}

G4double secRandGenFromFile::PDF(G4double X, size_t i)
{
    if( XYvectors.size() - 1 < i )
    {
        //error msg!!
        G4cerr << "===========================================================\n"
               << "                   Warning From sec2\n"
               << "In function secRandGenFromFile::PDF(), Index OverFlow!\n"
               << G4endl;

        return 0.;
    }

    return ( XYvectors[i] ).Value(X);
}
