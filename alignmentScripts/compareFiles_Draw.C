#include <TStyle.h>
#include <TSystem.h>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "TTree.h"
#include "TString.h"
#include "TAxis.h"
#include "TProfile.h"
#include "TH2F.h"
#include "TROOT.h"
#include "TDirectory.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TDirectoryFile.h"
#include "TLegend.h"
#include "THStack.h"
#include <exception>
#include "TKey.h"
#include "TPad.h"
#include "TPaveText.h"
#include "TPaveStats.h"
#include "TF1.h"


TString _name[5];
TString _savePath;
TString _treeName;
TFile *_file[5];
int nFiles=0;
TDirectory *_dir[5];
TH1 *h1[5]; TH2 *h2[5];
TProfile *p1[5];
TTree *_tree[5];
int _color[5];
int _style[5];
char temp[50];

void addFile(const char* filePath, const char* name, int color, int style);
void setSavePath(const char* path);
void setDir(const char* dir);
void setTreeName(const char* treeName);
void drawHist(const char* histName="", const char* fileName="noname", const char* title = "", bool norm=0, bool logscale=0);
void drawProf(const char* histName="", const char* fileName="noname", const char* title = "", bool logscale=0);
void drawTree(const char* variable="", const char* cutString = "", const char* title = "", Int_t nbins=30, Double_t xlow=0, Double_t xhigh=100, const char* fileName = "noname", bool norm=0, bool logscale=0);
void setDir(const char* dir);
void normalize(TH1 *hist);
void finish();


void compareFiles_Draw() {

}

void setSavePath(const char* path){
  _savePath=TString(path);
}

void setTreeName(const char* treeName){
  _treeName = TString(treeName);
}

void drawHist(const char* histName, const char* fileName, const char* title, bool norm, bool logscale){
  gStyle->SetOptStat(0);
  TCanvas c1("c1","",800,600);
  if(logscale) gPad->SetLogy(1);
  float min=1000.;
  float max=-1000.;
  float x1,x2,y1,y2;
  x1=0.55; x2=0.90; y2=0.90; y1=y2-nFiles*0.06;
  TLegend *leg=new TLegend(x1,y1,x2,y2,"");
  leg->SetTextFont(52);
  leg->SetTextSize(0.042);
  leg->SetFillStyle(1001);
  leg->SetFillColor(19);
  leg->SetBorderSize(1);
  for(int k=0;k<nFiles;k++){
    h1[k]=(TH1*)_dir[k]->Get(histName);
    if(!h1[k]) {printf("\n\t***ERROR***: No histogram found: %s/%s\n\n",_dir[k]->GetPath(),histName); return;}
    if(norm) normalize(h1[k]);
    if(k==0) h1[k]->Draw(); else h1[k]->Draw("same");
    if(h1[k]->GetMinimum()<min) min=h1[k]->GetMinimum();
    if(h1[k]->GetMaximum()>max) max=h1[k]->GetMaximum();
    h1[k]->SetLineColor(_color[k]);
    h1[k]->SetLineStyle(_style[k]);
    h1[k]->SetTitle(title);
    sprintf(temp,"%s E: %d M: %.3e",_name[k].Data(),(int)h1[k]->GetEntries(),h1[k]->GetMean());

    leg->AddEntry(h1[k],TString(temp),"l");
  }
  h1[0]->GetYaxis()->SetRangeUser(min,max+TMath::Abs(max-min)*0.15);
  h1[0]->GetYaxis()->SetTitleOffset(1.3);
  leg->Draw();
  c1.SaveAs(_savePath+TString(fileName)+".eps");
  for(int k=0; k<nFiles;k++) delete h1[k];
}

void drawProf(const char* histName, const char* fileName, const char* title, bool logscale){
  gStyle->SetOptStat(0);
  TCanvas c1("c1","",800,600);
  if(logscale) gPad->SetLogy(1);
  float min=1000.;
  float max=-1000.;
  float x1,x2,y1,y2;
  x1=0.55; x2=0.90; y2=0.90; y1=y2-nFiles*0.04;
  TLegend *leg=new TLegend(x1,y1,x2,y2,"");
  leg->SetTextFont(52);
  leg->SetTextSize(0.028);
  leg->SetFillStyle(1001);
  leg->SetFillColor(19);
  leg->SetBorderSize(1);
  for(int k=0;k<nFiles;k++){
    p1[k]=(TProfile*)_dir[k]->Get(histName);
    if(!p1[k]) {printf("\n\t***ERROR***: No profile found: %s/%s\n\n",_dir[k]->GetPath(),histName); return;}
    if(k==0) p1[k]->Draw(); else p1[k]->Draw("same");
    if(p1[k]->GetYmax()<min) min=p1[k]->GetYmax();
    if(p1[k]->GetYmin()>max) max=p1[k]->GetYmin();
    p1[k]->SetLineColor(_color[k]);
    p1[k]->SetLineStyle(_style[k]);
    p1[k]->SetTitle(title);
    sprintf(temp,"%s E: %d M: %.3e",_name[k].Data(),(int)p1[k]->GetEntries(),p1[k]->GetMean());

    leg->AddEntry(p1[k],TString(temp),"l");
  }
  p1[0]->GetYaxis()->SetRangeUser(min,max+TMath::Abs(max-min)*0.1);
  p1[0]->GetYaxis()->SetTitleOffset(1.3);
  leg->Draw();
  c1.SaveAs(_savePath+TString(fileName)+".eps");
  for(int k=0; k<nFiles;k++) delete p1[k];
}
void drawTree(const char* variable, const char* cutString, const char* title, Int_t nbins, Double_t xlow, Double_t xhigh, const char* fileName, bool norm, bool logscale){
  gStyle->SetOptStat(0);
  TCanvas c1("c1","",800,600);
  if(logscale) gPad->SetLogy(1);
  float min=1000.;
  float max=-1000.;
  float x1,x2,y1,y2;
  x1=0.42; x2=0.90; y2=0.90; y1=y2-nFiles*0.06;
  TLegend *leg=new TLegend(x1,y1,x2,y2,"");
  leg->SetTextFont(52);
  leg->SetTextSize(0.042);
  leg->SetFillStyle(3003);
  leg->SetFillColor(19);
  leg->SetBorderSize(1);
  for(int k=0;k<nFiles;k++){
    TString ttl(title);
    if(ttl.Length()<1) printf("\nERROR: NO title for tree histogram\n\n");
    sprintf(temp,"h%d",k);
    h1[k]=new TH1F(temp,ttl,nbins,xlow,xhigh);
    _tree[k]=(TTree*)_dir[k]->Get(_treeName);
    sprintf(temp,"%s>>h%d",variable,k);
    if(k==0) _tree[k]->Draw(temp,cutString); else _tree[k]->Draw(temp,cutString,"same");
    if(norm) normalize(h1[k]);
    if(k==0) h1[k]->Draw(); else h1[k]->Draw("same");
    if(h1[k]->GetMinimum()<min) min=h1[k]->GetMinimum();
    if(h1[k]->GetMaximum()>max) max=h1[k]->GetMaximum();
    h1[k]->SetLineColor(_color[k]);
    h1[k]->SetLineStyle(_style[k]);
    h1[k]->SetTitle(title);
    sprintf(temp,"%s E: %d M: %.3e",_name[k].Data(),(int)h1[k]->GetEntries(),h1[k]->GetMean());

    leg->AddEntry(h1[k],TString(temp),"l");
  }
  h1[0]->GetYaxis()->SetRangeUser(min,max+TMath::Abs(max-min)*0.1);
  h1[0]->GetYaxis()->SetTitleOffset(1.3);
  leg->Draw();
  c1.SaveAs(_savePath+TString(fileName)+".eps");
  for(int k=0; k<nFiles;k++) delete h1[k];
}


void addFile(const char* filePath, const char* name, int color, int style){
  _file[nFiles]=new TFile(filePath);
  _name[nFiles]=TString(name);
  _color[nFiles]=color;
  _style[nFiles]=style;
  nFiles++;
}

void setDir(const char* dir){
  for(int k=0;k<nFiles;k++){
    _dir[k]=_file[k]->GetDirectory(TString(_file[k]->GetName())+":"+dir);
  }
}

void finish(){
  for(int k=0;k<nFiles;k++) _file[k]->Close();
}

void normalize(TH1 *hist){
  hist->Scale(1.0/hist->Integral());
}
