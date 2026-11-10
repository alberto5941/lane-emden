#include "OdeSolver.h"
#include "Vector3.h"
#include "MatPoint.h"

#include <cmath>
#include <fstream>
#include <vector>

#include <TGraph.h>
#include <TMultiGraph.h>
#include <TF1.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TH2D.h>
#include <TApplication.h>
#include <TStyle.h>
#include <TSystem.h>

using namespace std;

double n=1; //indice che varia

Vector3 ForzaInterna(unsigned int i,unsigned int j, double t, vector<MatPoint> P) {
    return Vector3();
}

Vector3 ForzaEsterna(unsigned int i, double t, vector<MatPoint> p){
    double theta = p[i].R().X();
    double theta_primo = p[i].V().X();

    double theta_safe = (theta > 0.0) ? theta : 0.0; //controlla questo pezzo!!!
    double t_safe = (t > 1e-5) ? t : 1e-5;

    double theta_secondo = (-2/t_safe)*theta_primo - pow(theta_safe, n);
    return Vector3(theta_secondo, 0, 0);
}

int main() {
    TApplication app("app",0,NULL);
    gStyle->SetOptStat(0);

    double M_sole = 1.9884e30;
    double R_sole = 6.957e8;


    OdeSolver ode = OdeSolver("Rk4");
    double xi = 0.01;
    ode.T(xi);
    Vector3 R = Vector3(1-(1./6)*(xi*xi),0,0);
    Vector3 V = Vector3(-(1./3)*xi, 0, 0);
    MatPoint P = MatPoint(1, 0, R, V);
    ode.AddMatPoint(P);


    // Creazione classe OdeSolver (per la soluzione dell'equ. diff.)
    ode.fInternal = ForzaInterna;
    ode.fExternal = ForzaEsterna;
    ode.DeltaT(0.01);

    TCanvas *c = new TCanvas("c", "Confronto equazione di Lane-Emden");
    c->SetName("Confronto equazione di Lane-Emden");

    //creazione di un TMultiGraph e di una TCanvas per il confronto dei tre indici
    TMultiGraph *mg = new TMultiGraph("mg", "Confronto equazione di Lane-Emden");
    TLegend *leg = new TLegend(0.65, 0.70, 0.90, 0.90);
    leg->SetTextSize(0.025);
    leg->SetTextColor(kBlack);
    leg->SetFillStyle(0);
    leg->SetBorderSize(1);
    leg->SetFillColor(kWhite);

    vector<double> lista_n = {1,1.5,3};
    vector<int> colori = {kRed, kBlue, kGreen+2};
    vector<string> etichette = {"n = 1.0", "n = 1.5", "n = 3.0"};

    //vector per salvare i punti
    vector<double> R1, V1; //vector in cui salvo il primo zero e la derivata in quel punto, per ogni valore di n
    vector<vector<double>> vec_xi(3);
    vector<vector<double>> vec_V(3);
    vector<vector<double>> tempi(3);

    cout << endl;

    for(unsigned int j=0; j<lista_n.size(); j++) {

        n = lista_n[j];
        vector<double>& vec_j = vec_xi[j];
        vector<double>& vecV_j = vec_V[j];
        vector<double>& tmp = tempi[j];
        OdeSolver ode = OdeSolver("Rk4");
        double xi = 0.01; //punto di partenza per l'integrazione
        ode.T(xi);
        //setto i valori iniziali. MatPoint utilizza Vector3: nel caso di
        //equazione scalare, lavoriamo solo sulla prima componente, settando
        //le altre a zero
        Vector3 R = Vector3(1-(1./6)*(xi*xi),0,0);
        Vector3 V = Vector3(-(1./3)*xi, 0, 0);
        MatPoint P = MatPoint(1, 0, R, V);
        ode.AddMatPoint(P);

        // assegno le funzioni per forza interna
        // e per forza esterna e setto il passo
        ode.fInternal = ForzaInterna;
        ode.fExternal = ForzaEsterna;
        ode.DeltaT(0.0005);


        TGraph *gr = new TGraph();
        gr->SetLineColor(colori[j]);
        gr->SetLineWidth(2);

        double x_prec = ode.GetMatPoint(0).R().X();
        double v_prec = ode.GetMatPoint(0).V().X();
        double t_prec;

        unsigned int k=0;
        gr->SetPoint(0,0,1);

        //ciclo while con il metodo Step()
        while (ode.T()<7){
            x_prec = ode.GetMatPoint(0).R().X();
            v_prec = ode.GetMatPoint(0).V().X();
            t_prec = ode.T();
            k = k+1;
            ode.Step();
            double x = ode.GetMatPoint(0).R().X();
            double v = ode.GetMatPoint(0).V().X();
            double t = ode.T();
            vec_j.push_back(x);
            vecV_j.push_back(v);
            tmp.push_back(t);
            if(x<=0) {
                double xi1 = t_prec - x_prec*((t-t_prec)/(x-x_prec)); //interpolazione lineare
                double v1 = v_prec + ((xi1-t_prec)/(t-t_prec))*(v-v_prec); //interpolazione lineare
                R1.push_back(xi1);
                V1.push_back(v1);
                cout << "grafico " << j << ": " << "primo zero in " << xi1 << endl;
                break;
            }
            gr->SetPoint(k,t_prec,x_prec);
        }
        mg->Add(gr);
        leg->AddEntry(gr, etichette[j].c_str(), "l");
        mg->GetXaxis()->SetTitle("#xi");
        mg->GetYaxis()->SetTitle("#theta");
    }

    mg->Draw("AL");
    leg->Draw();


    //creo una nuova canvas per disegnare l'andamento
    //della densità per i diversi valori di n
    TCanvas *c2 = new TCanvas("c2", "Confronto delle densita'");

    TMultiGraph *mg2 = new TMultiGraph("mg2", "Confronto delle densit#grave{a}");
    TLegend *leg2 = new TLegend(0.65, 0.70, 0.90, 0.90);
    leg2->SetTextSize(0.025);
    leg2->SetTextColor(kBlack);
    leg2->SetFillStyle(0);
    leg2->SetBorderSize(1);
    leg2->SetFillColor(kWhite);

    //vector in cui salvo i valori di a e di rho_C
    vector<double> vec_a, vec_rhoC;

    //calcolo a e rho_c per ogni valore di n
    //e stampo a schermo i risultati
    for(unsigned int t=0; t<3; t++) {
        double n = lista_n[t];
        double xi1 = R1[t];
        double v1 = V1[t];
        double a = R_sole/xi1;
        double rho_C = -M_sole/(4*M_PI*a*a*a*xi1*xi1*v1);

        vec_a.push_back(a);
        vec_rhoC.push_back(rho_C);


        cout << endl;
        cout << "n = " << n << endl;
        cout << "xi = " << xi1 << endl;
        cout << "a = " << a << " m" << endl;
        cout << "rho_C = " << rho_C << " kg/m^3" << endl;
        cout << endl;

        vector<double> vec = vec_xi[t];
        vector<double> tmp = tempi[t];
        vector<double> rho;
        TGraph *gr = new TGraph();
        gr->SetLineColor(colori[t]);
        gr->SetLineWidth(2);
        for(unsigned int k=0; k<vec.size(); k++) {
            rho.push_back(rho_C*pow(vec[k], n));
            gr->SetPoint(k, 0.001*a*tmp[k], rho[k]/1000); //in g/cm^3
        }
        mg2->Add(gr);
        leg2->AddEntry(gr, etichette[t].c_str(), "l");
    }

    mg2->GetXaxis()->SetTitle("r [km]");
    mg2->GetYaxis()->SetTitle("#rho [g/cm^{3}]");
    c2->SetLogy();
    mg2->SetMinimum(1e-1);
    mg2->Draw("AL");
    leg2->Draw();


    //creo una nuova canvas per visualizzare m(r)/M_sole
    TCanvas *c3 = new TCanvas("c3", "Confronto di m(r)/M");

    TMultiGraph *mg3 = new TMultiGraph("mg3", "Confronto di #frac{m(r)}{M}");
    TLegend *leg3 = new TLegend(0.10, 0.70, 0.30, 0.90);
    leg3->SetTextSize(0.025);
    leg3->SetTextColor(kBlack);
    leg3->SetFillStyle(0);
    leg3->SetBorderSize(1);
    leg3->SetFillColor(kWhite);



    for(unsigned int t=0; t<3; t++) {
        cout << endl;
        vector<double> vec_m;
        double a = vec_a[t];
        vector<double> tmp = tempi[t];
        vector<double> vecV = vec_V[t];

        TGraph *graph = new TGraph();

        double r1 = R1[t];
        double v1 = V1[t];

    
        for(unsigned int k=0; k<tmp.size(); k++) {
            //calcolo di m(r)
            double C = pow(tmp[k]*(1/r1), 2);
            double D = vecV[k]*(1/v1);
            double m = C*D;
            vec_m.push_back(m);
            graph->SetPoint(k, 0.001*a*tmp[k], vec_m[k]);
        }
        graph->SetLineColor(colori[t]);
        graph->SetLineWidth(2);
        mg3->Add(graph);
        leg3->AddEntry(graph, etichette[t].c_str(), "l");
    }

    mg3->GetXaxis()->SetTitle("r [km]");
    mg3->GetYaxis()->SetTitle("#frac{m(r)}{M}");
    mg3->Draw("AL");
    leg3->Draw();

    //confrontiamo la soluzione ottenuta numericamente con quella
    //analitica, che corrisponde a un seno cardinale

    TCanvas *c4 = new TCanvas("c4", "Confronto con seno cardinale");
    vector<double> tmp = tempi[0];
    vector<double> vec = vec_xi[0];
    TF1 *f = new TF1("f", "sin(x)/x", 0.01, R1[0]);
    TGraph *gr = new TGraph();
    gr->SetTitle("#theta(#xi)(n=1)");
    gr->SetMarkerStyle(20);

    TLegend *leg4 = new TLegend(0.65, 0.70, 0.90, 0.90);

    for(unsigned int k=0; k<tmp.size(); k++) {
        gr->SetPoint(k, tmp[k], vec[k]);
    }

    gr->GetXaxis()->SetTitle("#xi");
    gr->GetYaxis()->SetTitle("#theta");

    f->SetLineColor(kBlue);
    f->Draw();

    gr->SetLineColor(colori[0]);
    gr->Draw("same");

    leg4->AddEntry(gr);
    leg4->AddEntry(f);
    leg4->Draw();

    //settiamo n=5

    TCanvas *c5 = new TCanvas("c5", "n=5");

    TGraph *graph = new TGraph();
    graph->SetTitle("#theta(#xi)(n=5)");
    graph->SetLineColor(kBlue);

    int k=0;

    while (ode.T()<100){
            ode.Step();
            double x = ode.GetMatPoint(0).R().X();
            double t = ode.T();
            graph->SetPoint(k,t,x);
            k = k+1;
        }

    graph->Draw();


    app.Run(true);



    return 0;
}
