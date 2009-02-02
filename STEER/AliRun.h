#ifndef ALIRUN_H
#define ALIRUN_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id$ */

//
// General configuration class for Simulation and Reconstruction
// Basic driver for AliRoot runs
// Containing pointers to data elements for AliRoot
//

#include <TSystem.h>

class TRandom;

#include "AliRunLoader.h"
class AliGenEventHeader;
class AliGenerator;
class AliHeader;
class AliMC;
class AliMagF;
class AliStack;


class AliRun : public TNamed {

public:
   // Creators - distructors
   AliRun();
   AliRun(const char *name, const char *title);
   virtual ~AliRun();

   TObjArray     *Detectors() const {return fModules;}
   TObjArray     *Modules() const {return fModules;}
   void           AddModule(AliModule* mod);
   Int_t          GetEvNumber() const;
   void           SetEventNrInRun(Int_t event) {fEventNrInRun=event;}
   Int_t          GetEventNrInRun() const {return fEventNrInRun;}
   Int_t          GetNdets() const {return fNdets;}
   AliModule     *GetModule(const char *name) const;
   AliDetector   *GetDetector(const char *name) const;
   Int_t          GetModuleID(const char *name) const;
   virtual  const char *GetBaseFile() const 
    {return fBaseFileName.Data();}
   virtual  Int_t GetEvent(Int_t event);
   virtual  void  SetEvent(Int_t event) {fEvent=event;}
   virtual  void  SetConfigFunction(const char * config="Config();")
   {fConfigFunction=config;}
   virtual  const char *GetConfigFunction() const 
   {return fConfigFunction.Data();}
   virtual  void  SetGenEventHeader(AliGenEventHeader* header);
   AliMC*         GetMCApp() const {return fMCApp;}
   virtual  void  Hits2Digits(const char *detector=0); 
   virtual  void  Hits2SDigits(const char *detector=0)   {Tree2Tree("S",detector);}
   virtual  void  SDigits2Digits(const char *detector=0) {Tree2Tree("D",detector);}
   virtual  void  Digits2Reco(const char *detector=0)    {Tree2Tree("R",detector);}
   Bool_t         IsFolder() const {return kTRUE;}
   Bool_t         IsRootGeometry() const {return fIsRootGeometry;}
   void           SetRootGeometry(Bool_t flag=kTRUE);
   const char*    GetGeometryFileName() const {return fGeometryFileName.Data();}
   void           SetGeometryFromFile(const char *filename) {
     SetRootGeometry();
     fGeometryFileName = filename;
   }
   void           SetGeometryFromCDB();
   Bool_t         IsGeomFromCDB() const {return fGeometryFromCDB;}
   const char*    GetTriggerDescriptor() const {return fTriggerDescriptor.Data();}
   void           SetTriggerDescriptor(const char *name) {fTriggerDescriptor = name;}
   virtual  void  SetBaseFile(const char *filename="galice.root");
   //
   // End of MC Application

   void SetRunLoader(AliRunLoader* rloader);

  virtual  void Announce() const;
   
  virtual  void  InitLoaders(); //prepares run (i.e. creates getters)

protected:
  virtual  void  Tree2Tree(Option_t *option, const char *detector=0);
  Int_t          fEvent;             //! Current event number (from 1)
  Int_t          fEventNrInRun;      //! Current unique event number in run
  TObjArray     *fModules;           //  List of Detectors
  AliMC         *fMCApp;             //  Pointer to virtual MC Application
  Int_t          fNdets;             //  Number of detectors
  TString        fConfigFunction;    //  Configuration file to be executed
  TRandom       *fRandom;            //  Pointer to the random number generator
  TString        fBaseFileName;      //  Name of the base root file
  Bool_t         fIsRootGeometry;    //! Flag telling if the geometry is loaded from file
  Bool_t         fGeometryFromCDB;   //! Flag telling if the geometry is to be loaded from OCDB
  TString        fGeometryFileName;  //! Name of the geometry file
  TString        fTriggerDescriptor; //  Trigger descriptor identifier
  AliRunLoader  *fRunLoader;         //!run getter - written as a separate object
private:
  AliRun(const AliRun&); // Not implemented
  AliRun& operator = (const AliRun&); // Not implemented

  ClassDef(AliRun,13)      //Supervisor class for all Alice detectors
};
 
R__EXTERN  AliRun *gAlice;
  
#endif
