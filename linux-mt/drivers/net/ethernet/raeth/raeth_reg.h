/* Copyright  2016 MediaTek Inc.
 * Author: Nelson Chang <nelson.chang@mediatek.com>
 * Author: Carlos Huang <carlos.huang@mediatek.com>
 * Author: Harry Huang <harry.huang@mediatek.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
#ifndef RAETH_REG_H
#define RAETH_REG_H

#include <linux/mii.h>		/* for struct mii_if_info in ra2882ethreg.h */
#include <linux/version.h>	/* check linux version */
#include <linux/interrupt.h>	/* for "struct tasklet_struct" */
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/workqueue.h>
#include <linux/inet_lro.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>

#include "raether.h"

#define MAX_PACKET_SIZE	1514
#define	MIN_PACKET_SIZE 60
#if defined(CONFIG_MACH_MT7623)
#define MAX_PTXD_LEN 0x3fff	/* 16k */
#define MAX_QTXD_LEN 0x3fff	/* 16k */
#else
#define MAX_PTXD_LEN 0x3fff	/* 16k */
#define MAX_QTXD_LEN 0xffff
#endif

#define phys_to_bus(a) (a)

extern void __iomem *ethdma_sysctl_base;
extern void __iomem *ethdma_frame_engine_base;

/* bits range: for example BITS(16,23) = 0xFF0000
 *   ==>  (BIT(m)-1)   = 0x0000FFFF     ~(BIT(m)-1)   => 0xFFFF0000
 *   ==>  (BIT(n+1)-1) = 0x00FFFFFF
 */
#define BITS(m, n)   (~(BIT(m) - 1) & ((BIT(n) - 1) | BIT(n)))

#define ETHER_ADDR_LEN  6

/*  Phy Vender ID list */

#define EV_ICPLUS_PHY_ID0 0x0243
#define EV_ICPLUS_PHY_ID1 0x0D90
#define EV_MARVELL_PHY_ID0 0x0141
#define EV_MARVELL_PHY_ID1 0x0CC2
#define EV_VTSS_PHY_ID0 0x0007
#define EV_VTSS_PHY_ID1 0x0421

/* ETHDMASYS base address
 * for I2S/PCM/GDMA/HSDMA/FE/GMAC
 */
#define ETHDMASYS_BASE			ethdma_sysctl_base
#define ETHDMASYS_FRAME_ENGINE_BASE	ethdma_frame_engine_base

#define ETHDMASYS_SYSCTL_BASE            ETHDMASYS_BASE
#define ETHDMASYS_PPE_BASE		(ETHDMASYS_FRAME_ENGINE_BASE + 0x0C00)
#define ETHDMASYS_ETH_MAC_BASE		(ETHDMASYS_FRAME_ENGINE_BASE + 0x10000)
#define ETHDMASYS_ETH_SW_BASE		(ETHDMASYS_FRAME_ENGINE_BASE + 0x18000)

#define RALINK_FRAME_ENGINE_BASE	ETHDMASYS_FRAME_ENGINE_BASE
#define RALINK_PPE_BASE                 ETHDMASYS_PPE_BASE
#define RALINK_SYSCTL_BASE		ETHDMASYS_SYSCTL_BASE
#define RALINK_ETH_MAC_BASE		ETHDMASYS_ETH_MAC_BASE
#define RALINK_ETH_SW_BASE		ETHDMASYS_ETH_SW_BASE

#define RSTCTL_FE_RST			BIT(6)
#define RALINK_FE_RST			RSTCTL_FE_RST

/* FE_INT_STATUS */
#define RX_COHERENT      BIT(31)
#define RX_DLY_INT       BIT(30)
#define TX_COHERENT      BIT(29)
#define TX_DLY_INT       BIT(28)
#define RING3_RX_DLY_INT    BIT(27)
#define RING2_RX_DLY_INT    BIT(26)
#define RING1_RX_DLY_INT    BIT(25)

#define RXD_ERROR	 BIT(24)
#define ALT_RPLC_INT3    BIT(23)
#define ALT_RPLC_INT2    BIT(22)
#define ALT_RPLC_INT1    BIT(21)

#define RX_DONE_INT3     BIT(19)
#define RX_DONE_INT2     BIT(18)
#define RX_DONE_INT1     BIT(17)
#define RX_DONE_INT0     BIT(16)

#define TX_DONE_INT3     BIT(3)
#define TX_DONE_INT2     BIT(2)
#define TX_DONE_INT1     BIT(1)
#define TX_DONE_INT0     BIT(0)

#define RLS_COHERENT     BIT(29)
#define RLS_DLY_INT      BIT(28)
#define RLS_DONE_INT     BIT(0)

#define FE_INT_ALL		(TX_DONE_INT3 | TX_DONE_INT2 | \
				 TX_DONE_INT1 | TX_DONE_INT0 | \
				 RX_DONE_INT0 | RX_DONE_INT1 | \
				 RX_DONE_INT2 | RX_DONE_INT3)

#define QFE_INT_ALL		(RLS_DONE_INT | RX_DONE_INT0 | \
				 RX_DONE_INT1 | RX_DONE_INT2 | RX_DONE_INT3)
#define QFE_INT_DLY_INIT	(RLS_DLY_INT | RX_DLY_INT)

#define NUM_QDMA_PAGE	    512
#define QDMA_PAGE_SIZE      2048

/* SW_INT_STATUS */
#define ESW_PHY_POLLING		(RALINK_ETH_MAC_BASE + 0x0000)

#define P5_LINK_CH		BIT(5)
#define P4_LINK_CH		BIT(4)
#define P3_LINK_CH		BIT(3)
#define P2_LINK_CH		BIT(2)
#define P1_LINK_CH		BIT(1)
#define P0_LINK_CH		BIT(0)

#define RX_BUF_ALLOC_SIZE	2000
#define FASTPATH_HEADROOM	64

#define ETHER_BUFFER_ALIGN	32	/* /// Align on a cache line */

#define ETHER_ALIGNED_RX_SKB_ADDR(addr) \
	((((unsigned long)(addr) + ETHER_BUFFER_ALIGN - 1) & \
	~(ETHER_BUFFER_ALIGN - 1)) - (unsigned long)(addr))

struct PSEUDO_ADAPTER {
	struct net_device *raeth_dev;
	struct net_device *pseudo_dev;
	struct net_device_stats stat;
	struct mii_if_info mii_info;
};

#define MAX_PSEUDO_ENTRY               1

/* Register Categories Definition */
#define RAFRAMEENGINE_OFFSET	0x0000
#define RAGDMA_OFFSET		0x0020
#define RAPSE_OFFSET		0x0040
#define RAGDMA2_OFFSET		0x0060
#define RACDMA_OFFSET		0x0080
#define RAPDMA_OFFSET		0x0800
#define SDM_OFFSET		0x0C00
#define RAPPE_OFFSET		0x0200
#define RACMTABLE_OFFSET	0x0400
#define RAPOLICYTABLE_OFFSET	0x1000

/* Register Map Detail */
/* RT3883 */
#define SYSCFG1			(RALINK_SYSCTL_BASE + 0x14)

/* Old FE with New PDMA */
#define PDMA_RELATED            0x0800
/* 1. PDMA */
#define TX_BASE_PTR0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x000)
#define TX_MAX_CNT0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x004)
#define TX_CTX_IDX0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x008)
#define TX_DTX_IDX0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x00C)

#define TX_BASE_PTR1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x010)
#define TX_MAX_CNT1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x014)
#define TX_CTX_IDX1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x018)
#define TX_DTX_IDX1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x01C)

#define TX_BASE_PTR2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x020)
#define TX_MAX_CNT2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x024)
#define TX_CTX_IDX2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x028)
#define TX_DTX_IDX2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x02C)

#define TX_BASE_PTR3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x030)
#define TX_MAX_CNT3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x034)
#define TX_CTX_IDX3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x038)
#define TX_DTX_IDX3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x03C)

#define RX_BASE_PTR0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x100)
#define RX_MAX_CNT0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x104)
#define RX_CALC_IDX0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x108)
#define RX_DRX_IDX0	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x10C)

#define RX_BASE_PTR1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x110)
#define RX_MAX_CNT1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x114)
#define RX_CALC_IDX1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x118)
#define RX_DRX_IDX1	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x11C)

#define RX_BASE_PTR2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x120)
#define RX_MAX_CNT2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x124)
#define RX_CALC_IDX2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x128)
#define RX_DRX_IDX2	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x12C)

#define RX_BASE_PTR3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x130)
#define RX_MAX_CNT3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x134)
#define RX_CALC_IDX3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x138)
#define RX_DRX_IDX3	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x13C)

#define PDMA_INFO	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x200)
#define PDMA_GLO_CFG	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x204)
#define PDMA_RST_IDX	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x208)
#define PDMA_RST_CFG	(PDMA_RST_IDX)
#define DLY_INT_CFG	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x20C)
#define FREEQ_THRES	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x210)
#define INT_STATUS	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x220)
#define FE_INT_STATUS	(INT_STATUS)
#define INT_MASK	(RALINK_FRAME_ENGINE_BASE + PDMA_RELATED + 0x228)
#define FE_INT_ENABLE	(INT_MASK)
#define SCH_Q01_CFG	(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET + 0x280)
#define SCH_Q23_CFG	(RALINK_FRAME_ENGINE_BASE + RAPDMA_OFFSET + 0x284)

#define FE_GLO_CFG		(RALINK_FRAME_ENGINE_BASE + 0x00)
#define FE_RST_GL		(RALINK_FRAME_ENGINE_BASE + 0x04)
#define FE_INT_STATUS2		(RALINK_FRAME_ENGINE_BASE + 0x08)
#define FE_INT_ENABLE2		(RALINK_FRAME_ENGINE_BASE + 0x0c)
/* #define FC_DROP_STA         RALINK_FRAME_ENGINE_BASE + 0x18 */
#define FOE_TS_T		(RALINK_FRAME_ENGINE_BASE + 0x10)

#define FE_GLO_MISC		(RALINK_FRAME_ENGINE_BASE + 0x124)

#define GDMA1_RELATED       0x0500
#define GDMA1_FWD_CFG       (RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x00)
#define GDMA1_SHPR_CFG      (RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x04)
#define GDMA1_MAC_ADRL      (RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x08)
#define GDMA1_MAC_ADRH      (RALINK_FRAME_ENGINE_BASE + GDMA1_RELATED + 0x0C)

#define GDMA2_RELATED       0x1500
#define GDMA2_FWD_CFG       (RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x00)
#define GDMA2_SHPR_CFG      (RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x04)
#define GDMA2_MAC_ADRL      (RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x08)
#define GDMA2_MAC_ADRH      (RALINK_FRAME_ENGINE_BASE + GDMA2_RELATED + 0x0C)

#define GDMA1_SCH_CFG       GDMA1_SHPR_CFG
#define GDMA2_SCH_CFG       GDMA2_SHPR_CFG

#define PSE_RELATED         0x0040
#define PSE_FQ_CFG          (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x00)
#define CDMA_FC_CFG         (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x04)
#define GDMA1_FC_CFG        (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x08)
#define GDMA2_FC_CFG        (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x0C)
#define CDMA_OQ_STA         (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x10)
#define GDMA1_OQ_STA        (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x14)
#define GDMA2_OQ_STA        (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x18)
#define PSE_IQ_STA          (RALINK_FRAME_ENGINE_BASE + PSE_RELATED + 0x1C)

#define CDMA_RELATED        0x0400
#define CDMA_CSG_CFG        (RALINK_FRAME_ENGINE_BASE + CDMA_RELATED + 0x00)
#define CDMP_IG_CTRL        (RALINK_FRAME_ENGINE_BASE + CDMA_RELATED + 0x00)
#define CDMP_EG_CTRL        (RALINK_FRAME_ENGINE_BASE + CDMA_RELATED + 0x04)

#define PDMA_FC_CFG	    (RALINK_FRAME_ENGINE_BASE + 0x100)

/*kurtis: add QDMA define*/
#define CLK_CFG_0		(RALINK_SYSCTL_BASE + 0x2C)
#define PAD_RGMII2_MDIO_CFG     (RALINK_SYSCTL_BASE + 0x58)

#define QDMA_RELATED            0x1800
#define  QTX_CFG_0          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x000)
#define  QTX_SCH_0          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x004)
#define  QTX_HEAD_0         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x008)
#define  QTX_TAIL_0         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x00C)
#define  QTX_CFG_1          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x010)
#define  QTX_SCH_1          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x014)
#define  QTX_HEAD_1         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x018)
#define  QTX_TAIL_1         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x01C)
#define  QTX_CFG_2          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x020)
#define  QTX_SCH_2          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x024)
#define  QTX_HEAD_2         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x028)
#define  QTX_TAIL_2         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x02C)
#define  QTX_CFG_3          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x030)
#define  QTX_SCH_3          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x034)
#define  QTX_HEAD_3         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x038)
#define  QTX_TAIL_3         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x03C)
#define  QTX_CFG_4          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x040)
#define  QTX_SCH_4          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x044)
#define  QTX_HEAD_4         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x048)
#define  QTX_TAIL_4         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x04C)
#define  QTX_CFG_5          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x050)
#define  QTX_SCH_5          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x054)
#define  QTX_HEAD_5         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x058)
#define  QTX_TAIL_5         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x05C)
#define  QTX_CFG_6          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x060)
#define  QTX_SCH_6          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x064)
#define  QTX_HEAD_6         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x068)
#define  QTX_TAIL_6         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x06C)
#define  QTX_CFG_7          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x070)
#define  QTX_SCH_7          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x074)
#define  QTX_HEAD_7         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x078)
#define  QTX_TAIL_7         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x07C)
#define  QTX_CFG_8          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x080)
#define  QTX_SCH_8          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x084)
#define  QTX_HEAD_8         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x088)
#define  QTX_TAIL_8         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x08C)
#define  QTX_CFG_9          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x090)
#define  QTX_SCH_9          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x094)
#define  QTX_HEAD_9         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x098)
#define  QTX_TAIL_9         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x09C)
#define  QTX_CFG_10         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0A0)
#define  QTX_SCH_10         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0A4)
#define  QTX_HEAD_10        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0A8)
#define  QTX_TAIL_10        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0AC)
#define  QTX_CFG_11         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0B0)
#define  QTX_SCH_11         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0B4)
#define  QTX_HEAD_11        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0B8)
#define  QTX_TAIL_11        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0BC)
#define  QTX_CFG_12         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0C0)
#define  QTX_SCH_12         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0C4)
#define  QTX_HEAD_12        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0C8)
#define  QTX_TAIL_12        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0CC)
#define  QTX_CFG_13         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0D0)
#define  QTX_SCH_13         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0D4)
#define  QTX_HEAD_13        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0D8)
#define  QTX_TAIL_13        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0DC)
#define  QTX_CFG_14         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0E0)
#define  QTX_SCH_14         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0E4)
#define  QTX_HEAD_14        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0E8)
#define  QTX_TAIL_14        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0EC)
#define  QTX_CFG_15         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0F0)
#define  QTX_SCH_15         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0F4)
#define  QTX_HEAD_15        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0F8)
#define  QTX_TAIL_15        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x0FC)
#define  QRX_BASE_PTR_0     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x100)
#define  QRX_MAX_CNT_0      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x104)
#define  QRX_CRX_IDX_0      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x108)
#define  QRX_DRX_IDX_0      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x10C)
#define  QRX_BASE_PTR_1     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x110)
#define  QRX_MAX_CNT_1      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x114)
#define  QRX_CRX_IDX_1      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x118)
#define  QRX_DRX_IDX_1      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x11C)

#define  VQTX_TB_BASE_0     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x180)
#define  VQTX_TB_BASE_1     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x184)
#define  VQTX_TB_BASE_2     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x188)
#define  VQTX_TB_BASE_3     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x18C)

#define  QDMA_INFO          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x200)
#define  QDMA_GLO_CFG       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x204)
#define  QDMA_RST_IDX       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x208)
#define  QDMA_RST_CFG       (QDMA_RST_IDX)
#define  QDMA_DELAY_INT     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x20C)
#define  QDMA_FC_THRES      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x210)
#define  QDMA_TX_SCH        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x214)
#define  QDMA_INT_STS       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x218)
#define  QFE_INT_STATUS		  (QDMA_INT_STS)
#define  QDMA_INT_MASK      (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x21C)
#define  QFE_INT_ENABLE		  (QDMA_INT_MASK)
#define  QDMA_TRTCM         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x220)
#define  QDMA_DATA0         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x224)
#define  QDMA_DATA1         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x228)
#define  QDMA_RED_THRES     (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x22C)
#define  QDMA_TEST          (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x230)
#define  QDMA_DMA           (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x234)
#define  QDMA_BMU           (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x238)
#define  QDMA_HRED1         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x240)
#define  QDMA_HRED2         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x244)
#define  QDMA_SRED1         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x248)
#define  QDMA_SRED2         (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x24C)
#define  QTX_CTX_PTR        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x300)
#define  QTX_DTX_PTR        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x304)
#define  QTX_FWD_CNT        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x308)
#define  QTX_CRX_PTR        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x310)
#define  QTX_DRX_PTR        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x314)
#define  QTX_RLS_CNT        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x318)
#define  QDMA_FQ_HEAD       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x320)
#define  QDMA_FQ_TAIL       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x324)
#define  QDMA_FQ_CNT        (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x328)
#define  QDMA_FQ_BLEN       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x32C)
#define  QTX_Q0MIN_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x350)
#define  QTX_Q1MIN_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x354)
#define  QTX_Q2MIN_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x358)
#define  QTX_Q3MIN_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x35C)
#define  QTX_Q0MAX_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x360)
#define  QTX_Q1MAX_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x364)
#define  QTX_Q2MAX_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x368)
#define  QTX_Q3MAX_BK       (RALINK_FRAME_ENGINE_BASE + QDMA_RELATED + 0x36C)

#define DELAY_INT_INIT		0x84048404
#define FE_INT_DLY_INIT		(TX_DLY_INT | RX_DLY_INT)

/* 6. Counter and Meter Table */
/* PPE Accounting Group 0 Byte Cnt */
#define PPE_AC_BCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x000)
/* PPE Accounting Group 0 Packet Cnt */
#define PPE_AC_PCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x004)
/* 0 ~ 63 */
#define PPE_MTR_CNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x200)
/* skip... */
#define PPE_MTR_CNT63	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x2FC)

/* Transmit good byte cnt for GEport */
#define GDMA_TX_GBCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x300)
/* Transmit good pkt cnt for GEport */
#define GDMA_TX_GPCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x304)
/* Transmit skip cnt for GEport */
#define GDMA_TX_SKIPCNT0 (RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x308)
/* Transmit collision cnt for GEport */
#define GDMA_TX_COLCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x30C)

/* update these address mapping to fit data sheet v0.26,
 * by bobtseng, 2007.6.14
 */
#define GDMA_RX_GBCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x320)
#define GDMA_RX_GPCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x324)
#define GDMA_RX_OERCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x328)
#define GDMA_RX_FERCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x32C)
#define GDMA_RX_SERCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x330)
#define GDMA_RX_LERCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x334)
#define GDMA_RX_CERCNT0	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x338)
#define GDMA_RX_FCCNT1	(RALINK_FRAME_ENGINE_BASE + RACMTABLE_OFFSET + 0x33C)

/* LRO global control */
/* Bits [15:0]:LRO_ALT_RFSH_TIMER, Bits [20:16]:LRO_ALT_TICK_TIMER */
#define LRO_ALT_REFRESH_TIMER   (RALINK_FRAME_ENGINE_BASE + 0x001C)

/* LRO auto-learn table info */
#define PDMA_FE_ALT_CF8		(RALINK_FRAME_ENGINE_BASE + 0x0300)
#define PDMA_FE_ALT_SGL_CFC	(RALINK_FRAME_ENGINE_BASE + 0x0304)
#define PDMA_FE_ALT_SEQ_CFC	(RALINK_FRAME_ENGINE_BASE + 0x0308)

/* LRO controls */
#define ADMA_LRO_CTRL_OFFSET    0x0980
/*Bit [0]:LRO_EN, Bit [1]:LRO_IPv6_EN, Bit [2]:MULTIPLE_NON_LRO_RX_RING_EN,
 * Bit [3]:MULTIPLE_RXD_PREFETCH_EN, Bit [4]:RXD_PREFETCH_EN,
 * Bit [5]:LRO_DLY_INT_EN, Bit [6]:LRO_CRSN_BNW, Bit [7]:L3_CKS_UPD_EN,
 * Bit [20]:first_ineligible_pkt_redirect_en, Bit [21]:cr_lro_alt_score_mode,
 * Bit [22]:cr_lro_alt_rplc_mode, Bit [23]:cr_lro_l4_ctrl_psh_en,
 * Bits [28:26]:LRO_RING_RELINGUISH_REQ, Bits [31:29]:LRO_RING_RELINGUISH_DONE
 */
#define ADMA_LRO_CTRL_DW0 (RALINK_FRAME_ENGINE_BASE + \
			   ADMA_LRO_CTRL_OFFSET + 0x00)
/* Bits [31:0]:LRO_CPU_REASON */
#define ADMA_LRO_CTRL_DW1 (RALINK_FRAME_ENGINE_BASE + \
			   ADMA_LRO_CTRL_OFFSET + 0x04)
/* Bits [31:0]:AUTO_LEARN_LRO_ELIGIBLE_THRESHOLD */
#define ADMA_LRO_CTRL_DW2 (RALINK_FRAME_ENGINE_BASE + \
			   ADMA_LRO_CTRL_OFFSET + 0x08)
/*Bits [7:0]:LRO_MAX_AGGREGATED_CNT,
 * Bits [11:8]:LRO_VLAN_EN, Bits [13:12]:LRO_VLAN_VID_CMP_DEPTH,
 * Bit [14]:ADMA_FW_RSTN_REQ, Bit [15]:ADMA_MODE, Bits [31:16]:LRO_MIN_RXD_SDL0
 */
#define ADMA_LRO_CTRL_DW3 (RALINK_FRAME_ENGINE_BASE + \
			   ADMA_LRO_CTRL_OFFSET + 0x0C)

/* LRO RX delay interrupt configurations */
#define LRO_RX1_DLY_INT        (RALINK_FRAME_ENGINE_BASE + 0x0a70)
#define LRO_RX2_DLY_INT        (RALINK_FRAME_ENGINE_BASE + 0x0a74)
#define LRO_RX3_DLY_INT        (RALINK_FRAME_ENGINE_BASE + 0x0a78)

/* LRO auto-learn configurations */
#define PDMA_LRO_ATL_OVERFLOW_ADJ_OFFSET    0x0990
#define PDMA_LRO_ATL_OVERFLOW_ADJ (RALINK_FRAME_ENGINE_BASE + \
				   PDMA_LRO_ATL_OVERFLOW_ADJ_OFFSET)
#define LRO_ALT_SCORE_DELTA   (RALINK_FRAME_ENGINE_BASE + 0x0a4c)

/* LRO agg timer configurations */
#define LRO_MAX_AGG_TIME       (RALINK_FRAME_ENGINE_BASE + 0x0a5c)

/* LRO configurations of RX ring #0 */
#define LRO_RXRING0_OFFSET          0x0b00
#define LRO_RX_RING0_DIP_DW0 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING0_OFFSET + 0x04)
#define LRO_RX_RING0_DIP_DW1 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING0_OFFSET + 0x08)
#define LRO_RX_RING0_DIP_DW2 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING0_OFFSET + 0x0C)
#define LRO_RX_RING0_DIP_DW3 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING0_OFFSET + 0x10)
#define LRO_RX_RING0_CTRL_DW1 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING0_OFFSET + 0x28)
/* Bit [8]:RING0_VLD, Bit [9]:RING0_MYIP_VLD */
#define LRO_RX_RING0_CTRL_DW2 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING0_OFFSET + 0x2C)
#define LRO_RX_RING0_CTRL_DW3 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING0_OFFSET + 0x30)
/* LRO configurations of RX ring #1 */
#define LRO_RXRING1_OFFSET          0x0b40
#define LRO_RX_RING1_STP_DTP_DW (RALINK_FRAME_ENGINE_BASE + \
				 LRO_RXRING1_OFFSET + 0x00)
#define LRO_RX_RING1_DIP_DW0 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x04)
#define LRO_RX_RING1_DIP_DW1 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x08)
#define LRO_RX_RING1_DIP_DW2 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x0C)
#define LRO_RX_RING1_DIP_DW3 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x10)
#define LRO_RX_RING1_SIP_DW0 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x14)
#define LRO_RX_RING1_SIP_DW1 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x18)
#define LRO_RX_RING1_SIP_DW2 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x1C)
#define LRO_RX_RING1_SIP_DW3 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING1_OFFSET + 0x20)
#define LRO_RX_RING1_CTRL_DW0 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING1_OFFSET + 0x24)
#define LRO_RX_RING1_CTRL_DW1 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING1_OFFSET + 0x28)
#define LRO_RX_RING1_CTRL_DW2 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING1_OFFSET + 0x2C)
#define LRO_RX_RING1_CTRL_DW3 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING1_OFFSET + 0x30)
#define LRO_RXRING2_OFFSET          0x0b80
#define LRO_RX_RING2_STP_DTP_DW (RALINK_FRAME_ENGINE_BASE + \
				 LRO_RXRING2_OFFSET + 0x00)
#define LRO_RX_RING2_DIP_DW0 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x04)
#define LRO_RX_RING2_DIP_DW1 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x08)
#define LRO_RX_RING2_DIP_DW2 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x0C)
#define LRO_RX_RING2_DIP_DW3 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x10)
#define LRO_RX_RING2_SIP_DW0 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x14)
#define LRO_RX_RING2_SIP_DW1 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x18)
#define LRO_RX_RING2_SIP_DW2 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x1C)
#define LRO_RX_RING2_SIP_DW3 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING2_OFFSET + 0x20)
#define LRO_RX_RING2_CTRL_DW0 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING2_OFFSET + 0x24)
#define LRO_RX_RING2_CTRL_DW1 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING2_OFFSET + 0x28)
#define LRO_RX_RING2_CTRL_DW2 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING2_OFFSET + 0x2C)
#define LRO_RX_RING2_CTRL_DW3 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING2_OFFSET + 0x30)
#define LRO_RXRING3_OFFSET          0x0bc0
#define LRO_RX_RING3_STP_DTP_DW (RALINK_FRAME_ENGINE_BASE + \
				 LRO_RXRING3_OFFSET + 0x00)
#define LRO_RX_RING3_DIP_DW0 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x04)
#define LRO_RX_RING3_DIP_DW1 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x08)
#define LRO_RX_RING3_DIP_DW2 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x0C)
#define LRO_RX_RING3_DIP_DW3 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x10)
#define LRO_RX_RING3_SIP_DW0 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x14)
#define LRO_RX_RING3_SIP_DW1 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x18)
#define LRO_RX_RING3_SIP_DW2 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x1C)
#define LRO_RX_RING3_SIP_DW3 (RALINK_FRAME_ENGINE_BASE + \
			      LRO_RXRING3_OFFSET + 0x20)
#define LRO_RX_RING3_CTRL_DW0 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING3_OFFSET + 0x24)
#define LRO_RX_RING3_CTRL_DW1 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING3_OFFSET + 0x28)
#define LRO_RX_RING3_CTRL_DW2 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING3_OFFSET + 0x2C)
#define LRO_RX_RING3_CTRL_DW3 (RALINK_FRAME_ENGINE_BASE + \
			       LRO_RXRING3_OFFSET + 0x30)

#define ADMA_DBG_OFFSET	0x0a30
#define ADMA_TX_DBG0	(RALINK_FRAME_ENGINE_BASE + ADMA_DBG_OFFSET + 0x00)
#define ADMA_TX_DBG1	(RALINK_FRAME_ENGINE_BASE + ADMA_DBG_OFFSET + 0x04)
#define ADMA_RX_DBG0	(RALINK_FRAME_ENGINE_BASE + ADMA_DBG_OFFSET + 0x08)
#define ADMA_RX_DBG1	(RALINK_FRAME_ENGINE_BASE + ADMA_DBG_OFFSET + 0x0C)

/* LRO RX ring mode */
#define PDMA_RX_NORMAL_MODE         (0x0)
#define PDMA_RX_PSE_MODE            (0x1)
#define PDMA_RX_FORCE_PORT          (0x2)
#define PDMA_RX_AUTO_LEARN          (0x3)

#define ADMA_RX_RING0   (0)
#define ADMA_RX_RING1   (1)
#define ADMA_RX_RING2   (2)
#define ADMA_RX_RING3   (3)

#define ADMA_RX_LEN0_MASK   (0x3fff)
#define ADMA_RX_LEN1_MASK   (0x3)

#define SET_ADMA_RX_LEN0(x)    ((x) & ADMA_RX_LEN0_MASK)
#define SET_ADMA_RX_LEN1(x)    ((x) & ADMA_RX_LEN1_MASK)

#define QDMA_PAGE	(ETHDMASYS_FRAME_ENGINE_BASE + 0x19f0)

/*SFQ use*/
#define VQTX_TB_BASE0 (ETHDMASYS_FRAME_ENGINE_BASE + 0x1980)
#define VQTX_TB_BASE1 (ETHDMASYS_FRAME_ENGINE_BASE + 0x1984)
#define VQTX_TB_BASE2 (ETHDMASYS_FRAME_ENGINE_BASE + 0x1988)
#define VQTX_TB_BASE3 (ETHDMASYS_FRAME_ENGINE_BASE + 0x198C)
#define SFQ_OFFSET 0x1A80
#define VQTX_GLO (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET)
#define VQTX_INVLD_PTR (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x0C)
#define VQTX_NUM (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x10)
#define VQTX_SCH (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x18)
#define VQTX_HASH_CFG (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x20)
#define VQTX_HASH_SD (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x24)
#define VQTX_VLD_CFG (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x30)
#define VQTX_MIB_IF (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x3C)
#define VQTX_MIB_PCNT (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x40)
#define VQTX_MIB_BCNT0 (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x44)
#define VQTX_MIB_BCNT1 (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x48)
#define VQTX_0_BIND_QID	(PQ0 << 0)
#define VQTX_1_BIND_QID (PQ1 << 8)
#define VQTX_2_BIND_QID (PQ2 << 16)
#define VQTX_3_BIND_QID (PQ3 << 24)
#define VQTX_4_BIND_QID (PQ4 << 0)
#define VQTX_5_BIND_QID (PQ5 << 8)
#define VQTX_6_BIND_QID (PQ6 << 16)
#define VQTX_7_BIND_QID (PQ7 << 24)
#define VQTX_TB_BASE4 (ETHDMASYS_FRAME_ENGINE_BASE + 0x1990)
#define VQTX_TB_BASE5 (ETHDMASYS_FRAME_ENGINE_BASE + 0x1994)
#define VQTX_TB_BASE6 (ETHDMASYS_FRAME_ENGINE_BASE + 0x1998)
#define VQTX_TB_BASE7 (ETHDMASYS_FRAME_ENGINE_BASE + 0x199C)
#define VQTX_0_3_BIND_QID (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x140)
#define VQTX_4_7_BIND_QID (ETHDMASYS_FRAME_ENGINE_BASE + SFQ_OFFSET + 0x144)
#define PQ0	0
#define PQ1	1
#define PQ2	15
#define PQ3	16
#define PQ4	30
#define PQ5	31
#define PQ6	43
#define PQ7	63

#if defined(CONFIG_MACH_MT7623)
#define VQ_NUM0	256
#define VQ_NUM1	256
#define VQ_NUM2	256
#define VQ_NUM3	256
#define VQ_NUM4	0
#define VQ_NUM5	0
#define VQ_NUM6	0
#define VQ_NUM7	0
#define VQTX_NUM_0  (4 << 0)
#define VQTX_NUM_1  (4 << 4)
#define VQTX_NUM_2  (4 << 8)
#define VQTX_NUM_3  (4 << 12)
#define VQTX_NUM_4   0
#define VQTX_NUM_5   0
#define VQTX_NUM_6   0
#define VQTX_NUM_7   0
#else
#define VQ_NUM0	128
#define VQ_NUM1	128
#define VQ_NUM2	128
#define VQ_NUM3	128
#define VQ_NUM4	128
#define VQ_NUM5	128
#define VQ_NUM6	128
#define VQ_NUM7	128
#define VQTX_NUM_0  (3 << 0)
#define VQTX_NUM_1  (3 << 4)
#define VQTX_NUM_2  (3 << 8)
#define VQTX_NUM_3  (3 << 12)
#define VQTX_NUM_4  (3 << 16)
#define VQTX_NUM_5  (3 << 20)
#define VQTX_NUM_6  (3 << 24)
#define VQTX_NUM_7  (3 << 28)
#endif

#define VQTX_MIB_EN BIT(17)

/*=========================================
 *    SFQ Table Format define
 *=========================================
 */
struct SFQ_INFO1_T {
	unsigned int VQHPTR;
};

struct SFQ_INFO2_T {
	unsigned int VQTPTR;
};

struct SFQ_INFO3_T {
	unsigned int QUE_DEPTH:16;
	unsigned int DEFICIT_CNT:16;
};

struct SFQ_INFO4_T {
	unsigned int RESV;
};

struct SFQ_INFO5_T {
	unsigned int PKT_CNT;
};

struct SFQ_INFO6_T {
	unsigned int BYTE_CNT;
};

struct SFQ_INFO7_T {
	unsigned int BYTE_CNT;
};

struct SFQ_INFO8_T {
	unsigned int RESV;
};

struct SFQ_table {
	struct SFQ_INFO1_T sfq_info1;
	struct SFQ_INFO2_T sfq_info2;
	struct SFQ_INFO3_T sfq_info3;
	struct SFQ_INFO4_T sfq_info4;
	struct SFQ_INFO5_T sfq_info5;
	struct SFQ_INFO6_T sfq_info6;
	struct SFQ_INFO7_T sfq_info7;
	struct SFQ_INFO8_T sfq_info8;
};

#if defined(CONFIG_RAETH_HW_LRO) || defined(CONFIG_RAETH_MULTIPLE_RX_RING)
#define FE_GDM_RXID1_OFFSET	(0x0130)
#define FE_GDM_RXID1		(RALINK_FRAME_ENGINE_BASE + FE_GDM_RXID1_OFFSET)
#define GDM_VLAN_PRI7_RXID_SEL	BITS(30, 31)
#define GDM_VLAN_PRI6_RXID_SEL	BITS(28, 29)
#define GDM_VLAN_PRI5_RXID_SEL	BITS(26, 27)
#define GDM_VLAN_PRI4_RXID_SEL	BITS(24, 25)
#define GDM_VLAN_PRI3_RXID_SEL	BITS(22, 23)
#define GDM_VLAN_PRI2_RXID_SEL	BITS(20, 21)
#define GDM_VLAN_PRI1_RXID_SEL	BITS(18, 19)
#define GDM_VLAN_PRI0_RXID_SEL	BITS(16, 17)
#define GDM_TCP_ACK_RXID_SEL	BITS(4, 5)
#define GDM_TCP_ACK_WZPC	BIT(3)
#define GDM_RXID_PRI_SEL	BITS(0, 2)

#define FE_GDM_RXID2_OFFSET	(0x0134)
#define FE_GDM_RXID2		(RALINK_FRAME_ENGINE_BASE + FE_GDM_RXID2_OFFSET)
#define GDM_STAG7_RXID_SEL	BITS(30, 31)
#define GDM_STAG6_RXID_SEL	BITS(28, 29)
#define GDM_STAG5_RXID_SEL	BITS(26, 27)
#define GDM_STAG4_RXID_SEL	BITS(24, 25)
#define GDM_STAG3_RXID_SEL	BITS(22, 23)
#define GDM_STAG2_RXID_SEL	BITS(20, 21)
#define GDM_STAG1_RXID_SEL	BITS(18, 19)
#define GDM_STAG0_RXID_SEL	BITS(16, 17)
#define GDM_PID2_RXID_SEL	BITS(2, 3)
#define GDM_PID1_RXID_SEL	BITS(0, 1)

#define GDM_PRI_PID              (0)
#define GDM_PRI_VLAN_PID         (1)
#define GDM_PRI_ACK_PID          (2)
#define GDM_PRI_VLAN_ACK_PID     (3)
#define GDM_PRI_ACK_VLAN_PID     (4)

#define SET_GDM_VLAN_PRI_RXID_SEL(x, y) \
{ \
unsigned int reg_val = sys_reg_read(FE_GDM_RXID1); \
reg_val &= ~(0x03 << (((x) << 1) + 16));   \
reg_val |= ((y) & 0x3) << (((x) << 1) + 16);  \
sys_reg_write(FE_GDM_RXID1, reg_val); \
}

#define SET_GDM_TCP_ACK_RXID_SEL(x) \
{ \
unsigned int reg_val = sys_reg_read(FE_GDM_RXID1); \
reg_val &= ~(GDM_TCP_ACK_RXID_SEL);   \
reg_val |= ((x) & 0x3) << 4;  \
sys_reg_write(FE_GDM_RXID1, reg_val); \
}

#define SET_GDM_TCP_ACK_WZPC(x) \
{ \
unsigned int reg_val = sys_reg_read(FE_GDM_RXID1); \
reg_val &= ~(GDM_TCP_ACK_WZPC);   \
reg_val |= ((x) & 0x1) << 3;  \
sys_reg_write(FE_GDM_RXID1, reg_val); \
}

#define SET_GDM_RXID_PRI_SEL(x) \
{ \
unsigned int reg_val = sys_reg_read(FE_GDM_RXID1); \
reg_val &= ~(GDM_RXID_PRI_SEL);   \
reg_val |= (x) & 0x7;  \
sys_reg_write(FE_GDM_RXID1, reg_val); \
}

#define GDM_STAG_RXID_SEL(x, y) \
{ \
unsigned int reg_val = sys_reg_read(FE_GDM_RXID2); \
reg_val &= ~(0x03 << (((x) << 1) + 16));   \
reg_val |= ((y) & 0x3) << (((x) << 1) + 16);  \
sys_reg_write(FE_GDM_RXID2, reg_val); \
}

#define SET_GDM_PID2_RXID_SEL(x) \
{ \
unsigned int reg_val = sys_reg_read(FE_GDM_RXID2); \
reg_val &= ~(GDM_PID2_RXID_SEL);   \
reg_val |= ((x) & 0x3) << 2;  \
sys_reg_write(FE_GDM_RXID2, reg_val); \
}

#define SET_GDM_PID1_RXID_SEL(x) \
{ \
unsigned int reg_val = sys_reg_read(FE_GDM_RXID2); \
reg_val &= ~(GDM_PID1_RXID_SEL);   \
reg_val |= ((x) & 0x3);  \
sys_reg_write(FE_GDM_RXID2, reg_val); \
}

#endif /* CONFIG_RAETH_MULTIPLE_RX_RING */
/* Per Port Packet Counts in RT3052, added by bobtseng 2009.4.17. */
#define	PORT0_PKCOUNT		(0xb01100e8)
#define	PORT1_PKCOUNT		(0xb01100ec)
#define	PORT2_PKCOUNT		(0xb01100f0)
#define	PORT3_PKCOUNT		(0xb01100f4)
#define	PORT4_PKCOUNT		(0xb01100f8)
#define	PORT5_PKCOUNT		(0xb01100fc)

#define sys_reg_read(phys)	 (__raw_readl((void __iomem *)phys))
#define sys_reg_write(phys, val) (__raw_writel(val, (void __iomem *)phys))

/* ====================================== */
#define GDM1_DISPAD       BIT(18)
#define GDM1_DISCRC       BIT(17)

/* GDMA1 uni-cast frames destination port */
#define GDM1_ICS_EN	   (0x1 << 22)
#define GDM1_TCS_EN	   (0x1 << 21)
#define GDM1_UCS_EN	   (0x1 << 20)
#define GDM1_JMB_EN	   (0x1 << 19)
#define GDM1_STRPCRC	   (0x1 << 16)
#define GDM1_UFRC_P_CPU     (0 << 12)

/* GDMA1 broad-cast MAC address frames */
#define GDM1_BFRC_P_CPU     (0 << 8)

/* GDMA1 multi-cast MAC address frames */
#define GDM1_MFRC_P_CPU     (0 << 4)

/* GDMA1 other MAC address frames destination port */
#define GDM1_OFRC_P_CPU     (0 << 0)

/* checksum generator registers are removed */
#define ICS_GEN_EN          (0 << 2)
#define UCS_GEN_EN          (0 << 1)
#define TCS_GEN_EN          (0 << 0)

/* MDIO_CFG     bit */
#define MDIO_CFG_GP1_FC_TX	BIT(11)
#define MDIO_CFG_GP1_FC_RX	BIT(10)

/* ====================================== */
/* ====================================== */
#define GP1_LNK_DWN     BIT(9)
#define GP1_AN_FAIL     BIT(8)
/* ====================================== */
/* ====================================== */
#define PSE_RESET       BIT(0)
/* ====================================== */
#define PST_DRX_IDX3       BIT(19)
#define PST_DRX_IDX2       BIT(18)
#define PST_DRX_IDX1       BIT(17)
#define PST_DRX_IDX0       BIT(16)
#define PST_DTX_IDX3       BIT(3)
#define PST_DTX_IDX2       BIT(2)
#define PST_DTX_IDX1       BIT(1)
#define PST_DTX_IDX0       BIT(0)

#define RX_2B_OFFSET	  BIT(31)
#define CSR_CLKGATE_BYP	  BIT(30)
#define MULTI_EN	  BIT(10)
#define DESC_32B_EN	  BIT(8)
#define TX_WB_DDONE       BIT(6)
#define RX_DMA_BUSY       BIT(3)
#define TX_DMA_BUSY       BIT(1)
#define RX_DMA_EN         BIT(2)
#define TX_DMA_EN         BIT(0)

#define PDMA_BT_SIZE_4DWORDS		(0 << 4)
#define PDMA_BT_SIZE_8DWORDS		(1 << 4)
#define PDMA_BT_SIZE_16DWORDS		(2 << 4)
#define PDMA_BT_SIZE_32DWORDS		(3 << 4)

#define ADMA_RX_BT_SIZE_4DWORDS		(0 << 11)
#define ADMA_RX_BT_SIZE_8DWORDS		(1 << 11)
#define ADMA_RX_BT_SIZE_16DWORDS	(2 << 11)
#define ADMA_RX_BT_SIZE_32DWORDS	(3 << 11)

/* Register bits.
 */

#define MACCFG_RXEN	BIT(2)
#define MACCFG_TXEN	BIT(3)
#define MACCFG_PROMISC	BIT(18)
#define MACCFG_RXMCAST	BIT(19)
#define MACCFG_FDUPLEX	BIT(20)
#define MACCFG_PORTSEL	BIT(27)
#define MACCFG_HBEATDIS	BIT(28)

#define DMACTL_SR	BIT(1)	/* Start/Stop Receive */
#define DMACTL_ST	BIT(13)	/* Start/Stop Transmission Command */

#define DMACFG_SWR	BIT(0)	/* Software Reset */
#define DMACFG_BURST32		(32 << 8)

#define DMASTAT_TS		0x00700000	/* Transmit Process State */
#define DMASTAT_RS		0x000e0000	/* Receive Process State */

#define MACCFG_INIT		0   /* (MACCFG_FDUPLEX) // | MACCFG_PORTSEL) */

/* Descriptor bits.
 */
#define R_OWN		0x80000000	/* Own Bit */
#define RD_RER		0x02000000	/* Receive End Of Ring */
#define RD_LS		0x00000100	/* Last Descriptor */
#define RD_ES		0x00008000	/* Error Summary */
#define RD_CHAIN	0x01000000	/* Chained */

/* Word 0 */
#define T_OWN		0x80000000	/* Own Bit */
#define TD_ES		0x00008000	/* Error Summary */

/* Word 1 */
#define TD_LS		0x40000000	/* Last Segment */
#define TD_FS		0x20000000	/* First Segment */
#define TD_TER		0x08000000	/* Transmit End Of Ring */
#define TD_CHAIN	0x01000000	/* Chained */

#define TD_SET		0x08000000	/* Setup Packet */

#define POLL_DEMAND 1

#define RSTCTL	(0x34)
#define RSTCTL_RSTENET1	BIT(19)
#define RSTCTL_RSTENET2	BIT(20)

#define INIT_VALUE_OF_RT2883_PSE_FQ_CFG		0xff908000
#define INIT_VALUE_OF_PSE_FQFC_CFG		0x80504000
#define INIT_VALUE_OF_FORCE_100_FD		0x1001BC01
#define INIT_VALUE_OF_FORCE_1000_FD		0x1F01DC01

/* Define Whole FE Reset Register */
#define RSTCTRL			(RALINK_SYSCTL_BASE + 0x34)
#define RT2880_AGPIOCFG_REG	(RALINK_SYSCTL_BASE + 0x3C)

/*=========================================
 *    PDMA RX Descriptor Format define
 *=========================================
 */

struct PDMA_RXD_INFO1_T {
	unsigned int PDP0;
};

struct PDMA_RXD_INFO2_T {
	unsigned int PLEN1:2;
	unsigned int LRO_AGG_CNT:8;
	unsigned int REV:5;
	unsigned int TAG:1;
	unsigned int PLEN0:14;
	unsigned int LS0:1;
	unsigned int DDONE_bit:1;
};

struct PDMA_RXD_INFO3_T {
	unsigned int VID:16;
	unsigned int TPID:16;
};

struct PDMA_RXD_INFO4_T {
	unsigned int FOE_ENTRY:14;
	unsigned int CRSN:5;
	unsigned int SP:4;
	unsigned int L4F:1;
	unsigned int L4VLD:1;
	unsigned int TACK:1;
	unsigned int IP4F:1;
	unsigned int IP4:1;
	unsigned int IP6:1;
	unsigned int UN_USE1:3;
};

struct PDMA_rxdesc {
	struct PDMA_RXD_INFO1_T rxd_info1;
	struct PDMA_RXD_INFO2_T rxd_info2;
	struct PDMA_RXD_INFO3_T rxd_info3;
	struct PDMA_RXD_INFO4_T rxd_info4;
#ifdef CONFIG_32B_DESC
	unsigned int rxd_info5;
	unsigned int rxd_info6;
	unsigned int rxd_info7;
	unsigned int rxd_info8;
#endif
};

/*=========================================
 *    PDMA TX Descriptor Format define
 *=========================================
 */
struct PDMA_TXD_INFO1_T {
	unsigned int SDP0;
};

struct PDMA_TXD_INFO2_T {
	unsigned int SDL1:14;
	unsigned int LS1_bit:1;
	unsigned int BURST_bit:1;
	unsigned int SDL0:14;
	unsigned int LS0_bit:1;
	unsigned int DDONE_bit:1;
};

struct PDMA_TXD_INFO3_T {
	unsigned int SDP1;
};

struct PDMA_TXD_INFO4_T {
	unsigned int VLAN_TAG:17;	/* INSV(1)+VPRI(3)+CFI(1)+VID(12) */
	unsigned int RESV:2;
	unsigned int UDF:6;
	unsigned int FPORT:3;
	unsigned int TSO:1;
	unsigned int TUI_CO:3;
};

struct PDMA_txdesc {
	struct PDMA_TXD_INFO1_T txd_info1;
	struct PDMA_TXD_INFO2_T txd_info2;
	struct PDMA_TXD_INFO3_T txd_info3;
	struct PDMA_TXD_INFO4_T txd_info4;
#ifdef CONFIG_32B_DESC
	unsigned int txd_info5;
	unsigned int txd_info6;
	unsigned int txd_info7;
	unsigned int txd_info8;
#endif
};

/*=========================================
 *    QDMA TX Descriptor Format define
 *=========================================
 */
struct QDMA_TXD_INFO1_T {
	unsigned int SDP;
};

struct QDMA_TXD_INFO2_T {
	unsigned int NDP;
};

struct QDMA_TXD_INFO3_T {
	unsigned int QID:4;	/* Q0~Q15 */
	/* unsigned int    VQID                  : 10; */
	unsigned int PROT:3;
	unsigned int IPOFST:7;
	unsigned int SWC_bit:1;
	unsigned int BURST_bit:1;
	unsigned int SDL:14;
	unsigned int LS_bit:1;
	unsigned int OWN_bit:1;
};

struct QDMA_TXD_INFO4_T {
	unsigned int VLAN_TAG:17;	/* INSV(1)+VPRI(3)+CFI(1)+VID(12) */
	unsigned int VQID0:1;
	unsigned int SDL:2;
	unsigned int QID:2;	/* Q16~Q63 */
	unsigned int RESV:3;
	unsigned int FPORT:3;
	unsigned int TSO:1;
	unsigned int TUI_CO:3;
};

struct QDMA_txdesc {
	struct QDMA_TXD_INFO1_T txd_info1;
	struct QDMA_TXD_INFO2_T txd_info2;
	struct QDMA_TXD_INFO3_T txd_info3;
	struct QDMA_TXD_INFO4_T txd_info4;
#ifdef CONFIG_32B_DESC
	unsigned int txd_info5;
	unsigned int txd_info6;
	unsigned int txd_info7;
	unsigned int txd_info8;
#endif
};

#define QTXD_LEN (sizeof(struct QDMA_txdesc))
#define PHY_ENABLE_AUTO_NEGO	0x1000
#define PHY_RESTART_AUTO_NEGO	0x0200

/* PHY_STAT_REG = 1; */
#define PHY_AUTO_NEGO_COMP	0x0020
#define PHY_LINK_STATUS		0x0004

/* PHY_AUTO_NEGO_REG = 4; */
#define PHY_CAP_10_HALF		0x0020
#define PHY_CAP_10_FULL		0x0040
#define	PHY_CAP_100_HALF	0x0080
#define	PHY_CAP_100_FULL	0x0100

/* proc definition */

#define PROCREG_CONTROL_FILE      "/var/run/procreg_control"
#if defined(CONFIG_MACH_MT7623)
#define PROCREG_DIR             "mt7623"
#else
#define PROCREG_DIR             "mt7622"
#endif
#define PROCREG_SKBFREE		"skb_free"
#define PROCREG_TXRING		"tx_ring"
#define PROCREG_RXRING		"rx_ring"
#define PROCREG_RXRING1		"rx_ring1"
#define PROCREG_RXRING2		"rx_ring2"
#define PROCREG_RXRING3		"rx_ring3"
#define PROCREG_NUM_OF_TXD	"num_of_txd"
#define PROCREG_TSO_LEN		"tso_len"
#define PROCREG_LRO_STATS	"lro_stats"
#define PROCREG_HW_LRO_STATS	"hw_lro_stats"
#define PROCREG_HW_LRO_AUTO_TLB	"hw_lro_auto_tlb"
#define PROCREG_HW_IO_COHERENT	"hw_iocoherent"
#define PROCREG_GMAC		"gmac"
#define PROCREG_GMAC2           "gmac2"
#define PROCREG_CP0		"cp0"
#define PROCREG_RAQOS		"qos"
#define PROCREG_READ_VAL	"regread_value"
#define PROCREG_WRITE_VAL	"regwrite_value"
#define PROCREG_ADDR		"reg_addr"
#define PROCREG_CTL		"procreg_control"
#define PROCREG_RXDONE_INTR	"rxdone_intr_count"
#define PROCREG_ESW_INTR	"esw_intr_count"
#define PROCREG_ESW_CNT		"esw_cnt"
#define PROCREG_SNMP		"snmp"
#define PROCREG_SET_LAN_IP	"set_lan_ip"
#if defined(TASKLET_WORKQUEUE_SW)
#define PROCREG_SCHE		"schedule"
#endif
#define PROCREG_QDMA            "qdma"
#define PROCREG_INT_DBG		"int_dbg"
struct rt2880_reg_op_data {
	char name[64];
	unsigned int reg_addr;
	unsigned int op;
	unsigned int reg_value;
};

struct lro_counters {
	u32 lro_aggregated;
	u32 lro_flushed;
	u32 lro_no_desc;
};

struct lro_para_struct {
	unsigned int lan_ip1;
};

struct parse_result {
	/* layer2 header */
	u8 dmac[6];
	u8 smac[6];

	/* vlan header */
	u16 vlan_tag;
	u16 vlan1_gap;
	u16 vlan1;
	u16 vlan2_gap;
	u16 vlan2;
	u16 vlan_layer;

	/* pppoe header */
	u32 pppoe_gap;
	u16 ppp_tag;
	u16 pppoe_sid;

	/* layer3 header */
	u16 eth_type;
	struct iphdr iph;
	struct ipv6hdr ip6h;

	/* layer4 header */
	struct tcphdr th;
	struct udphdr uh;

	u32 pkt_type;
	u8 is_mcast;
};

#define DMA_GLO_CFG PDMA_GLO_CFG

#if defined(CONFIG_RAETH_QDMATX_QDMARX)
#define GDMA1_FWD_PORT 0x5555
#define GDMA2_FWD_PORT 0x5555
#elif defined(CONFIG_RAETH_PDMATX_QDMARX)
#define GDMA1_FWD_PORT 0x5555
#define GDMA2_FWD_PORT 0x5555
#else
#define GDMA1_FWD_PORT 0x0000
#define GDMA2_FWD_PORT 0x0000
#endif

#if defined(CONFIG_RAETH_QDMATX_QDMARX)
#define RAETH_RX_CALC_IDX0 QRX_CRX_IDX_0
#define RAETH_RX_CALC_IDX1 QRX_CRX_IDX_1
#elif defined(CONFIG_RAETH_PDMATX_QDMARX)
#define RAETH_RX_CALC_IDX0 QRX_CRX_IDX_0
#define RAETH_RX_CALC_IDX1 QRX_CRX_IDX_1
#else
#define RAETH_RX_CALC_IDX0 RX_CALC_IDX0
#define RAETH_RX_CALC_IDX1 RX_CALC_IDX1
#endif
#define RAETH_RX_CALC_IDX2 RX_CALC_IDX2
#define RAETH_RX_CALC_IDX3 RX_CALC_IDX3
#define RAETH_FE_INT_STATUS FE_INT_STATUS
#define RAETH_FE_INT_ALL FE_INT_ALL
#define RAETH_FE_INT_ENABLE FE_INT_ENABLE
#define RAETH_FE_INT_DLY_INIT FE_INT_DLY_INIT
#define RAETH_FE_INT_SETTING (RX_DONE_INT0 | RX_DONE_INT1 | \
			      TX_DONE_INT0 | TX_DONE_INT1 | \
			      TX_DONE_INT2 | TX_DONE_INT3)
#define QFE_INT_SETTING (RX_DONE_INT0 | RX_DONE_INT1 | \
			 TX_DONE_INT0 | TX_DONE_INT1 | \
			 TX_DONE_INT2 | TX_DONE_INT3)
#define RAETH_TX_DLY_INT TX_DLY_INT
#define RAETH_TX_DONE_INT0 TX_DONE_INT0
#define RAETH_DLY_INT_CFG DLY_INT_CFG

#endif	/* RAETH_REG_H */
