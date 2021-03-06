/* -*- c++ -*- */
/* 
 * Copyright 2013 Jiří Pinkava <j-pi@seznam.cz>.
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
 */

#include "qa_noise_level_estimator.h"
#include <rstt/noise_level_estimator.h>
#include "noise_level_estimator_impl.h"
#include <cppunit/TestAssert.h>
#include <cmath>

namespace gr {
  namespace rstt {

    void
    qa_noise_level_estimator::t1_1()
    {
      const float data[] = {1.8, };
      const float idata[] = {1.8, };
      noise_level_estimator_impl::Chunk chunk(0, sizeof(idata) / sizeof(float), idata);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 0., 0.0001);

      CPPUNIT_ASSERT_EQUAL(chunk.start(), 0);
      CPPUNIT_ASSERT_EQUAL(chunk.len(), 1);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.extreme_val(), 0., 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk.extreme_rel_idx(), 0);

      chunk.shift_mean(data);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 1.8, 0.0001);
    }

    void
    qa_noise_level_estimator::t1_2()
    {
      const float data[] = {1.8, 0.4};
      const float idata[] = {1.8, 2.2};
      noise_level_estimator_impl::Chunk chunk(0, sizeof(idata) / sizeof(float), idata);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 0.4, 0.0001);

      CPPUNIT_ASSERT_EQUAL(chunk.start(), 0);
      CPPUNIT_ASSERT_EQUAL(chunk.len(), 2);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.extreme_val(), 0., 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk.extreme_rel_idx(), 0);

      chunk.shift_mean(data);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 1.1, 0.0001);
    }

    void
    qa_noise_level_estimator::t1_3()
    {
      const float data[] = {1.8, 0.4, 1.4};
      const float idata[] = {1.8, 2.2, 3.6};
      noise_level_estimator_impl::Chunk chunk(0, sizeof(idata) / sizeof(float), idata);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 0.9, 0.0001);

      CPPUNIT_ASSERT_EQUAL(chunk.start(), 0);
      CPPUNIT_ASSERT_EQUAL(chunk.len(), 3);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.extreme_val(), std::fabs(2.2-1.8-0.9), 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk.extreme_rel_idx(), 1);

      chunk.shift_mean(data);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 1.2, 0.0001);
    }

    void
    qa_noise_level_estimator::t1_4()
    {
      const float data[] = {0.4, 0.5, 0.7, 0.9, 1.1, 1.2, 0.6, 0.65, 0.85, 0.45, 0.4, 0.65, 0.8 };
      const float idata[] = {0.4, 0.9, 1.6, 2.5, 3.6, 4.8, 5.4, 6.05, 6.9, 7.35, 7.75, 8.4, 9.2 };
      noise_level_estimator_impl::Chunk chunk(0, sizeof(idata) / sizeof(float), idata);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 0.733333333, 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk.start(), 0);
      CPPUNIT_ASSERT_EQUAL(chunk.len(), 13);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.extreme_val(), std::fabs(4.8-0.4-5*0.733333333), 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk.extreme_rel_idx(), 5);

      const noise_level_estimator_impl::Chunk chunk2 = chunk.split(idata);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk2.mean(), (9.2 - 5.4)/6, 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk2.start(), 6);
      CPPUNIT_ASSERT_EQUAL(chunk2.len(), 13-6);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk2.extreme_val(), std::fabs(6.9-5.4-2*0.63333333), 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk2.extreme_rel_idx(), 2);

      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 4.4/5, 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk.start(), 0);
      CPPUNIT_ASSERT_EQUAL(chunk.len(), 6);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.extreme_val(), std::fabs(1.6-0.4-2*0.88), 0.0001);
      CPPUNIT_ASSERT_EQUAL(chunk.extreme_rel_idx(), 2);

      chunk.shift_mean(data);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk.mean(), 4.8/6, 0.0001);
      noise_level_estimator_impl::Chunk chunk3 = chunk2;
      chunk3.shift_mean(data);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(chunk3.mean(), (9.2 - 4.8)/7, 0.0001);
    }

    void
    qa_noise_level_estimator::t2_1()
    {
        const float data[] = {
            6.2720, 6.3208, 6.3699, 6.3037, 6.2971, 6.2082, 6.0295, 6.1586,
            6.3164, 6.3267, 6.2072, 6.2756, 6.3345, 6.1993, 6.2139, 6.2715,
            6.1807, 6.1910, 6.2933, 6.2554, 6.1830, 6.2120, 6.2564, 6.1112,
            5.9963, 6.1225, 6.1350, 6.0866, 6.1709, 6.3973, 6.4723, 6.4210,
            6.4392, 6.3797, 6.2373, 6.2607, 6.3294, 6.2895, 6.2736, 6.2172,
            6.2060, 6.2504, 6.2823, 6.1992, 6.1429, 6.2483, 6.2258, 6.2003,
            6.2968, 6.3499, 6.3391, 6.4031, 6.4613, 6.4556, 6.4600, 6.2985,
            6.0205, 6.1085, 6.1986, 6.2595, 6.2944, 6.3538, 6.4079, 6.3773,
            6.2341, 6.1971, 6.3370, 6.4049, 6.3664, 6.3682, 6.3618, 6.3523,
            6.2695, 6.1975, 6.2039, 6.2148, 6.2481, 6.3105, 6.2636, 6.2051,
            6.1922, 6.2252, 6.2616, 6.2415, 6.2572, 6.2761, 6.1984, 6.3029,
            6.3815, 6.3216, 6.2988, 6.3090, 6.2284, 6.1585, 6.1929, 6.2873,
            6.3489, 6.2566, 6.2153, 6.2881, 6.3587, 6.3714, 6.3580, 6.2507,
            6.1537, 6.2137, 6.1932, 6.2412, 6.3639, 6.4343, 6.3662, 6.3105,
            6.2913, 6.2615, 6.2643, 6.3050, 6.2765, 6.2664, 6.2351, 6.2719,
            6.3206, 6.2800, 6.2713, 6.4042, 6.5004, 6.4717, 6.3502, 6.2453,
            6.2906, 6.3179, 6.1486, 6.0931, 6.2445, 6.3181, 6.3793, 6.3719,
            6.3184, 6.2869, 6.2350, 6.2648, 6.2963, 6.2787, 6.2851, 6.2362,
            6.2040, 6.2257, 6.2971, 6.2802, 6.2617, 6.1182, 6.1078, 6.2645,
            6.3730, 6.4235, 6.3674, 6.2985, 6.3556, 6.3991, 6.3319, 6.2436,
            6.2418, 6.2459, 6.1704, 6.2696, 6.2924, 6.1125, 5.9715, 6.0318,
            6.1011, 6.1549, 6.1819, 6.1590, 6.1849, 6.2229, 6.2454, 6.3981,
            6.4019, 6.2292, 6.0958, 6.2224, 6.2701, 6.2470, 6.3231, 6.2529,
            6.4115, 6.5164, 6.3794, 6.3089, 6.2578, 6.2104, 6.2058, 6.2024,
            6.2817, 6.3412, 6.2710, 6.2616, 6.3324, 6.2941, 6.2158, 6.1630,
            6.2139, 6.1492, 6.1079, 6.2021, 6.3346, 6.3452, 6.2760, 6.3535,
            6.4000, 6.3155, 6.1670, 6.1141, 6.1511, 6.2344, 6.2697, 6.2999,
            6.2968, 6.2063, 6.2002, 6.1743, 6.1401, 6.2208, 6.3374, 6.3384,
            6.1961, 6.1149, 6.0770, 6.0264, 6.1415, 6.2903, 6.3767, 6.3461,
            6.3152, 6.3294, 6.3415, 6.3164, 6.2953, 6.1465, 6.0841, 6.1317,
            6.1201, 6.1910, 6.2998, 6.3591, 6.3183, 6.2879, 6.4293, 6.5017,
            6.5562, 6.8611, 7.2485, 7.6119, 7.9837, 8.3908, 8.7472, 8.9180,
            8.9536, 8.8708, 8.4929, 7.7298, 7.3788, 8.0281, 8.4518, 8.3774,
            7.8275, 7.3785, 7.9427, 8.3327, 8.0977, 7.5211, 7.7680, 8.1127,
            7.8195, 7.5707, 8.0172, 7.9919, 7.5784, 7.7899, 7.9424, 7.6252,
            7.7655, 7.9327, 7.6580, 7.7436, 7.7627, 7.6219, 7.8711, 7.7522,
            7.6567, 7.7490, 7.5226, 7.6774, 7.6324, 7.6367, 7.7922, 7.6235,
            7.7453, 7.6337, 7.7141, 7.6834, 7.6371, 7.6501, 7.5827, 7.5884,
            7.5584, 7.6510, 7.5846, 7.6234, 7.6006, 7.5813, 7.5993, 7.6379,
            7.7489, 7.6807, 7.6701, 7.5432, 7.6435, 7.6164, 7.7146, 7.5518,
            7.6530, 7.6940, 7.7382, 7.6888, 7.6824, 7.7502, 7.6374, 7.8066,
            7.6795, 7.6976, 7.6382, 7.5946, 7.6959, 7.5602, 7.7540, 7.7607,
            7.6562, 7.9312, 7.7430, 7.6355, 7.8697, 7.5928, 7.6155, 7.9683,
            7.7476, 7.6457, 8.0411, 7.8623, 7.3795, 7.7862, 8.0351, 7.6074,
            7.4773, 8.1423, 8.3065, 7.8675, 7.3594, 7.8696, 8.3757, 8.3147,
            7.8294, 7.3479, 7.8392, 8.5486, 8.9375, 9.0441, 8.9619, 8.7523,
            8.3939, 7.9175, 7.4186, 7.1106, 6.7307, 6.2948, 6.0524, 6.1275,
            6.3550, 6.4590, 6.5037, 6.3667, 6.3505, 6.3321, 6.2444, 6.2307,
            6.2392, 6.2949, 6.2539, 6.3582, 6.4471, 6.4713, 6.5183, 6.6367,
            6.8427, 7.1132, 7.2771, 7.4990, 7.6614, 7.8209, 8.0286, 8.1715,
            8.3654, 8.5151, 8.5459, 8.4732, 8.3440, 8.2086, 7.9843, 7.6061,
            6.9942, 6.6915, 6.8578, 7.3756, 7.7669, 8.0284, 8.1465, 8.0498,
            7.6219, 6.9472, 6.4457, 6.6325, 7.2794, 7.5625, 7.7489, 7.7453,
            7.3013, 6.6906, 6.6825, 7.2381, 7.6209, 7.7837, 7.5224, 7.1003,
            6.5445, 6.8556, 7.3689, 7.8451, 7.7106, 7.2091, 6.6060, 6.9031,
            7.4252, 7.6379, 7.5498, 6.9176, 6.9001, 7.2414, 7.6095, 7.5507,
            7.1753, 6.8884, 7.0836, 7.5573, 7.4217, 7.1145, 6.8647, 7.2358,
            7.6690, 7.4743, 7.1309, 6.9120, 7.3786, 7.6364, 7.1737, 6.9333,
            7.0289, 7.4029, 7.4191, 6.7305, 6.8216, 7.2212, 7.3657, 7.2100,
            6.7821, 7.2865, 7.3646, 6.8926, 6.9606, 7.1670, 7.3976, 7.2939,
            6.8612, 7.3790, 7.4429, 6.8163, 6.9858, 7.2990, 7.2277, 7.0197,
            6.9073, 7.2900, 7.3531, 6.7990, 7.0199, 7.2639, 6.7308, 6.8135,
            7.2600, 7.0605, 7.0686, 7.3582, 7.2942, 7.1876, 7.1723, 7.2260,
            7.2567, 7.1480, 7.1510, 7.2594, 7.0984, 7.0641, 7.3258, 7.2099,
            7.0706, 7.3791, 7.3442, 6.9274, 7.0274, 7.2683, 7.2002, 7.1176,
            7.1212, 7.0376, 7.0699, 7.1308, 7.2062, 7.2538, 7.0003, 7.1801,
            7.4746, 7.0865, 6.9726, 7.5429, 7.3428, 6.8492, 7.2064, 7.1309,
            6.8225, 6.9507, 7.1915, 7.3687, 7.0932, 6.9757, 7.4884, 7.3493,
            6.6714, 7.0217, 7.2387, 7.0613, 6.8461, 6.9534, 7.4591, 7.3462,
            6.6049, 6.9345, 7.3272, 7.4327, 7.0751, 6.8654, 7.5024, 7.4470,
            6.9773, 6.8591, 7.1083, 7.5413, 7.3078, 6.8522, 7.1093, 7.4669,
            7.6637, 7.1927, 6.9041, 7.1864, 7.4557, 7.5057, 6.9386, 6.7598,
            7.1084, 7.5663, 7.6423, 7.2314, 6.8576, 6.8974, 7.4818, 7.6071,
            7.4734, 7.0449, 6.8054, 7.1925, 7.5189, 7.6852, 7.3722, 6.9623,
            6.5407, 7.2110, 7.6783, 7.8076, 7.5148, 7.2071, 6.7577, 6.8808,
            7.4410, 7.7955, 7.8928, 7.7348, 7.3772, 6.7763, 6.5959, 7.1581,
            7.6815, 7.9642, 8.1182, 8.0419, 7.7449, 7.4256, 6.8878, 6.6170,
            6.9350, 7.4313, 7.7518, 8.1073, 8.2946, 8.4276, 8.4663, 8.4991,
            8.4342, 8.2595, 8.0877, 7.8905, 7.6540, 7.4227, 7.1700, 7.0807,
            6.9992, 6.6985, 6.5154, 6.4225, 6.2794, 6.2506, 6.3202, 6.3731,
            6.3526, 6.3434, 6.2800, 6.2059, 6.3051, 6.3779, 6.4750, 6.4471,
            6.3688, 6.4510, 6.6146, 6.8455, 6.9052, 7.2954, 7.3766, 7.9894,
            8.0524, 8.4201, 8.5469, 8.6044, 8.8908, 8.6063, 8.9085, 8.5798,
            8.6041, 8.3420, 7.6037, 7.3898, 6.7593, 6.5210, 7.4578, 8.2073,
            8.1702, 8.5517, 8.2811, 7.7227, 7.5451, 6.8846, 6.9135, 7.8233,
            8.3628, 7.9975, 8.0793, 7.7332, 6.6215, 6.5765, 7.8247, 8.1851,
            7.8618, 8.0281, 7.4331, 6.6058, 7.2526, 8.2213, 8.0959, 7.3670,
            7.3575, 7.5147, 7.7548, 7.6411, 8.0551, 7.7205, 7.0005, 7.0623,
            7.9627, 8.1284, 7.1344, 6.3478, 7.5119, 8.2702, 7.7841, 6.7748,
            7.0791, 8.1276, 8.2608, 7.3165, 6.5692, 7.4370, 8.2215, 7.7704,
            6.7444, 7.0324, 8.0567, 8.0932, 7.1172, 6.6504, 7.4084, 8.0636,
            7.6619, 7.5083, 7.3280, 7.1565, 7.3020, 7.9501, 8.2391, 7.3646,
            6.5438, 7.2394, 8.0014, 7.9333, 8.1322, 8.0317, 7.0725, 6.6421,
            7.5770, 8.1553, 8.0256, 8.3684, 7.9741, 6.8770, 6.8468, 7.5025,
            7.7252, 8.1418, 8.5790, 8.1587, 8.2147, 7.8312, 6.8917, 6.8111,
            7.3538, 7.5772, 8.1167, 8.5809, 8.4856, 8.8977, 8.6952, 8.8651,
            8.6619, 8.4473, 8.5165, 8.0607, 8.0473, 7.5503, 7.5349, 7.1617,
            6.9317, 6.8067, 6.5337, 6.5154, 6.2640, 6.1889, 6.3083, 6.2692,
            6.2172, 6.3248, 6.3113, 6.2472, 6.2706, 6.1946, 6.1505, 6.1700,
            6.2428, 6.2311, 6.2552, 6.3695, 6.3744, 6.2875, 6.3246, 6.4330,
            6.3502, 6.1324, 6.2116, 6.2835, 6.2019, 6.2325, 6.3111, 6.2201,
            6.0703, 6.1215, 6.1582, 6.2194, 6.2264, 6.1755, 6.3936, 6.4422,
            6.3487, 6.2816, 6.3109, 6.4736, 6.3907, 6.4570, 7.0335, 7.0050,
            6.7155, 7.4317, 7.6024, 7.0500, 7.9943, 8.4366, 7.7527, 8.2450,
            8.9132, 8.2783, 8.2919, 9.1924, 8.7740, 7.9888, 8.8989, 8.7580,
            7.4619, 7.6799, 7.8705, 7.0010, 6.3645, 6.3430, 6.4666, 7.7534,
            8.4462, 7.9093, 7.8776, 8.6900, 8.2552, 6.8679, 6.9603, 6.9161,
            6.8397, 7.7744, 7.9404, 7.3932, 8.2811, 8.6606, 7.7163, 6.5473,
            6.7325, 6.5661, 7.5015, 8.3233, 7.9047, 7.2502, 8.0716, 7.9306,
            6.8746, 6.9314, 6.9781, 6.8960, 8.1089, 8.4751, 7.5662, 6.5039,
            6.3058, 6.3817, 7.6182, 8.5207, 8.1015, 6.6578, 6.1569, 6.3166,
            7.1620, 8.3668, 8.4533, 7.2511, 6.2139, 6.2947, 6.5745, 7.9156,
            8.5648, 7.8424, 6.7123, 7.0013, 6.8197, 7.3052, 8.2864, 8.1599,
            7.3589, 8.0477, 8.1553, 7.1753, 6.5211, 6.6171, 6.6107, 7.9116,
            8.5800, 7.8798, 7.1240, 7.8474, 7.6011, 6.9360, 7.1651, 7.0581,
            7.2659, 8.4490, 8.5778, 7.6637, 8.1163, 8.4514, 7.5296, 6.4769,
            6.4759, 6.4216, 7.2126, 7.9670, 7.6382, 7.6720, 8.8109, 8.6914,
            7.8419, 8.9315, 9.1035, 8.1013, 8.5533, 8.9577, 8.1540, 8.0316,
            8.5077, 7.8752, 7.0735, 7.5807, 7.3281, 6.7309, 6.9925, 6.8544,
            6.4861, 6.6206, 6.6496, 6.5338, 6.5317, 6.4931, 6.3447, 6.2721,
            6.2639, 6.2699, 6.2621, 6.2733, 6.1671, 6.1750, 6.2727, 6.2913,
            6.3671, 6.3381, 6.1923, 6.1443, 6.2212, 6.3082, 6.2905, 6.2520,
            6.2374, 6.2163, 6.2324, 6.2745, 6.3053, 6.2621, 6.2210, 6.2368,
            6.2084, 6.1969, 6.1850, 6.2235, 6.2993, 6.2716, 6.2048, 6.2329,
            6.3317, 6.2831, 6.1705, 6.3249, 6.3525, 6.2754, 6.3251, 6.2870,
            6.2328, 6.2971, 6.2716, 6.1686, 6.1111, 5.9925, 5.9810, 6.0490,
            6.0105, 6.0560, 6.1151, 6.2358, 6.3558, 6.2797, 6.1399, 6.1505,
            6.1736, 6.1328, 6.3206, 6.3420, 6.0844, 6.0542, 6.1813, 6.2690,
        };

        gr::rstt::noise_model noise_exp(6.257948, 0.203387);

        noise_level_estimator::sptr nle = noise_level_estimator::make(0.2, 60);

        gr::rstt::noise_model noise =
            nle->estimate(data, sizeof(data) / sizeof(float));
        CPPUNIT_ASSERT_DOUBLES_EQUAL(noise.lvl(-1), noise_exp.lvl(-1), 0.001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(noise.mean_lvl(), noise_exp.mean_lvl(), 0.0001);
        CPPUNIT_ASSERT_DOUBLES_EQUAL(noise.lvl(1), noise_exp.lvl(1), 0.001);
    }

  } /* namespace rstt */
} /* namespace gr */

