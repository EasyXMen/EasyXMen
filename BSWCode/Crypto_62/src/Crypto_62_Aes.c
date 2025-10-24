/**
 * Copyright (C) 2008-2025 isoft Infrastructure Software Co., Ltd.
 * SPDX-License-Identifier: LGPL-2.1-only-with-exception
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the
 * GNU Lesser General Public License as published by the Free Software Foundation; version 2.1.
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License along with this library;
 * if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 * or see <https://www.gnu.org/licenses/>.
 */
/*
 ************************************************************************************************************************
 **
 **  @file               : Crypto_62_Aes.c
 **  @author             : Jie.gU
 **  @date               : 2024/01/01
 **  @vendor             : isoft
 **  @description        : Rebuid for Crypto
 **
 ***********************************************************************************************************************/

/* PRQA S 4544,4542,0488,1252,1253,1290,3387,3440,2463,3138,3218,3228,3418,3472 ++ */ /* VL_QAC_Crypto */
/* PRQA S 1338,1842,1844,1862,1863,3226,3344,4461,2985,0342 ++ */                     /* VL_QAC_Crypto */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,1532,1503,2889  ++ */                    /* VL_QAC_Crypto */
/* =================================================== inclusions =================================================== */
#include "Crypto_62_Internal.h"
#if (CRYPTO_ALGORITHMFAM_AES == STD_ON)

#include "Crypto_62_Aes.h"
#define CRYPTO_62_START_SEC_CODE
#include "Crypto_62_MemMap.h"

/* ===================================================== macros ===================================================== */
/* PRQA S 3412 ++ */ /* VL_QAC_Crypto */
#define GET_UINT32_LE(n, b, i)                                                       \
    (n) = ((uint32)(b)[(i)]) | ((uint32)(b)[(i) + CRYPTO_CONST_1] << CRYPTO_CONST_8) \
          | ((uint32)(b)[(i) + CRYPTO_CONST_2] << CRYPTO_CONST_16)                   \
          | ((uint32)(b)[(i) + CRYPTO_CONST_3] << CRYPTO_CONST_24);

#define PUT_UINT32_LE(n, b, i)                                                         \
    (b)[(i)]                  = (uint8)(((n)) & CRYPTO_CONST_0xFF);                    \
    (b)[(i) + CRYPTO_CONST_1] = (uint8)(((n) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF);  \
    (b)[(i) + CRYPTO_CONST_2] = (uint8)(((n) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF); \
    (b)[(i) + CRYPTO_CONST_3] = (uint8)(((n) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);
/* PRQA S 3412 -- */
#define AES_RT0(idx) RT0[idx]
#define AES_RT1(idx) RT1[idx]
#define AES_RT2(idx) RT2[idx]
#define AES_RT3(idx) RT3[idx]

#define AES_FT0(idx) FT0[idx]
#define AES_FT1(idx) FT1[idx]
#define AES_FT2(idx) FT2[idx]
#define AES_FT3(idx) FT3[idx]
/*
 * Forward tables
 */
#define FT                                                                                                            \
                                                                                                                      \
    V(A5, 63, 63, C6), V(84, 7C, 7C, F8), V(99, 77, 77, EE), V(8D, 7B, 7B, F6), V(0D, F2, F2, FF), V(BD, 6B, 6B, D6), \
        V(B1, 6F, 6F, DE), V(54, C5, C5, 91), V(50, 30, 30, 60), V(03, 01, 01, 02), V(A9, 67, 67, CE),                \
        V(7D, 2B, 2B, 56), V(19, FE, FE, E7), V(62, D7, D7, B5), V(E6, AB, AB, 4D), V(9A, 76, 76, EC),                \
        V(45, CA, CA, 8F), V(9D, 82, 82, 1F), V(40, C9, C9, 89), V(87, 7D, 7D, FA), V(15, FA, FA, EF),                \
        V(EB, 59, 59, B2), V(C9, 47, 47, 8E), V(0B, F0, F0, FB), V(EC, AD, AD, 41), V(67, D4, D4, B3),                \
        V(FD, A2, A2, 5F), V(EA, AF, AF, 45), V(BF, 9C, 9C, 23), V(F7, A4, A4, 53), V(96, 72, 72, E4),                \
        V(5B, C0, C0, 9B), V(C2, B7, B7, 75), V(1C, FD, FD, E1), V(AE, 93, 93, 3D), V(6A, 26, 26, 4C),                \
        V(5A, 36, 36, 6C), V(41, 3F, 3F, 7E), V(02, F7, F7, F5), V(4F, CC, CC, 83), V(5C, 34, 34, 68),                \
        V(F4, A5, A5, 51), V(34, E5, E5, D1), V(08, F1, F1, F9), V(93, 71, 71, E2), V(73, D8, D8, AB),                \
        V(53, 31, 31, 62), V(3F, 15, 15, 2A), V(0C, 04, 04, 08), V(52, C7, C7, 95), V(65, 23, 23, 46),                \
        V(5E, C3, C3, 9D), V(28, 18, 18, 30), V(A1, 96, 96, 37), V(0F, 05, 05, 0A), V(B5, 9A, 9A, 2F),                \
        V(09, 07, 07, 0E), V(36, 12, 12, 24), V(9B, 80, 80, 1B), V(3D, E2, E2, DF), V(26, EB, EB, CD),                \
        V(69, 27, 27, 4E), V(CD, B2, B2, 7F), V(9F, 75, 75, EA), V(1B, 09, 09, 12), V(9E, 83, 83, 1D),                \
        V(74, 2C, 2C, 58), V(2E, 1A, 1A, 34), V(2D, 1B, 1B, 36), V(B2, 6E, 6E, DC), V(EE, 5A, 5A, B4),                \
        V(FB, A0, A0, 5B), V(F6, 52, 52, A4), V(4D, 3B, 3B, 76), V(61, D6, D6, B7), V(CE, B3, B3, 7D),                \
        V(7B, 29, 29, 52), V(3E, E3, E3, DD), V(71, 2F, 2F, 5E), V(97, 84, 84, 13), V(F5, 53, 53, A6),                \
        V(68, D1, D1, B9), V(00, 00, 00, 00), V(2C, ED, ED, C1), V(60, 20, 20, 40), V(1F, FC, FC, E3),                \
        V(C8, B1, B1, 79), V(ED, 5B, 5B, B6), V(BE, 6A, 6A, D4), V(46, CB, CB, 8D), V(D9, BE, BE, 67),                \
        V(4B, 39, 39, 72), V(DE, 4A, 4A, 94), V(D4, 4C, 4C, 98), V(E8, 58, 58, B0), V(4A, CF, CF, 85),                \
        V(6B, D0, D0, BB), V(2A, EF, EF, C5), V(E5, AA, AA, 4F), V(16, FB, FB, ED), V(C5, 43, 43, 86),                \
        V(D7, 4D, 4D, 9A), V(55, 33, 33, 66), V(94, 85, 85, 11), V(CF, 45, 45, 8A), V(10, F9, F9, E9),                \
        V(06, 02, 02, 04), V(81, 7F, 7F, FE), V(F0, 50, 50, A0), V(44, 3C, 3C, 78), V(BA, 9F, 9F, 25),                \
        V(E3, A8, A8, 4B), V(F3, 51, 51, A2), V(FE, A3, A3, 5D), V(C0, 40, 40, 80), V(8A, 8F, 8F, 05),                \
        V(AD, 92, 92, 3F), V(BC, 9D, 9D, 21), V(48, 38, 38, 70), V(04, F5, F5, F1), V(DF, BC, BC, 63),                \
        V(C1, B6, B6, 77), V(75, DA, DA, AF), V(63, 21, 21, 42), V(30, 10, 10, 20), V(1A, FF, FF, E5),                \
        V(0E, F3, F3, FD), V(6D, D2, D2, BF), V(4C, CD, CD, 81), V(14, 0C, 0C, 18), V(35, 13, 13, 26),                \
        V(2F, EC, EC, C3), V(E1, 5F, 5F, BE), V(A2, 97, 97, 35), V(CC, 44, 44, 88), V(39, 17, 17, 2E),                \
        V(57, C4, C4, 93), V(F2, A7, A7, 55), V(82, 7E, 7E, FC), V(47, 3D, 3D, 7A), V(AC, 64, 64, C8),                \
        V(E7, 5D, 5D, BA), V(2B, 19, 19, 32), V(95, 73, 73, E6), V(A0, 60, 60, C0), V(98, 81, 81, 19),                \
        V(D1, 4F, 4F, 9E), V(7F, DC, DC, A3), V(66, 22, 22, 44), V(7E, 2A, 2A, 54), V(AB, 90, 90, 3B),                \
        V(83, 88, 88, 0B), V(CA, 46, 46, 8C), V(29, EE, EE, C7), V(D3, B8, B8, 6B), V(3C, 14, 14, 28),                \
        V(79, DE, DE, A7), V(E2, 5E, 5E, BC), V(1D, 0B, 0B, 16), V(76, DB, DB, AD), V(3B, E0, E0, DB),                \
        V(56, 32, 32, 64), V(4E, 3A, 3A, 74), V(1E, 0A, 0A, 14), V(DB, 49, 49, 92), V(0A, 06, 06, 0C),                \
        V(6C, 24, 24, 48), V(E4, 5C, 5C, B8), V(5D, C2, C2, 9F), V(6E, D3, D3, BD), V(EF, AC, AC, 43),                \
        V(A6, 62, 62, C4), V(A8, 91, 91, 39), V(A4, 95, 95, 31), V(37, E4, E4, D3), V(8B, 79, 79, F2),                \
        V(32, E7, E7, D5), V(43, C8, C8, 8B), V(59, 37, 37, 6E), V(B7, 6D, 6D, DA), V(8C, 8D, 8D, 01),                \
        V(64, D5, D5, B1), V(D2, 4E, 4E, 9C), V(E0, A9, A9, 49), V(B4, 6C, 6C, D8), V(FA, 56, 56, AC),                \
        V(07, F4, F4, F3), V(25, EA, EA, CF), V(AF, 65, 65, CA), V(8E, 7A, 7A, F4), V(E9, AE, AE, 47),                \
        V(18, 08, 08, 10), V(D5, BA, BA, 6F), V(88, 78, 78, F0), V(6F, 25, 25, 4A), V(72, 2E, 2E, 5C),                \
        V(24, 1C, 1C, 38), V(F1, A6, A6, 57), V(C7, B4, B4, 73), V(51, C6, C6, 97), V(23, E8, E8, CB),                \
        V(7C, DD, DD, A1), V(9C, 74, 74, E8), V(21, 1F, 1F, 3E), V(DD, 4B, 4B, 96), V(DC, BD, BD, 61),                \
        V(86, 8B, 8B, 0D), V(85, 8A, 8A, 0F), V(90, 70, 70, E0), V(42, 3E, 3E, 7C), V(C4, B5, B5, 71),                \
        V(AA, 66, 66, CC), V(D8, 48, 48, 90), V(05, 03, 03, 06), V(01, F6, F6, F7), V(12, 0E, 0E, 1C),                \
        V(A3, 61, 61, C2), V(5F, 35, 35, 6A), V(F9, 57, 57, AE), V(D0, B9, B9, 69), V(91, 86, 86, 17),                \
        V(58, C1, C1, 99), V(27, 1D, 1D, 3A), V(B9, 9E, 9E, 27), V(38, E1, E1, D9), V(13, F8, F8, EB),                \
        V(B3, 98, 98, 2B), V(33, 11, 11, 22), V(BB, 69, 69, D2), V(70, D9, D9, A9), V(89, 8E, 8E, 07),                \
        V(A7, 94, 94, 33), V(B6, 9B, 9B, 2D), V(22, 1E, 1E, 3C), V(92, 87, 87, 15), V(20, E9, E9, C9),                \
        V(49, CE, CE, 87), V(FF, 55, 55, AA), V(78, 28, 28, 50), V(7A, DF, DF, A5), V(8F, 8C, 8C, 03),                \
        V(F8, A1, A1, 59), V(80, 89, 89, 09), V(17, 0D, 0D, 1A), V(DA, BF, BF, 65), V(31, E6, E6, D7),                \
        V(C6, 42, 42, 84), V(B8, 68, 68, D0), V(C3, 41, 41, 82), V(B0, 99, 99, 29), V(77, 2D, 2D, 5A),                \
        V(11, 0F, 0F, 1E), V(CB, B0, B0, 7B), V(FC, 54, 54, A8), V(D6, BB, BB, 6D), V(3A, 16, 16, 2C)

/* PRQA S 3613 ++ */ /* VL_QAC_Crypto */
#define V(a, b, c, d) 0x##a##b##c##d##u
CRYPTO_62_LOCAL const uint32 FT0[CRYPTO_CONST_256] = {FT};
#undef V

#if !defined(CRYPTO_AES_FEWER_TABLES)

#define V(a, b, c, d) 0x##b##c##d##a##u
CRYPTO_62_LOCAL const uint32 FT1[CRYPTO_CONST_256] = {FT};
#undef V

#define V(a, b, c, d) 0x##c##d##a##b##u
CRYPTO_62_LOCAL const uint32 FT2[CRYPTO_CONST_256] = {FT};
#undef V

#define V(a, b, c, d) 0x##d##a##b##c##u
CRYPTO_62_LOCAL const uint32 FT3[CRYPTO_CONST_256] = {FT};
#undef V

#endif /* !CRYPTO_AES_FEWER_TABLES */

#undef FT

/*
 * Reverse tables
 */
#define RT                                                                                                            \
                                                                                                                      \
    V(50, A7, F4, 51), V(53, 65, 41, 7E), V(C3, A4, 17, 1A), V(96, 5E, 27, 3A), V(CB, 6B, AB, 3B), V(F1, 45, 9D, 1F), \
        V(AB, 58, FA, AC), V(93, 03, E3, 4B), V(55, FA, 30, 20), V(F6, 6D, 76, AD), V(91, 76, CC, 88),                \
        V(25, 4C, 02, F5), V(FC, D7, E5, 4F), V(D7, CB, 2A, C5), V(80, 44, 35, 26), V(8F, A3, 62, B5),                \
        V(49, 5A, B1, DE), V(67, 1B, BA, 25), V(98, 0E, EA, 45), V(E1, C0, FE, 5D), V(02, 75, 2F, C3),                \
        V(12, F0, 4C, 81), V(A3, 97, 46, 8D), V(C6, F9, D3, 6B), V(E7, 5F, 8F, 03), V(95, 9C, 92, 15),                \
        V(EB, 7A, 6D, BF), V(DA, 59, 52, 95), V(2D, 83, BE, D4), V(D3, 21, 74, 58), V(29, 69, E0, 49),                \
        V(44, C8, C9, 8E), V(6A, 89, C2, 75), V(78, 79, 8E, F4), V(6B, 3E, 58, 99), V(DD, 71, B9, 27),                \
        V(B6, 4F, E1, BE), V(17, AD, 88, F0), V(66, AC, 20, C9), V(B4, 3A, CE, 7D), V(18, 4A, DF, 63),                \
        V(82, 31, 1A, E5), V(60, 33, 51, 97), V(45, 7F, 53, 62), V(E0, 77, 64, B1), V(84, AE, 6B, BB),                \
        V(1C, A0, 81, FE), V(94, 2B, 08, F9), V(58, 68, 48, 70), V(19, FD, 45, 8F), V(87, 6C, DE, 94),                \
        V(B7, F8, 7B, 52), V(23, D3, 73, AB), V(E2, 02, 4B, 72), V(57, 8F, 1F, E3), V(2A, AB, 55, 66),                \
        V(07, 28, EB, B2), V(03, C2, B5, 2F), V(9A, 7B, C5, 86), V(A5, 08, 37, D3), V(F2, 87, 28, 30),                \
        V(B2, A5, BF, 23), V(BA, 6A, 03, 02), V(5C, 82, 16, ED), V(2B, 1C, CF, 8A), V(92, B4, 79, A7),                \
        V(F0, F2, 07, F3), V(A1, E2, 69, 4E), V(CD, F4, DA, 65), V(D5, BE, 05, 06), V(1F, 62, 34, D1),                \
        V(8A, FE, A6, C4), V(9D, 53, 2E, 34), V(A0, 55, F3, A2), V(32, E1, 8A, 05), V(75, EB, F6, A4),                \
        V(39, EC, 83, 0B), V(AA, EF, 60, 40), V(06, 9F, 71, 5E), V(51, 10, 6E, BD), V(F9, 8A, 21, 3E),                \
        V(3D, 06, DD, 96), V(AE, 05, 3E, DD), V(46, BD, E6, 4D), V(B5, 8D, 54, 91), V(05, 5D, C4, 71),                \
        V(6F, D4, 06, 04), V(FF, 15, 50, 60), V(24, FB, 98, 19), V(97, E9, BD, D6), V(CC, 43, 40, 89),                \
        V(77, 9E, D9, 67), V(BD, 42, E8, B0), V(88, 8B, 89, 07), V(38, 5B, 19, E7), V(DB, EE, C8, 79),                \
        V(47, 0A, 7C, A1), V(E9, 0F, 42, 7C), V(C9, 1E, 84, F8), V(00, 00, 00, 00), V(83, 86, 80, 09),                \
        V(48, ED, 2B, 32), V(AC, 70, 11, 1E), V(4E, 72, 5A, 6C), V(FB, FF, 0E, FD), V(56, 38, 85, 0F),                \
        V(1E, D5, AE, 3D), V(27, 39, 2D, 36), V(64, D9, 0F, 0A), V(21, A6, 5C, 68), V(D1, 54, 5B, 9B),                \
        V(3A, 2E, 36, 24), V(B1, 67, 0A, 0C), V(0F, E7, 57, 93), V(D2, 96, EE, B4), V(9E, 91, 9B, 1B),                \
        V(4F, C5, C0, 80), V(A2, 20, DC, 61), V(69, 4B, 77, 5A), V(16, 1A, 12, 1C), V(0A, BA, 93, E2),                \
        V(E5, 2A, A0, C0), V(43, E0, 22, 3C), V(1D, 17, 1B, 12), V(0B, 0D, 09, 0E), V(AD, C7, 8B, F2),                \
        V(B9, A8, B6, 2D), V(C8, A9, 1E, 14), V(85, 19, F1, 57), V(4C, 07, 75, AF), V(BB, DD, 99, EE),                \
        V(FD, 60, 7F, A3), V(9F, 26, 01, F7), V(BC, F5, 72, 5C), V(C5, 3B, 66, 44), V(34, 7E, FB, 5B),                \
        V(76, 29, 43, 8B), V(DC, C6, 23, CB), V(68, FC, ED, B6), V(63, F1, E4, B8), V(CA, DC, 31, D7),                \
        V(10, 85, 63, 42), V(40, 22, 97, 13), V(20, 11, C6, 84), V(7D, 24, 4A, 85), V(F8, 3D, BB, D2),                \
        V(11, 32, F9, AE), V(6D, A1, 29, C7), V(4B, 2F, 9E, 1D), V(F3, 30, B2, DC), V(EC, 52, 86, 0D),                \
        V(D0, E3, C1, 77), V(6C, 16, B3, 2B), V(99, B9, 70, A9), V(FA, 48, 94, 11), V(22, 64, E9, 47),                \
        V(C4, 8C, FC, A8), V(1A, 3F, F0, A0), V(D8, 2C, 7D, 56), V(EF, 90, 33, 22), V(C7, 4E, 49, 87),                \
        V(C1, D1, 38, D9), V(FE, A2, CA, 8C), V(36, 0B, D4, 98), V(CF, 81, F5, A6), V(28, DE, 7A, A5),                \
        V(26, 8E, B7, DA), V(A4, BF, AD, 3F), V(E4, 9D, 3A, 2C), V(0D, 92, 78, 50), V(9B, CC, 5F, 6A),                \
        V(62, 46, 7E, 54), V(C2, 13, 8D, F6), V(E8, B8, D8, 90), V(5E, F7, 39, 2E), V(F5, AF, C3, 82),                \
        V(BE, 80, 5D, 9F), V(7C, 93, D0, 69), V(A9, 2D, D5, 6F), V(B3, 12, 25, CF), V(3B, 99, AC, C8),                \
        V(A7, 7D, 18, 10), V(6E, 63, 9C, E8), V(7B, BB, 3B, DB), V(09, 78, 26, CD), V(F4, 18, 59, 6E),                \
        V(01, B7, 9A, EC), V(A8, 9A, 4F, 83), V(65, 6E, 95, E6), V(7E, E6, FF, AA), V(08, CF, BC, 21),                \
        V(E6, E8, 15, EF), V(D9, 9B, E7, BA), V(CE, 36, 6F, 4A), V(D4, 09, 9F, EA), V(D6, 7C, B0, 29),                \
        V(AF, B2, A4, 31), V(31, 23, 3F, 2A), V(30, 94, A5, C6), V(C0, 66, A2, 35), V(37, BC, 4E, 74),                \
        V(A6, CA, 82, FC), V(B0, D0, 90, E0), V(15, D8, A7, 33), V(4A, 98, 04, F1), V(F7, DA, EC, 41),                \
        V(0E, 50, CD, 7F), V(2F, F6, 91, 17), V(8D, D6, 4D, 76), V(4D, B0, EF, 43), V(54, 4D, AA, CC),                \
        V(DF, 04, 96, E4), V(E3, B5, D1, 9E), V(1B, 88, 6A, 4C), V(B8, 1F, 2C, C1), V(7F, 51, 65, 46),                \
        V(04, EA, 5E, 9D), V(5D, 35, 8C, 01), V(73, 74, 87, FA), V(2E, 41, 0B, FB), V(5A, 1D, 67, B3),                \
        V(52, D2, DB, 92), V(33, 56, 10, E9), V(13, 47, D6, 6D), V(8C, 61, D7, 9A), V(7A, 0C, A1, 37),                \
        V(8E, 14, F8, 59), V(89, 3C, 13, EB), V(EE, 27, A9, CE), V(35, C9, 61, B7), V(ED, E5, 1C, E1),                \
        V(3C, B1, 47, 7A), V(59, DF, D2, 9C), V(3F, 73, F2, 55), V(79, CE, 14, 18), V(BF, 37, C7, 73),                \
        V(EA, CD, F7, 53), V(5B, AA, FD, 5F), V(14, 6F, 3D, DF), V(86, DB, 44, 78), V(81, F3, AF, CA),                \
        V(3E, C4, 68, B9), V(2C, 34, 24, 38), V(5F, 40, A3, C2), V(72, C3, 1D, 16), V(0C, 25, E2, BC),                \
        V(8B, 49, 3C, 28), V(41, 95, 0D, FF), V(71, 01, A8, 39), V(DE, B3, 0C, 08), V(9C, E4, B4, D8),                \
        V(90, C1, 56, 64), V(61, 84, CB, 7B), V(70, B6, 32, D5), V(74, 5C, 6C, 48), V(42, 57, B8, D0)

#define V(a, b, c, d) 0x##a##b##c##d##u
CRYPTO_62_LOCAL const uint32 RT0[CRYPTO_CONST_256] = {RT};
#undef V

#if !defined(CRYPTO_AES_FEWER_TABLES)

#define V(a, b, c, d) 0x##b##c##d##a##u
CRYPTO_62_LOCAL const uint32 RT1[CRYPTO_CONST_256] = {RT};
#undef V

#define V(a, b, c, d) 0x##c##d##a##b##u
CRYPTO_62_LOCAL const uint32 RT2[CRYPTO_CONST_256] = {RT};
#undef V

#define V(a, b, c, d) 0x##d##a##b##c##u
CRYPTO_62_LOCAL const uint32 RT3[CRYPTO_CONST_256] = {RT};
#undef V
/* PRQA S 3613 -- */
#endif /* !CRYPTO_AES_FEWER_TABLES */

#undef RT

#define AES_FROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)                                                                 \
    do                                                                                                             \
    {                                                                                                              \
        (X0) = *RK++ ^ AES_FT0(((Y0)) & CRYPTO_CONST_0xFF) ^ AES_FT1(((Y1) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_FT2(((Y2) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_FT3(((Y3) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
                                                                                                                   \
        (X1) = *RK++ ^ AES_FT0(((Y1)) & CRYPTO_CONST_0xFF) ^ AES_FT1(((Y2) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_FT2(((Y3) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_FT3(((Y0) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
                                                                                                                   \
        (X2) = *RK++ ^ AES_FT0(((Y2)) & CRYPTO_CONST_0xFF) ^ AES_FT1(((Y3) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_FT2(((Y0) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_FT3(((Y1) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
                                                                                                                   \
        (X3) = *RK++ ^ AES_FT0(((Y3)) & CRYPTO_CONST_0xFF) ^ AES_FT1(((Y0) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_FT2(((Y1) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_FT3(((Y2) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
    } while (0)

#define AES_RROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3)                                                                 \
    do                                                                                                             \
    {                                                                                                              \
        (X0) = *RK++ ^ AES_RT0(((Y0)) & CRYPTO_CONST_0xFF) ^ AES_RT1(((Y3) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_RT2(((Y2) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_RT3(((Y1) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
                                                                                                                   \
        (X1) = *RK++ ^ AES_RT0(((Y1)) & CRYPTO_CONST_0xFF) ^ AES_RT1(((Y0) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_RT2(((Y3) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_RT3(((Y2) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
                                                                                                                   \
        (X2) = *RK++ ^ AES_RT0(((Y2)) & CRYPTO_CONST_0xFF) ^ AES_RT1(((Y1) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_RT2(((Y0) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_RT3(((Y3) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
                                                                                                                   \
        (X3) = *RK++ ^ AES_RT0(((Y3)) & CRYPTO_CONST_0xFF) ^ AES_RT1(((Y2) >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF) \
               ^ AES_RT2(((Y1) >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF)                                            \
               ^ AES_RT3(((Y0) >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF);                                           \
    } while (0)
/* ================================================ type definitions ================================================ */

/* ========================================== internal function Declarations ========================================*/

/* =========================================== Internal data Definitions =========================================== */

/*
 * Forward S-box
 */
CRYPTO_62_LOCAL const uint8 FSb[CRYPTO_CONST_256] = {
    0x63u, 0x7Cu, 0x77u, 0x7Bu, 0xF2u, 0x6Bu, 0x6Fu, 0xC5u, 0x30u, 0x01u, 0x67u, 0x2Bu, 0xFEu, 0xD7u, 0xABu, 0x76u,
    0xCAu, 0x82u, 0xC9u, 0x7Du, 0xFAu, 0x59u, 0x47u, 0xF0u, 0xADu, 0xD4u, 0xA2u, 0xAFu, 0x9Cu, 0xA4u, 0x72u, 0xC0u,
    0xB7u, 0xFDu, 0x93u, 0x26u, 0x36u, 0x3Fu, 0xF7u, 0xCCu, 0x34u, 0xA5u, 0xE5u, 0xF1u, 0x71u, 0xD8u, 0x31u, 0x15u,
    0x04u, 0xC7u, 0x23u, 0xC3u, 0x18u, 0x96u, 0x05u, 0x9Au, 0x07u, 0x12u, 0x80u, 0xE2u, 0xEBu, 0x27u, 0xB2u, 0x75u,
    0x09u, 0x83u, 0x2Cu, 0x1Au, 0x1Bu, 0x6Eu, 0x5Au, 0xA0u, 0x52u, 0x3Bu, 0xD6u, 0xB3u, 0x29u, 0xE3u, 0x2Fu, 0x84u,
    0x53u, 0xD1u, 0x00u, 0xEDu, 0x20u, 0xFCu, 0xB1u, 0x5Bu, 0x6Au, 0xCBu, 0xBEu, 0x39u, 0x4Au, 0x4Cu, 0x58u, 0xCFu,
    0xD0u, 0xEFu, 0xAAu, 0xFBu, 0x43u, 0x4Du, 0x33u, 0x85u, 0x45u, 0xF9u, 0x02u, 0x7Fu, 0x50u, 0x3Cu, 0x9Fu, 0xA8u,
    0x51u, 0xA3u, 0x40u, 0x8Fu, 0x92u, 0x9Du, 0x38u, 0xF5u, 0xBCu, 0xB6u, 0xDAu, 0x21u, 0x10u, 0xFFu, 0xF3u, 0xD2u,
    0xCDu, 0x0Cu, 0x13u, 0xECu, 0x5Fu, 0x97u, 0x44u, 0x17u, 0xC4u, 0xA7u, 0x7Eu, 0x3Du, 0x64u, 0x5Du, 0x19u, 0x73u,
    0x60u, 0x81u, 0x4Fu, 0xDCu, 0x22u, 0x2Au, 0x90u, 0x88u, 0x46u, 0xEEu, 0xB8u, 0x14u, 0xDEu, 0x5Eu, 0x0Bu, 0xDBu,
    0xE0u, 0x32u, 0x3Au, 0x0Au, 0x49u, 0x06u, 0x24u, 0x5Cu, 0xC2u, 0xD3u, 0xACu, 0x62u, 0x91u, 0x95u, 0xE4u, 0x79u,
    0xE7u, 0xC8u, 0x37u, 0x6Du, 0x8Du, 0xD5u, 0x4Eu, 0xA9u, 0x6Cu, 0x56u, 0xF4u, 0xEAu, 0x65u, 0x7Au, 0xAEu, 0x08u,
    0xBAu, 0x78u, 0x25u, 0x2Eu, 0x1Cu, 0xA6u, 0xB4u, 0xC6u, 0xE8u, 0xDDu, 0x74u, 0x1Fu, 0x4Bu, 0xBDu, 0x8Bu, 0x8Au,
    0x70u, 0x3Eu, 0xB5u, 0x66u, 0x48u, 0x03u, 0xF6u, 0x0Eu, 0x61u, 0x35u, 0x57u, 0xB9u, 0x86u, 0xC1u, 0x1Du, 0x9Eu,
    0xE1u, 0xF8u, 0x98u, 0x11u, 0x69u, 0xD9u, 0x8Eu, 0x94u, 0x9Bu, 0x1Eu, 0x87u, 0xE9u, 0xCEu, 0x55u, 0x28u, 0xDFu,
    0x8Cu, 0xA1u, 0x89u, 0x0Du, 0xBFu, 0xE6u, 0x42u, 0x68u, 0x41u, 0x99u, 0x2Du, 0x0Fu, 0xB0u, 0x54u, 0xBBu, 0x16u};

/*
 * Reverse S-box
 */
CRYPTO_62_LOCAL const uint8 RSb[CRYPTO_CONST_256] = {
    0x52U, 0x09U, 0x6AU, 0xD5U, 0x30U, 0x36U, 0xA5U, 0x38U, 0xBFU, 0x40U, 0xA3U, 0x9EU, 0x81U, 0xF3U, 0xD7U, 0xFBU,
    0x7CU, 0xE3U, 0x39U, 0x82U, 0x9BU, 0x2FU, 0xFFU, 0x87U, 0x34U, 0x8EU, 0x43U, 0x44U, 0xC4U, 0xDEU, 0xE9U, 0xCBU,
    0x54U, 0x7BU, 0x94U, 0x32U, 0xA6U, 0xC2U, 0x23U, 0x3DU, 0xEEU, 0x4CU, 0x95U, 0x0BU, 0x42U, 0xFAU, 0xC3U, 0x4EU,
    0x08U, 0x2EU, 0xA1U, 0x66U, 0x28U, 0xD9U, 0x24U, 0xB2U, 0x76U, 0x5BU, 0xA2U, 0x49U, 0x6DU, 0x8BU, 0xD1U, 0x25U,
    0x72U, 0xF8U, 0xF6U, 0x64U, 0x86U, 0x68U, 0x98U, 0x16U, 0xD4U, 0xA4U, 0x5CU, 0xCCU, 0x5DU, 0x65U, 0xB6U, 0x92U,
    0x6CU, 0x70U, 0x48U, 0x50U, 0xFDU, 0xEDU, 0xB9U, 0xDAU, 0x5EU, 0x15U, 0x46U, 0x57U, 0xA7U, 0x8DU, 0x9DU, 0x84U,
    0x90U, 0xD8U, 0xABU, 0x00U, 0x8CU, 0xBCU, 0xD3U, 0x0AU, 0xF7U, 0xE4U, 0x58U, 0x05U, 0xB8U, 0xB3U, 0x45U, 0x06U,
    0xD0U, 0x2CU, 0x1EU, 0x8FU, 0xCAU, 0x3FU, 0x0FU, 0x02U, 0xC1U, 0xAFU, 0xBDU, 0x03U, 0x01U, 0x13U, 0x8AU, 0x6BU,
    0x3AU, 0x91U, 0x11U, 0x41U, 0x4FU, 0x67U, 0xDCU, 0xEAU, 0x97U, 0xF2U, 0xCFU, 0xCEU, 0xF0U, 0xB4U, 0xE6U, 0x73U,
    0x96U, 0xACU, 0x74U, 0x22U, 0xE7U, 0xADU, 0x35U, 0x85U, 0xE2U, 0xF9U, 0x37U, 0xE8U, 0x1CU, 0x75U, 0xDFU, 0x6EU,
    0x47U, 0xF1U, 0x1AU, 0x71U, 0x1DU, 0x29U, 0xC5U, 0x89U, 0x6FU, 0xB7U, 0x62U, 0x0EU, 0xAAU, 0x18U, 0xBEU, 0x1BU,
    0xFCU, 0x56U, 0x3EU, 0x4BU, 0xC6U, 0xD2U, 0x79U, 0x20U, 0x9AU, 0xDBU, 0xC0U, 0xFEU, 0x78U, 0xCDU, 0x5AU, 0xF4U,
    0x1FU, 0xDDU, 0xA8U, 0x33U, 0x88U, 0x07U, 0xC7U, 0x31U, 0xB1U, 0x12U, 0x10U, 0x59U, 0x27U, 0x80U, 0xECU, 0x5FU,
    0x60U, 0x51U, 0x7FU, 0xA9U, 0x19U, 0xB5U, 0x4AU, 0x0DU, 0x2DU, 0xE5U, 0x7AU, 0x9FU, 0x93U, 0xC9U, 0x9CU, 0xEFU,
    0xA0U, 0xE0U, 0x3BU, 0x4DU, 0xAEU, 0x2AU, 0xF5U, 0xB0U, 0xC8U, 0xEBU, 0xBBU, 0x3CU, 0x83U, 0x53U, 0x99U, 0x61U,
    0x17U, 0x2BU, 0x04U, 0x7EU, 0xBAU, 0x77U, 0xD6U, 0x26U, 0xE1U, 0x69U, 0x14U, 0x63U, 0x55U, 0x21U, 0x0CU, 0x7DU};
/*
 * Round constants
 */
CRYPTO_62_LOCAL const uint32 RCON[CRYPTO_CONST_10] = {
    0x00000001U,
    0x00000002U,
    0x00000004U,
    0x00000008U,
    0x00000010U,
    0x00000020U,
    0x00000040U,
    0x00000080U,
    0x0000001BU,
    0x00000036U};
/* ========================================== external data definition ============================================= */

/* =================================================== Global Function Definitions ================================== */

/******************************************************************************/
/*
 * Brief               Generate round keys for encryption.
 *
 * Param-Name[in]      key: The original key for encryption.
 *                     keybits: The length of input key in bit.
 * Param-Name[in/out]  ctx: The context of AES.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* VL_QAC_Crypto */
void Crypto_Aes_Setkey_Enc(Crypto_AESData* ctx, uint8* key, uint16 keybits)
/* PRQA S 3673 -- */
{
    uint16  i;
    uint32* RK;
    switch (keybits)
    {
    case CRYPTO_CONST_128:
        ctx->nr = CRYPTO_CONST_10;
        break;
    case CRYPTO_CONST_192:
        ctx->nr = CRYPTO_CONST_12;
        break;
    case CRYPTO_CONST_256:
        ctx->nr = CRYPTO_CONST_14;
        break;
    default:
        /*do nothing*/
        break;
    }

    ctx->rk = RK = ctx->buf;

    for (i = 0; i < (keybits >> CRYPTO_CONST_5); i++)
    {
        GET_UINT32_LE(RK[i], key, i << CRYPTO_CONST_2); /* PRQA S 4397 */ /* VL_QAC_Crypto */
    }

    switch (ctx->nr)
    {
    case CRYPTO_CONST_10:

        for (i = 0; i < CRYPTO_CONST_10; i++, RK += CRYPTO_CONST_4)
        {
            RK[CRYPTO_CONST_4] =
                RK[0] ^ RCON[i] ^ ((uint32)FSb[(RK[CRYPTO_CONST_3] >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF])
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_3] >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_3] >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_3]) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

            RK[CRYPTO_CONST_5] = RK[CRYPTO_CONST_1] ^ RK[CRYPTO_CONST_4];
            RK[CRYPTO_CONST_6] = RK[CRYPTO_CONST_2] ^ RK[CRYPTO_CONST_5];
            RK[CRYPTO_CONST_7] = RK[CRYPTO_CONST_3] ^ RK[CRYPTO_CONST_6];
        }
        break;

    case CRYPTO_CONST_12:

        for (i = 0; i < CRYPTO_CONST_8; i++, RK += CRYPTO_CONST_6)
        {
            RK[CRYPTO_CONST_6] =
                RK[0] ^ RCON[i] ^ ((uint32)FSb[(RK[CRYPTO_CONST_5] >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF])
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_5] >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_5] >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_5]) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

            RK[CRYPTO_CONST_7]  = RK[CRYPTO_CONST_1] ^ RK[CRYPTO_CONST_6];
            RK[CRYPTO_CONST_8]  = RK[CRYPTO_CONST_2] ^ RK[CRYPTO_CONST_7];
            RK[CRYPTO_CONST_9]  = RK[CRYPTO_CONST_3] ^ RK[CRYPTO_CONST_8];
            RK[CRYPTO_CONST_10] = RK[CRYPTO_CONST_4] ^ RK[CRYPTO_CONST_9];
            RK[CRYPTO_CONST_11] = RK[CRYPTO_CONST_5] ^ RK[CRYPTO_CONST_10];
        }
        break;

    case CRYPTO_CONST_14:

        for (i = 0; i < CRYPTO_CONST_7; i++, RK += CRYPTO_CONST_8)
        {
            RK[CRYPTO_CONST_8] =
                RK[0] ^ RCON[i] ^ ((uint32)FSb[(RK[CRYPTO_CONST_7] >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF])
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_7] >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_7] >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_7]) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

            RK[CRYPTO_CONST_9]  = RK[CRYPTO_CONST_1] ^ RK[CRYPTO_CONST_8];
            RK[CRYPTO_CONST_10] = RK[CRYPTO_CONST_2] ^ RK[CRYPTO_CONST_9];
            RK[CRYPTO_CONST_11] = RK[CRYPTO_CONST_3] ^ RK[CRYPTO_CONST_10];

            RK[CRYPTO_CONST_12] =
                RK[CRYPTO_CONST_4] ^ ((uint32)FSb[(RK[CRYPTO_CONST_11]) & CRYPTO_CONST_0xFF])
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_11] >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_11] >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
                ^ ((uint32)FSb[(RK[CRYPTO_CONST_11] >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

            RK[CRYPTO_CONST_13] = RK[CRYPTO_CONST_5] ^ RK[CRYPTO_CONST_12];
            RK[CRYPTO_CONST_14] = RK[CRYPTO_CONST_6] ^ RK[CRYPTO_CONST_13];
            RK[CRYPTO_CONST_15] = RK[CRYPTO_CONST_7] ^ RK[CRYPTO_CONST_14];
        }
        break;
    default:
        /*do nothing*/
        break;
    }
}

/******************************************************************************/
/*
 * Brief               Generate round keys for encryption.
 *
 * Param-Name[in]      key: The original key for encryption.
 *                     keybits: The length of input key in bit.
 * Param-Name[in/out]  ctx: The context of AES.
 * Param-Name[out]     None
 * Return              None
 */
/******************************************************************************/
void Crypto_Aes_Setkey_Dec(Crypto_AESData* ctx, uint8* key, uint16 keybits)
{
    uint8          i, j;
    Crypto_AESData cty;
    uint32*        RK;
    uint32*        SK; /* PRQA S 3678 */ /* VL_QAC_Crypto */

    (void)IStdLib_MemSet(&cty, 0, sizeof(Crypto_AESData));
    ctx->rk = RK = ctx->buf;

    /* Also checks keybits */
    Crypto_Aes_Setkey_Enc(&cty, key, keybits);
    ctx->nr = cty.nr;

    SK = cty.rk + cty.nr * CRYPTO_CONST_4; /* PRQA S 3395 */ /* VL_QAC_Crypto */

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    /* PRQA S 2462 ++ */ /* VL_QAC_Crypto */
    for (i = ctx->nr - CRYPTO_CONST_1, SK -= CRYPTO_CONST_8; i > 0; i--, SK -= CRYPTO_CONST_8)
    /* PRQA S 2462 -- */
    {
        for (j = 0; j < CRYPTO_CONST_4; j++, SK++)
        {
            *RK++ = AES_RT0(FSb[(*SK) & CRYPTO_CONST_0xFF]) ^ AES_RT1(FSb[(*SK >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF])
                    ^ AES_RT2(FSb[(*SK >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF])
                    ^ AES_RT3(FSb[(*SK >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF]);
        }
    }

    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++;
    *RK++ = *SK++; /* PRQA S 2983 */ /* VL_QAC_Crypto */
}

/******************************************************************************/
/*
 * Brief               Inner AES encryption on a 128 bits block, so you do not
 *                     need to care about the group mode.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     input: Buffer filled with 128 bits plaintext.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer filled with ciphertext corresponded with
 *                             the input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* VL_QAC_Crypto */
Std_ReturnType
    Internal_Aes_Encrypt(Crypto_AESData* ctx, const uint8 input[CRYPTO_CONST_16], uint8 output[CRYPTO_CONST_16])
/* PRQA S 3673 -- */
{
    uint8          i;
    Std_ReturnType ret = E_OK;

    uint32 *RK, X0, X1, X2, X3, Y0, Y1, Y2, Y3; /* PRQA S 3678 */ /* VL_QAC_Crypto */

    RK = ctx->rk;

    GET_UINT32_LE(X0, input, 0); /* PRQA S 1840  */ /* VL_QAC_Crypto */
    X0 ^= *RK++;
    GET_UINT32_LE(X1, input, CRYPTO_CONST_4);
    X1 ^= *RK++;
    GET_UINT32_LE(X2, input, CRYPTO_CONST_8);
    X2 ^= *RK++;
    GET_UINT32_LE(X3, input, CRYPTO_CONST_12);
    X3 ^= *RK++;

    for (i = (ctx->nr >> CRYPTO_CONST_1) - CRYPTO_CONST_1; i > 0; i--)
    {
        AES_FROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3); /* PRQA S 3473,2743 */ /* VL_QAC_Crypto */
        AES_FROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3); /* PRQA S 3473,2743 */ /* VL_QAC_Crypto */
    }

    AES_FROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3); /* PRQA S 3473,2743 */ /* VL_QAC_Crypto */

    X0 = *RK++ ^ ((uint32)FSb[(Y0)&CRYPTO_CONST_0xFF])
         ^ ((uint32)FSb[(Y1 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)FSb[(Y2 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)FSb[(Y3 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    X1 = *RK++ ^ ((uint32)FSb[(Y1)&CRYPTO_CONST_0xFF])
         ^ ((uint32)FSb[(Y2 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)FSb[(Y3 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)FSb[(Y0 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    X2 = *RK++ ^ ((uint32)FSb[(Y2)&CRYPTO_CONST_0xFF])
         ^ ((uint32)FSb[(Y3 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)FSb[(Y0 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)FSb[(Y1 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    X3 = *RK++ ^ ((uint32)FSb[(Y3)&CRYPTO_CONST_0xFF])
         ^ ((uint32)FSb[(Y0 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)FSb[(Y1 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)FSb[(Y2 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    PUT_UINT32_LE(X0, output, CRYPTO_CONST_0);
    PUT_UINT32_LE(X1, output, CRYPTO_CONST_4);
    PUT_UINT32_LE(X2, output, CRYPTO_CONST_8);
    PUT_UINT32_LE(X3, output, CRYPTO_CONST_12);

    (void)IStdLib_MemSet(&X0, 0, sizeof(X0));
    (void)IStdLib_MemSet(&X1, 0, sizeof(X1));
    (void)IStdLib_MemSet(&X2, 0, sizeof(X2));
    (void)IStdLib_MemSet(&X3, 0, sizeof(X3));

    (void)IStdLib_MemSet(&Y0, 0, sizeof(Y0));
    (void)IStdLib_MemSet(&Y1, 0, sizeof(Y1));
    (void)IStdLib_MemSet(&Y2, 0, sizeof(Y2));
    (void)IStdLib_MemSet(&Y3, 0, sizeof(Y3));

    (void)IStdLib_MemSet(&RK, 0, sizeof(RK));

    return ret;
}

/******************************************************************************/
/*
 * Brief               Inner AES decryption on a 128 bits block, so you do not
 *                     need to care about the group mode.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     input: Buffer filled with 128 bits ciphertext.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: Buffer filled with plaintext corresponded with
 *                             the input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
/* PRQA S 3673 ++ */ /* VL_QAC_Crypto */
Std_ReturnType
    Internal_Aes_Decrypt(Crypto_AESData* ctx, const uint8 input[CRYPTO_CONST_16], uint8 output[CRYPTO_CONST_16])
/* PRQA S 3673 -- */
{
    uint8          i;
    Std_ReturnType ret = E_OK;
    uint32 *       RK, X0, X1, X2, X3, Y0, Y1, Y2, Y3; /* PRQA S 3678 */ /* VL_QAC_Crypto */

    RK = ctx->rk;

    GET_UINT32_LE(X0, input, CRYPTO_CONST_0);
    X0 ^= *RK++;
    GET_UINT32_LE(X1, input, CRYPTO_CONST_4);
    X1 ^= *RK++;
    GET_UINT32_LE(X2, input, CRYPTO_CONST_8);
    X2 ^= *RK++;
    GET_UINT32_LE(X3, input, CRYPTO_CONST_12);
    X3 ^= *RK++;

    for (i = (ctx->nr >> CRYPTO_CONST_1) - CRYPTO_CONST_1; i > 0; i--)
    {
        AES_RROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3); /* PRQA S 3473,2743 */ /* VL_QAC_Crypto */
        AES_RROUND(X0, X1, X2, X3, Y0, Y1, Y2, Y3); /* PRQA S 3473,2743 */ /* VL_QAC_Crypto */
    }

    AES_RROUND(Y0, Y1, Y2, Y3, X0, X1, X2, X3); /* PRQA S 3473,2743 */ /* VL_QAC_Crypto */

    X0 = *RK++ ^ ((uint32)RSb[(Y0)&CRYPTO_CONST_0xFF])
         ^ ((uint32)RSb[(Y3 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)RSb[(Y2 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)RSb[(Y1 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    X1 = *RK++ ^ ((uint32)RSb[(Y1)&CRYPTO_CONST_0xFF])
         ^ ((uint32)RSb[(Y0 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)RSb[(Y3 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)RSb[(Y2 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    X2 = *RK++ ^ ((uint32)RSb[(Y2)&CRYPTO_CONST_0xFF])
         ^ ((uint32)RSb[(Y1 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)RSb[(Y0 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)RSb[(Y3 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    X3 = *RK++ ^ ((uint32)RSb[(Y3)&CRYPTO_CONST_0xFF])
         ^ ((uint32)RSb[(Y2 >> CRYPTO_CONST_8) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_8)
         ^ ((uint32)RSb[(Y1 >> CRYPTO_CONST_16) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_16)
         ^ ((uint32)RSb[(Y0 >> CRYPTO_CONST_24) & CRYPTO_CONST_0xFF] << CRYPTO_CONST_24);

    PUT_UINT32_LE(X0, output, CRYPTO_CONST_0);
    PUT_UINT32_LE(X1, output, CRYPTO_CONST_4);
    PUT_UINT32_LE(X2, output, CRYPTO_CONST_8);
    PUT_UINT32_LE(X3, output, CRYPTO_CONST_12);

    (void)IStdLib_MemSet(&X0, 0, sizeof(X0));
    (void)IStdLib_MemSet(&X1, 0, sizeof(X1));
    (void)IStdLib_MemSet(&X2, 0, sizeof(X2));
    (void)IStdLib_MemSet(&X3, 0, sizeof(X3));

    (void)IStdLib_MemSet(&Y0, 0, sizeof(Y0));
    (void)IStdLib_MemSet(&Y1, 0, sizeof(Y1));
    (void)IStdLib_MemSet(&Y2, 0, sizeof(Y2));
    (void)IStdLib_MemSet(&Y3, 0, sizeof(Y3));

    (void)IStdLib_MemSet(&RK, 0, sizeof(RK));

    return ret;
}

/******************************************************************************/
/*
 * Brief               This function is used to perform AES with ECB group mode
 *                     on input.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     mode: The AES operation: CRYPTO_MODE_DECRYPT or
 *                           CRYPTO_MODE_ENCRYPT.
 *                     length: The length of input, calculated in byte.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Aes_Crypt_Ecb(Crypto_AESData* ctx, uint8 mode, uint32 length, const uint8* input, uint8* output)
{
    uint8          temp[CRYPTO_CONST_16];
    Std_ReturnType ret = E_NOT_OK;

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length > 0)
        {
            (void)IStdLib_MemCpy(temp, input, CRYPTO_CONST_16);
            ret = Internal_Aes_Decrypt(ctx, input, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */
            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    else
    {
        while (length > 0)
        {
            ret = Internal_Aes_Encrypt(ctx, input, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */
            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    return ret;
}

void CTR_Counter_Raise(uint8 arr[4])
{
    for (int i = 3; i >= 0; i--)
    {
        if (arr[i] < 0xFF)
        {
            arr[i]++;
            return;
        }
        else
        {
            arr[i] = 0x00;
        }
    }
    return;
}

/******************************************************************************/
/*
 * Brief               This function is used to perform AES with CTR group mode
 *                     on input.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     mode: The AES operation: CRYPTO_MODE_DECRYPT or
 *                           CRYPTO_MODE_ENCRYPT.
 *                     length: The length of input, calculated in byte.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Aes_Crypt_Ctr(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output)
{
    uint8          i;
    uint8          j;
    Std_ReturnType ret                        = E_NOT_OK;
    uint8          iv_Counter[CRYPTO_CONST_4] = {0x00};

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length > 0)
        {
            ret = Internal_Aes_Encrypt(ctx, iv, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */

            for (j = 0; j < CRYPTO_CONST_16; j++)
            {
                output[j] = input[j] ^ output[j];
            }

            // Counter ++
            for (i = 0; i < 4; i++)
            {
                iv_Counter[i] = (uint8)iv[i + 12];
            }
            CTR_Counter_Raise(iv_Counter);
            (void)IStdLib_MemCpy(iv + 12, iv_Counter, CRYPTO_CONST_4);

            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    else
    {
        while (length > 0)
        {
            ret = Internal_Aes_Encrypt(ctx, iv, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */

            for (j = 0; j < CRYPTO_CONST_16; j++)
            {
                output[j] = input[j] ^ output[j];
            }

            // Counter ++
            for (i = 0; i < 4; i++)
            {
                iv_Counter[i] = (uint8)iv[i + 12];
            }
            CTR_Counter_Raise(iv_Counter);
            (void)IStdLib_MemCpy(iv + 12, iv_Counter, CRYPTO_CONST_4);

            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function is used to perform AES with CBC group mode
 *                     on input.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     mode: The AES operation: CRYPTO_MODE_DECRYPT or
 *                           CRYPTO_MODE_ENCRYPT.
 *                     length: The length of input, calculated in byte.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Aes_Crypt_Cbc(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output)
{
    uint8          i;
    uint8          temp[CRYPTO_CONST_16];
    Std_ReturnType ret = E_NOT_OK;

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length > 0)
        {
            (void)IStdLib_MemCpy(temp, input, CRYPTO_CONST_16);
            ret = Internal_Aes_Decrypt(ctx, input, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */

            for (i = 0; i < CRYPTO_CONST_16; i++)
            {
                output[i] = (uint8)(output[i] ^ iv[i]);
            }

            (void)IStdLib_MemCpy(iv, temp, CRYPTO_CONST_16);

            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    else
    {
        while (length > 0)
        {
            for (i = 0; i < CRYPTO_CONST_16; i++)
            {
                output[i] = (uint8)(input[i] ^ iv[i]);
            }

            ret = Internal_Aes_Encrypt(ctx, output, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */

            (void)IStdLib_MemCpy(iv, output, CRYPTO_CONST_16);

            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function is used to perform AES on input, as a part
 *                     of CMAC calculation.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     mode: The AES operation: CRYPTO_MODE_DECRYPT or
 *                           CRYPTO_MODE_ENCRYPT.
 *                     length: The length of input, calculated in byte.
 *                     input: The message needs to be encrypted or decrypted.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The cipheretxt or plaintext corresponded with input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Aes_Crypt_Cmac(Crypto_AESData* ctx, uint8 mode, uint8 length, uint8* input, uint8* output)
{
    uint8 i;
    uint8 iv[CRYPTO_CONST_16] = {
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U,
        0x00U};
    uint8          temp[CRYPTO_CONST_16];
    Std_ReturnType ret = E_NOT_OK;

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length > 0)
        {
            (void)IStdLib_MemCpy(temp, input, CRYPTO_CONST_16);
            ret = Internal_Aes_Decrypt(ctx, input, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */

            for (i = 0; i < CRYPTO_CONST_16; i++)
            {
                output[i] = (uint8)(output[i] ^ iv[i]);
            }

            (void)IStdLib_MemCpy(iv, temp, CRYPTO_CONST_16);

            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    else
    {
        while (length > 0)
        {
            for (i = 0; i < CRYPTO_CONST_16; i++)
            {
                input[i] = (uint8)(input[i] ^ iv[i]);
            }

            ret = Internal_Aes_Encrypt(ctx, input, output); /* PRQA S 2784 */ /* VL_QAC_Crypto */
            (void)IStdLib_MemCpy(iv, output, CRYPTO_CONST_16);
            input += CRYPTO_CONST_16;
            output += CRYPTO_CONST_16;
            length = (length >= CRYPTO_CONST_16) ? (length - CRYPTO_CONST_16) : 0;
        }
    }
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function is used to perform AES with CFB128 group mode
 *                     on input.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     mode: The AES operation: meaning do encryption or decryption.
 *                     length: The length of input, calculated in byte.
 *                     iv_off: The offset in IV.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Aes_Crypt_Cfb128(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8*          iv_off,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output)
{
    uint8          c;
    Std_ReturnType ret = E_NOT_OK;
    uint8          n;

    n = *iv_off;

    if (n > CRYPTO_CONST_15)
    {
        return CRYPTO_E_KEY_NOT_VALID;
    }

    if (mode == CRYPTO_MODE_DECRYPT)
    {
        while (length--)
        {
            if (n == 0)
            {
                ret = Internal_Aes_Encrypt(ctx, iv, iv);
                if (ret != 0)
                {
                    goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
                }
            }

            c         = *input++;
            *output++ = (unsigned char)(c ^ iv[n]);
            iv[n]     = (unsigned char)c;

            n = (n + CRYPTO_CONST_1) & CRYPTO_CONST_0x0F;
        }
    }
    else
    {
        while (length--)
        {
            if (n == 0)
            {
                ret = Internal_Aes_Encrypt(ctx, iv, iv);
                if (ret != 0)
                {
                    goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
                }
            }

            iv[n] = *output++ = (unsigned char)(iv[n] ^ *input++);

            n = (n + CRYPTO_CONST_1) & CRYPTO_CONST_0x0F;
        }
    }

    *iv_off = n;
    ret     = E_OK;

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function is used to perform AES with CFB8 group mode
 *                     on input.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     mode: The AES operation: meaning do encryption or decryption.
 *                     length: The length of input, calculated in byte.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted, or the ciphertext
 *                            needed to be decrypted, depending on the mode.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext or the plaintext corresponded with
 *                             input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Aes_Crypt_Cfb8(
    Crypto_AESData* ctx,
    uint8           mode,
    uint32          length,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output)
{
    Std_ReturnType ret = E_NOT_OK;
    unsigned char  c;
    unsigned char  ov[CRYPTO_CONST_17];

    if ((mode != CRYPTO_MODE_ENCRYPT) && (mode != CRYPTO_MODE_DECRYPT))
    {
        return E_NOT_OK;
    }
    while (length--)
    {
        (void)IStdLib_MemCpy(ov, iv, CRYPTO_CONST_16);
        ret = Internal_Aes_Encrypt(ctx, iv, iv);
        if (ret != E_OK)
        {
            goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
        }

        if (mode == CRYPTO_MODE_DECRYPT)
        {
            ov[CRYPTO_CONST_16] = *input;
        }

        c = *output++ = (unsigned char)(iv[0] ^ *input++);

        if (mode == CRYPTO_MODE_ENCRYPT)
        {
            ov[CRYPTO_CONST_16] = c;
        }
        /* PRQA S 0489 ++ */ /* VL_QAC_Crypto */
        (void)IStdLib_MemCpy(iv, ov + 1U, CRYPTO_CONST_16);
        /* PRQA S 0489 -- */
    }
    ret = E_OK;

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    return ret;
}

/******************************************************************************/
/*
 * Brief               This function is used to perform AES with OFB group mode
 *                     on input.
 *
 * Param-Name[in]      ctx: The context of AES.
 *                     length: The length of input, calculated in byte.
 *                     iv_off: The offset in IV.
 *                     iv: The Initialization vector. It must be a readable and
 *                         writeable buffer of 16 Bytes.
 *                     input: The plaintext needed to be encrypted.
 * Param-Name[in/out]  None
 * Param-Name[out]     output: The ciphertext corresponded with input.
 * Return              Std_ReturnType:  E_OK: State accepted
 *                                      E_NOT_OK: State not accepted
 */
/******************************************************************************/
Std_ReturnType Crypto_Aes_Crypt_Ofb(
    Crypto_AESData* ctx,
    uint32          length,
    uint8*          iv_off,
    uint8           iv[CRYPTO_CONST_16],
    const uint8*    input,
    uint8*          output)
{
    Std_ReturnType ret = E_OK;
    uint8          n;

    n = *iv_off;

    if (n > CRYPTO_CONST_15)
    {
        return CRYPTO_E_KEY_NOT_VALID;
    }

    while (length--)
    {
        if (n == 0U)
        {
            ret = Internal_Aes_Encrypt(ctx, iv, iv);
            if (ret != E_OK)
            {
                goto exit; /* PRQA S 2001 */ /* VL_QAC_Crypto */
            }
        }
        *output++ = *input++ ^ iv[n];

        n = (n + CRYPTO_CONST_1) & CRYPTO_CONST_0x0F;
    }

    *iv_off = n;

exit: /* PRQA S 2015 */ /* VL_QAC_Crypto */
    return ret;
}

#define CRYPTO_62_STOP_SEC_CODE
#include "Crypto_62_MemMap.h"
#endif /* CRYPTO_ALGORITHMFAM_AES == STD_ON */
/* PRQA S 4544,4542,0488,1252,1253,1290,3387,3440,2463,3138,3218,3228,3418,3472 -- */
/* PRQA S 1338,1842,1844,1862,1863,3226,3344,4461,2985,0342 -- */
/* PRQA S 6050,6060,6070,6080,6010,6030,6040,1532,1503,2889  -- */
