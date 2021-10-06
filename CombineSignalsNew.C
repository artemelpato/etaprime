#include <iostream>
#include <TH1.h>
#include <TFile.h>

void CombineSignalsNew(const int firstCent,
                       const int lastCent,
                       const int firstBBC,
                       const int lastBBC,
                       const char *infile_name, 
                       const char *outfile_name)
{
    TFile *infile  = new TFile(infile_name,  "read");
    TFile *outfile = new TFile(outfile_name, "update");

    const int N_HISTOS = 6;
    const char hist_names[N_HISTOS][30] = {"PeakHistPID1",
                                           "PeakHistPID2",
                                           "PeakHistPID3",
               
                                           "PeakHistPID1_cut",
                                           "PeakHistPID2_cut",
                                           "PeakHistPID3_cut"};
               
    
    TH1D* hists[N_HISTOS];

    char dir[20];
    sprintf(dir, "c0%d_z0%d_r00", firstCent, firstBBC);

    for (int i = 0; i < N_HISTOS; ++i)
    {
        hists[i] = (TH1D*) ( (TDirectory*) infile->Get(dir) )->Get(hist_names[i]);
    }

    for (int icent = firstCent; icent <= lastCent; ++icent)
    {
        for (int ibbc = firstBBC; ibbc <= lastBBC; ++ibbc)
        {
            if (icent == firstBBC && ibbc == firstBBC) continue;

            sprintf(dir, "c0%d_z0%d_r00", icent, ibbc);
            printf("%s\n", dir);

            for (int ihist = 0; ihist < N_HISTOS; ++ihist)
            {
                TH1D *hist = (TH1D*) ( (TDirectory*)infile->Get(dir) )->Get(hist_names[ihist]);
                hists[ihist]->Add(hist);
            }
        } 
    } 

    char outdir[20];
    sprintf(outdir, "c%d-%d_z%d-%d_r00", firstCent, lastCent,  firstBBC, lastBBC);
    printf("%lu\n", strlen(outdir));
    
    if (!outfile->FindObjectAny(outdir))
    {
        outfile->mkdir(outdir);
    }
    outfile->cd(outdir);

    for (int ihist = 0; ihist < N_HISTOS; ++ihist)
    {
        hists[ihist]->Write();
    }

    outfile->cd();
    outfile->Close();

    printf("Done processing CombineSignals4\n");
    return;
}
