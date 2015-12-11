/* -*- c++ -*- */
/*
 * Copyright 2015 Stefan Wunsch
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


#include <math.h>
#include <volk_boxmuller/volk_boxmuller.h>

#ifndef INCLUDED_volk_boxmuller_32f_boxmuller_32f_a_H
#define INCLUDED_volk_boxmuller_32f_boxmuller_32f_a_H

#ifdef LV_HAVE_GENERIC
static inline void volk_boxmuller_32f_boxmuller_32f_generic(float* output, float* input, unsigned int num_points){
    unsigned int half_points = num_points/2; // separate input in two buffers

    for(unsigned int k=0; k<half_points; k++){ // FIXME: only even input buffer size is allowed!
        const float calc_sqrt = sqrtf(-2.0*logf(input[k]));
        output[k] = calc_sqrt*cosf(2.0*M_PI*input[half_points+k]);
        output[half_points+k] = calc_sqrt*sinf(2.0*M_PI*input[half_points+k]);
    }
}
#endif /* LV_HAVE_GENERIC */


#ifdef LV_HAVE_SSE4_1
static inline void volk_boxmuller_32f_boxmuller_32f_a_sse4_1(float* output, float* input, unsigned int num_points){
    // FIXME: only even input buffer size is allowed!
    // FIXME: log2, sin and cos kernels does only need sse2!
    const unsigned int half_points = num_points/2;
    const unsigned int half_quarter_points = num_points/4/2; // split input in two buffers with 4 float segments

    // do SIMDed box muller
    const __m128 *in1 = (__m128*) (input+half_points);
    float *out0 = (float*) output;
    float *out1 = (float*) (output+half_points);

    // calc log2(half_input)
    __m128 *calc_sqrt = (__m128*) volk_boxmuller_malloc(half_quarter_points*4*4, volk_boxmuller_get_alignment());
    volk_boxmuller_32f_log2_32f((float*) calc_sqrt, input, half_quarter_points*4);

    // calc prev * log(2) to get log to the basis of e, multiply -2.0 as well to get -2.0*log(2)==-2.0*0.69314718==-1.3862943611
    const __m128 m2_log2 = _mm_set_ps1(-1.3862943611);
    for(unsigned int k=0; k<half_quarter_points; k++){
        *(calc_sqrt+k) = _mm_mul_ps(*(calc_sqrt+k), m2_log2);
    }

    // calc sqrt of prev
    volk_boxmuller_32f_sqrt_32f((float*) calc_sqrt, (float*) calc_sqrt, half_quarter_points*4);

    // multiply second half of the input buffer with 2*pi and write to buffer, this could be improved if output buffer == input buffer would be a requirement
    __m128 *half_input = (__m128*) volk_boxmuller_malloc(half_quarter_points*4*4, volk_boxmuller_get_alignment());
    const __m128 two_pi = _mm_set_ps1(2.0*M_PI);
    for(unsigned int k=0; k<half_quarter_points; k++){
        *(half_input+k) = _mm_mul_ps(*(in1+k), two_pi);
    }

    // write cos part to first half of output buffer and multiply calc_sqrt buffer
    volk_boxmuller_32f_cos_32f(out0, (float*) half_input, half_quarter_points*4);
    volk_boxmuller_32f_x2_multiply_32f(out0, out0, (float*) calc_sqrt, half_quarter_points*4);

    // write sin part to second half of output buffer and multiply calc_sqrt buffer
    volk_boxmuller_32f_sin_32f(out1, (float*) half_input, half_quarter_points*4);
    volk_boxmuller_32f_x2_multiply_32f(out1, out1, (float*) calc_sqrt, half_quarter_points*4);

    // clean-up
    volk_boxmuller_free(calc_sqrt);
    volk_boxmuller_free(half_input);

    // process 'rest' of input buffer with the generic implementation
    for(unsigned int k=half_quarter_points*4; k<num_points/2; k++){
        const float calc_sqrt = sqrtf(-2.0*logf(input[k]));
        output[k] = calc_sqrt*cosf(2.0*M_PI*input[half_points+k]);
        output[half_points+k] = calc_sqrt*sinf(2.0*M_PI*input[half_points+k]);
    }
}
#endif /* LV_HAVE_SSE4_1 */

#endif /* INCLUDED_volk_boxmuller_32f_boxmuller_32f_a_H */
