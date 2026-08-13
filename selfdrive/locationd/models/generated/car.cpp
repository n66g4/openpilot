#include "car.h"

namespace {
#define DIM 9
#define EDIM 9
#define MEDIM 9
typedef void (*Hfun)(double *, double *, double *);

double mass;

void set_mass(double x){ mass = x;}

double rotational_inertia;

void set_rotational_inertia(double x){ rotational_inertia = x;}

double center_to_front;

void set_center_to_front(double x){ center_to_front = x;}

double center_to_rear;

void set_center_to_rear(double x){ center_to_rear = x;}

double stiffness_front;

void set_stiffness_front(double x){ stiffness_front = x;}

double stiffness_rear;

void set_stiffness_rear(double x){ stiffness_rear = x;}
const static double MAHA_THRESH_25 = 3.8414588206941227;
const static double MAHA_THRESH_24 = 5.991464547107981;
const static double MAHA_THRESH_30 = 3.8414588206941227;
const static double MAHA_THRESH_26 = 3.8414588206941227;
const static double MAHA_THRESH_27 = 3.8414588206941227;
const static double MAHA_THRESH_29 = 3.8414588206941227;
const static double MAHA_THRESH_28 = 3.8414588206941227;
const static double MAHA_THRESH_31 = 3.8414588206941227;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_3001115831286256944) {
   out_3001115831286256944[0] = delta_x[0] + nom_x[0];
   out_3001115831286256944[1] = delta_x[1] + nom_x[1];
   out_3001115831286256944[2] = delta_x[2] + nom_x[2];
   out_3001115831286256944[3] = delta_x[3] + nom_x[3];
   out_3001115831286256944[4] = delta_x[4] + nom_x[4];
   out_3001115831286256944[5] = delta_x[5] + nom_x[5];
   out_3001115831286256944[6] = delta_x[6] + nom_x[6];
   out_3001115831286256944[7] = delta_x[7] + nom_x[7];
   out_3001115831286256944[8] = delta_x[8] + nom_x[8];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_1270014336152208748) {
   out_1270014336152208748[0] = -nom_x[0] + true_x[0];
   out_1270014336152208748[1] = -nom_x[1] + true_x[1];
   out_1270014336152208748[2] = -nom_x[2] + true_x[2];
   out_1270014336152208748[3] = -nom_x[3] + true_x[3];
   out_1270014336152208748[4] = -nom_x[4] + true_x[4];
   out_1270014336152208748[5] = -nom_x[5] + true_x[5];
   out_1270014336152208748[6] = -nom_x[6] + true_x[6];
   out_1270014336152208748[7] = -nom_x[7] + true_x[7];
   out_1270014336152208748[8] = -nom_x[8] + true_x[8];
}
void H_mod_fun(double *state, double *out_4231008996866728218) {
   out_4231008996866728218[0] = 1.0;
   out_4231008996866728218[1] = 0.0;
   out_4231008996866728218[2] = 0.0;
   out_4231008996866728218[3] = 0.0;
   out_4231008996866728218[4] = 0.0;
   out_4231008996866728218[5] = 0.0;
   out_4231008996866728218[6] = 0.0;
   out_4231008996866728218[7] = 0.0;
   out_4231008996866728218[8] = 0.0;
   out_4231008996866728218[9] = 0.0;
   out_4231008996866728218[10] = 1.0;
   out_4231008996866728218[11] = 0.0;
   out_4231008996866728218[12] = 0.0;
   out_4231008996866728218[13] = 0.0;
   out_4231008996866728218[14] = 0.0;
   out_4231008996866728218[15] = 0.0;
   out_4231008996866728218[16] = 0.0;
   out_4231008996866728218[17] = 0.0;
   out_4231008996866728218[18] = 0.0;
   out_4231008996866728218[19] = 0.0;
   out_4231008996866728218[20] = 1.0;
   out_4231008996866728218[21] = 0.0;
   out_4231008996866728218[22] = 0.0;
   out_4231008996866728218[23] = 0.0;
   out_4231008996866728218[24] = 0.0;
   out_4231008996866728218[25] = 0.0;
   out_4231008996866728218[26] = 0.0;
   out_4231008996866728218[27] = 0.0;
   out_4231008996866728218[28] = 0.0;
   out_4231008996866728218[29] = 0.0;
   out_4231008996866728218[30] = 1.0;
   out_4231008996866728218[31] = 0.0;
   out_4231008996866728218[32] = 0.0;
   out_4231008996866728218[33] = 0.0;
   out_4231008996866728218[34] = 0.0;
   out_4231008996866728218[35] = 0.0;
   out_4231008996866728218[36] = 0.0;
   out_4231008996866728218[37] = 0.0;
   out_4231008996866728218[38] = 0.0;
   out_4231008996866728218[39] = 0.0;
   out_4231008996866728218[40] = 1.0;
   out_4231008996866728218[41] = 0.0;
   out_4231008996866728218[42] = 0.0;
   out_4231008996866728218[43] = 0.0;
   out_4231008996866728218[44] = 0.0;
   out_4231008996866728218[45] = 0.0;
   out_4231008996866728218[46] = 0.0;
   out_4231008996866728218[47] = 0.0;
   out_4231008996866728218[48] = 0.0;
   out_4231008996866728218[49] = 0.0;
   out_4231008996866728218[50] = 1.0;
   out_4231008996866728218[51] = 0.0;
   out_4231008996866728218[52] = 0.0;
   out_4231008996866728218[53] = 0.0;
   out_4231008996866728218[54] = 0.0;
   out_4231008996866728218[55] = 0.0;
   out_4231008996866728218[56] = 0.0;
   out_4231008996866728218[57] = 0.0;
   out_4231008996866728218[58] = 0.0;
   out_4231008996866728218[59] = 0.0;
   out_4231008996866728218[60] = 1.0;
   out_4231008996866728218[61] = 0.0;
   out_4231008996866728218[62] = 0.0;
   out_4231008996866728218[63] = 0.0;
   out_4231008996866728218[64] = 0.0;
   out_4231008996866728218[65] = 0.0;
   out_4231008996866728218[66] = 0.0;
   out_4231008996866728218[67] = 0.0;
   out_4231008996866728218[68] = 0.0;
   out_4231008996866728218[69] = 0.0;
   out_4231008996866728218[70] = 1.0;
   out_4231008996866728218[71] = 0.0;
   out_4231008996866728218[72] = 0.0;
   out_4231008996866728218[73] = 0.0;
   out_4231008996866728218[74] = 0.0;
   out_4231008996866728218[75] = 0.0;
   out_4231008996866728218[76] = 0.0;
   out_4231008996866728218[77] = 0.0;
   out_4231008996866728218[78] = 0.0;
   out_4231008996866728218[79] = 0.0;
   out_4231008996866728218[80] = 1.0;
}
void f_fun(double *state, double dt, double *out_3852593309757630922) {
   out_3852593309757630922[0] = state[0];
   out_3852593309757630922[1] = state[1];
   out_3852593309757630922[2] = state[2];
   out_3852593309757630922[3] = state[3];
   out_3852593309757630922[4] = state[4];
   out_3852593309757630922[5] = dt*((-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]))*state[6] - 9.8100000000000005*state[8] + stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*state[1]) + (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*state[4])) + state[5];
   out_3852593309757630922[6] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*state[4])) + state[6];
   out_3852593309757630922[7] = state[7];
   out_3852593309757630922[8] = state[8];
}
void F_fun(double *state, double dt, double *out_8513599791742951419) {
   out_8513599791742951419[0] = 1;
   out_8513599791742951419[1] = 0;
   out_8513599791742951419[2] = 0;
   out_8513599791742951419[3] = 0;
   out_8513599791742951419[4] = 0;
   out_8513599791742951419[5] = 0;
   out_8513599791742951419[6] = 0;
   out_8513599791742951419[7] = 0;
   out_8513599791742951419[8] = 0;
   out_8513599791742951419[9] = 0;
   out_8513599791742951419[10] = 1;
   out_8513599791742951419[11] = 0;
   out_8513599791742951419[12] = 0;
   out_8513599791742951419[13] = 0;
   out_8513599791742951419[14] = 0;
   out_8513599791742951419[15] = 0;
   out_8513599791742951419[16] = 0;
   out_8513599791742951419[17] = 0;
   out_8513599791742951419[18] = 0;
   out_8513599791742951419[19] = 0;
   out_8513599791742951419[20] = 1;
   out_8513599791742951419[21] = 0;
   out_8513599791742951419[22] = 0;
   out_8513599791742951419[23] = 0;
   out_8513599791742951419[24] = 0;
   out_8513599791742951419[25] = 0;
   out_8513599791742951419[26] = 0;
   out_8513599791742951419[27] = 0;
   out_8513599791742951419[28] = 0;
   out_8513599791742951419[29] = 0;
   out_8513599791742951419[30] = 1;
   out_8513599791742951419[31] = 0;
   out_8513599791742951419[32] = 0;
   out_8513599791742951419[33] = 0;
   out_8513599791742951419[34] = 0;
   out_8513599791742951419[35] = 0;
   out_8513599791742951419[36] = 0;
   out_8513599791742951419[37] = 0;
   out_8513599791742951419[38] = 0;
   out_8513599791742951419[39] = 0;
   out_8513599791742951419[40] = 1;
   out_8513599791742951419[41] = 0;
   out_8513599791742951419[42] = 0;
   out_8513599791742951419[43] = 0;
   out_8513599791742951419[44] = 0;
   out_8513599791742951419[45] = dt*(stiffness_front*(-state[2] - state[3] + state[7])/(mass*state[1]) + (-stiffness_front - stiffness_rear)*state[5]/(mass*state[4]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[6]/(mass*state[4]));
   out_8513599791742951419[46] = -dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(mass*pow(state[1], 2));
   out_8513599791742951419[47] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_8513599791742951419[48] = -dt*stiffness_front*state[0]/(mass*state[1]);
   out_8513599791742951419[49] = dt*((-1 - (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*pow(state[4], 2)))*state[6] - (-stiffness_front*state[0] - stiffness_rear*state[0])*state[5]/(mass*pow(state[4], 2)));
   out_8513599791742951419[50] = dt*(-stiffness_front*state[0] - stiffness_rear*state[0])/(mass*state[4]) + 1;
   out_8513599791742951419[51] = dt*(-state[4] + (-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(mass*state[4]));
   out_8513599791742951419[52] = dt*stiffness_front*state[0]/(mass*state[1]);
   out_8513599791742951419[53] = -9.8100000000000005*dt;
   out_8513599791742951419[54] = dt*(center_to_front*stiffness_front*(-state[2] - state[3] + state[7])/(rotational_inertia*state[1]) + (-center_to_front*stiffness_front + center_to_rear*stiffness_rear)*state[5]/(rotational_inertia*state[4]) + (-pow(center_to_front, 2)*stiffness_front - pow(center_to_rear, 2)*stiffness_rear)*state[6]/(rotational_inertia*state[4]));
   out_8513599791742951419[55] = -center_to_front*dt*stiffness_front*(-state[2] - state[3] + state[7])*state[0]/(rotational_inertia*pow(state[1], 2));
   out_8513599791742951419[56] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_8513599791742951419[57] = -center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_8513599791742951419[58] = dt*(-(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])*state[5]/(rotational_inertia*pow(state[4], 2)) - (-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])*state[6]/(rotational_inertia*pow(state[4], 2)));
   out_8513599791742951419[59] = dt*(-center_to_front*stiffness_front*state[0] + center_to_rear*stiffness_rear*state[0])/(rotational_inertia*state[4]);
   out_8513599791742951419[60] = dt*(-pow(center_to_front, 2)*stiffness_front*state[0] - pow(center_to_rear, 2)*stiffness_rear*state[0])/(rotational_inertia*state[4]) + 1;
   out_8513599791742951419[61] = center_to_front*dt*stiffness_front*state[0]/(rotational_inertia*state[1]);
   out_8513599791742951419[62] = 0;
   out_8513599791742951419[63] = 0;
   out_8513599791742951419[64] = 0;
   out_8513599791742951419[65] = 0;
   out_8513599791742951419[66] = 0;
   out_8513599791742951419[67] = 0;
   out_8513599791742951419[68] = 0;
   out_8513599791742951419[69] = 0;
   out_8513599791742951419[70] = 1;
   out_8513599791742951419[71] = 0;
   out_8513599791742951419[72] = 0;
   out_8513599791742951419[73] = 0;
   out_8513599791742951419[74] = 0;
   out_8513599791742951419[75] = 0;
   out_8513599791742951419[76] = 0;
   out_8513599791742951419[77] = 0;
   out_8513599791742951419[78] = 0;
   out_8513599791742951419[79] = 0;
   out_8513599791742951419[80] = 1;
}
void h_25(double *state, double *unused, double *out_8228495743354687604) {
   out_8228495743354687604[0] = state[6];
}
void H_25(double *state, double *unused, double *out_708028346877236541) {
   out_708028346877236541[0] = 0;
   out_708028346877236541[1] = 0;
   out_708028346877236541[2] = 0;
   out_708028346877236541[3] = 0;
   out_708028346877236541[4] = 0;
   out_708028346877236541[5] = 0;
   out_708028346877236541[6] = 1;
   out_708028346877236541[7] = 0;
   out_708028346877236541[8] = 0;
}
void h_24(double *state, double *unused, double *out_5374008317928448214) {
   out_5374008317928448214[0] = state[4];
   out_5374008317928448214[1] = state[5];
}
void H_24(double *state, double *unused, double *out_2849897251925086595) {
   out_2849897251925086595[0] = 0;
   out_2849897251925086595[1] = 0;
   out_2849897251925086595[2] = 0;
   out_2849897251925086595[3] = 0;
   out_2849897251925086595[4] = 1;
   out_2849897251925086595[5] = 0;
   out_2849897251925086595[6] = 0;
   out_2849897251925086595[7] = 0;
   out_2849897251925086595[8] = 0;
   out_2849897251925086595[9] = 0;
   out_2849897251925086595[10] = 0;
   out_2849897251925086595[11] = 0;
   out_2849897251925086595[12] = 0;
   out_2849897251925086595[13] = 0;
   out_2849897251925086595[14] = 1;
   out_2849897251925086595[15] = 0;
   out_2849897251925086595[16] = 0;
   out_2849897251925086595[17] = 0;
}
void h_30(double *state, double *unused, double *out_6580798802100841662) {
   out_6580798802100841662[0] = state[4];
}
void H_30(double *state, double *unused, double *out_6208661994614380214) {
   out_6208661994614380214[0] = 0;
   out_6208661994614380214[1] = 0;
   out_6208661994614380214[2] = 0;
   out_6208661994614380214[3] = 0;
   out_6208661994614380214[4] = 1;
   out_6208661994614380214[5] = 0;
   out_6208661994614380214[6] = 0;
   out_6208661994614380214[7] = 0;
   out_6208661994614380214[8] = 0;
}
void h_26(double *state, double *unused, double *out_6223215400527753119) {
   out_6223215400527753119[0] = state[7];
}
void H_26(double *state, double *unused, double *out_4449531665751292765) {
   out_4449531665751292765[0] = 0;
   out_4449531665751292765[1] = 0;
   out_4449531665751292765[2] = 0;
   out_4449531665751292765[3] = 0;
   out_4449531665751292765[4] = 0;
   out_4449531665751292765[5] = 0;
   out_4449531665751292765[6] = 0;
   out_4449531665751292765[7] = 1;
   out_4449531665751292765[8] = 0;
}
void h_27(double *state, double *unused, double *out_8986345888515506414) {
   out_8986345888515506414[0] = state[3];
}
void H_27(double *state, double *unused, double *out_3012130605820901522) {
   out_3012130605820901522[0] = 0;
   out_3012130605820901522[1] = 0;
   out_3012130605820901522[2] = 0;
   out_3012130605820901522[3] = 1;
   out_3012130605820901522[4] = 0;
   out_3012130605820901522[5] = 0;
   out_3012130605820901522[6] = 0;
   out_3012130605820901522[7] = 0;
   out_3012130605820901522[8] = 0;
}
void h_29(double *state, double *unused, double *out_1579125534439996017) {
   out_1579125534439996017[0] = state[1];
}
void H_29(double *state, double *unused, double *out_4725493332690452555) {
   out_4725493332690452555[0] = 0;
   out_4725493332690452555[1] = 1;
   out_4725493332690452555[2] = 0;
   out_4725493332690452555[3] = 0;
   out_4725493332690452555[4] = 0;
   out_4725493332690452555[5] = 0;
   out_4725493332690452555[6] = 0;
   out_4725493332690452555[7] = 0;
   out_4725493332690452555[8] = 0;
}
void h_28(double *state, double *unused, double *out_3003422049209364661) {
   out_3003422049209364661[0] = state[0];
}
void H_28(double *state, double *unused, double *out_2761863061125126304) {
   out_2761863061125126304[0] = 1;
   out_2761863061125126304[1] = 0;
   out_2761863061125126304[2] = 0;
   out_2761863061125126304[3] = 0;
   out_2761863061125126304[4] = 0;
   out_2761863061125126304[5] = 0;
   out_2761863061125126304[6] = 0;
   out_2761863061125126304[7] = 0;
   out_2761863061125126304[8] = 0;
}
void h_31(double *state, double *unused, double *out_347223092600638419) {
   out_347223092600638419[0] = state[8];
}
void H_31(double *state, double *unused, double *out_677382385000276113) {
   out_677382385000276113[0] = 0;
   out_677382385000276113[1] = 0;
   out_677382385000276113[2] = 0;
   out_677382385000276113[3] = 0;
   out_677382385000276113[4] = 0;
   out_677382385000276113[5] = 0;
   out_677382385000276113[6] = 0;
   out_677382385000276113[7] = 0;
   out_677382385000276113[8] = 1;
}
#include <eigen3/Eigen/Dense>
#include <iostream>

typedef Eigen::Matrix<double, DIM, DIM, Eigen::RowMajor> DDM;
typedef Eigen::Matrix<double, EDIM, EDIM, Eigen::RowMajor> EEM;
typedef Eigen::Matrix<double, DIM, EDIM, Eigen::RowMajor> DEM;

void predict(double *in_x, double *in_P, double *in_Q, double dt) {
  typedef Eigen::Matrix<double, MEDIM, MEDIM, Eigen::RowMajor> RRM;

  double nx[DIM] = {0};
  double in_F[EDIM*EDIM] = {0};

  // functions from sympy
  f_fun(in_x, dt, nx);
  F_fun(in_x, dt, in_F);


  EEM F(in_F);
  EEM P(in_P);
  EEM Q(in_Q);

  RRM F_main = F.topLeftCorner(MEDIM, MEDIM);
  P.topLeftCorner(MEDIM, MEDIM) = (F_main * P.topLeftCorner(MEDIM, MEDIM)) * F_main.transpose();
  P.topRightCorner(MEDIM, EDIM - MEDIM) = F_main * P.topRightCorner(MEDIM, EDIM - MEDIM);
  P.bottomLeftCorner(EDIM - MEDIM, MEDIM) = P.bottomLeftCorner(EDIM - MEDIM, MEDIM) * F_main.transpose();

  P = P + dt*Q;

  // copy out state
  memcpy(in_x, nx, DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
}

// note: extra_args dim only correct when null space projecting
// otherwise 1
template <int ZDIM, int EADIM, bool MAHA_TEST>
void update(double *in_x, double *in_P, Hfun h_fun, Hfun H_fun, Hfun Hea_fun, double *in_z, double *in_R, double *in_ea, double MAHA_THRESHOLD) {
  typedef Eigen::Matrix<double, ZDIM, ZDIM, Eigen::RowMajor> ZZM;
  typedef Eigen::Matrix<double, ZDIM, DIM, Eigen::RowMajor> ZDM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, EDIM, Eigen::RowMajor> XEM;
  //typedef Eigen::Matrix<double, EDIM, ZDIM, Eigen::RowMajor> EZM;
  typedef Eigen::Matrix<double, Eigen::Dynamic, 1> X1M;
  typedef Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> XXM;

  double in_hx[ZDIM] = {0};
  double in_H[ZDIM * DIM] = {0};
  double in_H_mod[EDIM * DIM] = {0};
  double delta_x[EDIM] = {0};
  double x_new[DIM] = {0};


  // state x, P
  Eigen::Matrix<double, ZDIM, 1> z(in_z);
  EEM P(in_P);
  ZZM pre_R(in_R);

  // functions from sympy
  h_fun(in_x, in_ea, in_hx);
  H_fun(in_x, in_ea, in_H);
  ZDM pre_H(in_H);

  // get y (y = z - hx)
  Eigen::Matrix<double, ZDIM, 1> pre_y(in_hx); pre_y = z - pre_y;
  X1M y; XXM H; XXM R;
  if (Hea_fun){
    typedef Eigen::Matrix<double, ZDIM, EADIM, Eigen::RowMajor> ZAM;
    double in_Hea[ZDIM * EADIM] = {0};
    Hea_fun(in_x, in_ea, in_Hea);
    ZAM Hea(in_Hea);
    XXM A = Hea.transpose().fullPivLu().kernel();


    y = A.transpose() * pre_y;
    H = A.transpose() * pre_H;
    R = A.transpose() * pre_R * A;
  } else {
    y = pre_y;
    H = pre_H;
    R = pre_R;
  }
  // get modified H
  H_mod_fun(in_x, in_H_mod);
  DEM H_mod(in_H_mod);
  XEM H_err = H * H_mod;

  // Do mahalobis distance test
  if (MAHA_TEST){
    XXM a = (H_err * P * H_err.transpose() + R).inverse();
    double maha_dist = y.transpose() * a * y;
    if (maha_dist > MAHA_THRESHOLD){
      R = 1.0e16 * R;
    }
  }

  // Outlier resilient weighting
  double weight = 1;//(1.5)/(1 + y.squaredNorm()/R.sum());

  // kalman gains and I_KH
  XXM S = ((H_err * P) * H_err.transpose()) + R/weight;
  XEM KT = S.fullPivLu().solve(H_err * P.transpose());
  //EZM K = KT.transpose(); TODO: WHY DOES THIS NOT COMPILE?
  //EZM K = S.fullPivLu().solve(H_err * P.transpose()).transpose();
  //std::cout << "Here is the matrix rot:\n" << K << std::endl;
  EEM I_KH = Eigen::Matrix<double, EDIM, EDIM>::Identity() - (KT.transpose() * H_err);

  // update state by injecting dx
  Eigen::Matrix<double, EDIM, 1> dx(delta_x);
  dx  = (KT.transpose() * y);
  memcpy(delta_x, dx.data(), EDIM * sizeof(double));
  err_fun(in_x, delta_x, x_new);
  Eigen::Matrix<double, DIM, 1> x(x_new);

  // update cov
  P = ((I_KH * P) * I_KH.transpose()) + ((KT.transpose() * R) * KT);

  // copy out state
  memcpy(in_x, x.data(), DIM * sizeof(double));
  memcpy(in_P, P.data(), EDIM * EDIM * sizeof(double));
  memcpy(in_z, y.data(), y.rows() * sizeof(double));
}




}
extern "C" {

void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_25, H_25, NULL, in_z, in_R, in_ea, MAHA_THRESH_25);
}
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<2, 3, 0>(in_x, in_P, h_24, H_24, NULL, in_z, in_R, in_ea, MAHA_THRESH_24);
}
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_30, H_30, NULL, in_z, in_R, in_ea, MAHA_THRESH_30);
}
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_26, H_26, NULL, in_z, in_R, in_ea, MAHA_THRESH_26);
}
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_27, H_27, NULL, in_z, in_R, in_ea, MAHA_THRESH_27);
}
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_29, H_29, NULL, in_z, in_R, in_ea, MAHA_THRESH_29);
}
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_28, H_28, NULL, in_z, in_R, in_ea, MAHA_THRESH_28);
}
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<1, 3, 0>(in_x, in_P, h_31, H_31, NULL, in_z, in_R, in_ea, MAHA_THRESH_31);
}
void car_err_fun(double *nom_x, double *delta_x, double *out_3001115831286256944) {
  err_fun(nom_x, delta_x, out_3001115831286256944);
}
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1270014336152208748) {
  inv_err_fun(nom_x, true_x, out_1270014336152208748);
}
void car_H_mod_fun(double *state, double *out_4231008996866728218) {
  H_mod_fun(state, out_4231008996866728218);
}
void car_f_fun(double *state, double dt, double *out_3852593309757630922) {
  f_fun(state,  dt, out_3852593309757630922);
}
void car_F_fun(double *state, double dt, double *out_8513599791742951419) {
  F_fun(state,  dt, out_8513599791742951419);
}
void car_h_25(double *state, double *unused, double *out_8228495743354687604) {
  h_25(state, unused, out_8228495743354687604);
}
void car_H_25(double *state, double *unused, double *out_708028346877236541) {
  H_25(state, unused, out_708028346877236541);
}
void car_h_24(double *state, double *unused, double *out_5374008317928448214) {
  h_24(state, unused, out_5374008317928448214);
}
void car_H_24(double *state, double *unused, double *out_2849897251925086595) {
  H_24(state, unused, out_2849897251925086595);
}
void car_h_30(double *state, double *unused, double *out_6580798802100841662) {
  h_30(state, unused, out_6580798802100841662);
}
void car_H_30(double *state, double *unused, double *out_6208661994614380214) {
  H_30(state, unused, out_6208661994614380214);
}
void car_h_26(double *state, double *unused, double *out_6223215400527753119) {
  h_26(state, unused, out_6223215400527753119);
}
void car_H_26(double *state, double *unused, double *out_4449531665751292765) {
  H_26(state, unused, out_4449531665751292765);
}
void car_h_27(double *state, double *unused, double *out_8986345888515506414) {
  h_27(state, unused, out_8986345888515506414);
}
void car_H_27(double *state, double *unused, double *out_3012130605820901522) {
  H_27(state, unused, out_3012130605820901522);
}
void car_h_29(double *state, double *unused, double *out_1579125534439996017) {
  h_29(state, unused, out_1579125534439996017);
}
void car_H_29(double *state, double *unused, double *out_4725493332690452555) {
  H_29(state, unused, out_4725493332690452555);
}
void car_h_28(double *state, double *unused, double *out_3003422049209364661) {
  h_28(state, unused, out_3003422049209364661);
}
void car_H_28(double *state, double *unused, double *out_2761863061125126304) {
  H_28(state, unused, out_2761863061125126304);
}
void car_h_31(double *state, double *unused, double *out_347223092600638419) {
  h_31(state, unused, out_347223092600638419);
}
void car_H_31(double *state, double *unused, double *out_677382385000276113) {
  H_31(state, unused, out_677382385000276113);
}
void car_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
void car_set_mass(double x) {
  set_mass(x);
}
void car_set_rotational_inertia(double x) {
  set_rotational_inertia(x);
}
void car_set_center_to_front(double x) {
  set_center_to_front(x);
}
void car_set_center_to_rear(double x) {
  set_center_to_rear(x);
}
void car_set_stiffness_front(double x) {
  set_stiffness_front(x);
}
void car_set_stiffness_rear(double x) {
  set_stiffness_rear(x);
}
}

const EKF car = {
  .name = "car",
  .kinds = { 25, 24, 30, 26, 27, 29, 28, 31 },
  .feature_kinds = {  },
  .f_fun = car_f_fun,
  .F_fun = car_F_fun,
  .err_fun = car_err_fun,
  .inv_err_fun = car_inv_err_fun,
  .H_mod_fun = car_H_mod_fun,
  .predict = car_predict,
  .hs = {
    { 25, car_h_25 },
    { 24, car_h_24 },
    { 30, car_h_30 },
    { 26, car_h_26 },
    { 27, car_h_27 },
    { 29, car_h_29 },
    { 28, car_h_28 },
    { 31, car_h_31 },
  },
  .Hs = {
    { 25, car_H_25 },
    { 24, car_H_24 },
    { 30, car_H_30 },
    { 26, car_H_26 },
    { 27, car_H_27 },
    { 29, car_H_29 },
    { 28, car_H_28 },
    { 31, car_H_31 },
  },
  .updates = {
    { 25, car_update_25 },
    { 24, car_update_24 },
    { 30, car_update_30 },
    { 26, car_update_26 },
    { 27, car_update_27 },
    { 29, car_update_29 },
    { 28, car_update_28 },
    { 31, car_update_31 },
  },
  .Hes = {
  },
  .sets = {
    { "mass", car_set_mass },
    { "rotational_inertia", car_set_rotational_inertia },
    { "center_to_front", car_set_center_to_front },
    { "center_to_rear", car_set_center_to_rear },
    { "stiffness_front", car_set_stiffness_front },
    { "stiffness_rear", car_set_stiffness_rear },
  },
  .extra_routines = {
  },
};

ekf_lib_init(car)
