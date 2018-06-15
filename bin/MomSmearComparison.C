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

TString momSmear = "true";
TString decayMaker = "./hadronicCocktail.app DecayMaker.xml --CHANNEL=";
TString channel;
TString cut;
TString MC = "MuonCocktail_";
TString dashChannel = " --CHANNEL=";
TString dashMomSmear = " --SMEAR=";
TString r = "{mod}.root";
TString N = " --N=100000000";


//Choices 
std::cout << "Which decay channel?" << endl;
std::cout << "Options: omega_pi0mumu_dalitz  omega_pi0ee_dalitz  omega_mumu  omega_ee" << endl;
std::cout << "eta_gammamumu_dalitz  eta_gammaee_dalitz  eta_mumu" << endl;
std::cout << "etaprime_gammamumu_dalitz  etaprime_gammaee_dalitz" << endl;
std::cout << "phi_etaee_dalitz  phi_mumu  phi_ee  pi0_gammaee_dalitz"
std::cout << "jpsi_mumu  jpsi_ee  rho_mumu  psi_mumu"
	std::cin >> channel;

std::cout << "Which cut? (RapCut, AccCut, or FullAcc)" << endl;
	std::cin >> cut;
	
//Cocktail builder
TCanvas* c1 = new TCanvas("c1","c1",1200,800);
TString command; command = decayMaker + dashChannel + channel + dashMomSmear + momSmear + N;
TString fileName = ""; fileName += MC; fileName += channel; fileName += r; 
	gSystem->Exec(command);
	TFile* f1 = new TFile(fileName);
	if (false == f1->IsOpen()) continue;
	
	TH1D* h1 = new TH1D("h1","",1000,0,5);
	if (cut == "RapCut") {
		TH1D *h1 = (TH1D*)f1->Get("RapCut_dNdM");
		cout <<"h1=" << h1 << endl;
		h1->SetDirectory(0);
		h1->SetLineColor(kBlue);
		h1->Draw();
		}
	if (cut == "FullAcc") {
		TH1D *h1 = (TH1D*)f1->Get("FullAcc_dNdM");
		cout <<"h1=" << h1 << endl;
		h1->SetDirectory(0);
		h1->SetLineColor(kBlue);
		h1->Draw();	
	}
	if (cut == "AccCut") {
		TH1D *h1 = (TH1D*)f1->Get("AccCut_dNdM");
		cout <<"h1=" << h1 << endl;
		h1->SetDirectory(0);
		h1->SetLineColor(kBlue);
		h1->Draw();
	}
	f1->Close();

momSmear = "false";
	TString command; command = decayMaker + dashChannel + channel + dashMomSmear + momSmear + N;
	gSystem->Exec(command);
	TFile* f1 = new TFile(fileName);
	if (false == f1->IsOpen()) continue;
	
	TH1D* h1 = new TH1D("h1","",1000,0,5);
	if (cut == "RapCut") {
		TH1D *h1 = (TH1D*)f1->Get("RapCut_dNdM");
		cout <<"h1=" << h1 << endl;
		h1->SetDirectory(0);
		h1->SetLineColor(kRed);
		h1->Draw("same");
		}
	if (cut == "FullAcc") {
		TH1D *h1 = (TH1D*)f1->Get("FullAcc_dNdM");
		cout <<"h1=" << h1 << endl;
		h1->SetDirectory(0);
		h1->SetLineColor(kRed);
		h1->Draw("same");	
	}
	if (cut == "AccCut") {
		TH1D *h1 = (TH1D*)f1->Get("AccCut_dNdM");
		cout <<"h1=" << h1 << endl;
		h1->SetDirectory(0);
		h1->SetLineColor(kRed);
		h1->Draw("same");
	}
	f1->Close();
	c1->SetLogy(1);
	c1->BuildLegend();
	c1->Print("h1.pdf)","pdf");
}