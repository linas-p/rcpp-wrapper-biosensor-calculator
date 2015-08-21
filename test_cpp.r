F = 96.485*1e3;
#96.485 # C/mol;
N = 100#;0;
d = d_delta = 100*1e-6;
# 1e-6 meter
D_Se = D_Pe = 300*1e-6;
D_Sb = D_Pb = 600*1e-6;
h =  (d+d_delta)/N;
# h = dx/dy

N_d = (d)/h;

#########
tau = 1e-6;
# tau = dt

tau <= h^2/(2*D_Sb);
# check stability

V_max = 10*1e-3;
K_m = S_0 = 100*1e-3;

library(Rcpp);
params = c(tau, N, d, d_delta, D_Se, D_Sb, D_Pe, D_Pb, S_0, V_max, K_m);

dyn.load("testing.so");
Sys.time()->start;
{
    result<- .Call("calculate", params);
    plot(result$t, result$I);
}
print(Sys.time()-start);

