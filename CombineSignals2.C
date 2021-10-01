#include <iostream>
#include <TH1.h>
#include <TFile.h>
#include <sstream>

int CombineSignals2( int firstCent,
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

    for (int iCent = firstCent; iCent < lastCent; ++iCent)
    {
        for (int iBBC = firstBBC; iBBC < lastBBC; ++iBBC)
        {
            std::stringstream dir;
            dir.str("");
            dir << "c0" << iCent << "_z0" << iBBC << "_r00";

            std::cout << dir.str() << std::endl;

            if (iCent == firstCent && iBBC == firstBBC)
            {
                for (auto &histname : histosNames)
                {
                    std::cout << histname.c_str() << std::endl;
                    TH1D *hist = infile->Get<TDirectory>(dir.str().c_str())->Get<TH1D>(histname.c_str());
                    std::cout << hist->GetName() << std::endl;
                    histos.push_back(hist);
                }

                for (auto hist : histos)
                {
                    std::cout << (*hist->GetName()) << std::endl;
                }        
            } 

            else 
            {
               for (auto &firstHist : histos) 
               {    
                    TH1D *hist = infile->Get<TDirectory>(dir.str().c_str())->Get<TH1D>(firstHist->GetName());
                    firstHist->Add(hist);
               }
            }
        }
    }

    std::stringstream outdir;
    outdir << "c" << firstCent << "-" << lastCent << "_z" << firstBBC << "-" << lastBBC << "_r00";
    std::cout << outdir.str() << std::endl;
    
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

    return 1;
}
