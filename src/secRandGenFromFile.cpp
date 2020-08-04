#include "secRandGenFromFile.hh"
#include "secRandMacro.hh"
#include "globals.hh"
#include "G4ios.hh"
#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>

secRandGenFromFile::secRandGenFromFile() : 
    secVRandGen(),
{
    RandMacro = new secRandMacro(this);
}
secRandGenFromFile::~secRandGenFromFile()
{
    delete RandMacro;
}

secRandGenFromFile* secRandGenFromFile::GetInstance()
{
    static secRandGenFromFile Instance;
    return &Instance;
}

void secRandGenFromFile::LoadFile(const std::string& Name, secVRandGen::DistFuncType Type)
{
    std::ifstream ifstrm;
    std::istringstream istrstrm;
    std::vector<G4double> Xval;
    std::vector<G4double> Yval;
    
    ifstrm.open(Name, std::ifstream::in);
    G4double Max = 0;//store the max y value of pdf!
    if( !ifstrm.is_open() )
    {
        //error msg!!
	    std::cerr << "===========================================================\n"
               << "                   Error From sec2\n"
               << "In function secRandGenFromFile::LoadFile() FileName called \n" 
               << Name << " Not found!, probably wrong name.\n"
               << std::endl;
	    return;
    }
    else
    {
        std::string line;
        while( getline(ifstrm, line) )
        {
	        istrstrm.clear();
            istrstrm.str(line);
            
            G4double X = NAN, Y = NAN;
            istrstrm >> X >> Y;
            //std::cout << "X = " << X << ' ' << "Y = " << Y << std::endl;            
	        if( std::isnan(X) || std::isnan(Y) )
	        {
                break;
	        }    
            Xval.push_back(X);
	        Yval.push_back(Y);

            //record the max val of the PDF
            if( Y > Max )
            {
                Max = Y;
            }
        }
    }

    G4PhysicsOrderedFreeVector XYpairs( &(Xval[0]), &(Yval[0]), Xval.size() );//set the x - y data pairs

    if(Type == secVRandGen::PDF_TYPE )
    {
        PDFXYvectors.push_back( XYpairs );

        XmaxVect.push_back( XYpairs.GetMaxEnergy() );
        XminVect.push_back( XYpairs.Energy(0) );
        YmaxVect.push_back(Max);
    }
    else if( Type == secVRandGen::CDF_TYPE )
    {
        CDFXYvectors.push_back( XYpairs );
    }
    else
    {
	    std::cerr << "===========================================================\n"
               << "                   Error From sec2\n"
               << "In function secRandGenFromFile::LoadFile(), Illegal distri-!\n"
               << "bution function type\n"
               << std::endl;
        exit(1);
    }
    
}

G4double secRandGenFromFile::PDF(G4double X, size_t i)
{
    if( PDFXYvectors.empty() || PDFXYvectors.size() - 1 < i )
    {
        //error msg!!
        std::cerr << "===========================================================\n"
                  << "                   Error From sec2\n"
                  << "In function secRandGenFromFile::PDF(), Index OverFlow!\n"
                  << std::endl;

        exit(1);
        return 0.;
    }

    return ( PDFXYvectors[i] ).Value(X);
}

G4double secRandGenFromFile::InverseCDF(G4double Y, size_t i)
{
    if( CDFXYvectors.empty() || CDFXYvectors.size() - 1 < i )
    {
        //error msg!!
        std::cerr << "===========================================================\n"
                  << "                   Error From sec2\n"
                  << "In function secRandGenFromFile::InverseCDF(), Index OverFlow!\n"
                  << G4endl;

        exit(1);
        return 0.;
    }

    return ( CDFXYvectors[i] ).GetEnergy(Y);
}
