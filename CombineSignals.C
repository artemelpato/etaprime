#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <iostream>
#include <sstream>

int CombineSignals( int firstCent, 
                      int lastCent, 
                      int firstBBC, 
                      int lastBBC) 
{

    TFile *infile  = new TFile("temp/Signals.root", "read");
    TFile *outfile = new TFile("temp/CombinedSignals.root", "update");

    std::vector<std::string> histosNames;
    std::vector<TH1D*>       histos;
    
    histosNames.push_back("PeakHist_PID1");    
    histosNames.push_back("PeakHist_PID2");    
    histosNames.push_back("PeakHist_PID3");    
    histosNames.push_back("PeakHist_PID1_cut");    
    histosNames.push_back("PeakHist_PID2_cut");    
    histosNames.push_back("PeakHist_PID3_cut");    

    char name[50];

    for (int iCent = firstCent; iCent < lastCent; iCent++) 
    {
        for (int iBBC = firstBBC; iBBC < lastBBC; iBBC++) 
        {
            std::basic_stringstream<char> dir;
            dir << "c" << iCent << "_z" << iBBC << "_r00";

            if (iCent == firstCent && iBBC == firstBBC) 
            {
                for (int i = 0; i < histosNames.size(); i++) 
                {
                    if (infile->FindObjectAny(dir.str().c_str()))
                    {
                        TH1D *hist = infile->Get<TDirectory>(dir.str().c_str())->Get<TH1D>( histosNames[i].c_str() );
                        std::cout
                    }

                    if (!hist)
                    {
                        std::cout << "Error with hist " << histosNames[i] << std::endl;
                        continue;
                    }

                    histos.push_back(hist);
                }
            } 

            else 
            {
                for (int i = 0; i < histosNames.size(); i++) 
                {
                    TH1D *hist = infile->Get<TDirectory>(dir.str().c_str())->Get<TH1D>( histosNames[i].c_str() );
                    
                    if (!hist)
                    {
                        std::cout << "Not adding hist " << histosNames[i] << std::endl;
                        continue;
                    } 

                    histos[i]->Add(hist);
                }
            }
        }
    }    

    sprintf(name, "c%d-%d_z%d-%d_r00", firstCent, lastCent, firstBBC, lastBBC);

    if (!outfile->FindObjectAny(name)) 
    {
        outfile->mkdir(name);
    }
    outfile->cd(name);
                
    for (int i = 0; i < histosNames.size(); i++) 
    {
        histos[i]->Write();
    }

    infile->Close();
    outfile->Close();

    return 0;
}
