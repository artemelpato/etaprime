#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>
#include <vector>
#include <string>

int GetSignals(const double minPt,
               const double maxPt)
{

    const int nCents    = 10;
    const int nVertexes = 3;
    const double scalingFactor = 0.96;
    const double integralMin = 0.98;
    const double integralMax = 1.1;

    TFile *infile  = new TFile("cabanaboy-runs3/cabanaboy-merged.root",    "read");
    TFile *outfile = new TFile("temp/Signals.root", "recreate");
    
    for (int iCent = 0; iCent < nCents; iCent++)
    {
        for (int iVertex = 0; iVertex < nVertexes; iVertex++)
        {
            for (int pid = 0; pid < 3; pid++) 
            {
                std::stringstream dir;
                std::stringstream histName;

                dir.str("");     
                dir << "c0" << iCent << "_z0" << iVertex << "_r00";
                
                histName << "MassPtPID" << pid + 1 << "_FG12";
                TH2D *massPtFG = infile->Get<TDirectory>(dir.str().c_str())->Get<TH2D>(histName.str().c_str());
                histName.str("");
                
                histName << "MassPtPID" << pid + 1 << "_BG12";
                TH2D *massPtBG = infile->Get<TDirectory>(dir.str().c_str())->Get<TH2D>(histName.str().c_str());
                histName.str("");

                histName << "MassPtPID" << pid + 1 << "_cut_FG12";
                TH2D *massPtFG_cut = infile->Get<TDirectory>(dir.str().c_str())->Get<TH2D>(histName.str().c_str());
                histName.str("");
                
                histName << "MassPtPID" << pid + 1 << "_cut_BG12";
                TH2D *massPtBG_cut = infile->Get<TDirectory>(dir.str().c_str())->Get<TH2D>(histName.str().c_str());
                histName.str("");

                const double minPtBin = massPtFG->GetYaxis()->FindBin(minPt);
                const double maxPtBin = massPtFG->GetYaxis()->FindBin(maxPt);

                const double integralMinBin = massPtFG->GetXaxis()->FindBin(integralMin);
                const double integralMaxBin = massPtFG->GetXaxis()->FindBin(integralMax);
                
                histName << "Proj_PID" << pid + 1 << "_FG_";
                TH1D *fgProj = massPtFG->ProjectionX(histName.str().c_str(), minPtBin, maxPtBin);
                histName.str("");

                histName << "Proj_PID" << pid + 1 << "_BG_";
                TH1D *bgProj = massPtBG->ProjectionX(histName.str().c_str(), minPtBin, maxPtBin);
                histName.str("");

                histName << "Proj_PID" << pid + 1 << "_FG_cut";
                TH1D *fgProj_cut = massPtFG_cut->ProjectionX(histName.str().c_str(), minPtBin, maxPtBin);
                histName.str("");

                histName << "Proj_PID" << pid + 1 << "_BG_cut";
                TH1D *bgProj_cut = massPtBG_cut->ProjectionX(histName.str().c_str(), minPtBin, maxPtBin);
                histName.str("");
                
                const double fgIntegral = fgProj->Integral(integralMinBin, integralMaxBin);
                const double bgIntegral = bgProj->Integral(integralMinBin, integralMaxBin);
                const double scaling = fgIntegral / bgIntegral * scalingFactor;

                const double fgIntegral_cut = fgProj_cut->Integral(integralMinBin, integralMaxBin);
                const double bgIntegral_cut = bgProj_cut->Integral(integralMinBin, integralMaxBin);
                const double scaling_cut = fgIntegral_cut / bgIntegral_cut * scalingFactor;

                histName << "PeakHist_PID" << pid + 1;
                TH1D *peakHist     = (TH1D*)fgProj->Clone(histName.str().c_str());
                histName.str("");

                histName << "PeakHist_PID" << pid + 1 << "_cut";
                TH1D *peakHist_cut = (TH1D*)fgProj_cut->Clone(histName.str().c_str());
                histName.str("");

                if (fgIntegral && bgIntegral) 
                {
                    bgProj->Scale(scaling);
                    peakHist->Add(bgProj, -1);
                }
                else 
                {
                    std::cout << "Bad scaling at PID" << pid + 1 << " -- " << scaling <<  "   at dir " << dir.str().c_str() << std::endl;
                    peakHist->Reset();
                }
                

                if (fgIntegral_cut && bgIntegral_cut) 
                {
                    bgProj_cut->Scale(scaling_cut);
                    peakHist_cut->Add(bgProj_cut, -1);
                }
                else 
                {
                    std::cout << "Bad scaling at PID" << pid + 1 << "_cut -- " << scaling_cut << "   at dir " << dir.str().c_str() << std::endl;
                    peakHist_cut->Reset();
                }


                if (!outfile->FindObjectAny(dir.str().c_str()))
                {
                    outfile->mkdir(dir.str().c_str());
                }

                outfile->cd(dir.str().c_str());

                
                peakHist->Write(); 
                peakHist_cut->Write(); 
                
                outfile->cd();
                    
            }
        }
    }

    infile->Close();
    outfile->Close();

    return 0;
}
