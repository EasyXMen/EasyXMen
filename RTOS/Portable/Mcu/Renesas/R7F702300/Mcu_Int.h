/* PRQA S 3108++ */
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
/* PRQA S 3108-- */
/*
 ********************************************************************************
 **                                                                            **
 **  FILENAME    : Mcu_Int.h                                                   **
 **                                                                            **
 **  Created on  :                                                             **
 **  Author      : i-soft-os                                                   **
 **  Vendor      :                                                             **
 **  DESCRIPTION : Interrupt driver                                            **
 **                                                                            **
 **  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
 **  Version :   AUTOSAR classic Platform R19--Function Safety                 **
 **                                                                            **
 *******************************************************************************/
#ifndef MCU_INT_H
#define MCU_INT_H

/*=======[I N C L U D E S]====================================================*/
#include "Os_Types.h"
/*=======[M A C R O S]========================================================*/
#define OS_ARCH_INT_CPU0 0
#define OS_ARCH_INT_CPU1 1
#define OS_ARCH_INT_CPU2 2
#define OS_ARCH_INT_CPU3 3
/*max number of interrupt*/
#define OS_ARCH_EI_INTR_NUM (768U)
#define OS_ARCH_FE_INTR_NUM (32U)

/* FEINT Status Register */
/* This register contains information about which interrupt has generated the FE level maskable interrupt */
#define OS_FEINTF_FACTOR_0(Pen) (*(uint32 volatile*)(OS_FEINC_BASE_ADDR(Pen)))

/* FEINT Event Mask Register */
/* This register masks the FE level maskable interrupt (FEINT). */
#define OS_FEINTMSK_FACTOR_0(Pen) (*(uint32 volatile*)(OS_FEINC_BASE_ADDR(Pen) + 4))

/* FEINT Status Clear Register */
/* This register clears the FE level maskable interrupt flags. */
#define OS_FEINTC_0(Pen) (*(uint32 volatile*)(OS_FEINC_BASE_ADDR(Pen) + 8))

/* Register Base Address */
#define OS_IPIR_BASE_ADDR       (0xFFFB9000)
#define OS_INTC1_SELF_BASE_ADDR (0xFFFC0000)
#define OS_INTC1_BASE_ADDR(PEn) (0xFFFC4000 + PEn * 0x4000)
#define OS_INTC2_BASE_ADDR      (0xFFF80000)
#define OS_INTIF_BASE_ADDR      (0xFF090000)
#define OS_EINT_BASE_ADDR       (0xFFC00000)
#define OS_FENC_BASE_ADDR       (0xFF9A3A00)
#define OS_FEINC_BASE_ADDR(PEn) (0xFF9A3B00 + PEn * 0x0100)

#define OS_TPTM_BASE_ADDR       (0xFFFBB000)
#define OS_OSTM_BASE_ADDR(n)    (0xFFBF0000 + n * 0x100)

#if (OS_AUTOSAR_CORES > 1)
/* Inter-Process Interrupts Registers */
#define OS_IPIR_IPInENS(n)        (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x000 + 0x020 * n))
#define OS_IPIR_IPInFLGS(n)       (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x004 + 0x020 * n))
#define OS_IPIR_IPInFCLRS(n)      (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x008 + 0x020 * n))
#define OS_IPIR_IPInREQS(n)       (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x010 + 0x020 * n))
#define OS_IPIR_IPInRCLRS(n)      (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x014 + 0x020 * n))

#define OS_IPIR_IPInENm(PEn, n)   (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x800 + 0x020 * n + 0x100 * PEn))
#define OS_IPIR_IPInFLGm(PEn, n)  (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x804 + 0x020 * n + 0x100 * PEn))
#define OS_IPIR_IPInFCLRm(PEn, n) (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x808 + 0x020 * n + 0x100 * PEn))
#define OS_IPIR_IPInREQm(PEn, n)  (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x810 + 0x020 * n + 0x100 * PEn))
#define OS_IPIR_IPInRCLRm(PEn, n) (*(uint8 volatile*)(OS_IPIR_BASE_ADDR + 0x814 + 0x020 * n + 0x100 * PEn))
#endif /* OS_AUTOSAR_CORES > 1 */

/* Interrupts Register */
#define OS_INTC_SELF_EICn(n)                                                    \
    (*(uint16 volatile*)(n < 32 ? (OS_INTC1_SELF_BASE_ADDR + 0x0000 + n * 0x02) \
                                : (OS_INTC2_BASE_ADDR + 0x0000 + n * 0x02)))

#define OS_INTC_EICn(PEn, n)                                                    \
    (*(uint16 volatile*)(n < 32 ? (OS_INTC1_BASE_ADDR(PEn) + 0x0000 + n * 0x02) \
                                : (OS_INTC2_BASE_ADDR + 0x0000 + n * 0x02)))

#define OS_INTC_EEICn(PEn, n)                                                   \
    (*(uint16 volatile*)(n < 32 ? (OS_INTC1_BASE_ADDR(PEn) + 0x0200 + n * 0x04) \
                                : (OS_INTC2_BASE_ADDR + 0x4000 + n * 0x04)))

#define OS_INTC_IMRn(PEn, n) \
    (*(uint32 volatile*)(n == 0 ? (OS_INTC1_BASE_ADDR(PEn) + 0x00F0) : (OS_INTC2_BASE_ADDR + 0x1000 + n * 0x04)))

#define OS_INTC_EIBDn(PEn, n)                                                   \
    (*(uint16 volatile*)(n < 32 ? (OS_INTC1_BASE_ADDR(PEn) + 0x0100 + n * 0x04) \
                                : (OS_INTC2_BASE_ADDR + 0x2000 + n * 0x04)))

/* Interrupt Request Number */
#define OS_ISR_IPIR0      0
#define OS_ISR_IPIR1      1
#define OS_ISR_IPIR2      2
#define OS_ISR_IPIR3      3
#define OS_ISR_BRDC0      4
#define OS_ISR_BRDC1      5
#define OS_ISR_BRDC2      6
#define OS_ISR_BRDC3      7
#define OS_ISR_ECMMI      8
#define OS_ISR_ECMDCLSMI  9
#define OS_ISR_TAUD0I0    10
#define OS_ISR_TAUD0I2    11
#define OS_ISR_TAUD0I4    12
#define OS_ISR_TAUD0I6    13
#define OS_ISR_EINTSW0    14
#define OS_ISR_EINTSW1    15
#define OS_ISR_EINTSW2    16
#define OS_ISR_EINTSW3    17
#define OS_ISR_ICUPS      20
#define OS_ISR_ICUP       21
#define OS_ISR_WDTBnTIT   22
#define OS_ISR_TAUD0I8    23
#define OS_ISR_TAUD0I10   24
#define OS_ISR_TAUD0I12   25
#define OS_ISR_TAUD0I14   26
#define OS_ISR_TAPA0IPEK0 27
#define OS_ISR_TAPA0IVLY0 28
#define OS_ISR_SDMACERR   29
#define OS_ISR_DTSERR     30
#define OS_ISR_TPTMn      31

// #define OS_ISR_WDTB0TIT_Core0          22
#define OS_ISR_WDTB0TIT       22
#define OS_ISR_WDTB1TIT_Core1 22
#define OS_ISR_WDTB2TIT_Core2 22
#define OS_ISR_WDTB3TIT_Core3 22

#define OS_ISR_FL0ENDNM       34
#define OS_ISR_FL1ENDNM       36
#define OS_ISR_FL2ENDNM       38
#define OS_ISR_DTS31TO0       39
#define OS_ISR_DTS63TO32      40
#define OS_ISR_DTS95TO64      41
#define OS_ISR_DTS127TO96     42
#define OS_ISR_DTSCT31TO0     43
#define OS_ISR_DTSCT63TO32    44
#define OS_ISR_DTSCT95TO64    45
#define OS_ISR_DTSCT127TO96   46
#define OS_ISR_SDMAC0CH0      47
#define OS_ISR_SDMAC0CH1      48
#define OS_ISR_SDMAC0CH2      49
#define OS_ISR_SDMAC0CH3      50
#define OS_ISR_SDMAC0CH4      51
#define OS_ISR_SDMAC0CH5      52
#define OS_ISR_SDMAC0CH6      53
#define OS_ISR_SDMAC0CH7      54
#define OS_ISR_SDMAC0CH8      55
#define OS_ISR_SDMAC0CH9      56
#define OS_ISR_SDMAC0CH10     57
#define OS_ISR_SDMAC0CH11     58
#define OS_ISR_SDMAC0CH12     59
#define OS_ISR_SDMAC0CH13     60
#define OS_ISR_SDMAC0CH14     61
#define OS_ISR_SDMAC0CH15     62
#define OS_ISR_SDMAC1CH0      63
#define OS_ISR_SDMAC1CH1      64
#define OS_ISR_SDMAC1CH2      65
#define OS_ISR_SDMAC1CH3      66
#define OS_ISR_SDMAC1CH4      67
#define OS_ISR_SDMAC1CH5      68
#define OS_ISR_SDMAC1CH6      69
#define OS_ISR_SDMAC1CH7      70
#define OS_ISR_SDMAC1CH8      71
#define OS_ISR_SDMAC1CH9      72
#define OS_ISR_SDMAC1CH10     73
#define OS_ISR_SDMAC1CH11     74
#define OS_ISR_SDMAC1CH12     75
#define OS_ISR_SDMAC1CH13     76
#define OS_ISR_SDMAC1CH14     77
#define OS_ISR_SDMAC1CH15     78
#define OS_ISR_GTMA0TIM00     79
#define OS_ISR_GTMA0TIM01     80
#define OS_ISR_GTMA0TIM02     81
#define OS_ISR_GTMA0TIM03     82
#define OS_ISR_GTMA0TIM04     83
#define OS_ISR_GTMA0TIM05     84
#define OS_ISR_GTMA0TIM06     85
#define OS_ISR_GTMA0TIM07     86
#define OS_ISR_GTMA0TIM10     87
#define OS_ISR_GTMA0TIM11     88
#define OS_ISR_GTMA0TIM12     89
#define OS_ISR_GTMA0TIM13     90
#define OS_ISR_GTMA0TIM14     91
#define OS_ISR_GTMA0TIM15     92
#define OS_ISR_GTMA0TIM16     93
#define OS_ISR_GTMA0TIM17     94
#define OS_ISR_GTMA0TIM20     95
#define OS_ISR_GTMA0TIM21     96
#define OS_ISR_GTMA0TIM22     97
#define OS_ISR_GTMA0TIM23     98
#define OS_ISR_GTMA0TIM24     99
#define OS_ISR_GTMA0TIM25     100
#define OS_ISR_GTMA0TIM26     101
#define OS_ISR_GTMA0TIM27     102
#define OS_ISR_GTMA0TIM30     103
#define OS_ISR_GTMA0TIM31     104
#define OS_ISR_GTMA0TIM32     105
#define OS_ISR_GTMA0TIM33     106
#define OS_ISR_GTMA0TIM34     107
#define OS_ISR_GTMA0TIM35     108
#define OS_ISR_GTMA0TIM36     109
#define OS_ISR_GTMA0TIM37     110
#define OS_ISR_GTMA0MCS00     111
#define OS_ISR_GTMA0MCS01     112
#define OS_ISR_GTMA0MCS02     113
#define OS_ISR_GTMA0MCS03     114
#define OS_ISR_GTMA0MCS04     115
#define OS_ISR_GTMA0MCS05     116
#define OS_ISR_GTMA0MCS06     117
#define OS_ISR_GTMA0MCS07     118
#define OS_ISR_GTMA0MCS10     119
#define OS_ISR_GTMA0MCS11     120
#define OS_ISR_GTMA0MCS12     121
#define OS_ISR_GTMA0MCS13     122
#define OS_ISR_GTMA0MCS14     123
#define OS_ISR_GTMA0MCS15     124
#define OS_ISR_GTMA0MCS16     125
#define OS_ISR_GTMA0MCS17     126
#define OS_ISR_GTMA0MCS20     127
#define OS_ISR_GTMA0MCS21     128
#define OS_ISR_GTMA0MCS22     129
#define OS_ISR_GTMA0MCS23     130
#define OS_ISR_GTMA0MCS24     131
#define OS_ISR_GTMA0MCS25     132
#define OS_ISR_GTMA0MCS26     133
#define OS_ISR_GTMA0MCS27     134
#define OS_ISR_GTMA0MCS30     135
#define OS_ISR_GTMA0MCS31     136
#define OS_ISR_GTMA0MCS32     137
#define OS_ISR_GTMA0MCS33     138
#define OS_ISR_GTMA0MCS34     139
#define OS_ISR_GTMA0MCS35     140
#define OS_ISR_GTMA0MCS36     141
#define OS_ISR_GTMA0MCS37     142
#define OS_ISR_GTMA0ATOM00    143
#define OS_ISR_GTMA0ATOM01    144
#define OS_ISR_GTMA0ATOM02    145
#define OS_ISR_GTMA0ATOM03    146
#define OS_ISR_GTMA0ATOM10    147
#define OS_ISR_GTMA0ATOM11    148
#define OS_ISR_GTMA0ATOM12    149
#define OS_ISR_GTMA0ATOM13    150
#define OS_ISR_GTMA0ATOM20    151
#define OS_ISR_GTMA0ATOM21    152
#define OS_ISR_GTMA0ATOM22    153
#define OS_ISR_GTMA0ATOM23    154
#define OS_ISR_GTMA0ATOM30    155
#define OS_ISR_GTMA0ATOM31    156
#define OS_ISR_GTMA0ATOM32    157
#define OS_ISR_GTMA0ATOM33    158
#define OS_ISR_GTMA0AEI       159
#define OS_ISR_GTMA0ARU0      160
#define OS_ISR_GTMA0ARU1      161
#define OS_ISR_GTMA0ARU2      162
#define OS_ISR_GTMA0CMP       163
#define OS_ISR_GTMA0ERR       164
#define OS_ISR_TSG30I0        165
#define OS_ISR_TSG30I1        166
#define OS_ISR_TSG30I2        167
#define OS_ISR_TSG30I3        168
#define OS_ISR_TSG30I4        169
#define OS_ISR_TSG30I5        170
#define OS_ISR_TSG30I6        171
#define OS_ISR_TSG30I7        172
#define OS_ISR_TSG30I8        173
#define OS_ISR_TSG30I9        174
#define OS_ISR_TSG30I10       175
#define OS_ISR_TSG30I11       176
#define OS_ISR_TSG30I12       177
#define OS_ISR_TSG30IPEK      178
#define OS_ISR_TSG30IVLY      179
#define OS_ISR_TSG30IER       180
#define OS_ISR_TSG30IWN       181
#define OS_ISR_TSG31I0        182
#define OS_ISR_TSG31I1        183
#define OS_ISR_TSG31I2        184
#define OS_ISR_TSG31I3        185
#define OS_ISR_TSG31I4        186
#define OS_ISR_TSG31I5        187
#define OS_ISR_TSG31I6        188
#define OS_ISR_TSG31I7        189
#define OS_ISR_TSG31I8        190
#define OS_ISR_TSG31I9        191
#define OS_ISR_TSG31I10       192
#define OS_ISR_TSG31I11       193
#define OS_ISR_TSG31I12       194
#define OS_ISR_TSG31IPEK      195
#define OS_ISR_TSG31IVLY      196
#define OS_ISR_TSG31IER       197
#define OS_ISR_TSG31IWN       198
#define OS_ISR_OSTM0TINT      199
#define OS_ISR_OSTM1TINT      200
#define OS_ISR_OSTM2TINT      201
#define OS_ISR_OSTM3TINT      202
#define OS_ISR_OSTM4TINT      203
#define OS_ISR_OSTM5TINT      204
#define OS_ISR_OSTM6TINT      205
#define OS_ISR_OSTM7TINT      206
#define OS_ISR_OSTM8TINT      207
#define OS_ISR_OSTM9TINT      208
#define OS_ISR_TPTMU00        209
#define OS_ISR_TPTMU01        210
#define OS_ISR_TPTMU02        211
#define OS_ISR_TPTMU03        212
#define OS_ISR_TPTMU10        213
#define OS_ISR_TPTMU11        214
#define OS_ISR_TPTMU12        215
#define OS_ISR_TPTMU13        216
#define OS_ISR_TPTMU20        217
#define OS_ISR_TPTMU21        218
#define OS_ISR_TPTMU22        219
#define OS_ISR_TPTMU23        220
#define OS_ISR_TPTMU30        221
#define OS_ISR_TPTMU31        222
#define OS_ISR_TPTMU32        223
#define OS_ISR_TPTMU33        224
#define OS_ISR_ADCJ0SEC       225
#define OS_ISR_ADCJ0ERR       226
#define OS_ISR_ADCJ0I0        227
#define OS_ISR_ADCJ0I1        228
#define OS_ISR_ADCJ0I2        229
#define OS_ISR_ADCJ0I3        230
#define OS_ISR_ADCJ0I4        231
#define OS_ISR_ADCJ1ERR       232
#define OS_ISR_ADCJ1I0        233
#define OS_ISR_ADCJ1I1        234
#define OS_ISR_ADCJ1I2        235
#define OS_ISR_ADCJ1I3        236
#define OS_ISR_ADCJ1I4        237
#define OS_ISR_ADCJ2ERR       238
#define OS_ISR_ADCJ2I0        239
#define OS_ISR_ADCJ2I1        240
#define OS_ISR_ADCJ2I2        241
#define OS_ISR_ADCJ2I3        242
#define OS_ISR_ADCJ2I4        243
#define OS_ISR_MSPI0TX0       244
#define OS_ISR_MSPI0RX0       245
#define OS_ISR_MSPI0TX1       246
#define OS_ISR_MSPI0RX1       247
#define OS_ISR_MSPI0TX2       248
#define OS_ISR_MSPI0RX2       249
#define OS_ISR_MSPI0TX        250
#define OS_ISR_MSPI0RX        251
#define OS_ISR_MSPI0FE        252
#define OS_ISR_MSPI0ERR       253
#define OS_ISR_MSPI1TX0       254
#define OS_ISR_MSPI1RX0       255
#define OS_ISR_MSPI1TX1       256
#define OS_ISR_MSPI1RX1       257
#define OS_ISR_MSPI1TX2       258
#define OS_ISR_MSPI1RX2       259
#define OS_ISR_MSPI1TX        260
#define OS_ISR_MSPI1RX        261
#define OS_ISR_MSPI1FE        262
#define OS_ISR_MSPI1ERR       263
#define OS_ISR_MSPI2TX        264
#define OS_ISR_MSPI2RX        265
#define OS_ISR_MSPI2FE        266
#define OS_ISR_MSPI2ERR       267
#define OS_ISR_MSPI3TX        268
#define OS_ISR_MSPI3RX        269
#define OS_ISR_MSPI3FE        270
#define OS_ISR_MSPI3ERR       271
#define OS_ISR_MSPI4TX        272
#define OS_ISR_MSPI4RX        273
#define OS_ISR_MSPI4FE        274
#define OS_ISR_MSPI4ERR       275
#define OS_ISR_MSPI5TX        276
#define OS_ISR_MSPI5RX        277
#define OS_ISR_MSPI5FE        278
#define OS_ISR_MSPI5ERR       279
#define OS_ISR_MSPI6TX        280
#define OS_ISR_MSPI6RX        281
#define OS_ISR_MSPI6FE        282
#define OS_ISR_MSPI6ERR       283
#define OS_ISR_MSPI7TX        284
#define OS_ISR_MSPI7RX        285
#define OS_ISR_MSPI7FE        286
#define OS_ISR_MSPI7ERR       287
#define OS_ISR_MSPI8TX        288
#define OS_ISR_MSPI8RX        289
#define OS_ISR_MSPI8FE        290
#define OS_ISR_MSPI8ERR       291
#define OS_ISR_MSPI9TX        292
#define OS_ISR_MSPI9RX        293
#define OS_ISR_MSPI9FE        294
#define OS_ISR_MSPI9ERR       295
#define OS_ISR_RCANGERR0      296
#define OS_ISR_RCANGRECC0     297
#define OS_ISR_RCAN0ERR       298
#define OS_ISR_RCAN0REC       299
#define OS_ISR_RCAN0TRX       300
#define OS_ISR_RCAN1ERR       301
#define OS_ISR_RCAN1REC       302
#define OS_ISR_RCAN1TRX       303
#define OS_ISR_RCAN2ERR       304
#define OS_ISR_RCAN2REC       305
#define OS_ISR_RCAN2TRX       306
#define OS_ISR_RCAN3ERR       307
#define OS_ISR_RCAN3REC       308
#define OS_ISR_RCAN3TRX       309
#define OS_ISR_RCAN4ERR       310
#define OS_ISR_RCAN4REC       311
#define OS_ISR_RCAN4TRX       312
#define OS_ISR_RCAN5ERR       313
#define OS_ISR_RCAN5REC       314
#define OS_ISR_RCAN5TRX       315
#define OS_ISR_RCAN6ERR       316
#define OS_ISR_RCAN6REC       317
#define OS_ISR_RCAN6TRX       318
#define OS_ISR_RCAN7ERR       319
#define OS_ISR_RCAN7REC       320
#define OS_ISR_RCAN7TRX       321
#define OS_ISR_RCANGERR1      322
#define OS_ISR_RCANGRECC1     323
#define OS_ISR_RCAN8ERR       324
#define OS_ISR_RCAN8REC       325
#define OS_ISR_RCAN8TRX       326
#define OS_ISR_RCAN9ERR       327
#define OS_ISR_RCAN9REC       328
#define OS_ISR_RCAN9TRX       329
#define OS_ISR_RCAN10ERR      330
#define OS_ISR_RCAN10REC      331
#define OS_ISR_RCAN10TRX      332
#define OS_ISR_RCAN11ERR      333
#define OS_ISR_RCAN11REC      334
#define OS_ISR_RCAN11TRX      335
#define OS_ISR_RCAN12ERR      336
#define OS_ISR_RCAN12REC      337
#define OS_ISR_RCAN12TRX      338
#define OS_ISR_RCAN13ERR      339
#define OS_ISR_RCAN13REC      340
#define OS_ISR_RCAN13TRX      341
#define OS_ISR_RCAN14ERR      342
#define OS_ISR_RCAN14REC      343
#define OS_ISR_RCAN14TRX      344
#define OS_ISR_RCAN15ERR      345
#define OS_ISR_RCAN15REC      346
#define OS_ISR_RCAN15TRX      347
#define OS_ISR_SCI30ERI       348
#define OS_ISR_SCI30RXI       349
#define OS_ISR_SCI30TXI       350
#define OS_ISR_SCI30TEI       351
#define OS_ISR_SCI31ERI       352
#define OS_ISR_SCI31RXI       353
#define OS_ISR_SCI31TXI       354
#define OS_ISR_SCI31TEI       355
#define OS_ISR_SCI32ERI       356
#define OS_ISR_SCI32RXI       357
#define OS_ISR_SCI32TXI       358
#define OS_ISR_SCI32TEI       359
#define OS_ISR_TAUJ0I0        360
#define OS_ISR_TAUJ0I1        361
#define OS_ISR_TAUJ0I2        362
#define OS_ISR_TAUJ0I3        363
#define OS_ISR_TAUJ1I0        364
#define OS_ISR_TAUJ1I1        365
#define OS_ISR_TAUJ1I2        366
#define OS_ISR_TAUJ1I3        367
#define OS_ISR_TAUJ2I0        368
#define OS_ISR_TAUJ2I1        369
#define OS_ISR_TAUJ2I2        370
#define OS_ISR_TAUJ2I3        371
#define OS_ISR_TAUJ3I0        372
#define OS_ISR_TAUJ3I1        373
#define OS_ISR_TAUJ3I2        374
#define OS_ISR_TAUJ3I3        375
#define OS_ISR_TAUD0I1        376
#define OS_ISR_TAUD0I3        377
#define OS_ISR_TAUD0I5        378
#define OS_ISR_TAUD0I7        379
#define OS_ISR_TAUD0I9        380
#define OS_ISR_TAUD0I11       381
#define OS_ISR_TAUD0I13       382
#define OS_ISR_TAUD0I15       383
#define OS_ISR_TAUD1I0        384
#define OS_ISR_TAUD1I1        385
#define OS_ISR_TAUD1I2        386
#define OS_ISR_TAUD1I3        387
#define OS_ISR_TAUD1I4        388
#define OS_ISR_TAUD1I5        389
#define OS_ISR_TAUD1I6        390
#define OS_ISR_TAUD1I7        391
#define OS_ISR_TAUD1I8        392
#define OS_ISR_TAUD1I9        393
#define OS_ISR_TAUD1I10       394
#define OS_ISR_TAUD1I11       395
#define OS_ISR_TAUD1I12       396
#define OS_ISR_TAUD1I13       397
#define OS_ISR_TAUD1I14       398
#define OS_ISR_TAUD1I15       399
#define OS_ISR_TAUD2I0        400
#define OS_ISR_TAUD2I1        401
#define OS_ISR_TAUD2I2        402
#define OS_ISR_TAUD2I3        403
#define OS_ISR_TAUD2I4        404
#define OS_ISR_TAUD2I5        405
#define OS_ISR_TAUD2I6        406
#define OS_ISR_TAUD2I7        407
#define OS_ISR_TAUD2I8        408
#define OS_ISR_TAUD2I9        409
#define OS_ISR_TAUD2I10       410
#define OS_ISR_TAUD2I11       411
#define OS_ISR_TAUD2I12       412
#define OS_ISR_TAUD2I13       413
#define OS_ISR_TAUD2I14       414
#define OS_ISR_TAUD2I15       415
#define OS_ISR_RLIN30         416
#define OS_ISR_RLIN30UR0      417
#define OS_ISR_RLIN30UR1      418
#define OS_ISR_RLIN30UR2      419
#define OS_ISR_RLIN31         420
#define OS_ISR_RLIN31UR0      421
#define OS_ISR_RLIN31UR1      422
#define OS_ISR_RLIN31UR2      423
#define OS_ISR_RLIN32         424
#define OS_ISR_RLIN32UR0      425
#define OS_ISR_RLIN32UR1      426
#define OS_ISR_RLIN32UR2      427
#define OS_ISR_RLIN33         428
#define OS_ISR_RLIN33UR0      429
#define OS_ISR_RLIN33UR1      430
#define OS_ISR_RLIN33UR2      431
#define OS_ISR_RLIN34         432
#define OS_ISR_RLIN34UR0      433
#define OS_ISR_RLIN34UR1      434
#define OS_ISR_RLIN34UR2      435
#define OS_ISR_RLIN35         436
#define OS_ISR_RLIN35UR0      437
#define OS_ISR_RLIN35UR1      438
#define OS_ISR_RLIN35UR2      439
#define OS_ISR_RLIN36         440
#define OS_ISR_RLIN36UR0      441
#define OS_ISR_RLIN36UR1      442
#define OS_ISR_RLIN36UR2      443
#define OS_ISR_RLIN37         444
#define OS_ISR_RLIN37UR0      445
#define OS_ISR_RLIN37UR1      446
#define OS_ISR_RLIN37UR2      447
#define OS_ISR_RLIN38         448
#define OS_ISR_RLIN38UR0      449
#define OS_ISR_RLIN38UR1      450
#define OS_ISR_RLIN38UR2      451
#define OS_ISR_RLIN39         452
#define OS_ISR_RLIN39UR0      453
#define OS_ISR_RLIN39UR1      454
#define OS_ISR_RLIN39UR2      455
#define OS_ISR_RLIN310        456
#define OS_ISR_RLIN310UR0     457
#define OS_ISR_RLIN310UR1     458
#define OS_ISR_RLIN310UR2     459
#define OS_ISR_RLIN311        460
#define OS_ISR_RLIN311UR0     461
#define OS_ISR_RLIN311UR1     462
#define OS_ISR_RLIN311UR2     463
#define OS_ISR_RLIN312        464
#define OS_ISR_RLIN312UR0     465
#define OS_ISR_RLIN312UR1     466
#define OS_ISR_RLIN312UR2     467
#define OS_ISR_RLIN313        468
#define OS_ISR_RLIN313UR0     469
#define OS_ISR_RLIN313UR1     470
#define OS_ISR_RLIN313UR2     471
#define OS_ISR_RLIN314        472
#define OS_ISR_RLIN314UR0     473
#define OS_ISR_RLIN314UR1     474
#define OS_ISR_RLIN314UR2     475
#define OS_ISR_RLIN315        476
#define OS_ISR_RLIN315UR0     477
#define OS_ISR_RLIN315UR1     478
#define OS_ISR_RLIN315UR2     479
#define OS_ISR_RLIN316        480
#define OS_ISR_RLIN316UR0     481
#define OS_ISR_RLIN316UR1     482
#define OS_ISR_RLIN316UR2     483
#define OS_ISR_RLIN317        484
#define OS_ISR_RLIN317UR0     485
#define OS_ISR_RLIN317UR1     486
#define OS_ISR_RLIN317UR2     487
#define OS_ISR_RLIN318        488
#define OS_ISR_RLIN318UR0     489
#define OS_ISR_RLIN318UR1     490
#define OS_ISR_RLIN318UR2     491
#define OS_ISR_RLIN319        492
#define OS_ISR_RLIN319UR0     493
#define OS_ISR_RLIN319UR1     494
#define OS_ISR_RLIN319UR2     495
#define OS_ISR_RLIN320        496
#define OS_ISR_RLIN320UR0     497
#define OS_ISR_RLIN320UR1     498
#define OS_ISR_RLIN320UR2     499
#define OS_ISR_RLIN321        500
#define OS_ISR_RLIN321UR0     501
#define OS_ISR_RLIN321UR1     502
#define OS_ISR_RLIN321UR2     503
#define OS_ISR_RLIN322        504
#define OS_ISR_RLIN322UR0     505
#define OS_ISR_RLIN322UR1     506
#define OS_ISR_RLIN322UR2     507
#define OS_ISR_RLIN323        508
#define OS_ISR_RLIN323UR0     509
#define OS_ISR_RLIN323UR1     510
#define OS_ISR_RLIN323UR2     511
#define OS_ISR_RSENT0SI       512
#define OS_ISR_RSENT0RI       513
#define OS_ISR_RSENT1SI       514
#define OS_ISR_RSENT1RI       515
#define OS_ISR_RSENT2SI       516
#define OS_ISR_RSENT2RI       517
#define OS_ISR_RSENT3SI       518
#define OS_ISR_RSENT3RI       519
#define OS_ISR_RSENT4SI       520
#define OS_ISR_RSENT4RI       521
#define OS_ISR_RSENT5SI       522
#define OS_ISR_RSENT5RI       523
#define OS_ISR_RSENT6SI       524
#define OS_ISR_RSENT6RI       525
#define OS_ISR_RSENT7SI       526
#define OS_ISR_RSENT7RI       527
#define OS_ISR_TPBA0IPRD      528
#define OS_ISR_TPBA0IDTY      529
#define OS_ISR_TPBA0IPAT      530
#define OS_ISR_TPBA1IPRD      531
#define OS_ISR_TPBA1IDTY      532
#define OS_ISR_TPBA1IPAT      533
#define OS_ISR_ENCA0IOV       534
#define OS_ISR_ENCA0IUD       535
#define OS_ISR_ENCA0I0        536
#define OS_ISR_ENCA0I1        537
#define OS_ISR_ENCA0IEC       538
#define OS_ISR_ENCA1IOV       539
#define OS_ISR_ENCA1IUD       540
#define OS_ISR_ENCA1I0        541
#define OS_ISR_ENCA1I1        542
#define OS_ISR_ENCA1IEC       543
#define OS_ISR_P0             544
#define OS_ISR_P1             545
#define OS_ISR_P2             546
#define OS_ISR_P3             547
#define OS_ISR_P4             548
#define OS_ISR_P5             549
#define OS_ISR_P6             550
#define OS_ISR_P7             551
#define OS_ISR_P8             552
#define OS_ISR_P9             553
#define OS_ISR_P10            554
#define OS_ISR_P11            555
#define OS_ISR_P12            556
#define OS_ISR_P13            557
#define OS_ISR_P14            558
#define OS_ISR_P15            559
#define OS_ISR_P16            560
#define OS_ISR_P17            561
#define OS_ISR_P18            562
#define OS_ISR_P19            563
#define OS_ISR_P20            564
#define OS_ISR_P21            565
#define OS_ISR_P22            566
#define OS_ISR_P23            567
#define OS_ISR_P24            568
#define OS_ISR_P25            569
#define OS_ISR_P26            570
#define OS_ISR_P27            571
#define OS_ISR_P28            572
#define OS_ISR_P29            573
#define OS_ISR_P30            574
#define OS_ISR_P31            575
#define OS_ISR_P32            576
#define OS_ISR_P33            577
#define OS_ISR_P34            578
#define OS_ISR_P35            579
#define OS_ISR_P36            580
#define OS_ISR_P37            581
#define OS_ISR_P38            582
#define OS_ISR_P39            583
#define OS_ISR_PSI5S0INTCH0   584
#define OS_ISR_PSI5S0INTCH1   585
#define OS_ISR_PSI5S0INTCH2   586
#define OS_ISR_PSI5S0INTCH3   587
#define OS_ISR_PSI5S0INTCH4   588
#define OS_ISR_PSI5S0INTCH5   589
#define OS_ISR_PSI5S0INTCH6   590
#define OS_ISR_PSI5S0INTCH7   591
#define OS_ISR_PSI5S1INTCH0   592
#define OS_ISR_PSI5S1INTCH1   593
#define OS_ISR_PSI5S1INTCH2   594
#define OS_ISR_PSI5S1INTCH3   595
#define OS_ISR_PSI5S1INTCH4   596
#define OS_ISR_PSI5S1INTCH5   597
#define OS_ISR_PSI5S1INTCH6   598
#define OS_ISR_PSI5S1INTCH7   599
#define OS_ISR_PSI50SI        600
#define OS_ISR_PSI50RI        601
#define OS_ISR_PSI50TI        602
#define OS_ISR_PSI51SI        603
#define OS_ISR_PSI51RI        604
#define OS_ISR_PSI51TI        605
#define OS_ISR_PSI52SI        606
#define OS_ISR_PSI52RI        607
#define OS_ISR_PSI52TI        608
#define OS_ISR_PSI53SI        609
#define OS_ISR_PSI53RI        610
#define OS_ISR_PSI53TI        611
#define OS_ISR_RHSIF0TXCMP    612
#define OS_ISR_RHSIF0TXEX     613
#define OS_ISR_RHSIF0RXCMP    614
#define OS_ISR_RHSIF0RXEX     615
#define OS_ISR_RHSIF0ICLCR    616
#define OS_ISR_RHSIF0INTCH0   617
#define OS_ISR_RHSIF0INTCH1   618
#define OS_ISR_RHSIF0INTCH2   619
#define OS_ISR_RHSIF0INTCH3   620
#define OS_ISR_RHSIF0STR      621
#define OS_ISR_RHSIF0ERR      622
#define OS_ISR_FLXA0LINE0     623
#define OS_ISR_FLXA0LINE1     624
#define OS_ISR_FLXA0TIM0      625
#define OS_ISR_FLXA0TIM1      626
#define OS_ISR_FLXA0TIM2      627
#define OS_ISR_FLXA0FDA       628
#define OS_ISR_FLXA0FW        629
#define OS_ISR_FLXA0OW        630
#define OS_ISR_FLXA0OT        631
#define OS_ISR_FLXA0IQF       632
#define OS_ISR_FLXA0IQE       633
#define OS_ISR_FLXA1LINE0     634
#define OS_ISR_FLXA1LINE1     635
#define OS_ISR_FLXA1TIM0      636
#define OS_ISR_FLXA1TIM1      637
#define OS_ISR_FLXA1TIM2      638
#define OS_ISR_FLXA1FDA       639
#define OS_ISR_FLXA1FW        640
#define OS_ISR_FLXA1OW        641
#define OS_ISR_FLXA1OT        642
#define OS_ISR_FLXA1IQF       643
#define OS_ISR_FLXA1IQE       644
#define OS_ISR_ETNB0DATA      645
#define OS_ISR_ETNB0ERR       646
#define OS_ISR_ETNB0MNG       647
#define OS_ISR_ETNB0MAC       648
#define OS_ISR_ETNB1DATA      649
#define OS_ISR_ETNB1ERR       650
#define OS_ISR_ETNB1MNG       651
#define OS_ISR_ETNB1MAC       652
#define OS_ISR_ETNB1STA       653
#define OS_ISR_RTCA01S        654
#define OS_ISR_RTCA0AL        655
#define OS_ISR_RTCA0R         656
#define OS_ISR_OTSOTE         657
#define OS_ISR_OTSOTI         658
#define OS_ISR_OTSOTULI       659
#define OS_ISR_PWGCG00        660
#define OS_ISR_PWGCG01        661
#define OS_ISR_PWGCG02        662
#define OS_ISR_PWGCG10        663
#define OS_ISR_PWGCG11        664
#define OS_ISR_PWGCG12        665
#define OS_ISR_PWGCG20        666
#define OS_ISR_PWGCG21        667
#define OS_ISR_PWGCG22        668
#define OS_ISR_PWGCG30        669
#define OS_ISR_PWGCG31        670
#define OS_ISR_PWGCG32        671
#define OS_ISR_PWSD_INT_QFULL 672
#define OS_ISR_CXP10TI        673
#define OS_ISR_CXP10RI        674
#define OS_ISR_CXP10SI        675
#define OS_ISR_CXP11TI        676
#define OS_ISR_CXP11RI        677
#define OS_ISR_CXP11SI        678
#define OS_ISR_CXP12TI        679
#define OS_ISR_CXP12RI        680
#define OS_ISR_CXP12SI        681
#define OS_ISR_CXP13TI        682
#define OS_ISR_CXP13RI        683
#define OS_ISR_CXP13SI        684
#define OS_ISR_RIIC0EE        685
#define OS_ISR_RIIC0RI        686
#define OS_ISR_RIIC0TI        687
#define OS_ISR_RIIC0TEI       688
#define OS_ISR_RIIC1EE        689
#define OS_ISR_RIIC1RI        690
#define OS_ISR_RIIC1TI        691
#define OS_ISR_RIIC1TEI       692
#define OS_ISR_CWEND          693
#define OS_ISR_DPE            694
#define OS_ISR_MMCA0          696
#define OS_ISR_DCUDEGRADPE    697
#define OS_ISR_ICUMFATALERR   698
#define OS_ISR_DNFA1WUF0      699
#define OS_ISR_DNFA1WUF1      700
#define OS_ISR_DNFA2WUF0      701
#define OS_ISR_DNFA2WUF1      702
#define OS_ISR_DNFA0WUF0      703
#define OS_ISR_DNFA0WUF1      704
#define OS_ISR_WDTBATIT       705
#define OS_ISR_DCUTDI         706
#define OS_ISR_TAPA0ESO       715
#define OS_ISR_TAPA1ESO       716

#define OS_IRQTYPE_EIINT      0
#define OS_IRQTYPE_FEINT      1

#define INVALID_SELECTION_BIT (0xFFFFU)

/*=======[T Y P E   D E F I N I T I O N S]====================================*/
typedef void (*Os_isrhnd)(void);
typedef struct
{
    uint8 IntPrio;
    Os_isrhnd ISR;
    uint16 IntNumber;
    uint16 intc2SelectionBit;
} Os_IntInstallType;

/*=======[E X T E R N A L   D A T A]==========================================*/
extern VAR(Os_isrhnd, OS_VAR) Os_FE_Handler[OS_ARCH_FE_INTR_NUM];
extern VAR(Os_isrhnd, OS_VAR) Os_Vector_Handler[OS_ARCH_EI_INTR_NUM];

extern FUNC(void, OS_CODE) Os_InterruptInit(void);
extern FUNC(void, OS_CODE) Os_FEInterruptInstall(uint32 srcAddr, Os_isrhnd isrProc);
extern FUNC(void, OS_CODE) Os_InterruptInstall(const Os_IsrCfgType* IsrCfg, const Os_IntInstallType* IntInstall);

#endif /*#ifndef ARCH_IRQSOURCE_H*/
/*=======[E N D   O F   F I L E]==============================================*/
