# This is the 0.3 version of readme file.  
I love my orange cat!

## 1. What is the code package ?
This Package is a monte-carlo simulation program powered by Geant4-10.6.p01, CERN

## 2. How can I use it ?
### To BUILD this package, just
    mkdir build && cd build 
    cmake ..
    make

### To RUN sec2, just 
    ./sec2 MACRO_FILE_NAME 
    
### Available macro files are placed in 
    /macros
### Available data files are placed in
    /DataFiles
    
## 3. Others
Data output is done by the sensitive detector objects, secScintSD and secSiPMSD, specifically.
Browse secScintSD.hh for detailed information. 
