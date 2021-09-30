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

    std::string fullHistName;

    char name[50];

    for (int iCent = firstCent; iCent < lastCent; iCent++) {
        for (int iBBC = firstBBC; iBBC < lastBBC; iBBC++) {
            
            sprintf(name, "c0%d_z0%d_r00/", iCent, iBBC);

            if (iCent == firstCent && iBBC == firstBBC) {

                for (int i = 0; i < histosNames.size(); i++) {

                    fullHistName = name + histosNames[i];
                    histos.push_back( (TH1D*) infile->Get( fullHistName.c_str() ) );

                    if (!histos[i]) {
                        cerr << "!!! An error occured while opening " << fullHistName << " !!!" <<  endl;
                    }
                }

            } else {
                for (int i = 0; i < histosNames.size(); i++) {

                    fullHistName = name + histosNames[i];
                    histos[i]->Add(  (TH1D*) infile->Get( fullHistName.c_str() ), 1 );
                }
            }
        }
    }    

    sprintf(name, "c%d-%d_z%d-%d_r00", firstCent, lastCent, firstBBC, lastBBC);

    outfile->mkdir(name);
    outfile->cd(name);
                
    for (int i = 0; i < histosNames.size(); i++) {
        histos[i]->Write();
    }

    infile->Close();
    outfile->Close();

    return 0;
}
