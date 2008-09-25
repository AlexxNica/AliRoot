#ifndef AliTRDCALDCSFEE_H
#define AliTRDCALDCSFEE_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id: AliTRDCalDCSFEE.h 18952 2007-06-08 11:36:12Z cblume $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  TRD calibration class for FEE configuration parameters                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "TNamed.h"

class TString;

class AliTRDCalDCSFEE : public TNamed {

 public:

  AliTRDCalDCSFEE();
  AliTRDCalDCSFEE(const char *name, const char *title);
  virtual ~AliTRDCalDCSFEE() { };

  void    SetStatusBit(Int_t stbit)                  { fStatusBit           = stbit; }  
  void    SetDCSid(Int_t dcsid)                      { fDCSID               = dcsid; }  
  void    SetSM(Int_t smid)                          { fSM                  = smid;  }
  void    SetStack(Int_t stid)                       { fStack               = stid;  }
  void    SetLayer(Int_t lyid)                       { fLayer               = lyid;  }
  void    SetNumberOfTimeBins(Int_t value)           { fNumberOfTimeBins    = value; }
  void    SetConfigTag(Int_t cfgt)                   { fConfigTag           = cfgt;  }
  void    SetSingleHitThres(Int_t sht)               { fSingleHitThres      = sht;   }
  void    SetThreePadClustThres(Int_t tpct)          { fThrPdClsThres       = tpct;  }
  void    SetSelectiveNoZS(Int_t snzs)               { fSelNoZS             = snzs;  }
  void    SetFastStatNoise(Int_t fstn)               { fFastStatNoise       = fstn;  }
  void    SetTCFilterWeight(Int_t tcfw)              { fTCFilterWeight      = tcfw;  }
  void    SetTCFilterShortDecPar(Int_t sdp)          { fTCFilterShortDecPar = sdp;   }
  void    SetTCFilterLongDecPar(Int_t ldp)           { fTCFilterLongDecPar  = ldp;   }
  void    SetFilterType(TString fity)                { fFilterType          = fity;  }
  void    SetReadoutParam(TString rpar)              { fReadoutParam        = rpar;  }
  void    SetTestPattern(TString tpat)               { fTestPattern         = tpat;  }
  void    SetTrackletMode(TString tmde)              { fTrackletMode        = tmde;  }
  void    SetTrackletDef(TString tdef)               { fTrackletDef         = tdef;  }
  void    SetTriggerSetup(TString trse)              { fTriggerSetup        = trse;  }
  void    SetAddOptions(TString adop)                { fAddOptions          = adop;  }
  void    SetConfigName(TString cfgn)                { fConfigName          = cfgn;  }
  void    SetConfigVersion(TString cfgv)             { fConfigVersion       = cfgv;  }
  void    SetMCMGlobalState(Int_t r,Int_t m,Int_t g) { fRStateGSM[r][m]     = g;     }
  void    SetMCMStateNI(Int_t r,Int_t m,Int_t v)     { fRStateNI[r][m]      = v;     }
  void    SetMCMEventCnt(Int_t r,Int_t m,Int_t v)    { fRStateEV[r][m]      = v;     }
  void    SetMCMPtCnt(Int_t r,Int_t m,Int_t v)       { fRStatePTRG[r][m]    = v;     }

  Int_t   GetStatusBit() const                       { return fStatusBit;            }
  Int_t   GetDCSid() const                           { return fDCSID;                }
  Int_t   GetSM() const                              { return fSM;                   }
  Int_t   GetStack() const                           { return fStack;                }
  Int_t   GetLayer() const                           { return fLayer;                }
  Int_t   GetNumberOfTimeBins() const                { return fNumberOfTimeBins;     }
  Int_t   GetConfigTag() const                       { return fConfigTag;            }
  Int_t   GetSingleHitThres() const                  { return fSingleHitThres;       }
  Int_t   GetThreePadClustThres() const              { return fThrPdClsThres;        }
  Int_t   GetSelectiveNoZS() const                   { return fSelNoZS;              }
  Int_t   GetTCFilterWeight() const                  { return fTCFilterWeight;       }
  Int_t   GetTCFilterShortDecPar() const             { return fTCFilterShortDecPar;  }
  Int_t   GetTCFilterLongDecPar() const              { return fTCFilterLongDecPar;   }
  Int_t   GetFastStatNoise() const                   { return fFastStatNoise;        }
  TString GetFilterType() const                      { return fFilterType;           }
  TString GetReadoutParam() const                    { return fReadoutParam;         }
  TString GetTestPattern() const                     { return fTestPattern;          }
  TString GetTrackletMode() const                    { return fTrackletMode;         }
  TString GetTrackletDef() const                     { return fTrackletDef;          }
  TString GetTriggerSetup() const                    { return fTriggerSetup;         }
  TString GetAddOptions() const                      { return fAddOptions;           }
  TString GetConfigName() const                      { return fConfigName;           }
  TString GetConfigVersion() const                   { return fConfigVersion;        }
  Int_t   GetMCMGlobalState(Int_t r,Int_t m) const   { return fRStateGSM[r][m];      }
  Int_t   GetMCMStateNI(Int_t r,Int_t m) const       { return fRStateNI[r][m];       }
  Int_t   GetMCMEventCnt(Int_t r,Int_t m) const      { return fRStateEV[r][m];       }
  Int_t   GetMCMPtCnt(Int_t r,Int_t m) const         { return fRStatePTRG[r][m];     }

 protected:
  static const Int_t nROB = 8;
  static const Int_t nMCM = 18;
  
  Int_t   fStatusBit;              // 0 if everything is OK, otherwise !=0 (see impl. file)
  Int_t   fDCSID;                  // ID of the DCS-Board
  Int_t   fSM;                     // the number of the supermode 0..17
  Int_t   fStack;                  // the number of the stack 0..4
  Int_t   fLayer;                  // the number of the layer 0..5
  Int_t   fNumberOfTimeBins;       // Number of timebins  
  Int_t   fConfigTag;              // Configuration tag
  Int_t   fSingleHitThres;         // threshold of single hits (arg of readout param)
  Int_t   fThrPdClsThres;          // threshold of 3-pad clusters (arg of readout param)
  Int_t   fSelNoZS;                // write every fSelNoZS'th event without ZS
  Int_t   fTCFilterWeight;         // tail cancellation filter weight
  Int_t   fTCFilterShortDecPar;    // tail cancellation filter short decay parameter
  Int_t   fTCFilterLongDecPar;     // tail cancellation filter long decay parameter
  Int_t   fFastStatNoise;          // collect statistics for fast noise mode
  Int_t   fRStateGSM[nROB][nMCM];  // array of the global states of the MCMs
  Int_t   fRStateNI[nROB][nMCM];   // array of the network interface states of the MCMs
  Int_t   fRStateEV[nROB][nMCM];   // array of the event counters of the MCMs
  Int_t   fRStatePTRG[nROB][nMCM]; // array of the pretrigger counters of the MCMs
  TString fFilterType;             // filter type (p, pgt, nf)
  TString fReadoutParam;           // readout parameter (zs, nozs, testpattern)
  TString fTestPattern;            // value of testpattern (for readout param)
  TString fTrackletMode;           // tracklet mode (trk, csmtrk, notrk)
  TString fTrackletDef;            // definition for tracklet mode trk
  TString fTriggerSetup;           // trigger setup (ptrg, autotrg, autol0)
  TString fAddOptions;             // additional options (nopm, nion)
  TString fConfigName;             // Configuration name
  TString fConfigVersion;          // Configuration version

  ClassDef(AliTRDCalDCSFEE,3)      // TRD calibration class for TRD FEE parameters
};
#endif
