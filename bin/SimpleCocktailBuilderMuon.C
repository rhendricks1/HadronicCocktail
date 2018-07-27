#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TString.h"
#include "TRegexp.h"
#include <stdlib.h>
#include <iostream>


//This program produces a rudimentary hadronic cocktail-like histogram, with or without momentum smearing,
//and allows you to choose the cut you use.
//Run this in ROOT in HadronicCocktail/bin


void SimpleCocktailBuilderMuon () {
	TString momSmear = "";
	TString decayMaker = "./hadronicCocktail.app DecayMaker.xml";
	TString decayChannel[18] = {" --CHANNEL=omega_pi0mumu_dalitz",
		" --CHANNEL=omega_pi0ee_dalitz"," --CHANNEL=omega_mumu",
		" --CHANNEL=omega_ee"," --CHANNEL=eta_gammamumu_dalitz",
		" --CHANNEL=eta_gammaee_dalitz"," --CHANNEL=eta_mumu",
		" --CHANNEL=etaprime_gammamumu_dalitz"," --CHANNEL=etaprime_gammaee_dalitz",
		" --CHANNEL=phi_gammamumu_dalitz"," --CHANNEL=phi_etaee_dalitz",
		" --CHANNEL=phi_mumu"," --CHANNEL=phi_ee",
		" --CHANNEL=pi0_gammaee_dalitz"," --CHANNEL=jpsi_mumu",
		" --CHANNEL=jpsi_ee"," --CHANNEL=rho_mumu",
		" --CHANNEL=psi_mumu"};
	Double_t branchingRatio[18] = {1.3e-4, 0, 9.0e-5, 0, 3.1e-4, 0, 5.8e-6, 
		1.08e-4, 0, 1.4e-5, 0, 2.87e-4, 0, 0, 5.96e-2, 0, 4.55e-5, 8.0e-3};
	Double_t dNdY[18] = {1.33e-1, 0, 1.33e-1, 0, 1.7e-1, 0, 1.7e-1, 
		4.07e-2, 0, 1.73e-2, 0, 1.73e-2, 0, 0, 2.44e-5, 0, 2.22e-1, 2.44e-5};
	Int_t colorWheel[18]={5,41,46,42,2,6,29, 11,
		7,4,9,7,3,8,28,38,30,33};
	TH1D* hsum = new TH1D("hsum", "",1000,0,5);
	TString MC = "Cocktail_";
	TString r = "_{mod}.root";
	TString N = " --N=100000";
	//Important note: If you change N, you must also change the the "entries" scaling quantity if you look at AccCut

	
	//Momentum smearing
	std::cout << "Momentum Smearing? (y/n)" << endl;
	TString p; std::cin >> p;
	if (p == "n") {
		momSmear = " --SMEAR=false";
		TString smearName; smearName = "nosmear";
		std::cout << endl << "Momentum smearing is off" << endl;
	} else if (p == "y") {
		std::cout << endl << "Momentum smearing is on" << endl;
		TString smearName; smearName = "smear";
	} else {
		std::cout << endl << "Error: Didn't select y or n for momentum smearing" << endl;
		return;
	}
	
	//Choice of cut
	std::cout << "Which cut to use? (AccCut, FullAcc, or PairCut)" << endl;
	TString cut; std::cin >> cut;	
	if (cut == "AccCut") {
		std::cout << endl << "AccCut will be used" << endl;
	} else if (cut == "FullAcc") { 
		std::cout << endl << "FullAcc will be used" << endl;
	} else if (cut == "PairCut") {
		std::cout << endl << "PairCut will be used" << endl;
	} else {
		std::cout << endl << "Error: Did not enter FullAcc, PairCut, or AccCut" << endl;
		return;
	}
	
	//Cocktail generator and composer
	TCanvas* c1 = new TCanvas("c1","c1",1200,800);
	hsum->SetLineColor(kBlack);
    hsum->SetAxisRange(0,5,"X");
    hsum->Draw();
    c1->SetLogy(1);
	for (int i = 0; i < 18; ++i) {
		TString channel = decayChannel[i];
		Double_t scale = 1;
		TString command; command += decayMaker; command += channel; command += momSmear; command += N;
		TString fileName; fileName += MC; fileName += channel.Remove(0,11); fileName += r; 
		Int_t color; color = colorWheel[i];
		gSystem->Exec(command);
		TFile* f1 = new TFile(fileName);
		if (false == f1->IsOpen()) continue;

		TH1D* h1 = new TH1D("h1","",1000,0,5);
		if (cut == "FullAcc") {
			TH1D *h1 = (TH1D*)f1->Get("FullAcc_dNdM");
			cout <<"h1=" << h1 << endl;
			h1->SetDirectory(0);
			scale = 1 / 100000;
			h1->Scale(scale, "width");
			h1->SetLineColor(color);
			h1->Draw("same HIST");
			hsum->Add(h1,1);
		}
		if (cut == "PairCut") {
			TH1D *h1 = (TH1D*)f1->Get("PairCut_dNdM");
			cout <<"h1=" << h1 << endl;
            h1->SetDirectory(0);
			scale = branchingRatio[i] * dNdY[i] * 2 / 100000;
			h1->Scale(scale, "width");
			h1->SetLineColor(color);
			h1->Draw("same HIST");
			hsum->Add(h1,1);
		}
		if (cut == "AccCut") {
			TH1D *h1 = (TH1D*)f1->Get("AccCut_dNdM");
			cout <<"h1=" << h1 << endl;
            h1->SetDirectory(0);
			Double_t entries = h1->GetEntries();
			scale = branchingRatio[i] * dNdY[i] * 2 * (entries/100000) / 100000;
			h1->Scale(scale, "width");
			h1->SetLineColor(color);
			h1->Draw("same HIST");
			hsum->Add(h1,1);
		}
		f1->Close();
	}
	hsum->SetAxisRange(1e-8,1e-1,"Y");
	//c1->BuildLegend();
	TString Name; Name = "Muon_Cocktail_" + cut + "_" + smearName + ".pdf";
	c1->Print(Name,"pdf");
}
