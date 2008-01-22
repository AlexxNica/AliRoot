// $Id$
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

/**************************************************************************
 * Copyright(c) 1998-2008, ALICE Experiment at CERN, all rights reserved. *
 * See http://aliceinfo.cern.ch/Offline/AliRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/

#include "AliEveITSScaledModule.h"

#include <AliITSdigitSPD.h>
#include <AliITSdigitSDD.h>
#include <AliITSdigitSSD.h>


//______________________________________________________________________________
// AliEveDigitScaleInfo
//
ClassImp(AliEveDigitScaleInfo)

AliEveDigitScaleInfo::AliEveDigitScaleInfo():
  fScale(1),
  fStatType (kST_Average),
  fSyncPalette(kFALSE)
{
}

void AliEveDigitScaleInfo::ScaleChanged(Int_t s)
{
  fScale = s;

  AliEveITSScaledModule* sm;
  std::list<TEveElement*>::iterator i = fBackRefs.begin();
  while (i != fBackRefs.end())
  {
    sm = dynamic_cast<AliEveITSScaledModule*>(*i);
    if(sm) sm->LoadQuads();
    ++i;
  }
}

void AliEveDigitScaleInfo::StatTypeChanged(Int_t t)
{
  fStatType = t;
  fSyncPalette = kTRUE;

  AliEveITSScaledModule* sm;
  std::list<TEveElement*>::iterator i = fBackRefs.begin();
  while (i != fBackRefs.end())
  {
    sm = dynamic_cast<AliEveITSScaledModule*>(*i);
    if(sm) sm->SetQuadValues();
    ++i;
  }
}

//______________________________________________________________________________
// ScaledDigit
//
ScaledDigit::ScaledDigit():
  TObject(),
  N(0),
  sum(0), sqr_sum(0),
  min_i(-1), min_j(-1), max_i(-1), max_j(-1)
{
}

ScaledDigit::ScaledDigit(Int_t di, Int_t dj):
  TObject(),
  N(0),
  sum(0), sqr_sum(0),
  min_i(di), min_j(dj), max_i(di), max_j(dj)
{
}

void ScaledDigit::Dump() const
{
  printf("N %d, sum %f, sqr_sum %f",N, sum, sqr_sum);
}

//______________________________________________________________________________
// AliEveITSScaledModule
//

ClassImp(AliEveITSScaledModule)

AliEveITSScaledModule::AliEveITSScaledModule(Int_t gid, AliEveITSDigitsInfo* info, AliEveDigitScaleInfo* si):
  AliEveITSModule("AliEveITSScaledModule", "AliEveITSScaledModule"),
  fNx(-1),
  fNz(-1),
  fNCx(-1),
  fNCz(-1),
  fScaleInfo(si)
{
  SetOwnIds(kTRUE);

  SetDigitsInfo(info);
  SetID(gid);
  fScaleInfo->IncRefCount(this);
}

AliEveITSScaledModule::~AliEveITSScaledModule()
{
  fScaleInfo->DecRefCount(this);
}

/******************************************************************************/

void AliEveITSScaledModule::LoadQuads()
{
  // Here we still use 'z' for the name of axial coordinates.
  // The transforamtion matrix aplied rotates y -> z.
  // We need this as TEveQuadSet offers optimized treatment for
  // quads in the x-y plane.

  TClonesArray *digits;
  Float_t       x, z, zo, dpx, dpz; // orig cells size, pos
  Int_t         i, j, ndigits;   // orig cells idx
  Int_t         c1, c2;          // original coordinates

  Int_t id;
  std::map<Int_t, Int_t> dmap;
  std::map<Int_t, Int_t>::iterator miter;
  digits  = fInfo->GetDigits(fID, fDetID);
  ndigits = digits->GetEntriesFast();

  ScaledDigit* sd;
  Int_t scale = fScaleInfo->GetScale() -1;
  switch(fDetID)
  {
    case 0:
    {
      // SPD
      Reset(kQT_RectangleXZFixedY, kFALSE, 32);

      fNCz = fInfo->fSPDScaleZ[scale];
      fNCx = fInfo->fSPDScaleX[scale];
      fNz  = Int_t(fInfo->fSegSPD->Npz()/fNCz);
      fNx  = Int_t(fInfo->fSegSPD->Npx()/fNCx);
      dpz = 2*fDz/fNz;
      dpx = 2*fDx/fNx;
      //printf("SPD orig cells (%d, %d) (%d, %d)\n", fInfo->fSegSPD->Npx(), fInfo->fSegSPD->Npz(), Nx, Nz);

      AliITSdigitSPD *od ;
      for (Int_t k=0; k<ndigits; ++k)
      {
	od = (AliITSdigitSPD*) digits->UncheckedAt(k);

	fInfo->GetSPDLocalZ(od->GetCoord1(),zo);
        c1 = od->GetCoord1(); c2 = od->GetCoord2();
	i = Int_t((zo+fDz)/dpz);
	j = Int_t((od->GetCoord2()*fNx)/fInfo->fSegSPD->Npx());
	id = j*fNx + i;

        miter = dmap.find(id);
	if(miter == dmap.end())
	{
          dmap[id] = fPlex.Size();
          z = dpz*(i) - fDz;
          x = dpx*(j) - fDx;
          AddQuad(x, z, dpx, dpz);
          sd = new ScaledDigit(c1, c2);
	  QuadId(sd);
	}
        else
	{
	  sd = dynamic_cast<ScaledDigit*>(GetId(miter->second));
          if(c1 < sd->min_i)
	    sd->min_i = c1;
	  else if( c1 > sd->max_i)
            sd->max_i = c1;

          if(c2 < sd->min_j)
	    sd->min_j = c2;
	  else if( c2 > sd->max_j)
	    sd->max_j = c2;
	}

	sd->N++;
	sd->sum  += od->GetSignal();
	sd->sqr_sum += od->GetSignal()*od->GetSignal();
      }
      break;
    }
    case 1:
    {
      // SDD
      Reset(kQT_RectangleXZFixedY, kFALSE, 32);

      fNCz = fInfo->fSDDScaleZ[scale];
      fNCx = fInfo->fSDDScaleX[scale];
      fNz  = Int_t(fInfo->fSegSDD->Npz()/fNCz);
      fNx  = Int_t(fInfo->fSegSDD->Npx()/fNCx);
      dpz = 2*fDz/fNz;
      dpx = 2*fDx/fNx;

      AliITSdigitSDD *od=0;
      for (Int_t k=0; k<ndigits; k++) {
	od=(AliITSdigitSDD*)digits->UncheckedAt(k);
	fInfo->fSegSDD->DetToLocal(od->GetCoord2(), od->GetCoord1(),x,z);
	z+= fDz;
	x+= fDx;
	i = Int_t(z/dpz);
	j = Int_t(x/dpx);
	//printf("Mod %d coord %d,%d out of %d,%d :: ORIG coord %d,%d out of %d,%d \n",fID,
	//       i,j,Nz,Nx,od->GetCoord1(),od->GetCoord2(),fInfo->fSegSDD->Npz(),fInfo->fSegSDD->Npx());

	id = j*fNx + i;
	c1 = od->GetCoord1(); c2 = od->GetCoord2();

	miter = dmap.find(id);
	if(miter == dmap.end())
	{
	  dmap[id] = fPlex.Size();
	  z = dpz*(i) - fDz;
	  x = dpx*(j) - fDx;
	  AddQuad(x, z, dpx, dpz);
	  sd = new ScaledDigit(od->GetCoord1(),od->GetCoord2());
	  QuadId(sd);
	}
	else
	{
	  sd = dynamic_cast<ScaledDigit*>(GetId(miter->second));
	  if(c1 < sd->min_i)
	    sd->min_i = c1;
	  else if( c1 > sd->max_i)
	    sd->max_i = c1;

	  if(c2 < sd->min_j)
	    sd->min_j = c2;
	  else if( c2 > sd->max_j)
	    sd->max_j = c2;
	}
	sd->N++;
	sd->sum  += od->GetSignal();
	sd->sqr_sum += od->GetSignal()*od->GetSignal();
      }
      break;
    }
    case 2:
    {
      // SSD
      Reset(kQT_LineXZFixedY, kFALSE, 32);

      AliITSsegmentationSSD* seg = fInfo->fSegSSD;
      Float_t ap, an; // positive/negative angles -> offsets
      seg->Angles(ap, an);
      ap =   TMath::Tan(ap) * fDz;
      an = - TMath::Tan(an) * fDz;

      fNCx  = fInfo->fSSDScale[scale];
      fNz  = 1;
      fNx  = Int_t(fInfo->fSegSSD->Npx()/fNCx);
      dpz = 2*fDz/fNz;
      dpx = 2*fDx/fNx;

      AliITSdigitSSD *od=0;
      for (Int_t k=0; k<ndigits; k++) {
	od=(AliITSdigitSSD*)digits->UncheckedAt(k);
	if(od->GetCoord1() == 1)
	  i = 1; // p side
	else
	  i= -1; // n side
	j = Int_t(od->GetCoord2()/fNCx);
	c1 = od->GetCoord1(); c2 = od->GetCoord2();
	id = j*i;

	miter = dmap.find(id);
	ScaledDigit* sd;
	if(miter == dmap.end())
	{
	  // printf("orig digit %d,%d scaled %d,%d \n",od->GetCoord1(),od->GetCoord2(),i,j);
	  dmap[id] = fPlex.Size();
	  z = dpz*(i) - fDz;
	  x = dpx*(j) - fDx;
	  Float_t a = ( od->GetCoord1() == 1) ? ap : an;
	  AddLine(x-a, -fDz, 2*a, 2*fDz);

	  sd = new ScaledDigit(c1, c2);
	  QuadId(sd);
	}
	else
	{
	  sd = dynamic_cast<ScaledDigit*>(GetId(miter->second));
	  if(c1 < sd->min_i)
	    sd->min_i = c1;
	  else if( c1 > sd->max_i)
	    sd->max_i = c1;

	  if(c2 < sd->min_j)
	    sd->min_j = c2;
	  else if( c2 > sd->max_j)
	    sd->max_j = c2;
	}
	sd->N++;
	sd->sum  += od->GetSignal();
	sd->sqr_sum += od->GetSignal()*od->GetSignal();
      } // for digits
      break;
    } // end case 2
  } // end switch

  SetQuadValues();
  RefitPlex();
}

/******************************************************************************/

void AliEveITSScaledModule::SetQuadValues()
{
  if(fScaleInfo->GetSyncPalette()) SyncPalette();

  Int_t N = fPlex.Size();
  for (Int_t i = 0 ; i< N; i++)
  {
    ScaledDigit* sd = dynamic_cast<ScaledDigit*>(GetId(i));
    Int_t v = 0;
    switch(fScaleInfo->GetStatType())
    {
      using namespace TMath;

      case AliEveDigitScaleInfo::kST_Occup:
	v = Nint((100.0*sd->N) / (fNCx*fNCz));
	break;
      case AliEveDigitScaleInfo::kST_Average:
	v = Nint((Double_t) sd->sum / sd->N);
	break;
      case AliEveDigitScaleInfo::kST_Rms:
	v = Nint(Sqrt(sd->sqr_sum) / sd->N);
	break;
    }
    DigitBase_t* qb = GetDigit(i);
    qb->fValue = v;
  }
}

/******************************************************************************/

void AliEveITSScaledModule::SyncPalette()
{
  // printf("AliEveITSScaledModule::SyncPalette()\n");
  if(fScaleInfo->GetStatType() == AliEveDigitScaleInfo::kST_Occup)
  {
    // SPD
    AliEveITSModule::fgSPDPalette->SetLimits(0, 100);
    AliEveITSModule::fgSPDPalette->SetMinMax(0, 100);

    // SDD
    AliEveITSModule::fgSDDPalette->SetLimits(0, 100);
    AliEveITSModule::fgSDDPalette->SetMinMax(0, 100);

    // SSD
    AliEveITSModule::fgSSDPalette->SetLimits(0, 100);
    AliEveITSModule::fgSDDPalette->SetMinMax(0, 100);
  }
  else
  {
    AliEveITSDigitsInfo& DI = *fInfo;
    // SPD
    AliEveITSModule::fgSPDPalette->SetLimits(0, DI.fSPDHighLim);
    AliEveITSModule::fgSPDPalette->SetMinMax(DI.fSPDMinVal, DI.fSPDMaxVal);

    // SDD
    AliEveITSModule::fgSDDPalette->SetLimits(0, DI.fSDDHighLim);
    AliEveITSModule::fgSDDPalette->SetMinMax(DI.fSDDMinVal, DI.fSDDMaxVal);

    // SSD
    AliEveITSModule::fgSSDPalette->SetLimits(0, DI.fSSDHighLim);
    AliEveITSModule::fgSSDPalette->SetMinMax(DI.fSSDMinVal, DI.fSSDMaxVal);
  }

  fScaleInfo->SetSyncPalette(kFALSE);
}

/******************************************************************************/

void AliEveITSScaledModule::GetScaleData(Int_t& cnx, Int_t& cnz, Int_t& total)
{
  cnx =fNx;
  cnz =fNz;
  total = cnx*cnz;
}

/******************************************************************************/

void  AliEveITSScaledModule::DigitSelected(Int_t idx)
{
  // Override control-click from TEveQuadSet
  printf("AliEveITSScaledModule::DigitSelected "); Print();

  DigitBase_t* qb  = GetDigit(idx);
  TObject* obj  = qb->fId.GetObject();
  ScaledDigit* sd = dynamic_cast<ScaledDigit*>(obj);
  TClonesArray *digits = fInfo->GetDigits(fID, fDetID);
  Int_t ndigits = digits->GetEntriesFast();

  printf("%d digits in cell scaleX = %d,  scaleZ = %d \n", sd->N, fNCx, fNCz);

  Int_t il = 0;
  for(Int_t k=0; k<ndigits; k++)
  {
    AliITSdigit *d = (AliITSdigit*) digits->UncheckedAt(k);

    if(d->GetCoord1()>=sd->min_i && d->GetCoord1()<=sd->max_i &&
       d->GetCoord2()>=sd->min_j && d->GetCoord2()<=sd->max_j)
    {
      printf("%3d, %3d: %3d", d->GetCoord1(), d->GetCoord2(), d->GetSignal());
      printf(" | ");
      il++;
      if(il>5) {
	printf("\n");
	il = 0;
      }
    }
  }
  if(il) printf("\n");
}
