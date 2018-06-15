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
//List of contributions:
		//omega_pi0mumu_dalitz  omega_pi0ee_dalitz  omega_mumu  omega_ee
		//eta_gammamumu_dalitz  eta_gammaee_dalitz  eta_mumu
		//etaprime_gammamumu_dalitz  etaprime_gammaee_dalitz
		//phi_etaee_dalitz  phi_mumu  phi_ee  pi0_gammaee_dalitz
		//jpsi_mumu  jpsi_ee  rho_mumu  psi_mumu

void SimpleCocktailBuilder () {
	TString momSmear = "";
	TString decayMaker = "./hadronicCocktail.app DecayMaker.xml";
	TString decayChannel[17] = {" --CHANNEL=omega_pi0mumu_dalitz",
		" --CHANNEL=omega_pi0ee_dalitz"," --CHANNEL=omega_mumu",
		" --CHANNEL=omega_ee"," --CHANNEL=eta_gammamumu_dalitz",
		" --CHANNEL=eta_gammaee_dalitz"," --CHANNEL=eta_mumu",
		" --CHANNEL=etaprime_gammamumu_dalitz"," --CHANNEL=etaprime_gammaee_dalitz",
		" --CHANNEL=phi_etaee_dalitz"," --CHANNEL=phi_mumu",
		" --CHANNEL=phi_ee"," --CHANNEL=pi0_gammaee_dalitz",
		" --CHANNEL=jpsi_mumu"," --CHANNEL=jpsi_ee",
		" --CHANNEL=rho_mumu"," --CHANNEL=psi_mumu"};
	Int_t colorWheel[17]={5,41,46,42,2,6,29, 11,
		7,4,9,7,3,8,28,38,30};
	TH1D* hsum = new TH1D("hsum", "Sum",1000,0,5);
	TString MC = "MuonCocktail_";
	TString r = "{mod}.root";
	TString N = " --N=100000000";

	
	//Momentum smearing
	std::cout << "Momentum Smearing? (y/n)" << endl;
	TString p; std::cin >> p;
	if (p == "n") {
		momSmear = " --SMEAR=false";
		std::cout << endl << "Momentum smearing is off" << endl;
	} else if (p == "y") {
		std::cout << endl << "Momentum smearing is on" << endl;
	} else {
		std::cout << endl << "Error: Didn't select y or n for momentum smearing" << endl;
		return;
	}
	
	//Choice of cut
	std::cout << "Which cut to use? (AccCut, FullAcc, or RapCut)" << endl;
	TString cut; std::cin >> cut;	
	if (cut == "AccCut") {
		std::cout << endl << "AccCut will be used" << endl;
	} else if (cut == "FullAcc") { 
		std::cout << endl << "FullAcc will be used" << endl;
	} else if (cut == "RapCut") {
		std::cout << endl << "RapCut will be used" << endl;
	} else {
		std::cout << endl << "Error: Did not enter FullAcc, RapCut, or AccCut" << endl;
		return;
	}
	
	//Cocktail generator and composer
	TCanvas* c1 = new TCanvas("c1","c1",1200,800);
	hsum->SetLineColor(kBlack);
    hsum->SetAxisRange(0,5,"X");
    hsum->Draw();
    c1->SetLogy(1);
	for (int i = 0; i < 17; ++i) {
		TString channel = decayChannel[i];
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
			h1->SetLineColor(color);
			h1->Draw("same");
			hsum->Add(h1,1);
		}
		if (cut == "RapCut") {
			TH1D *h1 = (TH1D*)f1->Get("RapCut_dNdM");
			cout <<"h1=" << h1 << endl;
            h1->SetDirectory(0);
			h1->SetLineColor(color);
			h1->Draw("same");
			hsum->Add(h1,1);
		}
		if (cut == "AccCut") {
			TH1D *h1 = (TH1D*)f1->Get("AccCut_dNdM");
			cout <<"h1=" << h1 << endl;
            h1->SetDirectory(0);
			h1->SetLineColor(color);
			h1->Draw("same");
			hsum->Add(h1,1);
		}
		f1->Close();
	}
	c1->BuildLegend();
	c1->Print("h1.pdf)","pdf");

}
