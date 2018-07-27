#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TString.h"
#include "TRegexp.h"
#include <stdlib.h>
#include <iostream>

//This program compares the cocktail contributions of a certain decay channel and cut with and without momentum smearing.
//Run this in ROOT in HadronicCocktail/bin

MomSmearComparison () {

	TString decayChannel[17] = {" --CHANNEL=omega_pi0mumu_dalitz",
		" --CHANNEL=omega_pi0ee_dalitz"," --CHANNEL=omega_mumu",
		" --CHANNEL=omega_ee"," --CHANNEL=eta_gammamumu_dalitz",
		" --CHANNEL=eta_gammaee_dalitz"," --CHANNEL=eta_mumu",
		" --CHANNEL=etaprime_gammamumu_dalitz"," --CHANNEL=etaprime_gammaee_dalitz",
		" --CHANNEL=phi_etaee_dalitz"," --CHANNEL=phi_mumu",
		" --CHANNEL=phi_ee"," --CHANNEL=pi0_gammaee_dalitz",
		" --CHANNEL=jpsi_mumu"," --CHANNEL=jpsi_ee",
		" --CHANNEL=rho_mumu"," --CHANNEL=psi_mumu"};
	Double_t branchingRatio[17] = {1.0, 7.7e-4, 1.0, 7.28e-5, 1.0, 7.0e-3, 1.0, 
		1.0, 4.7e-4, 1.15e-4, 1.0, 2.954e-4, 1.174e-2, 1.0, 5.94e-2, 1.0, 1.0};
	Double_t dNdY[17] = {1.0, 1.33e-1, 1.0, 1.33e-1, 1.0, 1.7e-1, 1.0, 
		1.0, 4.07e-2, 1.73e-2, 1.0, 1.73e-2, 1.28, 1.0, 2.44e-5, 1.0, 1.0};
	TString momSmear = "true";
	TString decayMaker = "./hadronicCocktail.app DecayMaker.xml --CHANNEL=";
	TString channel;
	TString cut;
	TString MC = "Cocktail_";
	TString dashChannel = " --CHANNEL=";
	TString dashMomSmear = " --SMEAR=";
	TString r = "_{mod}.root";
	TString N = " --N=100000";
	Int_t j = 0;


	//Choices 
	std::cout << endl << "Which decay channel?" << endl << endl;
	std::cout << "Options:" << endl;
	for (Int_t i = 0; i <17; i+=2) {
		TString channel1; TString channel2;
		channel1 = decayChannel[i]; channel2 = decayChannel[i+1];
		channel1 = channel1.Remove(0,11); channel2 = channel2.Remove(0,11); 
		if (i<16) {
			std::cout << channel1 << "      " << channel2 << endl;
		}
		else std::cout << channel1 << endl << endl;
	}
	std::cin >> channel;
	for (i = 0; i <17; i++) {
		channel1 = decayChannel[i];
		channel1 = channel1.Remove(0,11);
		if (channel != channel1) {
			j++;
		}
	}
	if (j == 17) {
		std::count << "You didn't pick a valid channel" << endl;
		return;
	}

	std::cout << "Which cut? (PairCut, AccCut, or FullAcc)" << endl;
	std::cin >> cut;
	
	//Cocktail builder
	TCanvas* c1 = new TCanvas("c1","c1",1200,800);
	TString command; command = decayMaker + dashChannel + channel + dashMomSmear + momSmear + N;
	TString fileName = ""; fileName += MC; fileName += channel; fileName += r; 
	gSystem->Exec(command);
	TFile* f1 = new TFile(fileName);
	if (false == f1->IsOpen()) continue;
	
	TH1D* h1 = new TH1D("h1","",1000,0,5);
	if (cut == "PairCut") {
		TH1D *h1 = (TH1D*)f1->Get("PairCut_dNdM");
	}
	else if (cut == "FullAcc") {
		TH1D *h1 = (TH1D*)f1->Get("FullAcc_dNdM");
	}
	else if (cut == "AccCut") {
		TH1D *h1 = (TH1D*)f1->Get("AccCut_dNdM");
	}
	else {
		cout << "You didn't pick a valid cut" << endl;
		return;
	}
	cout << "h1=" << h1 << endl;
	h1->SetDirectory(0);
	h1->SetLineColor(kBlue);
	h1->Draw();
	f1->Close();

	momSmear = "false";
	TString command; command = decayMaker + dashChannel + channel + dashMomSmear + momSmear + N;
	gSystem->Exec(command);
	TFile* f1 = new TFile(fileName);
	if (false == f1->IsOpen()) continue;
	
	TH1D* h1 = new TH1D("h1","",1000,0,5);
	if (cut == "PairCut") {
		TH1D *h1 = (TH1D*)f1->Get("PairCut_dNdM");
		}
	else if (cut == "FullAcc") {
		TH1D *h1 = (TH1D*)f1->Get("FullAcc_dNdM");
	}
	else if (cut == "AccCut") {
		TH1D *h1 = (TH1D*)f1->Get("AccCut_dNdM");
	}
	cout <<"h1=" << h1 << endl;
	h1->SetDirectory(0);
	h1->SetLineColor(kRed);
	h1->Draw("same");
	f1->Close();
	c1->SetLogy(1);
	//c1->BuildLegend();
	TString Name; Name = "MomSmearComp_" + channel + "_" + cut + ".pdf";
	c1->Print(Name,"pdf");
}