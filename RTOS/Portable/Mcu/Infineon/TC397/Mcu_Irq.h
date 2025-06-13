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
**  FILENAME    : Mcu_Irq.h                                                   **
**                                                                            **
**  Created on  :                                                             **
**  Author      : i-soft-os                                                   **
**  Vendor      :                                                             **
**  DESCRIPTION : Deal with operations related to processor interrupts        **
**                                                                            **
**  SPECIFICATION(S) :   AUTOSAR classic Platform r19                         **
**  Version :   AUTOSAR classic Platform R19--Function Safety                 **
**                                                                            **
*******************************************************************************/

#ifndef MCU_IRQ_H
#define MCU_IRQ_H

/*=======[I N C L U D E S]=================================================*/
#include "Os_Types.h"

/*=======[M A C R O S]=====================================================*/
#define OS_SRC_SRE_BIT 10u

/* Interrupt Service Provider */
#define OS_ARCH_INT_CPU0 (0x00000000u)
#define OS_ARCH_INT_CPU1 (0x00001000u)
#define OS_ARCH_INT_CPU2 (0x00001800u)
#define OS_ARCH_INT_CPU3 (0x00002000u)
#define OS_ARCH_INT_CPU4 (0x00002800u)
#define OS_ARCH_INT_CPU5 (0x00003000u)
#define OS_ARCH_INT_DMA  (0x00000800u)

/*
 * INT register
 */
/* Service Request Broadcast Register 0 */
#define INT_SRB0 OS_REG32(0xF0037010u)
/* Service Request Broadcast Register 1 */
#define INT_SRB1 OS_REG32(0xF0037014u)
/* Service Request Broadcast Register 2 */
#define INT_SRB2 OS_REG32(0xF0037018u)
/* Service Request Broadcast Register 3 */
#define INT_SRB3 OS_REG32(0xF003701Bu)
/* Service Request Broadcast Register 4 */
#define INT_SRB4 OS_REG32(0xF0037020u)
/* Service Request Broadcast Register 4 */
#define INT_SRB5 OS_REG32(0xF0037024u)

/*
 * Service Request Control Register Address. Used in os_app.
 */
#define OS_ISR_CPU0_SB       (0xF0038000u)
#define OS_ISR_CPU1_SB       (0xF0038004u)
#define OS_ISR_CPU2_SB       (0xF0038008u)
#define OS_ISR_CPU3_SB       (0xF003800Cu)
#define OS_ISR_CPU4_SB       (0xF0038010u)
#define OS_ISR_CPU5_SB       (0xF0038014u)
#define OS_ISR_SBCU          (0xF0038020u)
#define OS_ISR_EBCU          (0xF0038024u)
#define OS_ISR_AGBT0_SR      (0xF003802Cu)
#define OS_ISR_XBAR0_SR      (0xF0038030u)
#define OS_ISR_XBAR1_SR      (0xF0038034u)
#define OS_ISR_XBAR2_SR      (0xF0038038u)
#define OS_ISR_CERBERUS_SR0  (0xF0038040u)
#define OS_ISR_CERBERUS_SR1  (0xF0038044u)
#define OS_ISR_TX            (0xF0038050u)
#define OS_ISR_RX            (0xF0038054u)
#define OS_ISR_ERR           (0xF0038058u)
#define OS_ISR_ASCLIN1_TX    (0xF003805Cu)
#define OS_ISR_ASCLIN1_RX    (0xF0038060u)
#define OS_ISR_ASCLIN1_ERR   (0xF0038064u)
#define OS_ISR_ASCLIN2_TX    (0xF0038068u)
#define OS_ISR_ASCLIN2_RX    (0xF003806Cu)
#define OS_ISR_ASCLIN2_ERR   (0xF0038070u)
#define OS_ISR_ASCLIN3_TX    (0xF0038074u)
#define OS_ISR_ASCLIN3_RX    (0xF0038078u)
#define OS_ISR_ASCLIN3_ERR   (0xF003807Cu)
#define OS_ISR_ASCLIN4_TX    (0xF0038080u)
#define OS_ISR_ASCLIN4_RX    (0xF0038084u)
#define OS_ISR_ASCLIN4_ERR   (0xF0038088u)
#define OS_ISR_ASCLIN5_TX    (0xF003808Cu)
#define OS_ISR_ASCLIN5_RX    (0xF0038090u)
#define OS_ISR_ASCLIN5_ERR   (0xF0038094u)
#define OS_ISR_ASCLIN6_TX    (0xF0038098u)
#define OS_ISR_ASCLIN6_RX    (0xF003809Cu)
#define OS_ISR_ASCLIN6_ERR   (0xF00380A0u)
#define OS_ISR_ASCLIN7_TX    (0xF00380A4u)
#define OS_ISR_ASCLIN7_RX    (0xF00380A8u)
#define OS_ISR_ASCLIN7_ERR   (0xF00380ACu)
#define OS_ISR_ASCLIN8_TX    (0xF00380B0u)
#define OS_ISR_ASCLIN8_RX    (0xF00380B4u)
#define OS_ISR_ASCLIN8_ERR   (0xF00380B8u)
#define OS_ISR_ASCLIN9_TX    (0xF00380BCu)
#define OS_ISR_ASCLIN9_RX    (0xF00380C0u)
#define OS_ISR_ASCLIN9_ERR   (0xF00380C4u)
#define OS_ISR_ASCLIN10_TX   (0xF00380C8u)
#define OS_ISR_ASCLIN10_RX   (0xF00380CCu)
#define OS_ISR_ASCLIN10_ERR  (0xF00380D0u)
#define OS_ISR_ASCLIN11_TX   (0xF00380D4u)
#define OS_ISR_ASCLIN11_RX   (0xF00380D8u)
#define OS_ISR_ASCLIN11_ERR  (0xF00380DCu)
#define OS_ISR_MTUDONE       (0xF00380ECu)
#define OS_ISR_QSPI0_TX      (0xF00380F0u)
#define OS_ISR_QSPI0_RX      (0xF00380F4u)
#define OS_ISR_QSPI0_ERR     (0xF00380F8u)
#define OS_ISR_QSPI0_PT      (0xF00380FCu)
#define OS_ISR_QSPI0_U       (0xF0038100u)
#define OS_ISR_QSPI1_TX      (0xF0038104u)
#define OS_ISR_QSPI1_RX      (0xF0038108u)
#define OS_ISR_QSPI1_ERR     (0xF003810Cu)
#define OS_ISR_QSPI1_PT      (0xF0038110u)
#define OS_ISR_QSPI1_U       (0xF0038114u)
#define OS_ISR_QSPI2_TX      (0xF0038118u)
#define OS_ISR_QSPI2_RX      (0xF003811Cu)
#define OS_ISR_QSPI2_ERR     (0xF0038120u)
#define OS_ISR_QSPI2_PT      (0xF0038124u)
#define OS_ISR_QSPI2_U       (0xF0038128u)
#define OS_ISR_QSPI3_TX      (0xF003812Cu)
#define OS_ISR_QSPI3_RX      (0xF0038130u)
#define OS_ISR_QSPI3_ERR     (0xF0038134u)
#define OS_ISR_QSPI3_PT      (0xF0038138u)
#define OS_ISR_QSPI3_U       (0xF003813Cu)
#define OS_ISR_QSPI4_TX      (0xF0038140u)
#define OS_ISR_QSPI4_RX      (0xF0038144u)
#define OS_ISR_QSPI4_ERR     (0xF0038148u)
#define OS_ISR_QSPI4_PT      (0xF003814Cu)
#define OS_ISR_QSPI4_U       (0xF0038150u)
#define OS_ISR_QSPI5_TX      (0xF0038154u)
#define OS_ISR_QSPI5_RX      (0xF0038158u)
#define OS_ISR_QSPI5_ERR     (0xF003815Cu)
#define OS_ISR_QSPI5_PT      (0xF0038160u)
#define OS_ISR_QSPI5_U       (0xF0038164u)
#define OS_ISR_QSPI2HC       (0xF0038178u)
#define OS_ISR_QSPI3HC       (0xF003817Cu)
#define OS_ISR_HSCT0_SR      (0xF0038180u)
#define OS_ISR_HSCT1_SR      (0xF0038184u)
#define OS_ISR_HSSL0_CH0_COK (0xF0038190u)
#define OS_ISR_HSSL0_CH0_RDI (0xF0038194u)
#define OS_ISR_HSSL0_CH0_ERR (0xF0038198u)
#define OS_ISR_HSSL0_CH0_TRG (0xF003819Cu)
#define OS_ISR_HSSL0_CH1_COK (0xF00381A0u)
#define OS_ISR_HSSL0_CH1_RDI (0xF00381A4u)
#define OS_ISR_HSSL0_CH1_ERR (0xF00381A8u)
#define OS_ISR_HSSL0_CH1_TRG (0xF00381ACu)
#define OS_ISR_HSSL0_CH2_COK (0xF00381B0u)
#define OS_ISR_HSSL0_CH2_RDI (0xF00381B4u)
#define OS_ISR_HSSL0_CH2_ERR (0xF00381B8u)
#define OS_ISR_HSSL0_CH2_TRG (0xF00381BCu)
#define OS_ISR_HSSL0_CH3_COK (0xF00381C0u)
#define OS_ISR_HSSL0_CH3_RDI (0xF00381C4u)
#define OS_ISR_HSSL0_CH3_ERR (0xF00381C8u)
#define OS_ISR_HSSL0_CH3_TRG (0xF00381CCu)
#define OS_ISR_HSSL0_EXI     (0xF00381D0u)
#define OS_ISR_HSSL1_CH0_COK (0xF00381D4u)
#define OS_ISR_HSSL1_CH0_RDI (0xF00381D8u)
#define OS_ISR_HSSL1_CH0_ERR (0xF00381DCu)
#define OS_ISR_HSSL1_CH0_TRG (0xF00381E0u)
#define OS_ISR_HSSL1_CH1_COK (0xF00381E4u)
#define OS_ISR_HSSL1_CH1_RDI (0xF00381E8u)
#define OS_ISR_HSSL1_CH1_ERR (0xF00381ECu)
#define OS_ISR_HSSL1_CH1_TRG (0xF00381F0u)
#define OS_ISR_HSSL1_CH2_COK (0xF00381F4u)
#define OS_ISR_HSSL1_CH2_RDI (0xF00381F8u)
#define OS_ISR_HSSL1_CH2_ERR (0xF00381FCu)
#define OS_ISR_HSSL1_CH2_TRG (0xF0038200u)
#define OS_ISR_HSSL1_CH3_COK (0xF0038204u)
#define OS_ISR_HSSL1_CH3_RDI (0xF0038208u)
#define OS_ISR_HSSL1_CH3_ERR (0xF003820Cu)
#define OS_ISR_HSSL1_CH3_TRG (0xF0038210u)
#define OS_ISR_HSSL1_EXI     (0xF0038214u)
#define OS_ISR_I2C0_DTR      (0xF0038220u)
#define OS_ISR_I2C0_ERR      (0xF0038224u)
#define OS_ISR_I2C0_P        (0xF0038228u)
#define OS_ISR_I2C1_DTR      (0xF0038230u)
#define OS_ISR_I2C1_ERR      (0xF0038234u)
#define OS_ISR_I2C1_P        (0xF0038238u)
#define OS_ISR_SENT0_SR      (0xF0038240u)
#define OS_ISR_SENT1_SR      (0xF0038244u)
#define OS_ISR_SENT2_SR      (0xF0038248u)
#define OS_ISR_SENT3_SR      (0xF003824Cu)
#define OS_ISR_SENT4_SR      (0xF0038250u)
#define OS_ISR_SENT5_SR      (0xF0038254u)
#define OS_ISR_SENT6_SR      (0xF0038258u)
#define OS_ISR_SENT7_SR      (0xF003825Cu)
#define OS_ISR_SENT8_SR      (0xF0038260u)
#define OS_ISR_SENT9_SR      (0xF0038264u)
#define OS_ISR_MSC0_SR0      (0xF0038270u)
#define OS_ISR_MSC0_SR1      (0xF0038274u)
#define OS_ISR_MSC0_SR2      (0xF0038278u)
#define OS_ISR_MSC0_SR3      (0xF003827Cu)
#define OS_ISR_MSC0_SR4      (0xF0038280u)
#define OS_ISR_MSC1_SR0      (0xF0038284u)
#define OS_ISR_MSC1_SR1      (0xF0038288u)
#define OS_ISR_MSC1_SR2      (0xF003828Cu)
#define OS_ISR_MSC1_SR3      (0xF0038290u)
#define OS_ISR_MSC1_SR4      (0xF0038294u)
#define OS_ISR_MSC2_SR0      (0xF0038298u)
#define OS_ISR_MSC2_SR1      (0xF003829Cu)
#define OS_ISR_MSC2_SR2      (0xF00382A0u)
#define OS_ISR_MSC2_SR3      (0xF00382A4u)
#define OS_ISR_MSC2_SR4      (0xF00382A8u)
#define OS_ISR_MSC3_SR0      (0xF00382ACu)
#define OS_ISR_MSC3_SR1      (0xF00382B0u)
#define OS_ISR_MSC3_SR2      (0xF00382B4u)
#define OS_ISR_MSC3_SR3      (0xF00382B8u)
#define OS_ISR_MSC3_SR4      (0xF00382BCu)
#define OS_ISR_CCU6_CCU0_SR0 (0xF00382C0u)
#define OS_ISR_CCU6_CCU0_SR1 (0xF00382C4u)
#define OS_ISR_CCU6_CCU0_SR2 (0xF00382C8u)
#define OS_ISR_CCU6_CCU0_SR3 (0xF00382CCu)
#define OS_ISR_CCU6_CCU1_SR0 (0xF00382D0u)
#define OS_ISR_CCU6_CCU1_SR1 (0xF00382D4u)
#define OS_ISR_CCU6_CCU1_SR2 (0xF00382D8u)
#define OS_ISR_CCU6_CCU1_SR3 (0xF00382DCu)
#define OS_ISR_GPT120_CIRQ   (0xF00382E0u)
#define OS_ISR_GPT120_T2     (0xF00382E4u)
#define OS_ISR_GPT120_T3     (0xF00382E8u)
#define OS_ISR_GPT120_T4     (0xF00382ECu)
#define OS_ISR_GPT120_T5     (0xF00382F0u)
#define OS_ISR_GPT120_T6     (0xF00382F4u)
#define OS_ISR_STM0_SR0      (0xF0038300u)
#define OS_ISR_STM0_SR1      (0xF0038304u)
#define OS_ISR_STM1_SR0      (0xF0038308u)
#define OS_ISR_STM1_SR1      (0xF003830Cu)
#define OS_ISR_STM2_SR0      (0xF0038310u)
#define OS_ISR_STM2_SR1      (0xF0038314u)
#define OS_ISR_STM3_SR0      (0xF0038318u)
#define OS_ISR_STM3_SR1      (0xF003831Cu)
#define OS_ISR_STM4_SR0      (0xF0038320u)
#define OS_ISR_STM4_SR1      (0xF0038324u)
#define OS_ISR_STM5_SR0      (0xF0038328u)
#define OS_ISR_STM5_SR1      (0xF003832Cu)
#define OS_ISR_FCE0_SR       (0xF0038330u)
#define OS_ISR_DMA0_ERR0     (0xF0038340u)
#define OS_ISR_DMA0_ERR1     (0xF0038344u)
#define OS_ISR_DMA0_ERR2     (0xF0038348u)
#define OS_ISR_DMA0_ERR3     (0xF003834Cu)
#define OS_ISR_DMA0_CH0      (0xF0038370u)
#define OS_ISR_DMA0_CH1      (0xF0038374u)
#define OS_ISR_DMA0_CH2      (0xF0038378u)
#define OS_ISR_DMA0_CH3      (0xF003837Cu)
#define OS_ISR_DMA0_CH4      (0xF0038380u)
#define OS_ISR_DMA0_CH5      (0xF0038384u)
#define OS_ISR_DMA0_CH6      (0xF0038388u)
#define OS_ISR_DMA0_CH7      (0xF003838Cu)
#define OS_ISR_DMA0_CH8      (0xF0038390u)
#define OS_ISR_DMA0_CH9      (0xF0038394u)
#define OS_ISR_DMA0_CH10     (0xF0038398u)
#define OS_ISR_DMA0_CH11     (0xF003839Cu)
#define OS_ISR_DMA0_CH12     (0xF00383A0u)
#define OS_ISR_DMA0_CH13     (0xF00383A4u)
#define OS_ISR_DMA0_CH14     (0xF00383A8u)
#define OS_ISR_DMA0_CH15     (0xF00383ACu)
#define OS_ISR_DMA0_CH16     (0xF00383B0u)
#define OS_ISR_DMA0_CH17     (0xF00383B4u)
#define OS_ISR_DMA0_CH18     (0xF00383B8u)
#define OS_ISR_DMA0_CH19     (0xF00383BCu)
#define OS_ISR_DMA0_CH20     (0xF00383C0u)
#define OS_ISR_DMA0_CH21     (0xF00383C4u)
#define OS_ISR_DMA0_CH22     (0xF00383C8u)
#define OS_ISR_DMA0_CH23     (0xF00383CCu)
#define OS_ISR_DMA0_CH24     (0xF00383D0u)
#define OS_ISR_DMA0_CH25     (0xF00383D4u)
#define OS_ISR_DMA0_CH26     (0xF00383D8u)
#define OS_ISR_DMA0_CH27     (0xF00383DCu)
#define OS_ISR_DMA0_CH28     (0xF00383E0u)
#define OS_ISR_DMA0_CH29     (0xF00383E4u)
#define OS_ISR_DMA0_CH30     (0xF00383E8u)
#define OS_ISR_DMA0_CH31     (0xF00383ECu)
#define OS_ISR_DMA0_CH32     (0xF00383F0u)
#define OS_ISR_DMA0_CH33     (0xF00383F4u)
#define OS_ISR_DMA0_CH34     (0xF00383F8u)
#define OS_ISR_DMA0_CH35     (0xF00383FCu)
#define OS_ISR_DMA0_CH36     (0xF0038400u)
#define OS_ISR_DMA0_CH37     (0xF0038404u)
#define OS_ISR_DMA0_CH38     (0xF0038408u)
#define OS_ISR_DMA0_CH39     (0xF003840Cu)
#define OS_ISR_DMA0_CH40     (0xF0038410u)
#define OS_ISR_DMA0_CH41     (0xF0038414u)
#define OS_ISR_DMA0_CH42     (0xF0038418u)
#define OS_ISR_DMA0_CH43     (0xF003841Cu)
#define OS_ISR_DMA0_CH44     (0xF0038420u)
#define OS_ISR_DMA0_CH45     (0xF0038424u)
#define OS_ISR_DMA0_CH46     (0xF0038428u)
#define OS_ISR_DMA0_CH47     (0xF003842Cu)
#define OS_ISR_DMA0_CH48     (0xF0038430u)
#define OS_ISR_DMA0_CH49     (0xF0038434u)
#define OS_ISR_DMA0_CH50     (0xF0038438u)
#define OS_ISR_DMA0_CH51     (0xF003843Cu)
#define OS_ISR_DMA0_CH52     (0xF0038440u)
#define OS_ISR_DMA0_CH53     (0xF0038444u)
#define OS_ISR_DMA0_CH54     (0xF0038448u)
#define OS_ISR_DMA0_CH55     (0xF003844Cu)
#define OS_ISR_DMA0_CH56     (0xF0038450u)
#define OS_ISR_DMA0_CH57     (0xF0038454u)
#define OS_ISR_DMA0_CH58     (0xF0038458u)
#define OS_ISR_DMA0_CH59     (0xF003845Cu)
#define OS_ISR_DMA0_CH60     (0xF0038460u)
#define OS_ISR_DMA0_CH61     (0xF0038464u)
#define OS_ISR_DMA0_CH62     (0xF0038468u)
#define OS_ISR_DMA0_CH63     (0xF003846Cu)
#define OS_ISR_DMA0_CH64     (0xF0038470u)
#define OS_ISR_DMA0_CH65     (0xF0038474u)
#define OS_ISR_DMA0_CH66     (0xF0038478u)
#define OS_ISR_DMA0_CH67     (0xF003847Cu)
#define OS_ISR_DMA0_CH68     (0xF0038480u)
#define OS_ISR_DMA0_CH69     (0xF0038484u)
#define OS_ISR_DMA0_CH70     (0xF0038488u)
#define OS_ISR_DMA0_CH71     (0xF003848Cu)
#define OS_ISR_DMA0_CH72     (0xF0038490u)
#define OS_ISR_DMA0_CH73     (0xF0038494u)
#define OS_ISR_DMA0_CH74     (0xF0038498u)
#define OS_ISR_DMA0_CH75     (0xF003849Cu)
#define OS_ISR_DMA0_CH76     (0xF00384A0u)
#define OS_ISR_DMA0_CH77     (0xF00384A4u)
#define OS_ISR_DMA0_CH78     (0xF00384A8u)
#define OS_ISR_DMA0_CH79     (0xF00384ACu)
#define OS_ISR_DMA0_CH80     (0xF00384B0u)
#define OS_ISR_DMA0_CH81     (0xF00384B4u)
#define OS_ISR_DMA0_CH82     (0xF00384B8u)
#define OS_ISR_DMA0_CH83     (0xF00384BCu)
#define OS_ISR_DMA0_CH84     (0xF00384C0u)
#define OS_ISR_DMA0_CH85     (0xF00384C4u)
#define OS_ISR_DMA0_CH86     (0xF00384C8u)
#define OS_ISR_DMA0_CH87     (0xF00384CCu)
#define OS_ISR_DMA0_CH88     (0xF00384D0u)
#define OS_ISR_DMA0_CH89     (0xF00384D4u)
#define OS_ISR_DMA0_CH90     (0xF00384D8u)
#define OS_ISR_DMA0_CH91     (0xF00384DCu)
#define OS_ISR_DMA0_CH92     (0xF00384E0u)
#define OS_ISR_DMA0_CH93     (0xF00384E4u)
#define OS_ISR_DMA0_CH94     (0xF00384E8u)
#define OS_ISR_DMA0_CH95     (0xF00384ECu)
#define OS_ISR_DMA0_CH96     (0xF00384F0u)
#define OS_ISR_DMA0_CH97     (0xF00384F4u)
#define OS_ISR_DMA0_CH98     (0xF00384F8u)
#define OS_ISR_DMA0_CH99     (0xF00384FCu)
#define OS_ISR_DMA0_CH100    (0xF0038500u)
#define OS_ISR_DMA0_CH101    (0xF0038504u)
#define OS_ISR_DMA0_CH102    (0xF0038508u)
#define OS_ISR_DMA0_CH103    (0xF003850Cu)
#define OS_ISR_DMA0_CH104    (0xF0038510u)
#define OS_ISR_DMA0_CH105    (0xF0038514u)
#define OS_ISR_DMA0_CH106    (0xF0038518u)
#define OS_ISR_DMA0_CH107    (0xF003851Cu)
#define OS_ISR_DMA0_CH108    (0xF0038520u)
#define OS_ISR_DMA0_CH109    (0xF0038524u)
#define OS_ISR_DMA0_CH110    (0xF0038528u)
#define OS_ISR_DMA0_CH111    (0xF003852Cu)
#define OS_ISR_DMA0_CH112    (0xF0038530u)
#define OS_ISR_DMA0_CH113    (0xF0038534u)
#define OS_ISR_DMA0_CH114    (0xF0038538u)
#define OS_ISR_DMA0_CH115    (0xF003853Cu)
#define OS_ISR_DMA0_CH116    (0xF0038540u)
#define OS_ISR_DMA0_CH117    (0xF0038544u)
#define OS_ISR_DMA0_CH118    (0xF0038548u)
#define OS_ISR_DMA0_CH119    (0xF003854Cu)
#define OS_ISR_DMA0_CH120    (0xF0038550u)
#define OS_ISR_DMA0_CH121    (0xF0038554u)
#define OS_ISR_DMA0_CH122    (0xF0038558u)
#define OS_ISR_DMA0_CH123    (0xF003855Cu)
#define OS_ISR_DMA0_CH124    (0xF0038560u)
#define OS_ISR_DMA0_CH125    (0xF0038564u)
#define OS_ISR_DMA0_CH126    (0xF0038568u)
#define OS_ISR_DMA0_CH127    (0xF003856Cu)
#define OS_ISR_SDMMC0_ERR    (0xF0038570u)
#define OS_ISR_SDMMC0_DMA    (0xF0038574u)
#define OS_ISR_GETH0_SR0     (0xF0038580u)
#define OS_ISR_GETH0_SR1     (0xF0038584u)
#define OS_ISR_GETH0_SR2     (0xF0038588u)
#define OS_ISR_GETH0_SR3     (0xF003858Cu)
#define OS_ISR_GETH0_SR4     (0xF0038590u)
#define OS_ISR_GETH0_SR5     (0xF0038594u)
#define OS_ISR_GETH0_SR6     (0xF0038598u)
#define OS_ISR_GETH0_SR7     (0xF003859Cu)
#define OS_ISR_GETH0_SR8     (0xF00385A0u)
#define OS_ISR_GETH0_SR9     (0xF00385A4u)
#define OS_ISR_CAN0_INT0     (0xF00385B0u)
#define OS_ISR_CAN0_INT1     (0xF00385B4u)
#define OS_ISR_CAN0_INT2     (0xF00385B8u)
#define OS_ISR_CAN0_INT3     (0xF00385BCu)
#define OS_ISR_CAN0_INT4     (0xF00385C0u)
#define OS_ISR_CAN0_INT5     (0xF00385C4u)
#define OS_ISR_CAN0_INT6     (0xF00385C8u)
#define OS_ISR_CAN0_INT7     (0xF00385CCu)
#define OS_ISR_CAN0_INT8     (0xF00385D0u)
#define OS_ISR_CAN0_INT9     (0xF00385D4u)
#define OS_ISR_CAN0_INT10    (0xF00385D8u)
#define OS_ISR_CAN0_INT11    (0xF00385DCu)
#define OS_ISR_CAN0_INT12    (0xF00385E0u)
#define OS_ISR_CAN0_INT13    (0xF00385E4u)
#define OS_ISR_CAN0_INT14    (0xF00385E8u)
#define OS_ISR_CAN0_INT15    (0xF00385ECu)
#define OS_ISR_CAN1_INT0     (0xF00385F0u)
#define OS_ISR_CAN1_INT1     (0xF00385F4u)
#define OS_ISR_CAN1_INT2     (0xF00385F8u)
#define OS_ISR_CAN1_INT3     (0xF00385FCu)
#define OS_ISR_CAN1_INT4     (0xF0038600u)
#define OS_ISR_CAN1_INT5     (0xF0038604u)
#define OS_ISR_CAN1_INT6     (0xF0038608u)
#define OS_ISR_CAN1_INT7     (0xF003860Cu)
#define OS_ISR_CAN1_INT8     (0xF0038610u)
#define OS_ISR_CAN1_INT9     (0xF0038614u)
#define OS_ISR_CAN1_INT10    (0xF0038618u)
#define OS_ISR_CAN1_INT11    (0xF003861Cu)
#define OS_ISR_CAN1_INT12    (0xF0038620u)
#define OS_ISR_CAN1_INT13    (0xF0038624u)
#define OS_ISR_CAN1_INT14    (0xF0038628u)
#define OS_ISR_CAN1_INT15    (0xF003862Cu)
#define OS_ISR_CAN2_INT0     (0xF0038630u)
#define OS_ISR_CAN2_INT1     (0xF0038634u)
#define OS_ISR_CAN2_INT2     (0xF0038638u)
#define OS_ISR_CAN2_INT3     (0xF003863Cu)
#define OS_ISR_CAN2_INT4     (0xF0038640u)
#define OS_ISR_CAN2_INT5     (0xF0038644u)
#define OS_ISR_CAN2_INT6     (0xF0038648u)
#define OS_ISR_CAN2_INT7     (0xF003864Cu)
#define OS_ISR_CAN2_INT8     (0xF0038650u)
#define OS_ISR_CAN2_INT9     (0xF0038654u)
#define OS_ISR_CAN2_INT10    (0xF0038658u)
#define OS_ISR_CAN2_INT11    (0xF003865Cu)
#define OS_ISR_CAN2_INT12    (0xF0038660u)
#define OS_ISR_CAN2_INT13    (0xF0038664u)
#define OS_ISR_CAN2_INT14    (0xF0038668u)
#define OS_ISR_CAN2_INT15    (0xF003866Cu)
#define OS_ISR_VADC_G0_SR0   (0xF0038670u)
#define OS_ISR_VADC_G0_SR1   (0xF0038674u)
#define OS_ISR_VADC_G0_SR2   (0xF0038678u)
#define OS_ISR_VADC_G0_SR3   (0xF003867Cu)
#define OS_ISR_VADC_G1_SR0   (0xF0038680u)
#define OS_ISR_VADC_G1_SR1   (0xF0038684u)
#define OS_ISR_VADC_G1_SR2   (0xF0038688u)
#define OS_ISR_VADC_G1_SR3   (0xF003868Cu)
#define OS_ISR_VADC_G2_SR0   (0xF0038690u)
#define OS_ISR_VADC_G2_SR1   (0xF0038694u)
#define OS_ISR_VADC_G2_SR2   (0xF0038698u)
#define OS_ISR_VADC_G2_SR3   (0xF003869Cu)
#define OS_ISR_VADC_G3_SR0   (0xF00386A0u)
#define OS_ISR_VADC_G3_SR1   (0xF00386A4u)
#define OS_ISR_VADC_G3_SR2   (0xF00386A8u)
#define OS_ISR_VADC_G3_SR3   (0xF00386ACu)
#define OS_ISR_VADC_G4_SR0   (0xF00386B0u)
#define OS_ISR_VADC_G4_SR1   (0xF00386B4u)
#define OS_ISR_VADC_G4_SR2   (0xF00386B8u)
#define OS_ISR_VADC_G4_SR3   (0xF00386BCu)
#define OS_ISR_VADC_G5_SR0   (0xF00386C0u)
#define OS_ISR_VADC_G5_SR1   (0xF00386C4u)
#define OS_ISR_VADC_G5_SR2   (0xF00386C8u)
#define OS_ISR_VADC_G5_SR3   (0xF00386CCu)
#define OS_ISR_VADC_G6_SR0   (0xF00386D0u)
#define OS_ISR_VADC_G6_SR1   (0xF00386D4u)
#define OS_ISR_VADC_G6_SR2   (0xF00386D8u)
#define OS_ISR_VADC_G6_SR3   (0xF00386DCu)
#define OS_ISR_VADC_G7_SR0   (0xF00386E0u)
#define OS_ISR_VADC_G7_SR1   (0xF00386E4u)
#define OS_ISR_VADC_G7_SR2   (0xF00386E8u)
#define OS_ISR_VADC_G7_SR3   (0xF00386ECu)
#define OS_ISR_VADC_G8_SR0   (0xF00386F0u)
#define OS_ISR_VADC_G8_SR1   (0xF00386F4u)
#define OS_ISR_VADC_G8_SR2   (0xF00386F8u)
#define OS_ISR_VADC_G8_SR3   (0xF00386FCu)
#define OS_ISR_VADC_G9_SR0   (0xF0038700u)
#define OS_ISR_VADC_G9_SR1   (0xF0038704u)
#define OS_ISR_VADC_G9_SR2   (0xF0038708u)
#define OS_ISR_VADC_G9_SR3   (0xF003870Cu)
#define OS_ISR_VADC_G10_SR0  (0xF0038710u)
#define OS_ISR_VADC_G10_SR1  (0xF0038714u)
#define OS_ISR_VADC_G10_SR2  (0xF0038718u)
#define OS_ISR_VADC_G10_SR3  (0xF003871Cu)
#define OS_ISR_VADC_G11_SR0  (0xF0038720u)
#define OS_ISR_VADC_G11_SR1  (0xF0038724u)
#define OS_ISR_VADC_G11_SR2  (0xF0038728u)
#define OS_ISR_VADC_G11_SR3  (0xF003872Cu)
#define OS_ISR_VADC_FC0_SR0  (0xF0038730u)
#define OS_ISR_VADC_FC1_SR0  (0xF0038734u)
#define OS_ISR_VADC_FC2_SR0  (0xF0038738u)
#define OS_ISR_VADC_FC3_SR0  (0xF003873Cu)
#define OS_ISR_VADC_FC4_SR0  (0xF0038740u)
#define OS_ISR_VADC_FC5_SR0  (0xF0038744u)
#define OS_ISR_VADC_FC6_SR0  (0xF0038748u)
#define OS_ISR_VADC_FC7_SR0  (0xF003874Cu)
#define OS_ISR_VADC_CG0_SR0  (0xF0038750u)
#define OS_ISR_VADC_CG0_SR1  (0xF0038754u)
#define OS_ISR_VADC_CG0_SR2  (0xF0038758u)
#define OS_ISR_VADC_CG0_SR3  (0xF003875Cu)
#define OS_ISR_VADC_CG1_SR0  (0xF0038760u)
#define OS_ISR_VADC_CG1_SR1  (0xF0038764u)
#define OS_ISR_VADC_CG1_SR2  (0xF0038768u)
#define OS_ISR_VADC_CG1_SR3  (0xF003876Cu)
#define OS_ISR_DSADC0_SRM    (0xF0038770u)
#define OS_ISR_DSADC0_SRA    (0xF0038774u)
#define OS_ISR_DSADC1_SRM    (0xF0038778u)
#define OS_ISR_DSADC1_SRA    (0xF003877Cu)
#define OS_ISR_DSADC2_SRM    (0xF0038780u)
#define OS_ISR_DSADC2_SRA    (0xF0038784u)
#define OS_ISR_DSADC3_SRM    (0xF0038788u)
#define OS_ISR_DSADC3_SRA    (0xF003878Cu)
#define OS_ISR_DSADC4_SRM    (0xF0038790u)
#define OS_ISR_DSADC4_SRA    (0xF0038794u)
#define OS_ISR_DSADC5_SRM    (0xF0038798u)
#define OS_ISR_DSADC5_SRA    (0xF003879Cu)
#define OS_ISR_DSADC6_SRM    (0xF00387A0u)
#define OS_ISR_DSADC6_SRA    (0xF00387A4u)
#define OS_ISR_DSADC7_SRM    (0xF00387A8u)
#define OS_ISR_DSADC7_SRA    (0xF00387ACu)
#define OS_ISR_DSADC8_SRM    (0xF00387B0u)
#define OS_ISR_DSADC8_SRA    (0xF00387B4u)
#define OS_ISR_DSADC9_SRM    (0xF00387B8u)
#define OS_ISR_DSADC9_SRA    (0xF00387BCu)
#define OS_ISR_DSADC10_SRM   (0xF00387C0u)
#define OS_ISR_DSADC10_SRA   (0xF00387C4u)
#define OS_ISR_DSADC11_SRM   (0xF00387C8u)
#define OS_ISR_DSADC11_SRA   (0xF00387CCu)
#define OS_ISR_DSADC12_SRM   (0xF00387D0u)
#define OS_ISR_DSADC12_SRA   (0xF00387D4u)
#define OS_ISR_DSADC13_SRM   (0xF00387D8u)
#define OS_ISR_DSADC13_SRA   (0xF00387DCu)
#define OS_ISR_ERAY0_INT0    (0xF0038800u)
#define OS_ISR_ERAY0_INT1    (0xF0038804u)
#define OS_ISR_ERAY0_TINT0   (0xF0038808u)
#define OS_ISR_ERAY0_TINT1   (0xF003880Cu)
#define OS_ISR_ERAY0_NDAT0   (0xF0038810u)
#define OS_ISR_ERAY0_NDAT1   (0xF0038814u)
#define OS_ISR_ERAY0_MBSC0   (0xF0038818u)
#define OS_ISR_ERAY0_MBSC1   (0xF003881Cu)
#define OS_ISR_ERAY0_OBUSY   (0xF0038820u)
#define OS_ISR_ERAY0_IBUSY   (0xF0038824u)
#define OS_ISR_ERAY1_INT0    (0xF0038830u)
#define OS_ISR_ERAY1_INT1    (0xF0038834u)
#define OS_ISR_ERAY1_TINT0   (0xF0038838u)
#define OS_ISR_ERAY1_TINT1   (0xF003883Cu)
#define OS_ISR_ERAY1_NDAT0   (0xF0038840u)
#define OS_ISR_ERAY1_NDAT1   (0xF0038844u)
#define OS_ISR_ERAY1_MBSC0   (0xF0038848u)
#define OS_ISR_ERAY1_MBSC1   (0xF003884Cu)
#define OS_ISR_ERAY1_OBUSY   (0xF0038850u)
#define OS_ISR_ERAY1_IBUSY   (0xF0038854u)
#define OS_ISR_DMUHOST       (0xF0038860u)
#define OS_ISR_DMUFSI        (0xF0038864u)
#define OS_ISR_HSM0_HSM0     (0xF0038870u)
#define OS_ISR_HSM0_HSM1     (0xF0038874u)
#define OS_ISR_SCUERU0       (0xF0038880u)
#define OS_ISR_SCUERU1       (0xF0038884u)
#define OS_ISR_SCUERU2       (0xF0038888u)
#define OS_ISR_SCUERU3       (0xF003888Cu)
#define OS_ISR_PMSDTS        (0xF00388ACu)
#define OS_ISR_PMS0_SR       (0xF00388B0u)
#define OS_ISR_PMS1_SR       (0xF00388B4u)
#define OS_ISR_PMS2_SR       (0xF00388B8u)
#define OS_ISR_PMS3_SR       (0xF00388BCu)
#define OS_ISR_SCR           (0xF00388C0u)
#define OS_ISR_SMU0_SR0      (0xF00388D0u)
#define OS_ISR_SMU0_SR1      (0xF00388D4u)
#define OS_ISR_SMU0_SR2      (0xF00388D8u)
#define OS_ISR_PSI50_SR0     (0xF00388E0u)
#define OS_ISR_PSI50_SR1     (0xF00388E4u)
#define OS_ISR_PSI50_SR2     (0xF00388E8u)
#define OS_ISR_PSI50_SR3     (0xF00388ECu)
#define OS_ISR_PSI50_SR4     (0xF00388F0u)
#define OS_ISR_PSI50_SR5     (0xF00388F4u)
#define OS_ISR_PSI50_SR6     (0xF00388F8u)
#define OS_ISR_PSI50_SR7     (0xF00388FCu)
#define OS_ISR_HSPDM0_BFR    (0xF0038900u)
#define OS_ISR_HSPDM0_RAMP   (0xF0038904u)
#define OS_ISR_HSPDM0_ERR    (0xF0038908u)
#define OS_ISR_DAM0_LI0      (0xF0038910u)
#define OS_ISR_DAM0_RI0      (0xF0038914u)
#define OS_ISR_DAM0_LI1      (0xF0038918u)
#define OS_ISR_DAM0_RI1      (0xF003891Cu)
#define OS_ISR_DAM0_DR       (0xF0038920u)
#define OS_ISR_DAM0_ERR      (0xF0038924u)
#define OS_ISR_DAM1_LI0      (0xF0038928u)
#define OS_ISR_DAM1_RI0      (0xF003892Cu)
#define OS_ISR_DAM1_LI1      (0xF0038930u)
#define OS_ISR_DAM1_RI1      (0xF0038934u)
#define OS_ISR_DAM1_DR       (0xF0038938u)
#define OS_ISR_DAM1_ERR      (0xF003893Cu)
#define OS_ISR_PSI5S0_SR0    (0xF0038950u)
#define OS_ISR_PSI5S0_SR1    (0xF0038954u)
#define OS_ISR_PSI5S0_SR2    (0xF0038958u)
#define OS_ISR_PSI5S0_SR3    (0xF003895Cu)
#define OS_ISR_PSI5S0_SR4    (0xF0038960u)
#define OS_ISR_PSI5S0_SR5    (0xF0038964u)
#define OS_ISR_PSI5S0_SR6    (0xF0038968u)
#define OS_ISR_PSI5S0_SR7    (0xF003896Cu)
#define OS_ISR_RIF0_ERR      (0xF0038970u)
#define OS_ISR_RIF0_INT      (0xF0038974u)
#define OS_ISR_RIF1_ERR      (0xF0038978u)
#define OS_ISR_RIF1_INT      (0xF003897Cu)
#define OS_ISR_SPU0_INT      (0xF0038980u)
#define OS_ISR_SPU0_ERR      (0xF0038984u)
#define OS_ISR_SPU1_INT      (0xF0038988u)
#define OS_ISR_SPU1_ERR      (0xF003898Cu)
#define OS_ISR_GPSR0_SR0     (0xF0038990u)
#define OS_ISR_GPSR0_SR1     (0xF0038994u)
#define OS_ISR_GPSR0_SR2     (0xF0038998u)
#define OS_ISR_GPSR0_SR3     (0xF003899Cu)
#define OS_ISR_GPSR0_SR4     (0xF00389A0u)
#define OS_ISR_GPSR0_SR5     (0xF00389A4u)
#define OS_ISR_GPSR0_SR6     (0xF00389A8u)
#define OS_ISR_GPSR0_SR7     (0xF00389ACu)
#define OS_ISR_GPSR1_SR0     (0xF00389B0u)
#define OS_ISR_GPSR1_SR1     (0xF00389B4u)
#define OS_ISR_GPSR1_SR2     (0xF00389B8u)
#define OS_ISR_GPSR1_SR3     (0xF00389BCu)
#define OS_ISR_GPSR1_SR4     (0xF00389C0u)
#define OS_ISR_GPSR1_SR5     (0xF00389C4u)
#define OS_ISR_GPSR1_SR6     (0xF00389C8u)
#define OS_ISR_GPSR1_SR7     (0xF00389CCu)
#define OS_ISR_GPSR2_SR0     (0xF00389D0u)
#define OS_ISR_GPSR2_SR1     (0xF00389D4u)
#define OS_ISR_GPSR2_SR2     (0xF00389D8u)
#define OS_ISR_GPSR2_SR3     (0xF00389DCu)
#define OS_ISR_GPSR2_SR4     (0xF00389E0u)
#define OS_ISR_GPSR2_SR5     (0xF00389E4u)
#define OS_ISR_GPSR2_SR6     (0xF00389E8u)
#define OS_ISR_GPSR2_SR7     (0xF00389ECu)
#define OS_ISR_GPSR3_SR0     (0xF00389F0u)
#define OS_ISR_GPSR3_SR1     (0xF00389F4u)
#define OS_ISR_GPSR3_SR2     (0xF00389F8u)
#define OS_ISR_GPSR3_SR3     (0xF00389FCu)
#define OS_ISR_GPSR3_SR4     (0xF0038A00u)
#define OS_ISR_GPSR3_SR5     (0xF0038A04u)
#define OS_ISR_GPSR3_SR6     (0xF0038A08u)
#define OS_ISR_GPSR3_SR7     (0xF0038A0Cu)
#define OS_ISR_GPSR4_SR0     (0xF0038A10u)
#define OS_ISR_GPSR4_SR1     (0xF0038A14u)
#define OS_ISR_GPSR4_SR2     (0xF0038A18u)
#define OS_ISR_GPSR4_SR3     (0xF0038A1Cu)
#define OS_ISR_GPSR4_SR4     (0xF0038A20u)
#define OS_ISR_GPSR4_SR5     (0xF0038A24u)
#define OS_ISR_GPSR4_SR6     (0xF0038A28u)
#define OS_ISR_GPSR4_SR7     (0xF0038A2Cu)
#define OS_ISR_GPSR5_SR0     (0xF0038A30u)
#define OS_ISR_GPSR5_SR1     (0xF0038A34u)
#define OS_ISR_GPSR5_SR2     (0xF0038A38u)
#define OS_ISR_GPSR5_SR3     (0xF0038A3Cu)
#define OS_ISR_GPSR5_SR4     (0xF0038A40u)
#define OS_ISR_GPSR5_SR5     (0xF0038A44u)
#define OS_ISR_GPSR5_SR6     (0xF0038A48u)
#define OS_ISR_GPSR5_SR7     (0xF0038A4Cu)
#define OS_ISR_GTM_AEIIRQ    (0xF0038A70u)
#define OS_ISR_GTM_ARUIRQ0   (0xF0038A74u)
#define OS_ISR_GTM_ARUIRQ1   (0xF0038A78u)
#define OS_ISR_GTM_ARUIRQ2   (0xF0038A7Cu)
#define OS_ISR_GTM_BRCIRQ    (0xF0038A80u)
#define OS_ISR_GTM_CMBIRQ    (0xF0038A84u)
#define OS_ISR_GTM_SPEIRQ0   (0xF0038A88u)
#define OS_ISR_GTM_SPEIRQ1   (0xF0038A8Cu)
#define OS_ISR_GTM_SPEIRQ2   (0xF0038A90u)
#define OS_ISR_GTM_SPEIRQ3   (0xF0038A94u)
#define OS_ISR_GTM_SPEIRQ4   (0xF0038A98u)
#define OS_ISR_GTM_SPEIRQ5   (0xF0038A9Cu)
#define OS_ISR_GTM_PSM0_0    (0xF0038AA0u)
#define OS_ISR_GTM_PSM0_1    (0xF0038AA4u)
#define OS_ISR_GTM_PSM0_2    (0xF0038AA8u)
#define OS_ISR_GTM_PSM0_3    (0xF0038AACu)
#define OS_ISR_GTM_PSM0_4    (0xF0038AB0u)
#define OS_ISR_GTM_PSM0_5    (0xF0038AB4u)
#define OS_ISR_GTM_PSM0_6    (0xF0038AB8u)
#define OS_ISR_GTM_PSM0_7    (0xF0038ABCu)
#define OS_ISR_GTM_PSM1_0    (0xF0038AC0u)
#define OS_ISR_GTM_PSM1_1    (0xF0038AC4u)
#define OS_ISR_GTM_PSM1_2    (0xF0038AC8u)
#define OS_ISR_GTM_PSM1_3    (0xF0038ACCu)
#define OS_ISR_GTM_PSM1_4    (0xF0038AD0u)
#define OS_ISR_GTM_PSM1_5    (0xF0038AD4u)
#define OS_ISR_GTM_PSM1_6    (0xF0038AD8u)
#define OS_ISR_GTM_PSM1_7    (0xF0038ADCu)
#define OS_ISR_GTM_PSM2_0    (0xF0038AE0u)
#define OS_ISR_GTM_PSM2_1    (0xF0038AE4u)
#define OS_ISR_GTM_PSM2_2    (0xF0038AE8u)
#define OS_ISR_GTM_PSM2_3    (0xF0038AECu)
#define OS_ISR_GTM_PSM2_4    (0xF0038AF0u)
#define OS_ISR_GTM_PSM2_5    (0xF0038AF4u)
#define OS_ISR_GTM_PSM2_6    (0xF0038AF8u)
#define OS_ISR_GTM_PSM2_7    (0xF0038AFCu)
#define OS_ISR_GTM_DPLL0     (0xF0038B00u)
#define OS_ISR_GTM_DPLL1     (0xF0038B04u)
#define OS_ISR_GTM_DPLL2     (0xF0038B08u)
#define OS_ISR_GTM_DPLL3     (0xF0038B0Cu)
#define OS_ISR_GTM_DPLL4     (0xF0038B10u)
#define OS_ISR_GTM_DPLL5     (0xF0038B14u)
#define OS_ISR_GTM_DPLL6     (0xF0038B18u)
#define OS_ISR_GTM_DPLL7     (0xF0038B1Cu)
#define OS_ISR_GTM_DPLL8     (0xF0038B20u)
#define OS_ISR_GTM_DPLL9     (0xF0038B24u)
#define OS_ISR_GTM_DPLL10    (0xF0038B28u)
#define OS_ISR_GTM_DPLL11    (0xF0038B2Cu)
#define OS_ISR_GTM_DPLL12    (0xF0038B30u)
#define OS_ISR_GTM_DPLL13    (0xF0038B34u)
#define OS_ISR_GTM_DPLL14    (0xF0038B38u)
#define OS_ISR_GTM_DPLL15    (0xF0038B3Cu)
#define OS_ISR_GTM_DPLL16    (0xF0038B40u)
#define OS_ISR_GTM_DPLL17    (0xF0038B44u)
#define OS_ISR_GTM_DPLL18    (0xF0038B48u)
#define OS_ISR_GTM_DPLL19    (0xF0038B4Cu)
#define OS_ISR_GTM_DPLL20    (0xF0038B50u)
#define OS_ISR_GTM_DPLL21    (0xF0038B54u)
#define OS_ISR_GTM_DPLL22    (0xF0038B58u)
#define OS_ISR_GTM_DPLL23    (0xF0038B5Cu)
#define OS_ISR_GTM_DPLL24    (0xF0038B60u)
#define OS_ISR_GTM_DPLL25    (0xF0038B64u)
#define OS_ISR_GTM_DPLL26    (0xF0038B68u)
#define OS_ISR_GTM_ERR       (0xF0038B70u)
#define OS_ISR_GTM_TIM0_0    (0xF0038B90u)
#define OS_ISR_GTM_TIM0_1    (0xF0038B94u)
#define OS_ISR_GTM_TIM0_2    (0xF0038B98u)
#define OS_ISR_GTM_TIM0_3    (0xF0038B9Cu)
#define OS_ISR_GTM_TIM0_4    (0xF0038BA0u)
#define OS_ISR_GTM_TIM0_5    (0xF0038BA4u)
#define OS_ISR_GTM_TIM0_6    (0xF0038BA8u)
#define OS_ISR_GTM_TIM0_7    (0xF0038BACu)
#define OS_ISR_GTM_TIM1_0    (0xF0038BB0u)
#define OS_ISR_GTM_TIM1_1    (0xF0038BB4u)
#define OS_ISR_GTM_TIM1_2    (0xF0038BB8u)
#define OS_ISR_GTM_TIM1_3    (0xF0038BBCu)
#define OS_ISR_GTM_TIM1_4    (0xF0038BC0u)
#define OS_ISR_GTM_TIM1_5    (0xF0038BC4u)
#define OS_ISR_GTM_TIM1_6    (0xF0038BC8u)
#define OS_ISR_GTM_TIM1_7    (0xF0038BCCu)
#define OS_ISR_GTM_TIM2_0    (0xF0038BD0u)
#define OS_ISR_GTM_TIM2_1    (0xF0038BD4u)
#define OS_ISR_GTM_TIM2_2    (0xF0038BD8u)
#define OS_ISR_GTM_TIM2_3    (0xF0038BDCu)
#define OS_ISR_GTM_TIM2_4    (0xF0038BE0u)
#define OS_ISR_GTM_TIM2_5    (0xF0038BE4u)
#define OS_ISR_GTM_TIM2_6    (0xF0038BE8u)
#define OS_ISR_GTM_TIM2_7    (0xF0038BECu)
#define OS_ISR_GTM_TIM3_0    (0xF0038BF0u)
#define OS_ISR_GTM_TIM3_1    (0xF0038BF4u)
#define OS_ISR_GTM_TIM3_2    (0xF0038BF8u)
#define OS_ISR_GTM_TIM3_3    (0xF0038BFCu)
#define OS_ISR_GTM_TIM3_4    (0xF0038C00u)
#define OS_ISR_GTM_TIM3_5    (0xF0038C04u)
#define OS_ISR_GTM_TIM3_6    (0xF0038C08u)
#define OS_ISR_GTM_TIM3_7    (0xF0038C0Cu)
#define OS_ISR_GTM_TIM4_0    (0xF0038C10u)
#define OS_ISR_GTM_TIM4_1    (0xF0038C14u)
#define OS_ISR_GTM_TIM4_2    (0xF0038C18u)
#define OS_ISR_GTM_TIM4_3    (0xF0038C1Cu)
#define OS_ISR_GTM_TIM4_4    (0xF0038C20u)
#define OS_ISR_GTM_TIM4_5    (0xF0038C24u)
#define OS_ISR_GTM_TIM4_6    (0xF0038C28u)
#define OS_ISR_GTM_TIM4_7    (0xF0038C2Cu)
#define OS_ISR_GTM_TIM5_0    (0xF0038C30u)
#define OS_ISR_GTM_TIM5_1    (0xF0038C34u)
#define OS_ISR_GTM_TIM5_2    (0xF0038C38u)
#define OS_ISR_GTM_TIM5_3    (0xF0038C3Cu)
#define OS_ISR_GTM_TIM5_4    (0xF0038C40u)
#define OS_ISR_GTM_TIM5_5    (0xF0038C44u)
#define OS_ISR_GTM_TIM5_6    (0xF0038C48u)
#define OS_ISR_GTM_TIM5_7    (0xF0038C4Cu)
#define OS_ISR_GTM_TIM6_0    (0xF0038C50u)
#define OS_ISR_GTM_TIM6_1    (0xF0038C54u)
#define OS_ISR_GTM_TIM6_2    (0xF0038C58u)
#define OS_ISR_GTM_TIM6_3    (0xF0038C5Cu)
#define OS_ISR_GTM_TIM6_4    (0xF0038C60u)
#define OS_ISR_GTM_TIM6_5    (0xF0038C64u)
#define OS_ISR_GTM_TIM6_6    (0xF0038C68u)
#define OS_ISR_GTM_TIM6_7    (0xF0038C6Cu)
#define OS_ISR_GTM_TIM7_0    (0xF0038C70u)
#define OS_ISR_GTM_TIM7_1    (0xF0038C74u)
#define OS_ISR_GTM_TIM7_2    (0xF0038C78u)
#define OS_ISR_GTM_TIM7_3    (0xF0038C7Cu)
#define OS_ISR_GTM_TIM7_4    (0xF0038C80u)
#define OS_ISR_GTM_TIM7_5    (0xF0038C84u)
#define OS_ISR_GTM_TIM7_6    (0xF0038C88u)
#define OS_ISR_GTM_TIM7_7    (0xF0038C8Cu)
#define OS_ISR_GTM_MCS0_0    (0xF0038CB0u)
#define OS_ISR_GTM_MCS0_1    (0xF0038CB4u)
#define OS_ISR_GTM_MCS0_2    (0xF0038CB8u)
#define OS_ISR_GTM_MCS0_3    (0xF0038CBCu)
#define OS_ISR_GTM_MCS0_4    (0xF0038CC0u)
#define OS_ISR_GTM_MCS0_5    (0xF0038CC4u)
#define OS_ISR_GTM_MCS0_6    (0xF0038CC8u)
#define OS_ISR_GTM_MCS0_7    (0xF0038CCCu)
#define OS_ISR_GTM_MCS1_0    (0xF0038CD0u)
#define OS_ISR_GTM_MCS1_1    (0xF0038CD4u)
#define OS_ISR_GTM_MCS1_2    (0xF0038CD8u)
#define OS_ISR_GTM_MCS1_3    (0xF0038CDCu)
#define OS_ISR_GTM_MCS1_4    (0xF0038CE0u)
#define OS_ISR_GTM_MCS1_5    (0xF0038CE4u)
#define OS_ISR_GTM_MCS1_6    (0xF0038CE8u)
#define OS_ISR_GTM_MCS1_7    (0xF0038CECu)
#define OS_ISR_GTM_MCS2_0    (0xF0038CF0u)
#define OS_ISR_GTM_MCS2_1    (0xF0038CF4u)
#define OS_ISR_GTM_MCS2_2    (0xF0038CF8u)
#define OS_ISR_GTM_MCS2_3    (0xF0038CFCu)
#define OS_ISR_GTM_MCS2_4    (0xF0038D00u)
#define OS_ISR_GTM_MCS2_5    (0xF0038D04u)
#define OS_ISR_GTM_MCS2_6    (0xF0038D08u)
#define OS_ISR_GTM_MCS2_7    (0xF0038D0Cu)
#define OS_ISR_GTM_MCS3_0    (0xF0038D10u)
#define OS_ISR_GTM_MCS3_1    (0xF0038D14u)
#define OS_ISR_GTM_MCS3_2    (0xF0038D18u)
#define OS_ISR_GTM_MCS3_3    (0xF0038D1Cu)
#define OS_ISR_GTM_MCS3_4    (0xF0038D20u)
#define OS_ISR_GTM_MCS3_5    (0xF0038D24u)
#define OS_ISR_GTM_MCS3_6    (0xF0038D28u)
#define OS_ISR_GTM_MCS3_7    (0xF0038D2Cu)
#define OS_ISR_GTM_MCS4_0    (0xF0038D30u)
#define OS_ISR_GTM_MCS4_1    (0xF0038D34u)
#define OS_ISR_GTM_MCS4_2    (0xF0038D38u)
#define OS_ISR_GTM_MCS4_3    (0xF0038D3Cu)
#define OS_ISR_GTM_MCS4_4    (0xF0038D40u)
#define OS_ISR_GTM_MCS4_5    (0xF0038D44u)
#define OS_ISR_GTM_MCS4_6    (0xF0038D48u)
#define OS_ISR_GTM_MCS4_7    (0xF0038D4Cu)
#define OS_ISR_GTM_MCS5_0    (0xF0038D50u)
#define OS_ISR_GTM_MCS5_1    (0xF0038D54u)
#define OS_ISR_GTM_MCS5_2    (0xF0038D58u)
#define OS_ISR_GTM_MCS5_3    (0xF0038D5Cu)
#define OS_ISR_GTM_MCS5_4    (0xF0038D60u)
#define OS_ISR_GTM_MCS5_5    (0xF0038D64u)
#define OS_ISR_GTM_MCS5_6    (0xF0038D68u)
#define OS_ISR_GTM_MCS5_7    (0xF0038D6Cu)
#define OS_ISR_GTM_MCS6_0    (0xF0038D70u)
#define OS_ISR_GTM_MCS6_1    (0xF0038D74u)
#define OS_ISR_GTM_MCS6_2    (0xF0038D78u)
#define OS_ISR_GTM_MCS6_3    (0xF0038D7Cu)
#define OS_ISR_GTM_MCS6_4    (0xF0038D80u)
#define OS_ISR_GTM_MCS6_5    (0xF0038D84u)
#define OS_ISR_GTM_MCS6_6    (0xF0038D88u)
#define OS_ISR_GTM_MCS6_7    (0xF0038D8Cu)
#define OS_ISR_GTM_MCS7_0    (0xF0038D90u)
#define OS_ISR_GTM_MCS7_1    (0xF0038D94u)
#define OS_ISR_GTM_MCS7_2    (0xF0038D98u)
#define OS_ISR_GTM_MCS7_3    (0xF0038D9Cu)
#define OS_ISR_GTM_MCS7_4    (0xF0038DA0u)
#define OS_ISR_GTM_MCS7_5    (0xF0038DA4u)
#define OS_ISR_GTM_MCS7_6    (0xF0038DA8u)
#define OS_ISR_GTM_MCS7_7    (0xF0038DACu)
#define OS_ISR_GTM_MCS8_0    (0xF0038DB0u)
#define OS_ISR_GTM_MCS8_1    (0xF0038DB4u)
#define OS_ISR_GTM_MCS8_2    (0xF0038DB8u)
#define OS_ISR_GTM_MCS8_3    (0xF0038DBCu)
#define OS_ISR_GTM_MCS8_4    (0xF0038DC0u)
#define OS_ISR_GTM_MCS8_5    (0xF0038DC4u)
#define OS_ISR_GTM_MCS8_6    (0xF0038DC8u)
#define OS_ISR_GTM_MCS8_7    (0xF0038DCCu)
#define OS_ISR_GTM_MCS9_0    (0xF0038DD0u)
#define OS_ISR_GTM_MCS9_1    (0xF0038DD4u)
#define OS_ISR_GTM_MCS9_2    (0xF0038DD8u)
#define OS_ISR_GTM_MCS9_3    (0xF0038DDCu)
#define OS_ISR_GTM_MCS9_4    (0xF0038DE0u)
#define OS_ISR_GTM_MCS9_5    (0xF0038DE4u)
#define OS_ISR_GTM_MCS9_6    (0xF0038DE8u)
#define OS_ISR_GTM_MCS9_7    (0xF0038DECu)
#define OS_ISR_GTM_TOM0_0    (0xF0038E10u)
#define OS_ISR_GTM_TOM0_1    (0xF0038E14u)
#define OS_ISR_GTM_TOM0_2    (0xF0038E18u)
#define OS_ISR_GTM_TOM0_3    (0xF0038E1Cu)
#define OS_ISR_GTM_TOM0_4    (0xF0038E20u)
#define OS_ISR_GTM_TOM0_5    (0xF0038E24u)
#define OS_ISR_GTM_TOM0_6    (0xF0038E28u)
#define OS_ISR_GTM_TOM0_7    (0xF0038E2Cu)
#define OS_ISR_GTM_TOM1_0    (0xF0038E30u)
#define OS_ISR_GTM_TOM1_1    (0xF0038E34u)
#define OS_ISR_GTM_TOM1_2    (0xF0038E38u)
#define OS_ISR_GTM_TOM1_3    (0xF0038E3Cu)
#define OS_ISR_GTM_TOM1_4    (0xF0038E40u)
#define OS_ISR_GTM_TOM1_5    (0xF0038E44u)
#define OS_ISR_GTM_TOM1_6    (0xF0038E48u)
#define OS_ISR_GTM_TOM1_7    (0xF0038E4Cu)
#define OS_ISR_GTM_TOM2_0    (0xF0038E50u)
#define OS_ISR_GTM_TOM2_1    (0xF0038E54u)
#define OS_ISR_GTM_TOM2_2    (0xF0038E58u)
#define OS_ISR_GTM_TOM2_3    (0xF0038E5Cu)
#define OS_ISR_GTM_TOM2_4    (0xF0038E60u)
#define OS_ISR_GTM_TOM2_5    (0xF0038E64u)
#define OS_ISR_GTM_TOM2_6    (0xF0038E68u)
#define OS_ISR_GTM_TOM2_7    (0xF0038E6Cu)
#define OS_ISR_GTM_TOM3_0    (0xF0038E70u)
#define OS_ISR_GTM_TOM3_1    (0xF0038E74u)
#define OS_ISR_GTM_TOM3_2    (0xF0038E78u)
#define OS_ISR_GTM_TOM3_3    (0xF0038E7Cu)
#define OS_ISR_GTM_TOM3_4    (0xF0038E80u)
#define OS_ISR_GTM_TOM3_5    (0xF0038E84u)
#define OS_ISR_GTM_TOM3_6    (0xF0038E88u)
#define OS_ISR_GTM_TOM3_7    (0xF0038E8Cu)
#define OS_ISR_GTM_TOM4_0    (0xF0038E90u)
#define OS_ISR_GTM_TOM4_1    (0xF0038E94u)
#define OS_ISR_GTM_TOM4_2    (0xF0038E98u)
#define OS_ISR_GTM_TOM4_3    (0xF0038E9Cu)
#define OS_ISR_GTM_TOM4_4    (0xF0038EA0u)
#define OS_ISR_GTM_TOM4_5    (0xF0038EA4u)
#define OS_ISR_GTM_TOM4_6    (0xF0038EA8u)
#define OS_ISR_GTM_TOM4_7    (0xF0038EACu)
#define OS_ISR_GTM_TOM5_0    (0xF0038EB0u)
#define OS_ISR_GTM_TOM5_1    (0xF0038EB4u)
#define OS_ISR_GTM_TOM5_2    (0xF0038EB8u)
#define OS_ISR_GTM_TOM5_3    (0xF0038EBCu)
#define OS_ISR_GTM_TOM5_4    (0xF0038EC0u)
#define OS_ISR_GTM_TOM5_5    (0xF0038EC4u)
#define OS_ISR_GTM_TOM5_6    (0xF0038EC8u)
#define OS_ISR_GTM_TOM5_7    (0xF0038ECCu)
#define OS_ISR_GTM_ATOM0_0   (0xF0038EF0u)
#define OS_ISR_GTM_ATOM0_1   (0xF0038EF4u)
#define OS_ISR_GTM_ATOM0_2   (0xF0038EF8u)
#define OS_ISR_GTM_ATOM0_3   (0xF0038EFCu)
#define OS_ISR_GTM_ATOM1_0   (0xF0038F00u)
#define OS_ISR_GTM_ATOM1_1   (0xF0038F04u)
#define OS_ISR_GTM_ATOM1_2   (0xF0038F08u)
#define OS_ISR_GTM_ATOM1_3   (0xF0038F0Cu)
#define OS_ISR_GTM_ATOM2_0   (0xF0038F10u)
#define OS_ISR_GTM_ATOM2_1   (0xF0038F14u)
#define OS_ISR_GTM_ATOM2_2   (0xF0038F18u)
#define OS_ISR_GTM_ATOM2_3   (0xF0038F1Cu)
#define OS_ISR_GTM_ATOM3_0   (0xF0038F20u)
#define OS_ISR_GTM_ATOM3_1   (0xF0038F24u)
#define OS_ISR_GTM_ATOM3_2   (0xF0038F28u)
#define OS_ISR_GTM_ATOM3_3   (0xF0038F2Cu)
#define OS_ISR_GTM_ATOM4_0   (0xF0038F30u)
#define OS_ISR_GTM_ATOM4_1   (0xF0038F34u)
#define OS_ISR_GTM_ATOM4_2   (0xF0038F38u)
#define OS_ISR_GTM_ATOM4_3   (0xF0038F3Cu)
#define OS_ISR_GTM_ATOM5_0   (0xF0038F40u)
#define OS_ISR_GTM_ATOM5_1   (0xF0038F44u)
#define OS_ISR_GTM_ATOM5_2   (0xF0038F48u)
#define OS_ISR_GTM_ATOM5_3   (0xF0038F4Cu)
#define OS_ISR_GTM_ATOM6_0   (0xF0038F50u)
#define OS_ISR_GTM_ATOM6_1   (0xF0038F54u)
#define OS_ISR_GTM_ATOM6_2   (0xF0038F58u)
#define OS_ISR_GTM_ATOM6_3   (0xF0038F5Cu)
#define OS_ISR_GTM_ATOM7_0   (0xF0038F60u)
#define OS_ISR_GTM_ATOM7_1   (0xF0038F64u)
#define OS_ISR_GTM_ATOM7_2   (0xF0038F68u)
#define OS_ISR_GTM_ATOM7_3   (0xF0038F6Cu)
#define OS_ISR_GTM_ATOM8_0   (0xF0038F70u)
#define OS_ISR_GTM_ATOM8_1   (0xF0038F74u)
#define OS_ISR_GTM_ATOM8_2   (0xF0038F78u)
#define OS_ISR_GTM_ATOM8_3   (0xF0038F7Cu)
#define OS_ISR_GTM_ATOM9_0   (0xF0038F80u)
#define OS_ISR_GTM_ATOM9_1   (0xF0038F84u)
#define OS_ISR_GTM_ATOM9_2   (0xF0038F88u)
#define OS_ISR_GTM_ATOM9_3   (0xF0038F8Cu)
#define OS_ISR_GTM_ATOM10_0  (0xF0038F90u)
#define OS_ISR_GTM_ATOM10_1  (0xF0038F94u)
#define OS_ISR_GTM_ATOM10_2  (0xF0038F98u)
#define OS_ISR_GTM_ATOM10_3  (0xF0038F9Cu)
#define OS_ISR_GTM_ATOM11_0  (0xF0038FA0u)
#define OS_ISR_GTM_ATOM11_1  (0xF0038FA4u)
#define OS_ISR_GTM_ATOM11_2  (0xF0038FA8u)
#define OS_ISR_GTM_ATOM11_3  (0xF0038FACu)
#define OS_ISR_GTM_MCSW0     (0xF0038FD0u)
#define OS_ISR_GTM_MCSW1     (0xF0038FD4u)
#define OS_ISR_GTM_MCSW2     (0xF0038FD8u)
#define OS_ISR_GTM_MCSW3     (0xF0038FDCu)
#define OS_ISR_GTM_MCSW4     (0xF0038FE0u)
#define OS_ISR_GTM_MCSW5     (0xF0038FE4u)
#define OS_ISR_GTM_MCSW6     (0xF0038FE8u)
#define OS_ISR_GTM_MCSW7     (0xF0038FECu)
#define OS_ISR_GTM_MCSW8     (0xF0038FF0u)
#define OS_ISR_GTM_MCSW9     (0xF0038FF4u)

/**
 * control the specific interrupt source, based on the service request control register
 */
/* Install interrupt on tc275. If set prio to 0, this interrupt is disabled. */
#define OS_INTERRUPT_INSTALL(src, prio, srcType) \
    (*(uint32 volatile*)(src)) = (((*(uint32 volatile*)(src)) & 0xFFFFFF00u) | 0x00000400u | (srcType)) + (prio)

/* Clear the bit10 SRE(source request enable) */
#define OS_INTERRUPT_DISABLEREQ(src) (*((uint32 volatile*)src)) &= ~((uint32)1 << OS_SRC_SRE_BIT);

/* Set the bit23 SRE(source request enable) */
#define OS_INTERRUPT_ENABLEREQ(src)                                  \
    {                                                                \
        (*((uint32 volatile*)src)) |= ((uint32)1 << OS_SRC_SRE_BIT); \
    }

/* Write 1 to the bit25 CLRR(clear request) */
#define OS_INTERRUPT_CLEARREQ(src) (*(uint32 volatile*)(src)) = ((*(uint32 volatile*)(src)) | 0x02000000u)

/* Check the bit24 SRR(service request flag) */

#define OS_INTERRUPT_CHKREQ(src) (((*(uint32 volatile*)(src)) >> 24u) & 0x01u)

/* Set SRPN to 0 in src */
#define OS_INTERRUPT_SET_SRPN_ZERO(src) (*(uint32 volatile*)(src)) = ((*(uint32 volatile*)(src)) & 0xFFFFFF00u)

#define OS_INTERRUPT_SETREQ(src)        (*(uint32 volatile*)src) = ((*(uint32 volatile*)src) | 0x04000000u)

/* Get the value of the ISRid's status register */
#define OS_GETISRSTR(isrId) OS_INTERRUPT_CHKREQ(Os_IsrCfg[Os_GetObjLocalId(isrId)].OsIsrSrc)

/* Clear the status register of the ISR id */
#define OS_CLRISRSTR(isrId) OS_INTERRUPT_CLEARREQ(Os_IsrCfg[Os_GetObjLocalId(isrId)].OsIsrSrc)

/* cpu0 = bit0 set to 1, ..., DMA = bit3 set to 1 */
/* Request an interrupt service to the specified core. */
#define OS_INTERRUPT_SRB_SETREQ(core) INT_SRB0 = ((uint32)1u << (uint32)(core))

/* Request an interrupt service to the specified core. */
#define Os_ArchRemoteCall(coreId) /* PRQA S 3472 */ /* MISRA  Dir-4.9 */ /* ARCH_DEFINE_MACRO_001 */ \
    {                                                                                                \
        OS_INTERRUPT_SRB_SETREQ(coreId);                                                             \
    }

#define OS_INTERRUPT_TRIGGER(isrId) OS_INTERRUPT_SETREQ(Os_IsrCfg[Os_GetObjLocalId(isrId)].OsIsrSrc)
#define OS_INTERRUPT_CLEAR(isrId)   OS_INTERRUPT_CLEARREQ(Os_IsrCfg[Os_GetObjLocalId(isrId)].OsIsrSrc)

/*=======[T Y P E   D E F I N I T I O N S]==================================*/
/* Type of the interrupt service routine (ISR). */
typedef void (*Os_isrhnd)(void);
typedef struct
{
    uint8 IntPrio;
    Os_isrhnd ISR;
} Os_IntInstallType;
/*=======[E X T E R N A L   D A T A]========================================*/

/*=======[E X T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/

/******************************************************************************/
/*
 * Brief                <Installing an Interrupt.>
 *
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_InterruptInstall(const Os_IsrCfgType* IsrCfg, const Os_IntInstallType* IntInstall);

/******************************************************************************/
/*
 * Brief                <TryToGetSpinlock has the same functionality as GetSpinlock with the difference
                         that if the spinlock is already occupied by a TASK on a different core the function
                         sets the OUT parameter "Success" and returns with E_OK>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <None>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <TerminateApplication>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchDisableIntInApp(P2CONST(Os_ApplicationCfgType, AUTOMATIC, OS_VAR) posCurAppCfg);

/******************************************************************************/
/*
 * Brief                <Enable the given interrupt source vIsrSrc.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_EnableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
/******************************************************************************/
/*
 * Brief                <Disable the given interrupt source vIsrSrc.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_DisableInterruptSource(uint32 vIsrSrc, uint32 vIsrSrcType);
/******************************************************************************/
/*
 * Brief                <Clear the pending status of the given interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ClearPendingInterrupt(uint32 vIsrSrc, uint32 vIsrSrcType);
/******************************************************************************/
/*
 * Brief                <Get the status of the given interrupt.>
 * Service ID   :       <None>
 * Sync/Async   :       <Synchronous>
 * Reentrancy           <Reentrant>
 * param[in]            <srcAddr,prio,srcType,isrProc>
 * param[out]           <None>
 * param[in/out]        <None>
 * return               <None>
 * CallByAPI            <None>
 * REQ ID               <>
 */
/******************************************************************************/
extern FUNC(Os_IsrStateType, OS_CODE) Os_GetIsrSourceState(uint32 vIsrSrc, uint32 vIsrSrcType);

/* Timing protection. */
#if (TRUE == CFG_TIMING_PROTECTION_ENABLE)
/******************************************************************************/
/*
 * Brief                <Disable all interrupts except for those related to time protection.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_DisableAllInterrupts,Os_SuspendAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchDisableAllInt_ButTimingProtInt(void);
/******************************************************************************/
/*
 * Brief                <Enable all interrupts except for the time protection interrupt.>
 * ServiceId            <None>
 * Sync/Async           <Synchronous>
 * Reentrancy           <Non Reentrant>
 * Param-Name[in]       <None>
 * Param-Name[out]      <None>
 * Param-Name[in/out]   <None>
 * Return               <None>
 * PreCondition         <None>
 * CallByAPI            <Os_EnableAllInterrupts,Os_ResumeAllInterrupts>
 * REQ ID               <None>
 */
/******************************************************************************/
extern FUNC(void, OS_CODE) Os_ArchEnableAllInt_ButTimingProtInt(void);
#endif /* TRUE == CFG_TIMING_PROTECTION_ENABLE */

/*=======[I N T E R N A L   D A T A]========================================*/

/*=======[I N T E R N A L   F U N C T I O N   D E C L A R A T I O N S]======*/
#endif /* MCU_IRQ_H */
/*=======[E N D   O F   F I L E]==============================================*/
