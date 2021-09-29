int CombineCabanaBoy( int firstCent, 
                      int lastCent, 
                      int firstBBC, 
                      int lastBBC) 
{

    TFile *infile  = new TFile("cabanaboy-runs/cabanaboy-merged.root", "read");
    TFile *outfile = new TFile("temp/CombinedCabanaBoy.root", "update");

    std::vector<std::string> histosNames;
    std::vector<TH2D*>       histos;
    
    histosNames.push_back("MassPtPID1_BG12");
    histosNames.push_back("MassPtPID2_BG12");
    histosNames.push_back("MassPtPID3_BG12");

    histosNames.push_back("MassPtPID1_FG12");
    histosNames.push_back("MassPtPID2_FG12");
    histosNames.push_back("MassPtPID3_FG12");

    histosNames.push_back("MassPtPID1_cut_BG12");
    histosNames.push_back("MassPtPID2_cut_BG12");
    histosNames.push_back("MassPtPID3_cut_BG12");

    histosNames.push_back("MassPtPID1_cut_FG12");
    histosNames.push_back("MassPtPID2_cut_FG12");
    histosNames.push_back("MassPtPID3_cut_FG12");

    std::string fullHistName;

    char name[50];

    for (int iCent = firstCent; iCent < lastCent; iCent++) {
        for (int iBBC = firstBBC; iBBC < lastBBC; iBBC++) {
            
            sprintf(name, "c0%d_z0%d_r00/", iCent, iBBC);

            if (iCent == firstCent && iBBC == firstBBC) {

                for (int i = 0; i < histosNames.size(); i++) {

                    fullHistName = name + histosNames[i];
                    histos.push_back( (TH2D*) infile->Get( fullHistName.c_str() ) );

                    if (!histos[i]) {
                        cerr << "!!! An error occured while opening " << fullHistName << " !!!" <<  endl;
                    }
                }

            } else {
                for (int i = 0; i < histosNames.size(); i++) {

                    fullHistName = name + histosNames[i];
                    histos[i]->Add(  (TH2D*) infile->Get( fullHistName.c_str() ), 1 );
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
