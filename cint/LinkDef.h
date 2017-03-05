//! \file UserLinkdef.h
//! \brief User-defined portion of the CINT Linkdef file.
//!
//! \full This file is where you can tell CINT about the classes you've written to store and analyze
//! your data.  At the least, you will need to do this for any class that stores parameters that you
//! wish to view or variables used in calculating those parameters.
//! Usually the easiest option is link everything in the header files which define your classes. \n
//! Example:
//! \code
//! #pragma link C++ defined_in /path/to/my/header.hxx;
//! \endcode
//! (note that you need to specify the fill path to the header file).
//!
//! If the above doesn't work, you might have to specify each class (or namespace) individually
//! (or figure out why not). \n
//! Example:
//! \code
//! #pragma link C++ namespace my;
//! #pragma link C++ class my::Class+;
//! // -- OR -- //
//! #pragma link C++ class MyClass+;
//! // -- ETC -- //
//! \endcode
//! (note that the '+' at the end is required except in special cases).
#ifdef __MAKECINT__
//#include <RVersion.h>

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclasses;

class TObject;
class TH1;
#pragma link C++ function SonikFit::ArgBG(TH1D* hh, Double_t, Double_t, Double_t, Double_t*, Bool_t);
#pragma link C++ function SonikFit::Exp_3HeBG(TH1D* hh, Double_t, Double_t, Double_t*, Bool_t);
#pragma link C++ function SonikFit::ExpBG(TH1D* hh, Double_t, Double_t, Double_t, Double_t*, Bool_t);
#pragma link C++ function SonikFit::LandauBG(TH1D* hh, Double_t, Double_t, Double_t, Double_t*, Bool_t);
#pragma link C++ function SonikFit::L_ArgBG(TH1D* hh, Double_t, Double_t, Double_t, Double_t, Double_t*, Bool_t);

#pragma link C++ class SonikFit+;
//#pragma link C++ class sonik::Gainmatch+;
//#pragma link C++ class sonik::SonikCal+;
//#pragma link C++ defined_in ../include/SonikFit.hxx;


#endif
