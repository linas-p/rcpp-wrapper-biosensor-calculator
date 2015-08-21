#include <Rcpp.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iterator>
#include "constants.h"
#include "biosensor_information.h"
#include "explicit_calculator.h"

;
using namespace std;

void callback_crunched(void *ptr, int time, std::string info)
{
    if( !info.empty() ) {
        cout << info.c_str() ;
    }
    cout << "simulated "<< time << " (s) \n" << endl;

}

void SetGloParams(bio_params & pars, double dt,  int N) {
    pars.dt = dt;
    pars.n = N;
    pars.resp_t_meth = DEFAULT_TIME;
    pars.min_t = 200;
    //[s]
    pars.resp_t = 200;
    pars.out_file_name = "output.dat";
    pars.ne = 1;

}

void setLocParams(bio_params & pars, double d, double d_delta, double D_Se, double D_Sb, double D_Pe, double D_Pb, double S_0, double Vmax, double K_m) {

    pars.km = K_m;
    pars.v_max_ = Vmax;

    pars.s0 = S_0;
    //[mol/l] . [mol/m^3]
    pars.p0 = 0 * 1e-3;
    pars.layer_count = 2;
    pars.layers = new layer_params[2];

    //Užpildoma sluoksnių informacija
    // 0
    pars.layers[0].enz_layer = true;
    //[um^2/s] . [m^2/s]
    pars.layers[0].Ds = D_Se;
    //[um^2/s] . [m^2/s]
    pars.layers[0].Dp = D_Pe;
    //[um] . [m]
    pars.layers[0].d = d;
    //[mol/l] . [mol/m^3]

    // 1
    pars.layers[1].enz_layer = false;
    //[um^2/s] . [m^2/s]
    pars.layers[1].Ds = D_Sb;
    //[um^2/s] . [m^2/s]
    pars.layers[1].Dp = D_Pb;
    //[um] . [m]
    pars.layers[1].d = d_delta;
    //[mol/l] . [mol/m^3]

}


void setParams(bio_params & pars) {

    //[mol/m^3]
    pars.km = 100 * 1e-3;
    pars.v_max_ = 10 * 1e-3;
    //[s]
    pars.dt = 1e-6;
    pars.n = 40;
    pars.resp_t_meth = DEFAULT_TIME;//DEFAULT_TIME;
    //[s]
    pars.min_t = 200;
    //[s]
    pars.resp_t = 200;
    pars.out_file_name = "output.dat";
    pars.ne = 1;
    //[mol/l] . [mol/m^3] 4e-5;
    pars.s0 = 100 * 1e-3;
    //[mol/l] . [mol/m^3]
    pars.p0 = 0 * 1e-3;
    pars.layer_count = 2;
    pars.layers = new layer_params[2];

    //Užpildoma sluoksnių informacija
    // 0
    pars.layers[0].enz_layer = true;
    //[um^2/s] . [m^2/s]
    pars.layers[0].Ds = 300 * 1e-6;
    //[um^2/s] . [m^2/s]
    pars.layers[0].Dp = 300 * 1e-6;
    //[um] . [m]
    pars.layers[0].d = 100*1e-6;
    //[mol/l] . [mol/m^3]

    // 1
    pars.layers[1].enz_layer = false;
    //[um^2/s] . [m^2/s]
    pars.layers[1].Ds = 600 * 1e-6;
    //[um^2/s] . [m^2/s]
    pars.layers[1].Dp = 600 * 1e-6;
    //[um] . [m]
    pars.layers[1].d = 100*1e-6;
    //[mol/l] . [mol/m^3]

}



RcppExport SEXP calculate(SEXP x) {
    Rcpp::NumericVector params(x);
    Rcpp::NumericVector nv;

    bio_params* bio_pars = new bio_params();
    SetGloParams(*bio_pars, params[0], (int)params[1]);
    setLocParams(*bio_pars, params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10]);


    cout << "Parameters_loaded" << endl;
    std::vector<double> I, S, T, P;
    calculate_explicitly(bio_pars, NULL, &callback_crunched, I, S, T, P);
    Rcpp::NumericVector II(I.begin(), I.end());
    Rcpp::NumericVector SS(S.begin(), S.end());
    Rcpp::NumericVector TT(T.begin(), T.end());
    Rcpp::NumericVector PP(P.begin(), P.end());

    delete [] bio_pars->layers;
    delete bio_pars;

    std::cout<< "Done!" << std::endl;
    return( Rcpp::List::create(Rcpp::Named("I")=II,Rcpp::Named("time")=TT, Rcpp::Named("S")=SS, Rcpp::Named("P")=PP));
}
