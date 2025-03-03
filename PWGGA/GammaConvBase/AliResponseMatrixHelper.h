/**************************************************************************
 * Copyright(c) 1998-2020, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: Joshua Koenig <joshua.konig@cern.ch>                                        *
 * Version 1.0                                                            *
 *                                                                        *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//////////////////////////////////////////////////////////////////
//----------------------------------------------------------------
// Class to hanlde 2d and 4d response matrix
//----------------------------------------------------------------
//////////////////////////////////////////////////////////////////

#ifndef ALIRESPONSEMATRIXHELPER_h
#define ALIRESPONSEMATRIXHELPER_h

#include <array>
#include <vector>
#include "TH1.h"
#include "TH2.h"
#include "THnSparse.h"

class MatrixHandler4D
{

 public:
  MatrixHandler4D() = default;
  MatrixHandler4D(std::vector<double> arrMesonX, std::vector<double> arrMesonY, std::vector<double> arrJetX, std::vector<double> arrJetY, bool useTHN = false);
  MatrixHandler4D(std::vector<double> arrMesonX, std::vector<double> arrMesonY, std::vector<double> arrJetX, std::vector<double> arrJetY, THnSparse* h = nullptr);
  MatrixHandler4D(std::vector<double> arrMesonX, std::vector<double> arrMesonY, std::vector<double> arrJetX, std::vector<double> arrJetY, TH2F* h = nullptr);
  MatrixHandler4D(const MatrixHandler4D&) = delete;            // copy ctor
  MatrixHandler4D(MatrixHandler4D&&) = delete;                 // move ctor
  MatrixHandler4D& operator=(const MatrixHandler4D&) = delete; // copy assignment
  MatrixHandler4D& operator=(MatrixHandler4D&&) = delete;      // move assignment
  virtual ~MatrixHandler4D();

  int getBinIndexMesonX(const double val) const;
  int getBinIndexMesonY(const double val) const;
  int getBinIndexJetX(const double val) const;
  int getBinIndexJetY(const double val) const;

  double getValueForBinIndexMesonX(const int index) const;
  double getValueForBinIndexJetX(const int index) const;
  double getValueForBinIndexMesonY(const int index) const;
  double getValueForBinIndexJetY(const int index) const;

  void Fill(double valJetX, double valJetY, double valMesonX, double valMesonY, double val = 1);

  void AddBinContent(double valJetX, double valJetY, double valMesonX, double valMesonY, double val = 1, double err = 1);
  void GetAxisBinning(std::vector<double>& vecXBins, std::vector<double>& vecYBins);

  THnSparseF* GetTHnSparseClone(const char* name = "hSparseResponse_Clone");
  THnSparseF* GetTHnSparse(const char* name = "");

  TH2F* GetTH2(const char* name = "hSparseResponse_Clone");
  TH2F* GetResponseMatrix(int binX, int binY, const char* name = "dummy");

 private:
  bool useTHNSparese = false;
  int nBinsJet = 0;
  std::vector<double> vecBinsMesonX = {};
  std::vector<double> vecBinsMesonY = {};
  std::vector<double> vecBinsJetX = {};
  std::vector<double> vecBinsJetY = {};
  TH2F* h2d = nullptr;
  TH1F* h1dJet = nullptr;
  TH1F* h1dMeson = nullptr;
  THnSparseF* hSparseResponse = nullptr;

  ClassDef(MatrixHandler4D, 3)
};

#endif