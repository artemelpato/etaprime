#include <iostream>
#include <TH1.h>
#include <TH2.h>
#include <TFile.h>
#include <sstream>
#include <vector>
#include <string>

int IntegrateAfterCombining(const int first_cent,
               const int last_cent,
               const int first_bbc,
               const int last_bbc,
               const double pt_min,
               const double pt_max,
               const char *infile_name,
               const char *outfile_name)
{
    const double scaling_factor = 0.96;
    const double integral_min = 0.98;
    const double integral_max = 1.5;

    TFile *infile  = new TFile(infile_name,  "read");
    TFile *outfile = new TFile(outfile_name, "recreate");

    char dir[20];
    sprintf(dir, "c%d-%d_z%d-%d_r00", first_cent, last_cent, first_bbc, last_bbc);
    printf("%s\n", dir);
    
    for (int pid = 0; pid < 3; ++pid)
    {
        for (int cut = 0; cut < 2; ++cut)
        {
            char fg_hist_name[30];
            char bg_hist_name[30];
            char peak_hist_name[30];

            if (cut == 1) 
            {
                sprintf(fg_hist_name, "MassPtPID%d_cut_FG12", pid+1);
                sprintf(bg_hist_name, "MassPtPID%d_cut_BG12", pid+1);
                sprintf(peak_hist_name, "PeakPID%d_cut", pid+1);
            }
            if (cut == 0) 
            {
                sprintf(fg_hist_name, "MassPtPID%d_FG12", pid+1);
                sprintf(bg_hist_name, "MassPtPID%d_BG12", pid+1);
                sprintf(peak_hist_name, "PeakPID%d", pid+1);
            }
            
            TH2D *fg_hist = (TH2D*) ( (TDirectory*) infile->Get(dir) )->Get(fg_hist_name);
            TH2D *bg_hist = (TH2D*) ( (TDirectory*) infile->Get(dir) )->Get(bg_hist_name);

            const int pt_min_bin = fg_hist->GetYaxis()->FindBin(pt_min);
            const int pt_max_bin = fg_hist->GetYaxis()->FindBin(pt_max);
            
            const int integral_min_bin = bg_hist->GetXaxis()->FindBin(integral_min);
            const int integral_max_bin = bg_hist->GetXaxis()->FindBin(integral_max);
            
            strcat(fg_hist_name, "_proj"); 
            strcat(bg_hist_name, "_proj"); 

            TH1D *fg_hist_proj = fg_hist->ProjectionX(fg_hist_name, pt_min_bin, pt_max_bin);
            TH1D *bg_hist_proj = bg_hist->ProjectionX(bg_hist_name, pt_min_bin, pt_max_bin);
            TH1D *peak_hist = (TH1D*) fg_hist_proj->Clone(peak_hist_name);

            delete fg_hist, bg_hist;

            const double fg_integral = fg_hist_proj->Integral(integral_min_bin, integral_max_bin);
            const double bg_integral = bg_hist_proj->Integral(integral_min_bin, integral_max_bin);

            const double substracion_constant = fg_integral / bg_integral * scaling_factor;

            if (fg_integral && bg_integral)
            {
                bg_hist_proj->Scale(substracion_constant); 
                peak_hist->Add(bg_hist_proj, -1);
            }
            else
            {
                printf("Bad Scaling at %s: substracion_constant = %10.5f\n", peak_hist_name, substracion_constant);
            }


            if (!outfile->FindObjectAny(dir))
            {
                outfile->mkdir(dir);
            }
            outfile->cd(dir);
            peak_hist->Write();

            delete fg_hist_proj, bg_hist_proj, peak_hist;
        }
    }

    outfile->Close();
    infile->Close();

    delete outfile, infile;

    return 0;
}
