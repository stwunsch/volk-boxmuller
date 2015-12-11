#include "qa_utils.h"

#include <volk_boxmuller/volk_boxmuller.h>

#include <boost/assign/list_of.hpp>
#include <vector>

// macros for initializing volk_boxmuller_test_case_t. Maccros are needed to generate
// function names of the pattern kernel_name_*

// for puppets we need to get all the func_variants for the puppet and just
// keep track of the actual function name to write to results
#define VOLK_INIT_PUPP(func, puppet_master_func, test_params)\
    volk_boxmuller_test_case_t(func##_get_func_desc(), (void(*)())func##_manual, std::string(#func),\
    std::string(#puppet_master_func), test_params)

#define VOLK_INIT_TEST(func, test_params)\
    volk_boxmuller_test_case_t(func##_get_func_desc(), (void(*)())func##_manual, std::string(#func),\
    test_params)

std::vector<volk_boxmuller_test_case_t> init_test_list(volk_boxmuller_test_params_t test_params)
{

    // Some kernels need a lower tolerance
    volk_boxmuller_test_params_t test_params_inacc = volk_boxmuller_test_params_t(1e-3, test_params.scalar(),
            test_params.vlen(), test_params.iter(), test_params.benchmark_mode(), test_params.kernel_regex());
    volk_boxmuller_test_params_t test_params_int1 = volk_boxmuller_test_params_t(1, test_params.scalar(),
            test_params.vlen(), test_params.iter(), test_params.benchmark_mode(), test_params.kernel_regex());

    std::vector<volk_boxmuller_test_case_t> test_cases = boost::assign::list_of
        // no one uses these, so don't test them
        //VOLK_PROFILE(volk_boxmuller_16i_x5_add_quad_16i_x4, 1e-4, 2046, 10000, &results, benchmark_mode, kernel_regex);
        //VOLK_PROFILE(volk_boxmuller_16i_branch_4_state_8, 1e-4, 2046, 10000, &results, benchmark_mode, kernel_regex);
        //VOLK_PROFILE(volk_boxmuller_16i_max_star_16i, 0, 0, 204602, 10000, &results, benchmark_mode, kernel_regex);
        //VOLK_PROFILE(volk_boxmuller_16i_max_star_horizontal_16i, 0, 0, 204602, 10000, &results, benchmark_mode, kernel_regex);
        //VOLK_PROFILE(volk_boxmuller_16i_permute_and_scalar_add, 1e-4, 0, 2046, 10000, &results, benchmark_mode, kernel_regex);
        //VOLK_PROFILE(volk_boxmuller_16i_x4_quad_max_star_16i, 1e-4, 0, 2046, 10000, &results, benchmark_mode, kernel_regex);
        // we need a puppet for this one
        //(VOLK_INIT_TEST(volk_boxmuller_32fc_s32f_x2_power_spectral_density_32f,   test_params))
        (VOLK_INIT_TEST(volk_boxmuller_32f_sin_32f,                               test_params_inacc))
        (VOLK_INIT_TEST(volk_boxmuller_32f_cos_32f,                               test_params_inacc))
        (VOLK_INIT_TEST(volk_boxmuller_32f_sqrt_32f,                              test_params_inacc))
        (VOLK_INIT_TEST(volk_boxmuller_32f_log2_32f,           volk_boxmuller_test_params_t(3, test_params.scalar(), test_params.vlen(), test_params.iter(), test_params.benchmark_mode(), test_params.kernel_regex())))
        (VOLK_INIT_TEST(volk_boxmuller_32f_x2_multiply_32f,                       test_params))
        (VOLK_INIT_PUPP(volk_boxmuller_32f_boxmullerpuppet_32f, volk_boxmuller_32f_boxmuller_32f, volk_boxmuller_test_params_t(1e-0, test_params.scalar(), test_params.vlen(), test_params.iter(), test_params.benchmark_mode(), test_params.kernel_regex())))
        ;

    return test_cases;
}
