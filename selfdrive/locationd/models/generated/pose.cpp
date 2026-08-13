#include "pose.h"

namespace {
#define DIM 18
#define EDIM 18
#define MEDIM 18
typedef void (*Hfun)(double *, double *, double *);
const static double MAHA_THRESH_4 = 7.814727903251177;
const static double MAHA_THRESH_10 = 7.814727903251177;
const static double MAHA_THRESH_13 = 7.814727903251177;
const static double MAHA_THRESH_14 = 7.814727903251177;

/******************************************************************************
 *                      Code generated with SymPy 1.14.0                      *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                         This file is part of 'ekf'                         *
 ******************************************************************************/
void err_fun(double *nom_x, double *delta_x, double *out_6708923800649560446) {
   out_6708923800649560446[0] = delta_x[0] + nom_x[0];
   out_6708923800649560446[1] = delta_x[1] + nom_x[1];
   out_6708923800649560446[2] = delta_x[2] + nom_x[2];
   out_6708923800649560446[3] = delta_x[3] + nom_x[3];
   out_6708923800649560446[4] = delta_x[4] + nom_x[4];
   out_6708923800649560446[5] = delta_x[5] + nom_x[5];
   out_6708923800649560446[6] = delta_x[6] + nom_x[6];
   out_6708923800649560446[7] = delta_x[7] + nom_x[7];
   out_6708923800649560446[8] = delta_x[8] + nom_x[8];
   out_6708923800649560446[9] = delta_x[9] + nom_x[9];
   out_6708923800649560446[10] = delta_x[10] + nom_x[10];
   out_6708923800649560446[11] = delta_x[11] + nom_x[11];
   out_6708923800649560446[12] = delta_x[12] + nom_x[12];
   out_6708923800649560446[13] = delta_x[13] + nom_x[13];
   out_6708923800649560446[14] = delta_x[14] + nom_x[14];
   out_6708923800649560446[15] = delta_x[15] + nom_x[15];
   out_6708923800649560446[16] = delta_x[16] + nom_x[16];
   out_6708923800649560446[17] = delta_x[17] + nom_x[17];
}
void inv_err_fun(double *nom_x, double *true_x, double *out_9014600873516389172) {
   out_9014600873516389172[0] = -nom_x[0] + true_x[0];
   out_9014600873516389172[1] = -nom_x[1] + true_x[1];
   out_9014600873516389172[2] = -nom_x[2] + true_x[2];
   out_9014600873516389172[3] = -nom_x[3] + true_x[3];
   out_9014600873516389172[4] = -nom_x[4] + true_x[4];
   out_9014600873516389172[5] = -nom_x[5] + true_x[5];
   out_9014600873516389172[6] = -nom_x[6] + true_x[6];
   out_9014600873516389172[7] = -nom_x[7] + true_x[7];
   out_9014600873516389172[8] = -nom_x[8] + true_x[8];
   out_9014600873516389172[9] = -nom_x[9] + true_x[9];
   out_9014600873516389172[10] = -nom_x[10] + true_x[10];
   out_9014600873516389172[11] = -nom_x[11] + true_x[11];
   out_9014600873516389172[12] = -nom_x[12] + true_x[12];
   out_9014600873516389172[13] = -nom_x[13] + true_x[13];
   out_9014600873516389172[14] = -nom_x[14] + true_x[14];
   out_9014600873516389172[15] = -nom_x[15] + true_x[15];
   out_9014600873516389172[16] = -nom_x[16] + true_x[16];
   out_9014600873516389172[17] = -nom_x[17] + true_x[17];
}
void H_mod_fun(double *state, double *out_8328883818606545664) {
   out_8328883818606545664[0] = 1.0;
   out_8328883818606545664[1] = 0.0;
   out_8328883818606545664[2] = 0.0;
   out_8328883818606545664[3] = 0.0;
   out_8328883818606545664[4] = 0.0;
   out_8328883818606545664[5] = 0.0;
   out_8328883818606545664[6] = 0.0;
   out_8328883818606545664[7] = 0.0;
   out_8328883818606545664[8] = 0.0;
   out_8328883818606545664[9] = 0.0;
   out_8328883818606545664[10] = 0.0;
   out_8328883818606545664[11] = 0.0;
   out_8328883818606545664[12] = 0.0;
   out_8328883818606545664[13] = 0.0;
   out_8328883818606545664[14] = 0.0;
   out_8328883818606545664[15] = 0.0;
   out_8328883818606545664[16] = 0.0;
   out_8328883818606545664[17] = 0.0;
   out_8328883818606545664[18] = 0.0;
   out_8328883818606545664[19] = 1.0;
   out_8328883818606545664[20] = 0.0;
   out_8328883818606545664[21] = 0.0;
   out_8328883818606545664[22] = 0.0;
   out_8328883818606545664[23] = 0.0;
   out_8328883818606545664[24] = 0.0;
   out_8328883818606545664[25] = 0.0;
   out_8328883818606545664[26] = 0.0;
   out_8328883818606545664[27] = 0.0;
   out_8328883818606545664[28] = 0.0;
   out_8328883818606545664[29] = 0.0;
   out_8328883818606545664[30] = 0.0;
   out_8328883818606545664[31] = 0.0;
   out_8328883818606545664[32] = 0.0;
   out_8328883818606545664[33] = 0.0;
   out_8328883818606545664[34] = 0.0;
   out_8328883818606545664[35] = 0.0;
   out_8328883818606545664[36] = 0.0;
   out_8328883818606545664[37] = 0.0;
   out_8328883818606545664[38] = 1.0;
   out_8328883818606545664[39] = 0.0;
   out_8328883818606545664[40] = 0.0;
   out_8328883818606545664[41] = 0.0;
   out_8328883818606545664[42] = 0.0;
   out_8328883818606545664[43] = 0.0;
   out_8328883818606545664[44] = 0.0;
   out_8328883818606545664[45] = 0.0;
   out_8328883818606545664[46] = 0.0;
   out_8328883818606545664[47] = 0.0;
   out_8328883818606545664[48] = 0.0;
   out_8328883818606545664[49] = 0.0;
   out_8328883818606545664[50] = 0.0;
   out_8328883818606545664[51] = 0.0;
   out_8328883818606545664[52] = 0.0;
   out_8328883818606545664[53] = 0.0;
   out_8328883818606545664[54] = 0.0;
   out_8328883818606545664[55] = 0.0;
   out_8328883818606545664[56] = 0.0;
   out_8328883818606545664[57] = 1.0;
   out_8328883818606545664[58] = 0.0;
   out_8328883818606545664[59] = 0.0;
   out_8328883818606545664[60] = 0.0;
   out_8328883818606545664[61] = 0.0;
   out_8328883818606545664[62] = 0.0;
   out_8328883818606545664[63] = 0.0;
   out_8328883818606545664[64] = 0.0;
   out_8328883818606545664[65] = 0.0;
   out_8328883818606545664[66] = 0.0;
   out_8328883818606545664[67] = 0.0;
   out_8328883818606545664[68] = 0.0;
   out_8328883818606545664[69] = 0.0;
   out_8328883818606545664[70] = 0.0;
   out_8328883818606545664[71] = 0.0;
   out_8328883818606545664[72] = 0.0;
   out_8328883818606545664[73] = 0.0;
   out_8328883818606545664[74] = 0.0;
   out_8328883818606545664[75] = 0.0;
   out_8328883818606545664[76] = 1.0;
   out_8328883818606545664[77] = 0.0;
   out_8328883818606545664[78] = 0.0;
   out_8328883818606545664[79] = 0.0;
   out_8328883818606545664[80] = 0.0;
   out_8328883818606545664[81] = 0.0;
   out_8328883818606545664[82] = 0.0;
   out_8328883818606545664[83] = 0.0;
   out_8328883818606545664[84] = 0.0;
   out_8328883818606545664[85] = 0.0;
   out_8328883818606545664[86] = 0.0;
   out_8328883818606545664[87] = 0.0;
   out_8328883818606545664[88] = 0.0;
   out_8328883818606545664[89] = 0.0;
   out_8328883818606545664[90] = 0.0;
   out_8328883818606545664[91] = 0.0;
   out_8328883818606545664[92] = 0.0;
   out_8328883818606545664[93] = 0.0;
   out_8328883818606545664[94] = 0.0;
   out_8328883818606545664[95] = 1.0;
   out_8328883818606545664[96] = 0.0;
   out_8328883818606545664[97] = 0.0;
   out_8328883818606545664[98] = 0.0;
   out_8328883818606545664[99] = 0.0;
   out_8328883818606545664[100] = 0.0;
   out_8328883818606545664[101] = 0.0;
   out_8328883818606545664[102] = 0.0;
   out_8328883818606545664[103] = 0.0;
   out_8328883818606545664[104] = 0.0;
   out_8328883818606545664[105] = 0.0;
   out_8328883818606545664[106] = 0.0;
   out_8328883818606545664[107] = 0.0;
   out_8328883818606545664[108] = 0.0;
   out_8328883818606545664[109] = 0.0;
   out_8328883818606545664[110] = 0.0;
   out_8328883818606545664[111] = 0.0;
   out_8328883818606545664[112] = 0.0;
   out_8328883818606545664[113] = 0.0;
   out_8328883818606545664[114] = 1.0;
   out_8328883818606545664[115] = 0.0;
   out_8328883818606545664[116] = 0.0;
   out_8328883818606545664[117] = 0.0;
   out_8328883818606545664[118] = 0.0;
   out_8328883818606545664[119] = 0.0;
   out_8328883818606545664[120] = 0.0;
   out_8328883818606545664[121] = 0.0;
   out_8328883818606545664[122] = 0.0;
   out_8328883818606545664[123] = 0.0;
   out_8328883818606545664[124] = 0.0;
   out_8328883818606545664[125] = 0.0;
   out_8328883818606545664[126] = 0.0;
   out_8328883818606545664[127] = 0.0;
   out_8328883818606545664[128] = 0.0;
   out_8328883818606545664[129] = 0.0;
   out_8328883818606545664[130] = 0.0;
   out_8328883818606545664[131] = 0.0;
   out_8328883818606545664[132] = 0.0;
   out_8328883818606545664[133] = 1.0;
   out_8328883818606545664[134] = 0.0;
   out_8328883818606545664[135] = 0.0;
   out_8328883818606545664[136] = 0.0;
   out_8328883818606545664[137] = 0.0;
   out_8328883818606545664[138] = 0.0;
   out_8328883818606545664[139] = 0.0;
   out_8328883818606545664[140] = 0.0;
   out_8328883818606545664[141] = 0.0;
   out_8328883818606545664[142] = 0.0;
   out_8328883818606545664[143] = 0.0;
   out_8328883818606545664[144] = 0.0;
   out_8328883818606545664[145] = 0.0;
   out_8328883818606545664[146] = 0.0;
   out_8328883818606545664[147] = 0.0;
   out_8328883818606545664[148] = 0.0;
   out_8328883818606545664[149] = 0.0;
   out_8328883818606545664[150] = 0.0;
   out_8328883818606545664[151] = 0.0;
   out_8328883818606545664[152] = 1.0;
   out_8328883818606545664[153] = 0.0;
   out_8328883818606545664[154] = 0.0;
   out_8328883818606545664[155] = 0.0;
   out_8328883818606545664[156] = 0.0;
   out_8328883818606545664[157] = 0.0;
   out_8328883818606545664[158] = 0.0;
   out_8328883818606545664[159] = 0.0;
   out_8328883818606545664[160] = 0.0;
   out_8328883818606545664[161] = 0.0;
   out_8328883818606545664[162] = 0.0;
   out_8328883818606545664[163] = 0.0;
   out_8328883818606545664[164] = 0.0;
   out_8328883818606545664[165] = 0.0;
   out_8328883818606545664[166] = 0.0;
   out_8328883818606545664[167] = 0.0;
   out_8328883818606545664[168] = 0.0;
   out_8328883818606545664[169] = 0.0;
   out_8328883818606545664[170] = 0.0;
   out_8328883818606545664[171] = 1.0;
   out_8328883818606545664[172] = 0.0;
   out_8328883818606545664[173] = 0.0;
   out_8328883818606545664[174] = 0.0;
   out_8328883818606545664[175] = 0.0;
   out_8328883818606545664[176] = 0.0;
   out_8328883818606545664[177] = 0.0;
   out_8328883818606545664[178] = 0.0;
   out_8328883818606545664[179] = 0.0;
   out_8328883818606545664[180] = 0.0;
   out_8328883818606545664[181] = 0.0;
   out_8328883818606545664[182] = 0.0;
   out_8328883818606545664[183] = 0.0;
   out_8328883818606545664[184] = 0.0;
   out_8328883818606545664[185] = 0.0;
   out_8328883818606545664[186] = 0.0;
   out_8328883818606545664[187] = 0.0;
   out_8328883818606545664[188] = 0.0;
   out_8328883818606545664[189] = 0.0;
   out_8328883818606545664[190] = 1.0;
   out_8328883818606545664[191] = 0.0;
   out_8328883818606545664[192] = 0.0;
   out_8328883818606545664[193] = 0.0;
   out_8328883818606545664[194] = 0.0;
   out_8328883818606545664[195] = 0.0;
   out_8328883818606545664[196] = 0.0;
   out_8328883818606545664[197] = 0.0;
   out_8328883818606545664[198] = 0.0;
   out_8328883818606545664[199] = 0.0;
   out_8328883818606545664[200] = 0.0;
   out_8328883818606545664[201] = 0.0;
   out_8328883818606545664[202] = 0.0;
   out_8328883818606545664[203] = 0.0;
   out_8328883818606545664[204] = 0.0;
   out_8328883818606545664[205] = 0.0;
   out_8328883818606545664[206] = 0.0;
   out_8328883818606545664[207] = 0.0;
   out_8328883818606545664[208] = 0.0;
   out_8328883818606545664[209] = 1.0;
   out_8328883818606545664[210] = 0.0;
   out_8328883818606545664[211] = 0.0;
   out_8328883818606545664[212] = 0.0;
   out_8328883818606545664[213] = 0.0;
   out_8328883818606545664[214] = 0.0;
   out_8328883818606545664[215] = 0.0;
   out_8328883818606545664[216] = 0.0;
   out_8328883818606545664[217] = 0.0;
   out_8328883818606545664[218] = 0.0;
   out_8328883818606545664[219] = 0.0;
   out_8328883818606545664[220] = 0.0;
   out_8328883818606545664[221] = 0.0;
   out_8328883818606545664[222] = 0.0;
   out_8328883818606545664[223] = 0.0;
   out_8328883818606545664[224] = 0.0;
   out_8328883818606545664[225] = 0.0;
   out_8328883818606545664[226] = 0.0;
   out_8328883818606545664[227] = 0.0;
   out_8328883818606545664[228] = 1.0;
   out_8328883818606545664[229] = 0.0;
   out_8328883818606545664[230] = 0.0;
   out_8328883818606545664[231] = 0.0;
   out_8328883818606545664[232] = 0.0;
   out_8328883818606545664[233] = 0.0;
   out_8328883818606545664[234] = 0.0;
   out_8328883818606545664[235] = 0.0;
   out_8328883818606545664[236] = 0.0;
   out_8328883818606545664[237] = 0.0;
   out_8328883818606545664[238] = 0.0;
   out_8328883818606545664[239] = 0.0;
   out_8328883818606545664[240] = 0.0;
   out_8328883818606545664[241] = 0.0;
   out_8328883818606545664[242] = 0.0;
   out_8328883818606545664[243] = 0.0;
   out_8328883818606545664[244] = 0.0;
   out_8328883818606545664[245] = 0.0;
   out_8328883818606545664[246] = 0.0;
   out_8328883818606545664[247] = 1.0;
   out_8328883818606545664[248] = 0.0;
   out_8328883818606545664[249] = 0.0;
   out_8328883818606545664[250] = 0.0;
   out_8328883818606545664[251] = 0.0;
   out_8328883818606545664[252] = 0.0;
   out_8328883818606545664[253] = 0.0;
   out_8328883818606545664[254] = 0.0;
   out_8328883818606545664[255] = 0.0;
   out_8328883818606545664[256] = 0.0;
   out_8328883818606545664[257] = 0.0;
   out_8328883818606545664[258] = 0.0;
   out_8328883818606545664[259] = 0.0;
   out_8328883818606545664[260] = 0.0;
   out_8328883818606545664[261] = 0.0;
   out_8328883818606545664[262] = 0.0;
   out_8328883818606545664[263] = 0.0;
   out_8328883818606545664[264] = 0.0;
   out_8328883818606545664[265] = 0.0;
   out_8328883818606545664[266] = 1.0;
   out_8328883818606545664[267] = 0.0;
   out_8328883818606545664[268] = 0.0;
   out_8328883818606545664[269] = 0.0;
   out_8328883818606545664[270] = 0.0;
   out_8328883818606545664[271] = 0.0;
   out_8328883818606545664[272] = 0.0;
   out_8328883818606545664[273] = 0.0;
   out_8328883818606545664[274] = 0.0;
   out_8328883818606545664[275] = 0.0;
   out_8328883818606545664[276] = 0.0;
   out_8328883818606545664[277] = 0.0;
   out_8328883818606545664[278] = 0.0;
   out_8328883818606545664[279] = 0.0;
   out_8328883818606545664[280] = 0.0;
   out_8328883818606545664[281] = 0.0;
   out_8328883818606545664[282] = 0.0;
   out_8328883818606545664[283] = 0.0;
   out_8328883818606545664[284] = 0.0;
   out_8328883818606545664[285] = 1.0;
   out_8328883818606545664[286] = 0.0;
   out_8328883818606545664[287] = 0.0;
   out_8328883818606545664[288] = 0.0;
   out_8328883818606545664[289] = 0.0;
   out_8328883818606545664[290] = 0.0;
   out_8328883818606545664[291] = 0.0;
   out_8328883818606545664[292] = 0.0;
   out_8328883818606545664[293] = 0.0;
   out_8328883818606545664[294] = 0.0;
   out_8328883818606545664[295] = 0.0;
   out_8328883818606545664[296] = 0.0;
   out_8328883818606545664[297] = 0.0;
   out_8328883818606545664[298] = 0.0;
   out_8328883818606545664[299] = 0.0;
   out_8328883818606545664[300] = 0.0;
   out_8328883818606545664[301] = 0.0;
   out_8328883818606545664[302] = 0.0;
   out_8328883818606545664[303] = 0.0;
   out_8328883818606545664[304] = 1.0;
   out_8328883818606545664[305] = 0.0;
   out_8328883818606545664[306] = 0.0;
   out_8328883818606545664[307] = 0.0;
   out_8328883818606545664[308] = 0.0;
   out_8328883818606545664[309] = 0.0;
   out_8328883818606545664[310] = 0.0;
   out_8328883818606545664[311] = 0.0;
   out_8328883818606545664[312] = 0.0;
   out_8328883818606545664[313] = 0.0;
   out_8328883818606545664[314] = 0.0;
   out_8328883818606545664[315] = 0.0;
   out_8328883818606545664[316] = 0.0;
   out_8328883818606545664[317] = 0.0;
   out_8328883818606545664[318] = 0.0;
   out_8328883818606545664[319] = 0.0;
   out_8328883818606545664[320] = 0.0;
   out_8328883818606545664[321] = 0.0;
   out_8328883818606545664[322] = 0.0;
   out_8328883818606545664[323] = 1.0;
}
void f_fun(double *state, double dt, double *out_1076920247808564320) {
   out_1076920247808564320[0] = atan2((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), -(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]));
   out_1076920247808564320[1] = asin(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]));
   out_1076920247808564320[2] = atan2(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), -(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]));
   out_1076920247808564320[3] = dt*state[12] + state[3];
   out_1076920247808564320[4] = dt*state[13] + state[4];
   out_1076920247808564320[5] = dt*state[14] + state[5];
   out_1076920247808564320[6] = state[6];
   out_1076920247808564320[7] = state[7];
   out_1076920247808564320[8] = state[8];
   out_1076920247808564320[9] = state[9];
   out_1076920247808564320[10] = state[10];
   out_1076920247808564320[11] = state[11];
   out_1076920247808564320[12] = state[12];
   out_1076920247808564320[13] = state[13];
   out_1076920247808564320[14] = state[14];
   out_1076920247808564320[15] = state[15];
   out_1076920247808564320[16] = state[16];
   out_1076920247808564320[17] = state[17];
}
void F_fun(double *state, double dt, double *out_2893180287114808269) {
   out_2893180287114808269[0] = ((-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*cos(state[0])*cos(state[1]) - sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*cos(state[0])*cos(state[1]) - sin(dt*state[6])*sin(state[0])*cos(dt*state[7])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2893180287114808269[1] = ((-sin(dt*state[6])*sin(dt*state[8]) - sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*cos(state[1]) - (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*sin(state[1]) - sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(state[0]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*sin(state[1]) + (-sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) + sin(dt*state[8])*cos(dt*state[6]))*cos(state[1]) - sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(state[0]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2893180287114808269[2] = 0;
   out_2893180287114808269[3] = 0;
   out_2893180287114808269[4] = 0;
   out_2893180287114808269[5] = 0;
   out_2893180287114808269[6] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(dt*cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) - dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2893180287114808269[7] = (-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[6])*sin(dt*state[7])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[6])*sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) - dt*sin(dt*state[6])*sin(state[1])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + (-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))*(-dt*sin(dt*state[7])*cos(dt*state[6])*cos(state[0])*cos(state[1]) + dt*sin(dt*state[8])*sin(state[0])*cos(dt*state[6])*cos(dt*state[7])*cos(state[1]) - dt*sin(state[1])*cos(dt*state[6])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2893180287114808269[8] = ((dt*sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + dt*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (dt*sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]))*(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2)) + ((dt*sin(dt*state[6])*sin(dt*state[8]) + dt*sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (-dt*sin(dt*state[6])*cos(dt*state[8]) + dt*sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]))*(-(sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) + (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) - sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/(pow(-(sin(dt*state[6])*sin(dt*state[8]) + sin(dt*state[7])*cos(dt*state[6])*cos(dt*state[8]))*sin(state[1]) + (-sin(dt*state[6])*cos(dt*state[8]) + sin(dt*state[7])*sin(dt*state[8])*cos(dt*state[6]))*sin(state[0])*cos(state[1]) + cos(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2) + pow((sin(dt*state[6])*sin(dt*state[7])*sin(dt*state[8]) + cos(dt*state[6])*cos(dt*state[8]))*sin(state[0])*cos(state[1]) - (sin(dt*state[6])*sin(dt*state[7])*cos(dt*state[8]) - sin(dt*state[8])*cos(dt*state[6]))*sin(state[1]) + sin(dt*state[6])*cos(dt*state[7])*cos(state[0])*cos(state[1]), 2));
   out_2893180287114808269[9] = 0;
   out_2893180287114808269[10] = 0;
   out_2893180287114808269[11] = 0;
   out_2893180287114808269[12] = 0;
   out_2893180287114808269[13] = 0;
   out_2893180287114808269[14] = 0;
   out_2893180287114808269[15] = 0;
   out_2893180287114808269[16] = 0;
   out_2893180287114808269[17] = 0;
   out_2893180287114808269[18] = (-sin(dt*state[7])*sin(state[0])*cos(state[1]) - sin(dt*state[8])*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2893180287114808269[19] = (-sin(dt*state[7])*sin(state[1])*cos(state[0]) + sin(dt*state[8])*sin(state[0])*sin(state[1])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2893180287114808269[20] = 0;
   out_2893180287114808269[21] = 0;
   out_2893180287114808269[22] = 0;
   out_2893180287114808269[23] = 0;
   out_2893180287114808269[24] = 0;
   out_2893180287114808269[25] = (dt*sin(dt*state[7])*sin(dt*state[8])*sin(state[0])*cos(state[1]) - dt*sin(dt*state[7])*sin(state[1])*cos(dt*state[8]) + dt*cos(dt*state[7])*cos(state[0])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2893180287114808269[26] = (-dt*sin(dt*state[8])*sin(state[1])*cos(dt*state[7]) - dt*sin(state[0])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/sqrt(1 - pow(sin(dt*state[7])*cos(state[0])*cos(state[1]) - sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1]) + sin(state[1])*cos(dt*state[7])*cos(dt*state[8]), 2));
   out_2893180287114808269[27] = 0;
   out_2893180287114808269[28] = 0;
   out_2893180287114808269[29] = 0;
   out_2893180287114808269[30] = 0;
   out_2893180287114808269[31] = 0;
   out_2893180287114808269[32] = 0;
   out_2893180287114808269[33] = 0;
   out_2893180287114808269[34] = 0;
   out_2893180287114808269[35] = 0;
   out_2893180287114808269[36] = ((sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2893180287114808269[37] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-sin(dt*state[7])*sin(state[2])*cos(state[0])*cos(state[1]) + sin(dt*state[8])*sin(state[0])*sin(state[2])*cos(dt*state[7])*cos(state[1]) - sin(state[1])*sin(state[2])*cos(dt*state[7])*cos(dt*state[8]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(-sin(dt*state[7])*cos(state[0])*cos(state[1])*cos(state[2]) + sin(dt*state[8])*sin(state[0])*cos(dt*state[7])*cos(state[1])*cos(state[2]) - sin(state[1])*cos(dt*state[7])*cos(dt*state[8])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2893180287114808269[38] = ((-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (-sin(state[0])*sin(state[1])*sin(state[2]) - cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2893180287114808269[39] = 0;
   out_2893180287114808269[40] = 0;
   out_2893180287114808269[41] = 0;
   out_2893180287114808269[42] = 0;
   out_2893180287114808269[43] = (-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))*(dt*(sin(state[0])*cos(state[2]) - sin(state[1])*sin(state[2])*cos(state[0]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*sin(state[2])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + ((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))*(dt*(-sin(state[0])*sin(state[2]) - sin(state[1])*cos(state[0])*cos(state[2]))*cos(dt*state[7]) - dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[7])*sin(dt*state[8]) - dt*sin(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2893180287114808269[44] = (dt*(sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*sin(state[2])*cos(dt*state[7])*cos(state[1]))*(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2)) + (dt*(sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*cos(dt*state[7])*cos(dt*state[8]) - dt*sin(dt*state[8])*cos(dt*state[7])*cos(state[1])*cos(state[2]))*((-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) - (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) - sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]))/(pow(-(sin(state[0])*sin(state[2]) + sin(state[1])*cos(state[0])*cos(state[2]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*cos(state[2]) - sin(state[2])*cos(state[0]))*sin(dt*state[8])*cos(dt*state[7]) + cos(dt*state[7])*cos(dt*state[8])*cos(state[1])*cos(state[2]), 2) + pow(-(-sin(state[0])*cos(state[2]) + sin(state[1])*sin(state[2])*cos(state[0]))*sin(dt*state[7]) + (sin(state[0])*sin(state[1])*sin(state[2]) + cos(state[0])*cos(state[2]))*sin(dt*state[8])*cos(dt*state[7]) + sin(state[2])*cos(dt*state[7])*cos(dt*state[8])*cos(state[1]), 2));
   out_2893180287114808269[45] = 0;
   out_2893180287114808269[46] = 0;
   out_2893180287114808269[47] = 0;
   out_2893180287114808269[48] = 0;
   out_2893180287114808269[49] = 0;
   out_2893180287114808269[50] = 0;
   out_2893180287114808269[51] = 0;
   out_2893180287114808269[52] = 0;
   out_2893180287114808269[53] = 0;
   out_2893180287114808269[54] = 0;
   out_2893180287114808269[55] = 0;
   out_2893180287114808269[56] = 0;
   out_2893180287114808269[57] = 1;
   out_2893180287114808269[58] = 0;
   out_2893180287114808269[59] = 0;
   out_2893180287114808269[60] = 0;
   out_2893180287114808269[61] = 0;
   out_2893180287114808269[62] = 0;
   out_2893180287114808269[63] = 0;
   out_2893180287114808269[64] = 0;
   out_2893180287114808269[65] = 0;
   out_2893180287114808269[66] = dt;
   out_2893180287114808269[67] = 0;
   out_2893180287114808269[68] = 0;
   out_2893180287114808269[69] = 0;
   out_2893180287114808269[70] = 0;
   out_2893180287114808269[71] = 0;
   out_2893180287114808269[72] = 0;
   out_2893180287114808269[73] = 0;
   out_2893180287114808269[74] = 0;
   out_2893180287114808269[75] = 0;
   out_2893180287114808269[76] = 1;
   out_2893180287114808269[77] = 0;
   out_2893180287114808269[78] = 0;
   out_2893180287114808269[79] = 0;
   out_2893180287114808269[80] = 0;
   out_2893180287114808269[81] = 0;
   out_2893180287114808269[82] = 0;
   out_2893180287114808269[83] = 0;
   out_2893180287114808269[84] = 0;
   out_2893180287114808269[85] = dt;
   out_2893180287114808269[86] = 0;
   out_2893180287114808269[87] = 0;
   out_2893180287114808269[88] = 0;
   out_2893180287114808269[89] = 0;
   out_2893180287114808269[90] = 0;
   out_2893180287114808269[91] = 0;
   out_2893180287114808269[92] = 0;
   out_2893180287114808269[93] = 0;
   out_2893180287114808269[94] = 0;
   out_2893180287114808269[95] = 1;
   out_2893180287114808269[96] = 0;
   out_2893180287114808269[97] = 0;
   out_2893180287114808269[98] = 0;
   out_2893180287114808269[99] = 0;
   out_2893180287114808269[100] = 0;
   out_2893180287114808269[101] = 0;
   out_2893180287114808269[102] = 0;
   out_2893180287114808269[103] = 0;
   out_2893180287114808269[104] = dt;
   out_2893180287114808269[105] = 0;
   out_2893180287114808269[106] = 0;
   out_2893180287114808269[107] = 0;
   out_2893180287114808269[108] = 0;
   out_2893180287114808269[109] = 0;
   out_2893180287114808269[110] = 0;
   out_2893180287114808269[111] = 0;
   out_2893180287114808269[112] = 0;
   out_2893180287114808269[113] = 0;
   out_2893180287114808269[114] = 1;
   out_2893180287114808269[115] = 0;
   out_2893180287114808269[116] = 0;
   out_2893180287114808269[117] = 0;
   out_2893180287114808269[118] = 0;
   out_2893180287114808269[119] = 0;
   out_2893180287114808269[120] = 0;
   out_2893180287114808269[121] = 0;
   out_2893180287114808269[122] = 0;
   out_2893180287114808269[123] = 0;
   out_2893180287114808269[124] = 0;
   out_2893180287114808269[125] = 0;
   out_2893180287114808269[126] = 0;
   out_2893180287114808269[127] = 0;
   out_2893180287114808269[128] = 0;
   out_2893180287114808269[129] = 0;
   out_2893180287114808269[130] = 0;
   out_2893180287114808269[131] = 0;
   out_2893180287114808269[132] = 0;
   out_2893180287114808269[133] = 1;
   out_2893180287114808269[134] = 0;
   out_2893180287114808269[135] = 0;
   out_2893180287114808269[136] = 0;
   out_2893180287114808269[137] = 0;
   out_2893180287114808269[138] = 0;
   out_2893180287114808269[139] = 0;
   out_2893180287114808269[140] = 0;
   out_2893180287114808269[141] = 0;
   out_2893180287114808269[142] = 0;
   out_2893180287114808269[143] = 0;
   out_2893180287114808269[144] = 0;
   out_2893180287114808269[145] = 0;
   out_2893180287114808269[146] = 0;
   out_2893180287114808269[147] = 0;
   out_2893180287114808269[148] = 0;
   out_2893180287114808269[149] = 0;
   out_2893180287114808269[150] = 0;
   out_2893180287114808269[151] = 0;
   out_2893180287114808269[152] = 1;
   out_2893180287114808269[153] = 0;
   out_2893180287114808269[154] = 0;
   out_2893180287114808269[155] = 0;
   out_2893180287114808269[156] = 0;
   out_2893180287114808269[157] = 0;
   out_2893180287114808269[158] = 0;
   out_2893180287114808269[159] = 0;
   out_2893180287114808269[160] = 0;
   out_2893180287114808269[161] = 0;
   out_2893180287114808269[162] = 0;
   out_2893180287114808269[163] = 0;
   out_2893180287114808269[164] = 0;
   out_2893180287114808269[165] = 0;
   out_2893180287114808269[166] = 0;
   out_2893180287114808269[167] = 0;
   out_2893180287114808269[168] = 0;
   out_2893180287114808269[169] = 0;
   out_2893180287114808269[170] = 0;
   out_2893180287114808269[171] = 1;
   out_2893180287114808269[172] = 0;
   out_2893180287114808269[173] = 0;
   out_2893180287114808269[174] = 0;
   out_2893180287114808269[175] = 0;
   out_2893180287114808269[176] = 0;
   out_2893180287114808269[177] = 0;
   out_2893180287114808269[178] = 0;
   out_2893180287114808269[179] = 0;
   out_2893180287114808269[180] = 0;
   out_2893180287114808269[181] = 0;
   out_2893180287114808269[182] = 0;
   out_2893180287114808269[183] = 0;
   out_2893180287114808269[184] = 0;
   out_2893180287114808269[185] = 0;
   out_2893180287114808269[186] = 0;
   out_2893180287114808269[187] = 0;
   out_2893180287114808269[188] = 0;
   out_2893180287114808269[189] = 0;
   out_2893180287114808269[190] = 1;
   out_2893180287114808269[191] = 0;
   out_2893180287114808269[192] = 0;
   out_2893180287114808269[193] = 0;
   out_2893180287114808269[194] = 0;
   out_2893180287114808269[195] = 0;
   out_2893180287114808269[196] = 0;
   out_2893180287114808269[197] = 0;
   out_2893180287114808269[198] = 0;
   out_2893180287114808269[199] = 0;
   out_2893180287114808269[200] = 0;
   out_2893180287114808269[201] = 0;
   out_2893180287114808269[202] = 0;
   out_2893180287114808269[203] = 0;
   out_2893180287114808269[204] = 0;
   out_2893180287114808269[205] = 0;
   out_2893180287114808269[206] = 0;
   out_2893180287114808269[207] = 0;
   out_2893180287114808269[208] = 0;
   out_2893180287114808269[209] = 1;
   out_2893180287114808269[210] = 0;
   out_2893180287114808269[211] = 0;
   out_2893180287114808269[212] = 0;
   out_2893180287114808269[213] = 0;
   out_2893180287114808269[214] = 0;
   out_2893180287114808269[215] = 0;
   out_2893180287114808269[216] = 0;
   out_2893180287114808269[217] = 0;
   out_2893180287114808269[218] = 0;
   out_2893180287114808269[219] = 0;
   out_2893180287114808269[220] = 0;
   out_2893180287114808269[221] = 0;
   out_2893180287114808269[222] = 0;
   out_2893180287114808269[223] = 0;
   out_2893180287114808269[224] = 0;
   out_2893180287114808269[225] = 0;
   out_2893180287114808269[226] = 0;
   out_2893180287114808269[227] = 0;
   out_2893180287114808269[228] = 1;
   out_2893180287114808269[229] = 0;
   out_2893180287114808269[230] = 0;
   out_2893180287114808269[231] = 0;
   out_2893180287114808269[232] = 0;
   out_2893180287114808269[233] = 0;
   out_2893180287114808269[234] = 0;
   out_2893180287114808269[235] = 0;
   out_2893180287114808269[236] = 0;
   out_2893180287114808269[237] = 0;
   out_2893180287114808269[238] = 0;
   out_2893180287114808269[239] = 0;
   out_2893180287114808269[240] = 0;
   out_2893180287114808269[241] = 0;
   out_2893180287114808269[242] = 0;
   out_2893180287114808269[243] = 0;
   out_2893180287114808269[244] = 0;
   out_2893180287114808269[245] = 0;
   out_2893180287114808269[246] = 0;
   out_2893180287114808269[247] = 1;
   out_2893180287114808269[248] = 0;
   out_2893180287114808269[249] = 0;
   out_2893180287114808269[250] = 0;
   out_2893180287114808269[251] = 0;
   out_2893180287114808269[252] = 0;
   out_2893180287114808269[253] = 0;
   out_2893180287114808269[254] = 0;
   out_2893180287114808269[255] = 0;
   out_2893180287114808269[256] = 0;
   out_2893180287114808269[257] = 0;
   out_2893180287114808269[258] = 0;
   out_2893180287114808269[259] = 0;
   out_2893180287114808269[260] = 0;
   out_2893180287114808269[261] = 0;
   out_2893180287114808269[262] = 0;
   out_2893180287114808269[263] = 0;
   out_2893180287114808269[264] = 0;
   out_2893180287114808269[265] = 0;
   out_2893180287114808269[266] = 1;
   out_2893180287114808269[267] = 0;
   out_2893180287114808269[268] = 0;
   out_2893180287114808269[269] = 0;
   out_2893180287114808269[270] = 0;
   out_2893180287114808269[271] = 0;
   out_2893180287114808269[272] = 0;
   out_2893180287114808269[273] = 0;
   out_2893180287114808269[274] = 0;
   out_2893180287114808269[275] = 0;
   out_2893180287114808269[276] = 0;
   out_2893180287114808269[277] = 0;
   out_2893180287114808269[278] = 0;
   out_2893180287114808269[279] = 0;
   out_2893180287114808269[280] = 0;
   out_2893180287114808269[281] = 0;
   out_2893180287114808269[282] = 0;
   out_2893180287114808269[283] = 0;
   out_2893180287114808269[284] = 0;
   out_2893180287114808269[285] = 1;
   out_2893180287114808269[286] = 0;
   out_2893180287114808269[287] = 0;
   out_2893180287114808269[288] = 0;
   out_2893180287114808269[289] = 0;
   out_2893180287114808269[290] = 0;
   out_2893180287114808269[291] = 0;
   out_2893180287114808269[292] = 0;
   out_2893180287114808269[293] = 0;
   out_2893180287114808269[294] = 0;
   out_2893180287114808269[295] = 0;
   out_2893180287114808269[296] = 0;
   out_2893180287114808269[297] = 0;
   out_2893180287114808269[298] = 0;
   out_2893180287114808269[299] = 0;
   out_2893180287114808269[300] = 0;
   out_2893180287114808269[301] = 0;
   out_2893180287114808269[302] = 0;
   out_2893180287114808269[303] = 0;
   out_2893180287114808269[304] = 1;
   out_2893180287114808269[305] = 0;
   out_2893180287114808269[306] = 0;
   out_2893180287114808269[307] = 0;
   out_2893180287114808269[308] = 0;
   out_2893180287114808269[309] = 0;
   out_2893180287114808269[310] = 0;
   out_2893180287114808269[311] = 0;
   out_2893180287114808269[312] = 0;
   out_2893180287114808269[313] = 0;
   out_2893180287114808269[314] = 0;
   out_2893180287114808269[315] = 0;
   out_2893180287114808269[316] = 0;
   out_2893180287114808269[317] = 0;
   out_2893180287114808269[318] = 0;
   out_2893180287114808269[319] = 0;
   out_2893180287114808269[320] = 0;
   out_2893180287114808269[321] = 0;
   out_2893180287114808269[322] = 0;
   out_2893180287114808269[323] = 1;
}
void h_4(double *state, double *unused, double *out_4396317536037036464) {
   out_4396317536037036464[0] = state[6] + state[9];
   out_4396317536037036464[1] = state[7] + state[10];
   out_4396317536037036464[2] = state[8] + state[11];
}
void H_4(double *state, double *unused, double *out_2480802011854586943) {
   out_2480802011854586943[0] = 0;
   out_2480802011854586943[1] = 0;
   out_2480802011854586943[2] = 0;
   out_2480802011854586943[3] = 0;
   out_2480802011854586943[4] = 0;
   out_2480802011854586943[5] = 0;
   out_2480802011854586943[6] = 1;
   out_2480802011854586943[7] = 0;
   out_2480802011854586943[8] = 0;
   out_2480802011854586943[9] = 1;
   out_2480802011854586943[10] = 0;
   out_2480802011854586943[11] = 0;
   out_2480802011854586943[12] = 0;
   out_2480802011854586943[13] = 0;
   out_2480802011854586943[14] = 0;
   out_2480802011854586943[15] = 0;
   out_2480802011854586943[16] = 0;
   out_2480802011854586943[17] = 0;
   out_2480802011854586943[18] = 0;
   out_2480802011854586943[19] = 0;
   out_2480802011854586943[20] = 0;
   out_2480802011854586943[21] = 0;
   out_2480802011854586943[22] = 0;
   out_2480802011854586943[23] = 0;
   out_2480802011854586943[24] = 0;
   out_2480802011854586943[25] = 1;
   out_2480802011854586943[26] = 0;
   out_2480802011854586943[27] = 0;
   out_2480802011854586943[28] = 1;
   out_2480802011854586943[29] = 0;
   out_2480802011854586943[30] = 0;
   out_2480802011854586943[31] = 0;
   out_2480802011854586943[32] = 0;
   out_2480802011854586943[33] = 0;
   out_2480802011854586943[34] = 0;
   out_2480802011854586943[35] = 0;
   out_2480802011854586943[36] = 0;
   out_2480802011854586943[37] = 0;
   out_2480802011854586943[38] = 0;
   out_2480802011854586943[39] = 0;
   out_2480802011854586943[40] = 0;
   out_2480802011854586943[41] = 0;
   out_2480802011854586943[42] = 0;
   out_2480802011854586943[43] = 0;
   out_2480802011854586943[44] = 1;
   out_2480802011854586943[45] = 0;
   out_2480802011854586943[46] = 0;
   out_2480802011854586943[47] = 1;
   out_2480802011854586943[48] = 0;
   out_2480802011854586943[49] = 0;
   out_2480802011854586943[50] = 0;
   out_2480802011854586943[51] = 0;
   out_2480802011854586943[52] = 0;
   out_2480802011854586943[53] = 0;
}
void h_10(double *state, double *unused, double *out_5916006405786623891) {
   out_5916006405786623891[0] = 9.8100000000000005*sin(state[1]) - state[4]*state[8] + state[5]*state[7] + state[12] + state[15];
   out_5916006405786623891[1] = -9.8100000000000005*sin(state[0])*cos(state[1]) + state[3]*state[8] - state[5]*state[6] + state[13] + state[16];
   out_5916006405786623891[2] = -9.8100000000000005*cos(state[0])*cos(state[1]) - state[3]*state[7] + state[4]*state[6] + state[14] + state[17];
}
void H_10(double *state, double *unused, double *out_155472375533809188) {
   out_155472375533809188[0] = 0;
   out_155472375533809188[1] = 9.8100000000000005*cos(state[1]);
   out_155472375533809188[2] = 0;
   out_155472375533809188[3] = 0;
   out_155472375533809188[4] = -state[8];
   out_155472375533809188[5] = state[7];
   out_155472375533809188[6] = 0;
   out_155472375533809188[7] = state[5];
   out_155472375533809188[8] = -state[4];
   out_155472375533809188[9] = 0;
   out_155472375533809188[10] = 0;
   out_155472375533809188[11] = 0;
   out_155472375533809188[12] = 1;
   out_155472375533809188[13] = 0;
   out_155472375533809188[14] = 0;
   out_155472375533809188[15] = 1;
   out_155472375533809188[16] = 0;
   out_155472375533809188[17] = 0;
   out_155472375533809188[18] = -9.8100000000000005*cos(state[0])*cos(state[1]);
   out_155472375533809188[19] = 9.8100000000000005*sin(state[0])*sin(state[1]);
   out_155472375533809188[20] = 0;
   out_155472375533809188[21] = state[8];
   out_155472375533809188[22] = 0;
   out_155472375533809188[23] = -state[6];
   out_155472375533809188[24] = -state[5];
   out_155472375533809188[25] = 0;
   out_155472375533809188[26] = state[3];
   out_155472375533809188[27] = 0;
   out_155472375533809188[28] = 0;
   out_155472375533809188[29] = 0;
   out_155472375533809188[30] = 0;
   out_155472375533809188[31] = 1;
   out_155472375533809188[32] = 0;
   out_155472375533809188[33] = 0;
   out_155472375533809188[34] = 1;
   out_155472375533809188[35] = 0;
   out_155472375533809188[36] = 9.8100000000000005*sin(state[0])*cos(state[1]);
   out_155472375533809188[37] = 9.8100000000000005*sin(state[1])*cos(state[0]);
   out_155472375533809188[38] = 0;
   out_155472375533809188[39] = -state[7];
   out_155472375533809188[40] = state[6];
   out_155472375533809188[41] = 0;
   out_155472375533809188[42] = state[4];
   out_155472375533809188[43] = -state[3];
   out_155472375533809188[44] = 0;
   out_155472375533809188[45] = 0;
   out_155472375533809188[46] = 0;
   out_155472375533809188[47] = 0;
   out_155472375533809188[48] = 0;
   out_155472375533809188[49] = 0;
   out_155472375533809188[50] = 1;
   out_155472375533809188[51] = 0;
   out_155472375533809188[52] = 0;
   out_155472375533809188[53] = 1;
}
void h_13(double *state, double *unused, double *out_3965618053748515748) {
   out_3965618053748515748[0] = state[3];
   out_3965618053748515748[1] = state[4];
   out_3965618053748515748[2] = state[5];
}
void H_13(double *state, double *unused, double *out_5693075837186919744) {
   out_5693075837186919744[0] = 0;
   out_5693075837186919744[1] = 0;
   out_5693075837186919744[2] = 0;
   out_5693075837186919744[3] = 1;
   out_5693075837186919744[4] = 0;
   out_5693075837186919744[5] = 0;
   out_5693075837186919744[6] = 0;
   out_5693075837186919744[7] = 0;
   out_5693075837186919744[8] = 0;
   out_5693075837186919744[9] = 0;
   out_5693075837186919744[10] = 0;
   out_5693075837186919744[11] = 0;
   out_5693075837186919744[12] = 0;
   out_5693075837186919744[13] = 0;
   out_5693075837186919744[14] = 0;
   out_5693075837186919744[15] = 0;
   out_5693075837186919744[16] = 0;
   out_5693075837186919744[17] = 0;
   out_5693075837186919744[18] = 0;
   out_5693075837186919744[19] = 0;
   out_5693075837186919744[20] = 0;
   out_5693075837186919744[21] = 0;
   out_5693075837186919744[22] = 1;
   out_5693075837186919744[23] = 0;
   out_5693075837186919744[24] = 0;
   out_5693075837186919744[25] = 0;
   out_5693075837186919744[26] = 0;
   out_5693075837186919744[27] = 0;
   out_5693075837186919744[28] = 0;
   out_5693075837186919744[29] = 0;
   out_5693075837186919744[30] = 0;
   out_5693075837186919744[31] = 0;
   out_5693075837186919744[32] = 0;
   out_5693075837186919744[33] = 0;
   out_5693075837186919744[34] = 0;
   out_5693075837186919744[35] = 0;
   out_5693075837186919744[36] = 0;
   out_5693075837186919744[37] = 0;
   out_5693075837186919744[38] = 0;
   out_5693075837186919744[39] = 0;
   out_5693075837186919744[40] = 0;
   out_5693075837186919744[41] = 1;
   out_5693075837186919744[42] = 0;
   out_5693075837186919744[43] = 0;
   out_5693075837186919744[44] = 0;
   out_5693075837186919744[45] = 0;
   out_5693075837186919744[46] = 0;
   out_5693075837186919744[47] = 0;
   out_5693075837186919744[48] = 0;
   out_5693075837186919744[49] = 0;
   out_5693075837186919744[50] = 0;
   out_5693075837186919744[51] = 0;
   out_5693075837186919744[52] = 0;
   out_5693075837186919744[53] = 0;
}
void h_14(double *state, double *unused, double *out_6554627370156003242) {
   out_6554627370156003242[0] = state[6];
   out_6554627370156003242[1] = state[7];
   out_6554627370156003242[2] = state[8];
}
void H_14(double *state, double *unused, double *out_2045685485209703344) {
   out_2045685485209703344[0] = 0;
   out_2045685485209703344[1] = 0;
   out_2045685485209703344[2] = 0;
   out_2045685485209703344[3] = 0;
   out_2045685485209703344[4] = 0;
   out_2045685485209703344[5] = 0;
   out_2045685485209703344[6] = 1;
   out_2045685485209703344[7] = 0;
   out_2045685485209703344[8] = 0;
   out_2045685485209703344[9] = 0;
   out_2045685485209703344[10] = 0;
   out_2045685485209703344[11] = 0;
   out_2045685485209703344[12] = 0;
   out_2045685485209703344[13] = 0;
   out_2045685485209703344[14] = 0;
   out_2045685485209703344[15] = 0;
   out_2045685485209703344[16] = 0;
   out_2045685485209703344[17] = 0;
   out_2045685485209703344[18] = 0;
   out_2045685485209703344[19] = 0;
   out_2045685485209703344[20] = 0;
   out_2045685485209703344[21] = 0;
   out_2045685485209703344[22] = 0;
   out_2045685485209703344[23] = 0;
   out_2045685485209703344[24] = 0;
   out_2045685485209703344[25] = 1;
   out_2045685485209703344[26] = 0;
   out_2045685485209703344[27] = 0;
   out_2045685485209703344[28] = 0;
   out_2045685485209703344[29] = 0;
   out_2045685485209703344[30] = 0;
   out_2045685485209703344[31] = 0;
   out_2045685485209703344[32] = 0;
   out_2045685485209703344[33] = 0;
   out_2045685485209703344[34] = 0;
   out_2045685485209703344[35] = 0;
   out_2045685485209703344[36] = 0;
   out_2045685485209703344[37] = 0;
   out_2045685485209703344[38] = 0;
   out_2045685485209703344[39] = 0;
   out_2045685485209703344[40] = 0;
   out_2045685485209703344[41] = 0;
   out_2045685485209703344[42] = 0;
   out_2045685485209703344[43] = 0;
   out_2045685485209703344[44] = 1;
   out_2045685485209703344[45] = 0;
   out_2045685485209703344[46] = 0;
   out_2045685485209703344[47] = 0;
   out_2045685485209703344[48] = 0;
   out_2045685485209703344[49] = 0;
   out_2045685485209703344[50] = 0;
   out_2045685485209703344[51] = 0;
   out_2045685485209703344[52] = 0;
   out_2045685485209703344[53] = 0;
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

void pose_update_4(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_4, H_4, NULL, in_z, in_R, in_ea, MAHA_THRESH_4);
}
void pose_update_10(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_10, H_10, NULL, in_z, in_R, in_ea, MAHA_THRESH_10);
}
void pose_update_13(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_13, H_13, NULL, in_z, in_R, in_ea, MAHA_THRESH_13);
}
void pose_update_14(double *in_x, double *in_P, double *in_z, double *in_R, double *in_ea) {
  update<3, 3, 0>(in_x, in_P, h_14, H_14, NULL, in_z, in_R, in_ea, MAHA_THRESH_14);
}
void pose_err_fun(double *nom_x, double *delta_x, double *out_6708923800649560446) {
  err_fun(nom_x, delta_x, out_6708923800649560446);
}
void pose_inv_err_fun(double *nom_x, double *true_x, double *out_9014600873516389172) {
  inv_err_fun(nom_x, true_x, out_9014600873516389172);
}
void pose_H_mod_fun(double *state, double *out_8328883818606545664) {
  H_mod_fun(state, out_8328883818606545664);
}
void pose_f_fun(double *state, double dt, double *out_1076920247808564320) {
  f_fun(state,  dt, out_1076920247808564320);
}
void pose_F_fun(double *state, double dt, double *out_2893180287114808269) {
  F_fun(state,  dt, out_2893180287114808269);
}
void pose_h_4(double *state, double *unused, double *out_4396317536037036464) {
  h_4(state, unused, out_4396317536037036464);
}
void pose_H_4(double *state, double *unused, double *out_2480802011854586943) {
  H_4(state, unused, out_2480802011854586943);
}
void pose_h_10(double *state, double *unused, double *out_5916006405786623891) {
  h_10(state, unused, out_5916006405786623891);
}
void pose_H_10(double *state, double *unused, double *out_155472375533809188) {
  H_10(state, unused, out_155472375533809188);
}
void pose_h_13(double *state, double *unused, double *out_3965618053748515748) {
  h_13(state, unused, out_3965618053748515748);
}
void pose_H_13(double *state, double *unused, double *out_5693075837186919744) {
  H_13(state, unused, out_5693075837186919744);
}
void pose_h_14(double *state, double *unused, double *out_6554627370156003242) {
  h_14(state, unused, out_6554627370156003242);
}
void pose_H_14(double *state, double *unused, double *out_2045685485209703344) {
  H_14(state, unused, out_2045685485209703344);
}
void pose_predict(double *in_x, double *in_P, double *in_Q, double dt) {
  predict(in_x, in_P, in_Q, dt);
}
}

const EKF pose = {
  .name = "pose",
  .kinds = { 4, 10, 13, 14 },
  .feature_kinds = {  },
  .f_fun = pose_f_fun,
  .F_fun = pose_F_fun,
  .err_fun = pose_err_fun,
  .inv_err_fun = pose_inv_err_fun,
  .H_mod_fun = pose_H_mod_fun,
  .predict = pose_predict,
  .hs = {
    { 4, pose_h_4 },
    { 10, pose_h_10 },
    { 13, pose_h_13 },
    { 14, pose_h_14 },
  },
  .Hs = {
    { 4, pose_H_4 },
    { 10, pose_H_10 },
    { 13, pose_H_13 },
    { 14, pose_H_14 },
  },
  .updates = {
    { 4, pose_update_4 },
    { 10, pose_update_10 },
    { 13, pose_update_13 },
    { 14, pose_update_14 },
  },
  .Hes = {
  },
  .sets = {
  },
  .extra_routines = {
  },
};

ekf_lib_init(pose)
