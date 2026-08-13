#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void car_update_25(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_24(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_30(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_26(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_27(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_29(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_28(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_update_31(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void car_err_fun(double *nom_x, double *delta_x, double *out_3001115831286256944);
void car_inv_err_fun(double *nom_x, double *true_x, double *out_1270014336152208748);
void car_H_mod_fun(double *state, double *out_4231008996866728218);
void car_f_fun(double *state, double dt, double *out_3852593309757630922);
void car_F_fun(double *state, double dt, double *out_8513599791742951419);
void car_h_25(double *state, double *unused, double *out_8228495743354687604);
void car_H_25(double *state, double *unused, double *out_708028346877236541);
void car_h_24(double *state, double *unused, double *out_5374008317928448214);
void car_H_24(double *state, double *unused, double *out_2849897251925086595);
void car_h_30(double *state, double *unused, double *out_6580798802100841662);
void car_H_30(double *state, double *unused, double *out_6208661994614380214);
void car_h_26(double *state, double *unused, double *out_6223215400527753119);
void car_H_26(double *state, double *unused, double *out_4449531665751292765);
void car_h_27(double *state, double *unused, double *out_8986345888515506414);
void car_H_27(double *state, double *unused, double *out_3012130605820901522);
void car_h_29(double *state, double *unused, double *out_1579125534439996017);
void car_H_29(double *state, double *unused, double *out_4725493332690452555);
void car_h_28(double *state, double *unused, double *out_3003422049209364661);
void car_H_28(double *state, double *unused, double *out_2761863061125126304);
void car_h_31(double *state, double *unused, double *out_347223092600638419);
void car_H_31(double *state, double *unused, double *out_677382385000276113);
void car_predict(double *in_x, double *in_P, double *in_Q, double dt);
void car_set_mass(double x);
void car_set_rotational_inertia(double x);
void car_set_center_to_front(double x);
void car_set_center_to_rear(double x);
void car_set_stiffness_front(double x);
void car_set_stiffness_rear(double x);
}