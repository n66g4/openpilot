#pragma once
#include "rednose/helpers/ekf.h"
extern "C" {
void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea);
void pose_err_fun(double *nom_x, double *delta_x, double *out_6708923800649560446);
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9014600873516389172);
void pose_H_mod_fun(double *state, double *out_8328883818606545664);
void pose_f_fun(double *state, double dt, double *out_1076920247808564320);
void pose_F_fun(double *state, double dt, double *out_2893180287114808269);
void pose_h_4(double *state, double *unused, double *out_4396317536037036464);
void pose_H_4(double *state, double *unused, double *out_2480802011854586943);
void pose_h_10(double *state, double *unused, double *out_5916006405786623891);
void pose_H_10(double *state, double *unused, double *out_155472375533809188);
void pose_h_13(double *state, double *unused, double *out_3965618053748515748);
void pose_H_13(double *state, double *unused, double *out_5693075837186919744);
void pose_h_14(double *state, double *unused, double *out_6554627370156003242);
void pose_H_14(double *state, double *unused, double *out_2045685485209703344);
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt);
}