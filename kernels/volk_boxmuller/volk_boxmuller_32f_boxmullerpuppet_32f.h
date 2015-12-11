/* -*- c++ -*- */
/*
 * Copyright 2015 Free Software Foundation, Inc.
 *
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */


#include <volk_boxmuller/volk_boxmuller.h>
#include <volk_boxmuller/volk_boxmuller_32f_boxmuller_32f.h>

#ifndef INCLUDED_volk_boxmuller_32f_boxmullerpuppet_32f_H
#define INCLUDED_volk_boxmuller_32f_boxmullerpuppet_32f_H

#ifdef LV_HAVE_GENERIC
static inline void volk_boxmuller_32f_boxmullerpuppet_32f_generic(float* output, float* input, unsigned int num_points){
    unsigned int num_points_even = (num_points/2)*2;
    float* input_trans = (float*) volk_boxmuller_malloc(4*num_points_even, volk_boxmuller_get_alignment());
    for(unsigned int k=0; k<num_points_even; k++) input_trans[k] = fabs(input[k]+1.0)/2.0; // transform input from (-1,1) to (0,1)

    volk_boxmuller_32f_boxmuller_32f_generic(output, input_trans, num_points_even);

    for(unsigned int k=num_points_even; k<num_points; k++) output[k] = 0; // set output to zero, if intput is odd
    volk_boxmuller_free(input_trans);
}
#endif /* LV_HAVE_GENERIC */


#ifdef LV_HAVE_SSE4_1
static inline void volk_boxmuller_32f_boxmullerpuppet_32f_a_sse4_1(float* output, float* input, unsigned int num_points){
    unsigned int num_points_even = (num_points/2)*2;
    float* input_trans = (float*) volk_boxmuller_malloc(4*num_points, volk_boxmuller_get_alignment());
    for(unsigned int k=0; k<num_points_even; k++) input_trans[k] = fabs(input[k]+1.0)/2.0; // transform input from (-1,1) to (0,1)

    volk_boxmuller_32f_boxmuller_32f_a_sse4_1(output, input_trans, num_points_even);

    for(unsigned int k=num_points_even; k<num_points; k++) output[k] = 0; // set output to zero, if intput is odd
    volk_boxmuller_free(input_trans);
}
#endif /* LV_HAVE_SSE4_1 */

#endif /* INCLUDED_volk_boxmuller_32f_boxmullerpuppet_32f_H */
