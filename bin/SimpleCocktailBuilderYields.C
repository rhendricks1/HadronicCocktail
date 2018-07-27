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


void SimpleCocktailBuilderYields () {
	TString momSmear = "";
	TString decayMaker = "./hadronicCocktail.app DecayMaker.xml";
	TString decayChannel[8] = {" --CHANNEL=omega_pi0ee_dalitz",
		" --CHANNEL=omega_ee"," --CHANNEL=eta_gammaee_dalitz",
		" --CHANNEL=etaprime_gammaee_dalitz"," --CHANNEL=phi_etaee_dalitz",
		" --CHANNEL=phi_ee"," --CHANNEL=pi0_gammaee_dalitz",
		" --CHANNEL=jpsi_ee"};
	Double_t branchingRatio[8] = {7.7e-4, 7.28e-5, 7.0e-3, 
		4.7e-4, 1.15e-4, 2.954e-4, 1.174e-2, 5.94e-2};
	/*the dNdY array should be in this format: {omega yield, omega yield, eta yield, etaprime yield,
	phi yield, phi yield, pi0 yield, jpsi yield}	*/
	Double_t dNdY[8] = {2.645, 2.645, 3.544, 0.2193,
	0.6969, 0.6969, 41.28, 7.760e-5};
	Int_t colorWheel[8]={5,41,46,42,2,6,29,11};
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
	for (int i = 0; i < 8; ++i) {
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
			h1->Scale(scale,"width");
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
	TString Name; Name = "Electron_Cocktail_" + cut + "_" + smearName + ".pdf";
	c1->Print(Name,"pdf");
}
