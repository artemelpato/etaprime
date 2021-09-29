int GetEtaPrime()
{
    const double minPt = 0;
    const double maxPt = 20;
    const double integralMin = 1.3;
    const double integralMax = 1.5;
    const double rangeMin = 0.8;
    const double rangeMax = 1.2;

    TFile *infile  = new TFile("temp/CombinedCabanaBoy.root",    "read");
    TFile *outfile = new TFile("temp/EtaPrimeOut.root", "recreate");

    std::stringstream histName;
    //std::array<std::string, 4> dirs = { "c0-1_z0-2_r00",
                                        //"c2-3_z0-2_r00",
                                        //"c4-5_z0-2_r00",
                                        //"c6-9_z0-2_r00" };
    
   std::array<std::string, 1> dirs = {"c6-8_z0-2_r00" };
    
    for (auto &dir : dirs)
    {
        for (int pid = 0; pid < 3; pid++) 
        {
            histName << "MassPtPID" << pid + 1 << "_FG12";
            TH2D *massPtFG = infile->Get<TDirectory>(dir.c_str())->Get<TH2D>(histName.str().c_str());
            histName.str("");
            
            histName << "MassPtPID" << pid + 1 << "_BG12";
            TH2D *massPtBG = infile->Get<TDirectory>(dir.c_str())->Get<TH2D>(histName.str().c_str());
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
            
            //fgProj->GetXaxis()->SetRangeUser(rangeMin, rangeMax);
            //bgProj->GetXaxis()->SetRangeUser(rangeMin, rangeMax);

            const double fgIntegral = fgProj->Integral(integralMinBin, integralMaxBin);
            const double bgIntegral = bgProj->Integral(integralMinBin, integralMaxBin);
            
            const double scaling = fgIntegral / bgIntegral;

            std::cout << std::setw(10) << fgIntegral 
                      << std::setw(10) << bgIntegral 
                      << std::setw(10) << scaling << std::endl;

            bgProj->Scale(scaling);
            
            histName << "PeakHist_PID" << pid + 1;
            TH1D *peakHist = (TH1D*)fgProj->Clone(histName.str().c_str());
            histName.str("");
            peakHist->Add(bgProj, -1);
            
            outfile->mkdir(dir.c_str());
            outfile->cd(dir.c_str());
            
            fgProj->Write(); 
            bgProj->Write(); 
            peakHist->Write(); 
            
            outfile->cd();
                    
        }
    }

    infile->Close();
    outfile->Close();

    return 0;
}
