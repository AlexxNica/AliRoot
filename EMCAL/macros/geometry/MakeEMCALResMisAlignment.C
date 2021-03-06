void MakeEMCALResMisAlignment(TString geoname = "EMCAL_COMPLETE12SMV1_DCAL_8SM"){
  // Create TClonesArray of residual misalignment objects for EMCAL
  //
  const char* macroname = "MakeEMCALResMisAlignment.C";
  if(geoname=="")geoname=AliEMCALGeometry::GetDefaultGeometryName();
  const AliEMCALGeometry *emcalGeom = AliEMCALGeometry::GetInstance(geoname,"");
  if(!emcalGeom) {
    Error("MakeEMCALResMisAlignment","Cannot obtain AliEMCALGeometry singleton\n");
    return;
  }

  TClonesArray *array = new TClonesArray("AliAlignObjParams",emcalGeom->GetNumberOfSuperModules());
  TClonesArray &alobj = *array;


  // Activate CDB storage and load geometry from CDB
  AliCDBManager* cdb = AliCDBManager::Instance();
  if(!cdb->IsDefaultStorageSet()) cdb->SetDefaultStorage("local://$ALICE_ROOT/OCDB");
  cdb->SetRun(0);
  
  AliCDBStorage* storage;
  
  if( TString(gSystem->Getenv("TOCDB")) == TString("kTRUE") ){
    TString Storage = gSystem->Getenv("STORAGE");
    if(!Storage.BeginsWith("local://") && !Storage.BeginsWith("alien://")) {
      Error(macroname,"STORAGE variable set to %s is not valid. Exiting\n",Storage.Data());
      return;
    }
    storage = cdb->GetStorage(Storage.Data());
    if(!storage){
      Error(macroname,"Unable to open storage %s\n",Storage.Data());
      return;
    }
    AliCDBPath path("GRP","Geometry","Data");
    AliCDBEntry *entry = storage->Get(path.GetPath(),cdb->GetRun());
    if(!entry) Fatal(macroname,"Could not get the specified CDB entry!");
    entry->SetOwner(0);
    TGeoManager* geom = (TGeoManager*) entry->GetObject();
    AliGeomManager::SetGeometry(geom);
  }else{
    AliGeomManager::LoadGeometry(); //load geom from default CDB storage
  }    

  Double_t dx, dy, dz, dpsi, dtheta, dphi;

  const TString fbasepath = "EMCAL/FullSupermodule";
  const TString hbasepath = "EMCAL/HalfSupermodule";
  const TString tbasepath = "EMCAL/OneThrdSupermodule";
  const TString dbasepath = "EMCAL/DCALSupermodule";
  const TString debasepath= "EMCAL/DCALExtensionSM";
  TString pathstr;

  Int_t iIndex=0; // let all modules have index=0 in a layer with no LUT
  AliGeomManager::ELayerID iLayer = AliGeomManager::kInvalidLayer;
  UShort_t volid = AliGeomManager::LayerToVolUID(iLayer,iIndex);

  Int_t i;

  // RS = local
  // sigma translation = 1mm
  // sigma rotation = 0.1 degree
  TRandom *rnd   = new TRandom(4321);
  Double_t sigmatr = 0.05; // max shift in cm w.r.t. local RS
  Double_t sigmarot = 0.1; // max rot in degrees w.r.t. local RS

  Int_t nSM = emcalGeom->GetEMCGeometry()->GetNumberOfSuperModules();
  for(i=0; i<nSM; i++){
    dx = rnd->Gaus(0.,sigmatr);
    dy = rnd->Gaus(0.,sigmatr);
    dz = rnd->Gaus(0.,sigmatr);
    dpsi = rnd->Gaus(0.,sigmarot);
    dtheta = rnd->Gaus(0.,sigmarot);
    dphi = rnd->Gaus(0.,sigmarot);
    if(     emcalGeom->GetSMType(i) == kEMCAL_Standard) pathstr=fbasepath;
    else if(emcalGeom->GetSMType(i) == kEMCAL_Half    ) pathstr=hbasepath;
    else if(emcalGeom->GetSMType(i) == kEMCAL_3rd     ) pathstr=tbasepath;
    else if(emcalGeom->GetSMType(i) == kDCAL_Standard ) pathstr=dbasepath;
    else if(emcalGeom->GetSMType(i) == kDCAL_Ext      ) pathstr=debasepath;
    pathstr+=(i+1);
    new(alobj[i]) AliAlignObjParams(pathstr, volid, dx, dy, dz, dpsi, dtheta, dphi, kFALSE);
  }

  if( TString(gSystem->Getenv("TOCDB")) != TString("kTRUE") ){
    // save on file
    const char* filename = "EMCALresidualMisalignment.root";
    TFile f(filename,"RECREATE");
    if(!f){
      Error(macroname,"cannot open file for output\n");
      return;
    }
    Info(macroname,"Saving alignment objects to the file %s", filename);
    f.cd();
    f.WriteObject(array,"EMCALAlignObjs","kSingleKey");
    f.Close();
  }else{
    // save in CDB storage
    AliCDBMetaData* md = new AliCDBMetaData();
    md->SetResponsible("Jennifer Klay");
    md->SetComment("Residual misalignment for EMCAL, produced with sigmatr=0.05 and sigmarot=0.1 in the local RS");
    md->SetAliRootVersion(gSystem->Getenv("ARVERSION"));
    AliCDBId id("EMCAL/Align/Data",0,AliCDBRunRange::Infinity());
    storage->Put(array,id,md);
  }

  array->Delete();

}

