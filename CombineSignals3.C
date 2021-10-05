#include <iostream>
#include <TH1.h>
#include <TFile.h>
#include <sstream>
#include <TPad.h>

int CombineSignals3( int firstCent,
                     int lastCent,
                     int firstBBC,
                     int lastBBC )
{
    TFile *infile  = new TFile("temp/Signals.root",         "read");
    TFile *outfile = new TFile("temp/CombinedSignals.root", "update");

    std::vector<std::string> histosNames;
    
    histosNames.push_back("PeakHist_PID1");    
    histosNames.push_back("PeakHist_PID2");    
    histosNames.push_back("PeakHist_PID3");    
    histosNames.push_back("PeakHist_PID1_cut");    
    histosNames.push_back("PeakHist_PID2_cut");    
    histosNames.push_back("PeakHist_PID3_cut");    

    std::vector<TH1D*> histos;

    std::stringstream dir;
    dir.str("");
    dir << "c0" << firstCent << "_z0" << firstBBC << "_r00";

    for (auto &histname : histosNames)
    {
        TH1D *hist = infile->Get<TDirectory>(dir.str().c_str())->Get<TH1D>(histname.c_str());
        histos.push_back(hist);
    }
    
    histos[5]->Draw();
    usleep(800);

    for (int iCent = firstCent; iCent < lastCent + 1; ++iCent)
    {
        for (int iBBC = firstBBC; iBBC < lastBBC + 1; ++iBBC)
        {
            if (iBBC == firstBBC && iCent == firstCent) continue;

            for (auto &firstHist : histos) 
            {    
               dir.str("");
               dir << "c0" << iCent << "_z0" << iBBC << "_r00";

               TH1D *hist = infile->Get<TDirectory>(dir.str().c_str())->Get<TH1D>(firstHist->GetName());
               firstHist->Add(hist);
               
           }

           histos[5]->Draw();
           gPad->Update();
           usleep(800);
        }
    }


    std::stringstream outdir;
    outdir << "c" << firstCent << "-" << lastCent << "_z" << firstBBC << "-" << lastBBC << "_r00";
    
    if (!outfile->FindObjectAny(outdir.str().c_str()))
    {
        outfile->mkdir(outdir.str().c_str());
    }

    outfile->cd(outdir.str().c_str());

    for (auto &hist : histos)
    {
        hist->Write();
    }

    outfile->cd();
    
    outfile->Close();

    return 0;
}
