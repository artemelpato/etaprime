#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <cstring>

void GetSignals2(const double pt_min,
                 const double pt_max,
                 const char *infile_name,
                 const char *outfile_name)
{
    const int n_cents    = 10;
    const int n_vertexes = 3;
    const int n_pids     = 3;
    const int n_cuts     = 2;

    const double substraction_scaling = 0.9;
    const double integral_min = 0.7;
    const double integral_max = 0.91;

    TFile *infile  = new TFile(infile_name, "read");
    TFile *outfile = new TFile(outfile_name,"recreate");

    for (int cent = 0; cent < n_cents; ++cent)
    {
        for (int vertex = 0; vertex < n_vertexes; ++vertex)
        {
            for (int pid = 0; pid < n_pids; ++pid)
            {
                for (int cut = 0; cut < n_cuts; ++cut)
                {
                    char dirname[50];
                    sprintf(dirname, "c0%d_z0%d_r00", cent, vertex);

                    char fg_hist_name[100];
                    char bg_hist_name[100];
                    char peak_hist_name[100];

                    if (cut == 0)
                    {
                        sprintf(fg_hist_name,   "MassPtPID%d_FG12", pid + 1);
                        sprintf(bg_hist_name,   "MassPtPID%d_BG12", pid + 1);
                        sprintf(peak_hist_name, "PeakHistPID%d",    pid + 1);
                    }
                    if (cut == 1)
                    {
                        sprintf(fg_hist_name,   "MassPtPID%d_cut_FG12", pid + 1);
                        sprintf(bg_hist_name,   "MassPtPID%d_cut_BG12", pid + 1);
                        sprintf(peak_hist_name, "PeakHistPID_cut%d",    pid + 1);
                    }

                    TH2D *fg_hist = (TH2D*) ( (TDirectory*) infile->Get(dirname) )->Get(fg_hist_name);
                    TH2D *bg_hist = (TH2D*) ( (TDirectory*) infile->Get(dirname) )->Get(bg_hist_name);

                    const int pt_min_bin = fg_hist->GetYaxis()->FindBin(pt_min);
                    const int pt_max_bin = fg_hist->GetYaxis()->FindBin(pt_max);

                    const int integral_min_bin = bg_hist->GetXaxis()->FindBin(integral_min);
                    const int integral_max_bin = bg_hist->GetXaxis()->FindBin(integral_max);

                    TH1D *total_peak_hist = fg_hist->ProjectionX(peak_hist_name);
                    total_peak_hist->Reset();

                    
                    for (int pt_bin = pt_min_bin; pt_bin < pt_max_bin; ++pt_bin)
                    {
                        char fg_hist_proj_name[100];
                        char bg_hist_proj_name[100];

                        sprintf(fg_hist_proj_name, "%s_pt%d", fg_hist_name, pt_bin);
                        sprintf(bg_hist_proj_name, "%s_pt%d", bg_hist_name, pt_bin);

                        TH1D *fg_hist_proj = fg_hist->ProjectionX(fg_hist_proj_name, pt_bin, pt_bin);
                        TH1D *bg_hist_proj = bg_hist->ProjectionX(fg_hist_proj_name, pt_bin, pt_bin);

                        const double fg_integral = fg_hist_proj->Integral(integral_min_bin, integral_max_bin);
                        const double bg_integral = bg_hist_proj->Integral(integral_min_bin, integral_max_bin);

                        const double substracion_constant = fg_integral / bg_integral * substraction_scaling;

                        if (fg_integral != 0 && bg_integral != 0)
                        {
                            bg_hist_proj->Scale(substracion_constant); 
                            fg_hist_proj->Add(bg_hist_proj, -1);
                        }
                        else
                        {
                            printf("Bad Scaling at %s in %s: substracion_constant = %10.5f\n", peak_hist_name, dirname, substracion_constant);
                            fg_hist_proj->Reset();
                        }

                        total_peak_hist->Add(fg_hist_proj);

                        //delete fg_hist_proj;
                        //delete bg_hist_proj;
                    }

                    delete fg_hist;
                    delete bg_hist;

                    if (!outfile->FindObjectAny(dirname))
                    {
                        outfile->mkdir(dirname);
                    }

                    outfile->cd(dirname);
                    total_peak_hist->Write();
                    outfile->cd();

                    delete total_peak_hist;

                }
            }
        }
    }

    infile->Close();
    outfile->Close();

    delete infile;
    delete outfile;

    printf("Macro GetSignals2 done processing!\n");
    return;
}
