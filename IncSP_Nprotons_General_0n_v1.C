// ======================================================================
// Struct to hold chi2 results
// ======================================================================
struct Chi2Result {
    double chi2;
    int ndof;
    double pvalue;
};


// ======================================================================
// ComputeChi2 : Calculate chi2, ndof, and p-value between two TH1s
//
// h1, h2   = histograms
// useErrors = include uncertainties in denominator?
// binMin, binMax = range to use (1-based). If -1, uses full range.
//
// Returns Chi2Result {chi2, ndof, pvalue}.
// ======================================================================
Chi2Result ComputeChi2(TH1* h1,
                       TH1* h2,
                       bool useErrors = true,
                       int binMin = -1,
                       int binMax = -1)
{
    Chi2Result result{0.0, 0, 0.0};

    if (!h1 || !h2) {
        std::cerr << "ComputeChi2: Null histogram pointer!\n";
        return result;
    }

    int nb1 = h1->GetNbinsX();
    int nb2 = h2->GetNbinsX();

    if (nb1 != nb2) {
        std::cerr << "ComputeChi2: Bin mismatch!\n";
        return result;
    }

    // Use full range if unspecified
    if (binMin < 1) binMin = 1;
    if (binMax < 1 || binMax > nb1) binMax = nb1;
    if (binMin > binMax) {
        std::cerr << "ComputeChi2: Invalid bin range\n";
        return result;
    }

    double chi2 = 0.0;
    int ndof = 0;

    for (int i = binMin; i <= binMax; i++) {

        double d1 = h1->GetBinContent(i);
        double d2 = h2->GetBinContent(i);

        double e1 = h1->GetBinError(i);
        double e2 = h2->GetBinError(i);

        if (!useErrors) {
            if (d2 == 0) continue;
            chi2 += (d1 - d2)*(d1 - d2) / d2;
            ndof++;   // counts the bin
            continue;
        }

        double err2 = e1*e1 + e2*e2;

        if (err2 <= 0) continue; // skip unusable bins

        chi2 += (d1 - d2)*(d1 - d2) / err2;
        ndof++;
    }

    // Compute p-value
    double pval = 0.0;
    if (ndof > 0) pval = TMath::Prob(chi2, ndof);

    result.chi2 = chi2;
    result.ndof = ndof;
    result.pvalue = pval;

    return result;
}





// ======================================================================
// PrintChi2Smart : Draw chi2, ndof, chi2/ndof, and p-value on the plot
// using Chi2Result from ComputeChi2(...).
//
// h_data   = data histogram
// h_pred   = prediction histogram
// pad      = canvas/pad 
// legend   = optional (for auto-placement)
// useErrors = include statistical errors?
// binMin, binMax = limits for chi2 computation (default = full range)
// ======================================================================
void PrintChi2Smart(TH1* h_data,
                    TH1* h_pred,
                    TVirtualPad* pad,
                    TLegend* legend = nullptr,
                    bool useErrors = true,
                    int binMin = -1,
                    int binMax = -1)
{
    if (!h_data || !h_pred || !pad) return;

    // Compute chi2 using your new struct-returning helper
    Chi2Result R = ComputeChi2(h_data, h_pred, useErrors, binMin, binMax);

    double chi2  = R.chi2;
    int    ndof  = R.ndof;
    double pval  = R.pvalue;

    // Go to target pad
    pad->cd();

    // ----------------------------
    // Smart text placement
    // ----------------------------
    double x = 0.15, y = 0.88;

    if (legend) {
        double lx1 = legend->GetX1NDC();
        double ly1 = legend->GetY1NDC();
        double lx2 = legend->GetX2NDC();
        double ly2 = legend->GetY2NDC();

        // Legend in top-right → print top-left
        if (lx1 > 0.5 && ly1 > 0.5) { x = 0.15; y = 0.88; }
        // Legend in top-left → print top-right
        else if (lx2 < 0.5 && ly1 > 0.5) { x = 0.70; y = 0.88; }
        // Legend bottom-right → print top-left
        else if (lx1 > 0.5 && ly2 < 0.5) { x = 0.15; y = 0.88; }
        // Legend bottom-left → print top-right
        else if (lx2 < 0.5 && ly2 < 0.5) { x = 0.70; y = 0.88; }
    }

    // Auto text size
    double textSize = 0.045;
    if (pad->GetWh() < 400) textSize = 0.035;

    // Draw Chi2 information
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(textSize);

    latex.DrawLatex(x,     y-0.40,      Form("#chi^{2} = %.3g", chi2));
    latex.DrawLatex(x,     y-0.46, Form("NDOF = %d", ndof));
    latex.DrawLatex(x,     y-0.52, Form("#chi^{2}/NDOF = %.3g", (ndof>0 ? chi2/ndof : 0.0)));
    latex.DrawLatex(x,     y-0.58, Form("p = %.3g", pval));
}







// ======================================================================
// Helper: Set symmetric Y-axis around 1 for ratio plots
// paddedFrac = fractional extra padding (e.g. 1.10 = +10%)
// ======================================================================

void SetRatioAxisSymmetric(TH1 *h_ratio, double paddedFrac = 1.10)
{
    if (!h_ratio) return;

    double maxDev = 0.0;

    // Find the maximum deviation from 1
    for (int i = 1; i <= h_ratio->GetNbinsX(); i++) {
        double v = h_ratio->GetBinContent(i);
        if (v <= 0) continue;
        double dev = fabs(v - 1.0);
        if (dev > maxDev) maxDev = dev;
    }

    // Safety to avoid collapsed axis
    if (maxDev < 0.01) maxDev = 0.01;

    // Add padding
    double padded = maxDev * paddedFrac;

    // Apply symmetric range around 1
    h_ratio->SetMinimum(1.0 - padded);
    h_ratio->SetMaximum(1.0 + padded);
}










void IncSP_Nprotons_General_0n_v1(
    TString tag,
    TString file_DataON,
    TString file_DataOFF,
    TString file_Nu,
    TString file_Dirt,
    TString file_pi0,
    TString file_Nue,
    bool doRatio = false
) {
    gStyle->SetOptStat(0);
    gStyle->SetTextFont(22);
    gStyle->SetTextSize(0.08);

    // ---------------- Load Files ----------------
    TFile *file1 = TFile::Open(file_DataON);
    TFile *file2 = TFile::Open(file_DataOFF);
    TFile *file3 = TFile::Open(file_Nu);
    TFile *file4 = TFile::Open(file_Dirt);
    TFile *file5 = TFile::Open(file_pi0);
    TFile *file6 = TFile::Open(file_Nue);

    if (!file1 || file1->IsZombie() || !file2 || file2->IsZombie() ||
        !file3 || file3->IsZombie() || !file4 || file4->IsZombie() ||
        !file5 || file5->IsZombie() || !file6 || file6->IsZombie()) {
        std::cerr << "Error: Could not open one or more input files." << std::endl;
        return;
    }

    // ---------------- Output Directory ----------------
    TString outDir = Form("Outputs_IncSP_Nprotons_0n_%s", tag.Data());
    if (gSystem->AccessPathName(outDir)) gSystem->mkdir(outDir, true);

	TString h_title_name;
	TString h_label_name; 
	double max_y; 

	if (tag == "SIGNAL_0n_TEST") {
    	h_title_name = "Run4b 1#gammaX";
    	h_label_name = "SIG";
	max_y = 70; 
	}
	else if (tag == "SIGNAL_BTB_0n_TEST") {
        h_title_name = "Run4b 1#gammaX - Enhanced 0p/Np split";
        h_label_name = "SIG_CRT_BTB";
        max_y = 70;
        }
	else if (tag == "SIGNAL_CRT_0n_TEST") {
        h_title_name = "Run4b 1#gammaX - w/CRT-veto";
        h_label_name = "SIG_CRT";
        max_y = 70;
        }
	else if (tag == "SIGNAL_CRT_BTB_0n_TEST") {
        h_title_name = "Run4b 1#gammaX - Enhanced 0p/Np split - w/CRT-veto";
        h_label_name = "SIG_CRT_BTB";
        max_y = 70;
        }
	else if (tag == "SIDEBAND_0n_TEST") {
    	h_title_name = "Run4b NC #pi^{0} sideband";
    	h_label_name = "NCPi0";
	max_y = 350;
	}
	else if (tag == "SIDEBAND_BTB_0n_TEST") {
        h_title_name = "Run4b NC #pi^{0} sideband - Enhanced 0p/Np split";
        h_label_name = "NCPi0";
        max_y = 350;
        }
        else if (tag == "SIDEBAND_CRT_0n_TEST") {
        h_title_name = "Run4b NC #pi^{0} sideband - w/CRT-veto";
        h_label_name = "NCPi0_CRT";
        max_y = 350;
        }
        else if (tag == "SIDEBAND_CRT_BTB_0n_TEST") {
        h_title_name = "Run4b NC #pi^{0} sideband - Enhanced 0p/Np split - w/CRT-veto";
        h_label_name = "NCPi0_CRT";
        max_y = 350;
        }
	else {std::cout<<"Invalid tag"<<std::endl;    
	      std::cout << "Press Enter to continue..." << std::endl;
              std::cin.get();}

    // -- Normalization ---

//Data
float POT_BNB_On = 0.50 * 1.332E+20 ; //#BNB-ON
float TRIGGERS_BeamOn  = 31582916.  ; //#BNB-ON

float TRIGGERS_BeamOff = 88445969. ; //#BNB-OFF

//MC
float POT_BNB_Nu = 7.88E+20 ;//#BNB nu

float POT_DIRT = 3.06E+20  ;//#Dirt

float POT_NORM5=1. ; //#pi0
float POT_NORM6=1. ; //#Nue



float norm_bnb_off = TRIGGERS_BeamOn / TRIGGERS_BeamOff ; 

float norm_bnb_Nu = POT_BNB_On / POT_BNB_Nu ;

float norm_dirt = POT_BNB_On / POT_DIRT ;

    // --- Retrieve histograms  ---
    // --- BNB-ON --- 

    TH1F *BNB_ON_h_1gX_Nprotons_0n = (TH1F*)file1->Get("h_1gX_Nprotons_0n"); BNB_ON_h_1gX_Nprotons_0n->Sumw2();
   // TH1F *BNB_ON_h_1gX_Nprotons_0n_blip_corr = (TH1F*)file1->Get("h_1gX_Nprotons_0n_blip_corr"); BNB_ON_h_1gX_Nprotons_0n_blip_corr->Sumw2(); 
    //Signal
    TH1F *BNB_ON_h_SPNCDeltaSig_Nprotons_0n = (TH1F*)file1->Get("h_SPNCDeltaSig_Nprotons_0n");   BNB_ON_h_SPNCDeltaSig_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPNCPi0Sig_Nprotons_0n = (TH1F*)file1->Get("h_SPNCPi0Sig_Nprotons_0n");       BNB_ON_h_SPNCPi0Sig_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPNCOtherSig_Nprotons_0n = (TH1F*)file1->Get("h_SPNCOtherSig_Nprotons_0n");   BNB_ON_h_SPNCOtherSig_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPNumuCCSig_Nprotons_0n = (TH1F*)file1->Get("h_SPNumuCCSig_Nprotons_0n");     BNB_ON_h_SPNumuCCSig_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPOutFVSig_Nprotons_0n = (TH1F*)file1->Get("h_SPOutFVSig_Nprotons_0n");       BNB_ON_h_SPOutFVSig_Nprotons_0n->Sumw2();
    //Bkg
    TH1F *BNB_ON_h_SPoutFVBkg_Nprotons_0n = (TH1F*)file1->Get("h_SPoutFVBkg_Nprotons_0n");       BNB_ON_h_SPoutFVBkg_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPnumuCCBkg_Nprotons_0n = (TH1F*)file1->Get("h_SPnumuCCBkg_Nprotons_0n");     BNB_ON_h_SPnumuCCBkg_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPnumuCCpi0Bkg_Nprotons_0n = (TH1F*)file1->Get("h_SPnumuCCpi0Bkg_Nprotons_0n");BNB_ON_h_SPnumuCCpi0Bkg_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPnueCCBkg_Nprotons_0n = (TH1F*)file1->Get("h_SPnueCCBkg_Nprotons_0n");       BNB_ON_h_SPnueCCBkg_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPNCBkg_Nprotons_0n = (TH1F*)file1->Get("h_SPNCBkg_Nprotons_0n");             BNB_ON_h_SPNCBkg_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_SPNCpi0Bkg_Nprotons_0n = (TH1F*)file1->Get("h_SPNCpi0Bkg_Nprotons_0n");       BNB_ON_h_SPNCpi0Bkg_Nprotons_0n->Sumw2();
    TH1F *BNB_ON_h_CosmicBkg_Nprotons_0n = (TH1F*)file1->Get("h_CosmicBkg_Nprotons_0n");         BNB_ON_h_CosmicBkg_Nprotons_0n->Sumw2();





    // ---  BNB-OFF --- 
    TH1F *BNB_OFF_h_1gX_Nprotons_0n = (TH1F*)file2->Get("h_1gX_Nprotons_0n");			BNB_OFF_h_1gX_Nprotons_0n->Sumw2(); 	 	BNB_OFF_h_1gX_Nprotons_0n->Scale(norm_bnb_off); 
//    TH1F *BNB_OFF_h_1gX_Nprotons_0n_blip_corr = (TH1F*)file2->Get("h_1gX_Nprotons_0n_blip_corr"); BNB_OFF_h_1gX_Nprotons_0n_blip_corr->Sumw2();  BNB_OFF_h_1gX_Nprotons_0n_blip_corr->Scale(norm_bnb_off);
    
    //Signal
    TH1F *BNB_OFF_h_SPNCDeltaSig_Nprotons_0n = (TH1F*)file2->Get("h_SPNCDeltaSig_Nprotons_0n");	BNB_OFF_h_SPNCDeltaSig_Nprotons_0n->Sumw2();	BNB_OFF_h_SPNCDeltaSig_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPNCPi0Sig_Nprotons_0n = (TH1F*)file2->Get("h_SPNCPi0Sig_Nprotons_0n");		BNB_OFF_h_SPNCPi0Sig_Nprotons_0n->Sumw2();		BNB_OFF_h_SPNCPi0Sig_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPNCOtherSig_Nprotons_0n = (TH1F*)file2->Get("h_SPNCOtherSig_Nprotons_0n");	BNB_OFF_h_SPNCOtherSig_Nprotons_0n->Sumw2();	BNB_OFF_h_SPNCOtherSig_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPNumuCCSig_Nprotons_0n = (TH1F*)file2->Get("h_SPNumuCCSig_Nprotons_0n");		BNB_OFF_h_SPNumuCCSig_Nprotons_0n->Sumw2();	BNB_OFF_h_SPNumuCCSig_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPOutFVSig_Nprotons_0n = (TH1F*)file2->Get("h_SPOutFVSig_Nprotons_0n");		BNB_OFF_h_SPOutFVSig_Nprotons_0n->Sumw2();		BNB_OFF_h_SPOutFVSig_Nprotons_0n->Scale(norm_bnb_off);
    //Bkg   
    TH1F *BNB_OFF_h_SPoutFVBkg_Nprotons_0n = (TH1F*)file2->Get("h_SPoutFVBkg_Nprotons_0n");		BNB_OFF_h_SPoutFVBkg_Nprotons_0n->Sumw2();		BNB_OFF_h_SPoutFVBkg_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPnumuCCBkg_Nprotons_0n = (TH1F*)file2->Get("h_SPnumuCCBkg_Nprotons_0n");		BNB_OFF_h_SPnumuCCBkg_Nprotons_0n->Sumw2();	BNB_OFF_h_SPnumuCCBkg_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPnumuCCpi0Bkg_Nprotons_0n = (TH1F*)file2->Get("h_SPnumuCCpi0Bkg_Nprotons_0n");	BNB_OFF_h_SPnumuCCpi0Bkg_Nprotons_0n->Sumw2();	BNB_OFF_h_SPnumuCCpi0Bkg_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPnueCCBkg_Nprotons_0n = (TH1F*)file2->Get("h_SPnueCCBkg_Nprotons_0n");           BNB_OFF_h_SPnueCCBkg_Nprotons_0n->Sumw2();         BNB_OFF_h_SPnueCCBkg_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPNCBkg_Nprotons_0n = (TH1F*)file2->Get("h_SPNCBkg_Nprotons_0n");			BNB_OFF_h_SPNCBkg_Nprotons_0n->Sumw2();		BNB_OFF_h_SPNCBkg_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_SPNCpi0Bkg_Nprotons_0n = (TH1F*)file2->Get("h_SPNCpi0Bkg_Nprotons_0n");		BNB_OFF_h_SPNCpi0Bkg_Nprotons_0n->Sumw2();		BNB_OFF_h_SPNCpi0Bkg_Nprotons_0n->Scale(norm_bnb_off);
    TH1F *BNB_OFF_h_CosmicBkg_Nprotons_0n = (TH1F*)file2->Get("h_CosmicBkg_Nprotons_0n");		BNB_OFF_h_CosmicBkg_Nprotons_0n->Sumw2();		BNB_OFF_h_CosmicBkg_Nprotons_0n->Scale(norm_bnb_off); //Bkg_Cosmic



    // --- BNB Nu Overlay ---

    TH1F *BNB_Nu_h_1gX_Nprotons_0n = (TH1F*)file3->Get("h_1gX_Nprotons_0n"); 		   BNB_Nu_h_1gX_Nprotons_0n->Sumw2();			BNB_Nu_h_1gX_Nprotons_0n->Scale(norm_bnb_Nu);
  //  TH1F *BNB_Nu_h_1gX_Nprotons_0n_blip_corr = (TH1F*)file3->Get("h_1gX_Nprotons_0n_blip_corr"); BNB_Nu_h_1gX_Nprotons_0n_blip_corr->Sumw2();   BNB_Nu_h_1gX_Nprotons_0n_blip_corr->Scale(norm_bnb_Nu);

    //Signal
    TH1F *BNB_Nu_h_SPNCDeltaSig_Nprotons_0n = (TH1F*)file3->Get("h_SPNCDeltaSig_Nprotons_0n");   BNB_Nu_h_SPNCDeltaSig_Nprotons_0n->Sumw2();		BNB_Nu_h_SPNCDeltaSig_Nprotons_0n->Scale(norm_bnb_Nu); //Sig_1g
    TH1F *BNB_Nu_h_SPNCPi0Sig_Nprotons_0n = (TH1F*)file3->Get("h_SPNCPi0Sig_Nprotons_0n");       BNB_Nu_h_SPNCPi0Sig_Nprotons_0n->Sumw2();		BNB_Nu_h_SPNCPi0Sig_Nprotons_0n->Scale(norm_bnb_Nu);   //Sig_1g
    TH1F *BNB_Nu_h_SPNCOtherSig_Nprotons_0n = (TH1F*)file3->Get("h_SPNCOtherSig_Nprotons_0n");   BNB_Nu_h_SPNCOtherSig_Nprotons_0n->Sumw2();		BNB_Nu_h_SPNCOtherSig_Nprotons_0n->Scale(norm_bnb_Nu); //Sig_1g
    TH1F *BNB_Nu_h_SPNumuCCSig_Nprotons_0n = (TH1F*)file3->Get("h_SPNumuCCSig_Nprotons_0n");     BNB_Nu_h_SPNumuCCSig_Nprotons_0n->Sumw2();		BNB_Nu_h_SPNumuCCSig_Nprotons_0n->Scale(norm_bnb_Nu);  //Sig_1g
    TH1F *BNB_Nu_h_SPOutFVSig_Nprotons_0n = (TH1F*)file3->Get("h_SPOutFVSig_Nprotons_0n");       BNB_Nu_h_SPOutFVSig_Nprotons_0n->Sumw2();		BNB_Nu_h_SPOutFVSig_Nprotons_0n->Scale(norm_bnb_Nu);   //Sig_1g
    //Bkg
    TH1F *BNB_Nu_h_SPoutFVBkg_Nprotons_0n = (TH1F*)file3->Get("h_SPoutFVBkg_Nprotons_0n");       BNB_Nu_h_SPoutFVBkg_Nprotons_0n->Sumw2();		BNB_Nu_h_SPoutFVBkg_Nprotons_0n->Scale(norm_bnb_Nu);    //Bkg_DirtOutFV
    TH1F *BNB_Nu_h_SPnumuCCBkg_Nprotons_0n = (TH1F*)file3->Get("h_SPnumuCCBkg_Nprotons_0n");     BNB_Nu_h_SPnumuCCBkg_Nprotons_0n->Sumw2();		BNB_Nu_h_SPnumuCCBkg_Nprotons_0n->Scale(norm_bnb_Nu);   //Bkg_OtherInFV 
    TH1F *BNB_Nu_h_SPnumuCCpi0Bkg_Nprotons_0n = (TH1F*)file3->Get("h_SPnumuCCpi0Bkg_Nprotons_0n");BNB_Nu_h_SPnumuCCpi0Bkg_Nprotons_0n->Sumw2();		BNB_Nu_h_SPnumuCCpi0Bkg_Nprotons_0n->Scale(norm_bnb_Nu);//Bkg_CCpi0 
    TH1F *BNB_Nu_h_SPnueCCBkg_Nprotons_0n = (TH1F*)file3->Get("h_SPnueCCBkg_Nprotons_0n");       BNB_Nu_h_SPnueCCBkg_Nprotons_0n->Sumw2();               BNB_Nu_h_SPnueCCBkg_Nprotons_0n->Scale(norm_bnb_Nu);    //Bkg_CCNue
    TH1F *BNB_Nu_h_SPNCBkg_Nprotons_0n = (TH1F*)file3->Get("h_SPNCBkg_Nprotons_0n");             BNB_Nu_h_SPNCBkg_Nprotons_0n->Sumw2();			BNB_Nu_h_SPNCBkg_Nprotons_0n->Scale(norm_bnb_Nu);       //Bkg_OtherInFV
    TH1F *BNB_Nu_h_SPNCpi0Bkg_Nprotons_0n = (TH1F*)file3->Get("h_SPNCpi0Bkg_Nprotons_0n");       BNB_Nu_h_SPNCpi0Bkg_Nprotons_0n->Sumw2();		BNB_Nu_h_SPNCpi0Bkg_Nprotons_0n->Scale(norm_bnb_Nu);    //Bkg_NCpi0
    TH1F *BNB_Nu_h_CosmicBkg_Nprotons_0n = (TH1F*)file3->Get("h_CosmicBkg_Nprotons_0n");         BNB_Nu_h_CosmicBkg_Nprotons_0n->Sumw2();		BNB_Nu_h_CosmicBkg_Nprotons_0n->Scale(norm_bnb_Nu);     //Bkg_Cosmic



    //--- Dirt ---

    TH1F *Dirt_h_1gX_Nprotons_0n = (TH1F*)file4->Get("h_1gX_Nprotons_0n"); 		 Dirt_h_1gX_Nprotons_0n->Sumw2(); 		Dirt_h_1gX_Nprotons_0n->Scale(norm_dirt);
    //TH1F *Dirt_h_1gX_Nprotons_0n_blip_corr = (TH1F*)file4->Get("h_1gX_Nprotons_0n_blip_corr");   Dirt_h_1gX_Nprotons_0n_blip_corr->Sumw2();   Dirt_h_1gX_Nprotons_0n_blip_corr->Scale(norm_dirt);
    //Signal
    TH1F *Dirt_h_SPNCDeltaSig_Nprotons_0n = (TH1F*)file4->Get("h_SPNCDeltaSig_Nprotons_0n");   Dirt_h_SPNCDeltaSig_Nprotons_0n->Sumw2(); 	Dirt_h_SPNCDeltaSig_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPNCPi0Sig_Nprotons_0n = (TH1F*)file4->Get("h_SPNCPi0Sig_Nprotons_0n");       Dirt_h_SPNCPi0Sig_Nprotons_0n->Sumw2();		Dirt_h_SPNCPi0Sig_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPNCOtherSig_Nprotons_0n = (TH1F*)file4->Get("h_SPNCOtherSig_Nprotons_0n");   Dirt_h_SPNCOtherSig_Nprotons_0n->Sumw2();		Dirt_h_SPNCOtherSig_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPNumuCCSig_Nprotons_0n = (TH1F*)file4->Get("h_SPNumuCCSig_Nprotons_0n");     Dirt_h_SPNumuCCSig_Nprotons_0n->Sumw2();		Dirt_h_SPNumuCCSig_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPOutFVSig_Nprotons_0n = (TH1F*)file4->Get("h_SPOutFVSig_Nprotons_0n");       Dirt_h_SPOutFVSig_Nprotons_0n->Sumw2();		Dirt_h_SPOutFVSig_Nprotons_0n->Scale(norm_dirt);//Sig_1g
    //Bkg
    TH1F *Dirt_h_SPoutFVBkg_Nprotons_0n = (TH1F*)file4->Get("h_SPoutFVBkg_Nprotons_0n");       Dirt_h_SPoutFVBkg_Nprotons_0n->Sumw2();		Dirt_h_SPoutFVBkg_Nprotons_0n->Scale(norm_dirt); //Bkg_DirtOutFV
    TH1F *Dirt_h_SPnumuCCBkg_Nprotons_0n = (TH1F*)file4->Get("h_SPnumuCCBkg_Nprotons_0n");     Dirt_h_SPnumuCCBkg_Nprotons_0n->Sumw2();		Dirt_h_SPnumuCCBkg_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPnumuCCpi0Bkg_Nprotons_0n = (TH1F*)file4->Get("h_SPnumuCCpi0Bkg_Nprotons_0n");Dirt_h_SPnumuCCpi0Bkg_Nprotons_0n->Sumw2();	Dirt_h_SPnumuCCpi0Bkg_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPnueCCBkg_Nprotons_0n = (TH1F*)file4->Get("h_SPnueCCBkg_Nprotons_0n");       Dirt_h_SPnueCCBkg_Nprotons_0n->Sumw2();           Dirt_h_SPnueCCBkg_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPNCBkg_Nprotons_0n = (TH1F*)file4->Get("h_SPNCBkg_Nprotons_0n");             Dirt_h_SPNCBkg_Nprotons_0n->Sumw2();		Dirt_h_SPNCBkg_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_SPNCpi0Bkg_Nprotons_0n = (TH1F*)file4->Get("h_SPNCpi0Bkg_Nprotons_0n");       Dirt_h_SPNCpi0Bkg_Nprotons_0n->Sumw2();		Dirt_h_SPNCpi0Bkg_Nprotons_0n->Scale(norm_dirt);
    TH1F *Dirt_h_CosmicBkg_Nprotons_0n = (TH1F*)file4->Get("h_CosmicBkg_Nprotons_0n");         Dirt_h_CosmicBkg_Nprotons_0n->Sumw2();		Dirt_h_CosmicBkg_Nprotons_0n->Scale(norm_dirt);
	



	//--- NC pi0 ---

    TH1F *NC_pi0_h_1gX_Nprotons_0n = (TH1F*)file5->Get("h_1gX_Nprotons_0n"); NC_pi0_h_1gX_Nprotons_0n->Sumw2();

    //Signal
    TH1F *NC_pi0_h_SPNCDeltaSig_Nprotons_0n = (TH1F*)file5->Get("h_SPNCDeltaSig_Nprotons_0n");   NC_pi0_h_SPNCDeltaSig_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPNCPi0Sig_Nprotons_0n = (TH1F*)file5->Get("h_SPNCPi0Sig_Nprotons_0n");       NC_pi0_h_SPNCPi0Sig_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPNCOtherSig_Nprotons_0n = (TH1F*)file5->Get("h_SPNCOtherSig_Nprotons_0n");   NC_pi0_h_SPNCOtherSig_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPNumuCCSig_Nprotons_0n = (TH1F*)file5->Get("h_SPNumuCCSig_Nprotons_0n");     NC_pi0_h_SPNumuCCSig_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPOutFVSig_Nprotons_0n = (TH1F*)file5->Get("h_SPOutFVSig_Nprotons_0n");       NC_pi0_h_SPOutFVSig_Nprotons_0n->Sumw2();
    //Bkg
    TH1F *NC_pi0_h_SPoutFVBkg_Nprotons_0n = (TH1F*)file5->Get("h_SPoutFVBkg_Nprotons_0n");       NC_pi0_h_SPoutFVBkg_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPnumuCCBkg_Nprotons_0n = (TH1F*)file5->Get("h_SPnumuCCBkg_Nprotons_0n");     NC_pi0_h_SPnumuCCBkg_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPnumuCCpi0Bkg_Nprotons_0n = (TH1F*)file5->Get("h_SPnumuCCpi0Bkg_Nprotons_0n");NC_pi0_h_SPnumuCCpi0Bkg_Nprotons_0n->Sumw2(); //Bkg_CCpi0
    TH1F *NC_pi0_h_SPnueCCBkg_Nprotons_0n = (TH1F*)file5->Get("h_SPnueCCBkg_Nprotons_0n");       NC_pi0_h_SPnueCCBkg_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPNCBkg_Nprotons_0n = (TH1F*)file5->Get("h_SPNCBkg_Nprotons_0n");             NC_pi0_h_SPNCBkg_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_SPNCpi0Bkg_Nprotons_0n = (TH1F*)file5->Get("h_SPNCpi0Bkg_Nprotons_0n");       NC_pi0_h_SPNCpi0Bkg_Nprotons_0n->Sumw2();
    TH1F *NC_pi0_h_CosmicBkg_Nprotons_0n = (TH1F*)file5->Get("h_CosmicBkg_Nprotons_0n");         NC_pi0_h_CosmicBkg_Nprotons_0n->Sumw2();




    // --- Nu_e == 

    TH1F *Nue_h_1gX_Nprotons_0n = (TH1F*)file6->Get("h_1gX_Nprotons_0n"); Nue_h_1gX_Nprotons_0n->Sumw2();

    //Signal
    TH1F *Nue_h_SPNCDeltaSig_Nprotons_0n = (TH1F*)file6->Get("h_SPNCDeltaSig_Nprotons_0n");   Nue_h_SPNCDeltaSig_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPNCPi0Sig_Nprotons_0n = (TH1F*)file6->Get("h_SPNCPi0Sig_Nprotons_0n");       Nue_h_SPNCPi0Sig_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPNCOtherSig_Nprotons_0n = (TH1F*)file6->Get("h_SPNCOtherSig_Nprotons_0n");   Nue_h_SPNCOtherSig_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPNumuCCSig_Nprotons_0n = (TH1F*)file6->Get("h_SPNumuCCSig_Nprotons_0n");     Nue_h_SPNumuCCSig_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPOutFVSig_Nprotons_0n = (TH1F*)file6->Get("h_SPOutFVSig_Nprotons_0n");       Nue_h_SPOutFVSig_Nprotons_0n->Sumw2();
    //Bkg
    TH1F *Nue_h_SPoutFVBkg_Nprotons_0n = (TH1F*)file6->Get("h_SPoutFVBkg_Nprotons_0n");       Nue_h_SPoutFVBkg_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPnumuCCBkg_Nprotons_0n = (TH1F*)file6->Get("h_SPnumuCCBkg_Nprotons_0n");     Nue_h_SPnumuCCBkg_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPnumuCCpi0Bkg_Nprotons_0n = (TH1F*)file6->Get("h_SPnumuCCpi0Bkg_Nprotons_0n");Nue_h_SPnumuCCpi0Bkg_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPnueCCBkg_Nprotons_0n = (TH1F*)file6->Get("h_SPnueCCBkg_Nprotons_0n");       Nue_h_SPnueCCBkg_Nprotons_0n->Sumw2(); //Bkg_CCNue
    TH1F *Nue_h_SPNCBkg_Nprotons_0n = (TH1F*)file6->Get("h_SPNCBkg_Nprotons_0n");             Nue_h_SPNCBkg_Nprotons_0n->Sumw2();
    TH1F *Nue_h_SPNCpi0Bkg_Nprotons_0n = (TH1F*)file6->Get("h_SPNCpi0Bkg_Nprotons_0n");       Nue_h_SPNCpi0Bkg_Nprotons_0n->Sumw2();
    TH1F *Nue_h_CosmicBkg_Nprotons_0n = (TH1F*)file6->Get("h_CosmicBkg_Nprotons_0n");         Nue_h_CosmicBkg_Nprotons_0n->Sumw2();

//===============================================================================================================================






     BNB_ON_h_1gX_Nprotons_0n->SetLineColor(kBlack); BNB_ON_h_1gX_Nprotons_0n->SetMarkerColor(kBlack); BNB_ON_h_1gX_Nprotons_0n->SetMarkerStyle(20);
     BNB_OFF_h_1gX_Nprotons_0n->SetFillColorAlpha(28, 0.8); BNB_OFF_h_1gX_Nprotons_0n->SetLineColorAlpha(28, 0.8); BNB_OFF_h_1gX_Nprotons_0n->SetFillStyle(3354); //BNB-off (EXT)
     BNB_Nu_h_1gX_Nprotons_0n->SetFillColorAlpha(kRed, 0.5); BNB_Nu_h_1gX_Nprotons_0n->SetLineColorAlpha(kRed, 0.5);//Nu Overlay (Total)
 
     //--Dirt style --
     //Dirt_h_1gX_Nprotons_0n->SetFillColorAlpha(kOrange+1, 1); Dirt_h_1gX_Nprotons_0n->SetLineColorAlpha(kOrange+1, 1); Dirt_h_1gX_Nprotons_0n->SetFillStyle(3244); //Dirt
     //Dirt_h_SPoutFVBkg_Nprotons_0n->SetFillColorAlpha(kOrange+1, 1); Dirt_h_1gX_Nprotons_0n->SetLineColorAlpha(kOrange+1, 1); Dirt_h_1gX_Nprotons_0n->SetFillStyle(3244); //From Dirt

    // ---- Create sum histograms ----
    // -- signal--
    TH1F *h_1g = (TH1F*)BNB_Nu_h_1gX_Nprotons_0n->Clone("h_1g");
    h_1g->Reset(); 
    h_1g->SetFillColorAlpha(222, 0.8); h_1g->SetLineColorAlpha(222, 0.8); 
    h_1g->Add(BNB_Nu_h_SPNCDeltaSig_Nprotons_0n);
    h_1g->Add(BNB_Nu_h_SPNCPi0Sig_Nprotons_0n);
    h_1g->Add(BNB_Nu_h_SPNCOtherSig_Nprotons_0n);
    h_1g->Add(BNB_Nu_h_SPNumuCCSig_Nprotons_0n); 
    h_1g->Add(BNB_Nu_h_SPOutFVSig_Nprotons_0n);
    h_1g->Add(Dirt_h_SPOutFVSig_Nprotons_0n); //From Dirt
    
    // -- backgrounds --
    TH1F *h_Nue_CC = (TH1F*)BNB_Nu_h_1gX_Nprotons_0n->Clone("h_Nue_CC");
    h_Nue_CC->Reset();
    h_Nue_CC->SetFillColorAlpha(8, 0.8); h_Nue_CC->SetLineColorAlpha(8, 0.8); 
    h_Nue_CC->Add(BNB_Nu_h_SPnueCCBkg_Nprotons_0n);
 

    TH1F *h_CCpi0_FV = (TH1F*)BNB_Nu_h_1gX_Nprotons_0n->Clone("h_CCpi0_FV");
    h_CCpi0_FV->Reset();
    h_CCpi0_FV->SetFillColorAlpha(30, 0.8); h_CCpi0_FV->SetLineColorAlpha(30, 0.8);
    h_CCpi0_FV->Add(BNB_Nu_h_SPnumuCCpi0Bkg_Nprotons_0n);

    TH1F *h_NCpi0_FV = (TH1F*)BNB_Nu_h_1gX_Nprotons_0n->Clone("h_NCpi0_FV");
    h_NCpi0_FV->Reset();
    h_NCpi0_FV->SetFillColorAlpha(216, 0.8); h_NCpi0_FV->SetLineColorAlpha(216, 0.8);
    h_NCpi0_FV->Add(BNB_Nu_h_SPNCpi0Bkg_Nprotons_0n);

    TH1F *h_Other_FV = (TH1F*)BNB_Nu_h_1gX_Nprotons_0n->Clone("h_Other_FV");
    h_Other_FV->Reset();
    h_Other_FV->SetFillColorAlpha(66, 0.8); h_Other_FV->SetLineColorAlpha(66, 0.8);
    h_Other_FV->Add(BNB_Nu_h_SPnumuCCBkg_Nprotons_0n);
    h_Other_FV->Add(BNB_Nu_h_SPNCBkg_Nprotons_0n);   
 
    TH1F *h_Cosmic = (TH1F*)BNB_Nu_h_1gX_Nprotons_0n->Clone("h_Cosmic"); //BNB_Nu_Cosmic + BNB-Off
    h_Cosmic->Reset();
    h_Cosmic->SetFillColorAlpha(28, 0.8); h_Cosmic->SetLineColorAlpha(28, 0.8); h_Cosmic->SetFillStyle(3354); //BNB-off (EXT)
    h_Cosmic->Add(BNB_Nu_h_CosmicBkg_Nprotons_0n);
    h_Cosmic->Add(BNB_OFF_h_CosmicBkg_Nprotons_0n); //Cosmic Beam-Off

    TH1F *h_Dirt_OutFV = (TH1F*)BNB_Nu_h_1gX_Nprotons_0n->Clone("Dirt_OutFV");
    h_Dirt_OutFV->Reset();
    h_Dirt_OutFV->SetFillColorAlpha(kOrange+1, 1); h_Dirt_OutFV->SetLineColorAlpha(kOrange+1, 1); h_Dirt_OutFV->SetFillStyle(3244); //Dirt
    h_Dirt_OutFV->Add(BNB_Nu_h_SPoutFVBkg_Nprotons_0n);
    h_Dirt_OutFV->Add(Dirt_h_SPoutFVBkg_Nprotons_0n); 
 
 
 
 
     // --- Create the stack ---
    THStack *hs = new THStack("hs","Comparison of stacked vs single histogram");
    hs->Add(h_Cosmic);
    hs->Add(h_Dirt_OutFV);
    hs->Add(h_NCpi0_FV);
    hs->Add(h_CCpi0_FV);
    hs->Add(h_Other_FV);
    hs->Add(h_Nue_CC);
    hs->Add(h_1g);

  // -- Aux hist for total uncertainty on stacked histogram ---
    TH1F *h_total = (TH1F*)h_Cosmic->Clone("h_total");
    h_total->Reset();
    h_total->Sumw2();

    h_total->Add(h_Cosmic);
    h_total->Add(h_Dirt_OutFV);
    h_total->Add(h_NCpi0_FV);
    h_total->Add(h_CCpi0_FV);
    h_total->Add(h_Other_FV);
    h_total->Add(h_Nue_CC);
    h_total->Add(h_1g);

    // Style for total error band
    h_total->SetFillColorAlpha(kGray+2, 0.35); // transparent gray
    h_total->SetFillStyle(3004);               // hatched
    h_total->SetMarkerSize(0);
    h_total->SetLineColor(kGray+2);


    TCanvas* c1 = new TCanvas("c1", "Stack vs Single", 800, 700);

    TPad *pad1 = nullptr;
    TPad *pad2 = nullptr;

    if (!doRatio) {
        // normal layout
        c1->SetMargin(0.12, 0.05, 0.12, 0.05);
    } else {
        // --- split canvas into main plot and ratio plot ----
        pad1 = new TPad("pad1","pad1",0,0.30,1,1); // top 70%
        pad1->SetBottomMargin(0.02);
        pad1->SetLeftMargin(0.12);
        pad1->SetRightMargin(0.05);
        pad1->Draw();

        pad2 = new TPad("pad2","pad2",0,0,1,0.30); // bottom 30%
        pad2->SetTopMargin(0.05);
        pad2->SetBottomMargin(0.32);
        pad2->SetLeftMargin(0.12);
        pad2->SetRightMargin(0.05);
        pad2->Draw();

        pad1->cd();
    }
  
   
    BNB_ON_h_1gX_Nprotons_0n->Draw(" E1");
    hs->Draw("HIST  SAME");  h_total->Draw("E2 same");  
    BNB_ON_h_1gX_Nprotons_0n->Draw("E1 SAME");


	Chi2Result res = ComputeChi2(BNB_ON_h_1gX_Nprotons_0n, h_total, true, 1, 10);
	double chi2  = res.chi2;
	int ndof     = res.ndof;
	double pval  = res.pvalue;



    //Format histogram
    BNB_ON_h_1gX_Nprotons_0n->SetTitle(Form("%s", h_title_name.Data()));
    BNB_ON_h_1gX_Nprotons_0n->GetXaxis()->SetTitle("Number of Protons");
    BNB_ON_h_1gX_Nprotons_0n->GetYaxis()->SetTitle("Event counts");
    BNB_ON_h_1gX_Nprotons_0n->GetXaxis()->SetRangeUser(0, 4);
    BNB_ON_h_1gX_Nprotons_0n->GetYaxis()->SetRangeUser(0, max_y);
    BNB_ON_h_1gX_Nprotons_0n->GetXaxis()->SetNdivisions(505); //Remove mid point Labels 0.5, 1.5 ...
    BNB_ON_h_1gX_Nprotons_0n->GetXaxis()->SetLabelOffset(0.015);

    // --- Add legend ---
    auto legend = new TLegend(0.45, 0.6, 0.88, 0.88); 
    legend->SetNColumns(2);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);

//    legend->AddEntry(h_total, "", "l"); 
    legend->AddEntry(h_total, "Stat. Uncertainty", "f");
    legend->AddEntry(BNB_ON_h_1gX_Nprotons_0n, Form("BNB Data, %.0f", BNB_ON_h_1gX_Nprotons_0n->GetEntries()), "lp");
    legend->AddEntry(h_Cosmic,     Form("Cosmic Data, %.1f",h_Cosmic->GetSumOfWeights()) , "f");
    legend->AddEntry(h_Dirt_OutFV, Form("Dirt/out FV, %.1f" ,h_Dirt_OutFV->GetSumOfWeights()), "f");
    legend->AddEntry(h_NCpi0_FV, Form("NC #pi^{0} in FV, %.1f ",h_NCpi0_FV->GetSumOfWeights()), "f");
    legend->AddEntry(h_CCpi0_FV, Form("CC #pi^{0} in FV, %.1f", h_CCpi0_FV->GetSumOfWeights()),"f");
    legend->AddEntry(h_Other_FV, Form("Other in FV, %.1f", h_Other_FV->GetSumOfWeights()),"f");
    legend->AddEntry(h_1g, Form("1#gamma, %.1f ", h_1g->GetSumOfWeights()) ,"f");
    legend->AddEntry(h_Nue_CC, Form("#nu_{e} CC in FV, %.1f",h_Nue_CC->GetSumOfWeights()), "f");
//    legend->AddEntry(h_1g, "1#gamma ", "f");

   legend->Draw();
   TLatex latex;
    latex.SetNDC(); latex.SetTextSize(0.04);
    latex.DrawLatex(0.65, 0.91, Form("Data POT = %.2e", POT_BNB_On));


    pad1->cd();
 
   PrintChi2Smart(BNB_ON_h_1gX_Nprotons_0n, h_total, pad1, legend, true, 1, 4);
 //PrintChi2Smart(BNB_ON_h_1gX_Nprotons_0n, BNB_ON_h_1gX_Nprotons_0n, pad1, legend, true, 1, 4);
 //PrintChi2Smart(h_total, h_total, pad1, legend, true, 1, 4);

    if (doRatio) {
        pad2->cd();

        TH1F *ratio = (TH1F*)BNB_ON_h_1gX_Nprotons_0n->Clone("ratio");
        ratio->Divide(h_total);    // data / prediction

        ratio->SetTitle("");
        ratio->GetYaxis()->SetTitle("Data / Pred.");
        ratio->GetYaxis()->SetNdivisions(505);
        ratio->GetYaxis()->SetTitleSize(0.10);
        ratio->GetYaxis()->SetLabelSize(0.09);
        ratio->GetYaxis()->SetTitleOffset(0.5);

        ratio->GetXaxis()->SetTitle("Number of Protons");
        ratio->GetXaxis()->SetTitleSize(0.12);
        ratio->GetXaxis()->SetLabelSize(0.10);
        ratio->GetXaxis()->SetTickLength(0.08);

        ratio->SetMarkerStyle(20);
        ratio->Draw("E1");

        // Draw line at ratio = 1
        TLine *line = new TLine(0,1,4,1);
        line->SetLineWidth(1);
        line->SetLineStyle(2);
        line->Draw("SAME");

        SetRatioAxisSymmetric(ratio, 2.0);


    }



 
    // ---------------- Save outputs ----------------
    TString suffix = doRatio ? "_ratio" : "";
    c1->SaveAs(Form("%s/IncSP_Nprotons_%s_Stacked_Nprotons_0n%s.png", outDir.Data(), tag.Data(), suffix.Data()));
    c1->SaveAs(Form("%s/IncSP_Nprotons_%s_Stacked_Nprotons_0n%s.pdf", outDir.Data(), tag.Data(), suffix.Data()));
    c1->SaveAs(Form("%s/IncSP_Nprotons_%s_Stacked_Nprotons_0n%s.root",outDir.Data(), tag.Data(), suffix.Data()));

    TFile *outfile = new TFile(Form("%s/output_histograms_IncSP_Nprotons_0n_%s.root",
                                    outDir.Data(), tag.Data()), "RECREATE");
    hs->Write();
    h_total->Write();
    BNB_ON_h_1gX_Nprotons_0n->Write();
    outfile->Close();

}


