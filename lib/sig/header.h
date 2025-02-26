#ifndef __SIG_H__
#define __SIG_H__

#include "../../utils/struct.h"

PLACEHOLDER(sig);

sig_p sig_opposite(sig_p sig);

sig_p sig_add(sig_p sig_1, sig_p sig_2);
sig_p sig_sub(sig_p sig_1, sig_p sig_2);
sig_p sig_mul(sig_p sig_1, sig_p sig_2);
sig_p sig_div(sig_p sig_1, sig_p sig_2);

#endif
