#include "secMacros.hh"
#include "G4UIdirectory.hh"

secMacros::secMacros()
{
    CmdBaseDir = new G4UIdirectory("/sec/");
    CmdBaseDir->SetGuidence("Macro Commands for sec detector");
}

secMacros::~secMacros()
{
    delete CmdBaseDir;
}