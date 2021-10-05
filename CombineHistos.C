#include <iostream>
#include <TH1.h>
#include <TFile.h>

int CombineHistos(int firstCent,
                  int lastCent,
                  int firstBBC,
                  int lastBBC,
                  const char *infile_name, 
                  const char *outfile_name)
{
    TFile *infile  = new TFile(infile_name,  "read");
    TFile *outfile = new TFile(outfile_name, "update");

    const int N_HISTOS = 12;
    const char hist_names[N_HISTOS][30] = {"MassPtPID1_BG12",
                                           "MassPtPID2_BG12",
                                           "MassPtPID3_BG12",
               
                                           "MassPtPID1_FG12",
                                           "MassPtPID2_FG12",
                                           "MassPtPID3_FG12",

                                           "MassPtPID1_cut_BG12",
                                           "MassPtPID2_cut_BG12",
                                           "MassPtPID3_cut_BG12",
               
                                           "MassPtPID1_cut_FG12",
                                           "MassPtPID2_cut_FG12",
                                           "MassPtPID3_cut_FG12"};
    
    TH1D **hists = new TH1D*[N_HISTOS];

    char dir[20];
    sprintf(dir, "c0%d_z0%d_r00", firstCent, firstBBC);

    for (int i = 0; i < N_HISTOS; ++i)
    {
        hists[i] = (TH1D*) ( (TDirectory*)infile->Get(dir) )->Get(hist_names[i]);
    }

    for (int icent = firstCent; icent < lastCent; ++icent)
    {
        for (int ibbc = firstBBC; ibbc < lastBBC; ++ibbc)
        {
            if (icent == firstBBC && ibbc == firstBBC) continue;
            for (int ihist = 0; ihist < N_HISTOS; ++ihist)
            {
                TH1D *hist = (TH1D*) ( (TDirectory*)infile->Get(dir) )->Get(hist_names[ihist]);
                hists[ihist]->Add(hist);
            }
        } 
    } 

    char outdir[20];
    sprintf(outdir, "c%d-%d_z%d-%d_r00", firstCent, lastCent,  firstBBC, lastBBC);
    printf("%s\n", outdir);
    
    if (!outfile->FindObjectAny(dir))
    {
        outfile->mkdir(outdir);
    }
    outfile->cd(outdir);

    for (int ihist = 0; ihist < N_HISTOS; ++ihist)
    {
        hists[ihist]->Write();
        delete hists[ihist];
    }

    delete[] hists;

    outfile->cd();
    outfile->Close();


    return 0;
}
