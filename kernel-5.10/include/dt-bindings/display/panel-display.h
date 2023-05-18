/*
 * Copyright c 2020 Samsung Electronics Co., Ltd.
 *
 * Author: Gwnaghui Lee <gwanghui.lee@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Device Tree binding constants for panel display.
*/

#ifndef _DT_BINDINGS_PANEL_DISPLAY_H
#define _DT_BINDINGS_PANEL_DISPLAY_H

#define REFRESH_MODE_NS	(0)
#define REFRESH_MODE_HS	(1)
#define REFRESH_MODE_PASSIVE_HS	(2)

#define PANEL_REFRESH_MODE_NS	(0)
#define PANEL_REFRESH_MODE_HS	(1)

#define PANEL_DISPLAY_MODE_1080x2340_120HS    "1080x2340_120HS"
#define PANEL_DISPLAY_MODE_1080x2340_96HS     "1080x2340_96HS"
#define PANEL_DISPLAY_MODE_1080x2340_90HS     "1080x2340_90HS"
#define PANEL_DISPLAY_MODE_1080x2340_60HS     "1080x2340_60HS"
#define PANEL_DISPLAY_MODE_1080x2340_30HS_60HS_TE_HW_SKIP_1       "1080x2340_30HS_60HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2340_60HS_120HS_TE_SW_SKIP_1      "1080x2340_60HS_120HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2340_60HS_120HS_TE_HW_SKIP_1      "1080x2340_60HS_120HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2340_30HS_120HS_TE_HW_SKIP_3      "1080x2340_30HS_120HS_TE_HW_SKIP_3"
#define PANEL_DISPLAY_MODE_1080x2340_24HS_120HS_TE_HW_SKIP_4      "1080x2340_24HS_120HS_TE_HW_SKIP_4"
#define PANEL_DISPLAY_MODE_1080x2340_10HS_120HS_TE_HW_SKIP_11      "1080x2340_10HS_120HS_TE_HW_SKIP_11"
#define PANEL_DISPLAY_MODE_1080x2340_48HS     "1080x2340_48HS"
#define PANEL_DISPLAY_MODE_1080x2340_24HS_48HS_TE_HW_SKIP_1     "1080x2340_24HS_48HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2340_10HS_48HS_TE_HW_SKIP_4     "1080x2340_10HS_48HS_TE_SW_SKIP_4"
#define PANEL_DISPLAY_MODE_1080x2340_48HS_96HS_TE_SW_SKIP_1      "1080x2340_48HS_96HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2340_48HS_96HS_TE_HW_SKIP_1      "1080x2340_48HS_96HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2340_60NS     "1080x2340_60NS"
#define PANEL_DISPLAY_MODE_1080x2340_48NS     "1080x2340_48NS"
#define PANEL_DISPLAY_MODE_1080x2340_30NS     "1080x2340_30NS"

#define PANEL_DISPLAY_MODE_1440x3200_120HS    "1440x3200_120HS"
#define PANEL_DISPLAY_MODE_1440x3200_96HS     "1440x3200_96HS"
#define PANEL_DISPLAY_MODE_1440x3200_60HS     "1440x3200_60HS"
#define PANEL_DISPLAY_MODE_1440x3200_60HS_120HS_TE_SW_SKIP_1      "1440x3200_60HS_120HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_1440x3200_60HS_120HS_TE_HW_SKIP_1      "1440x3200_60HS_120HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1440x3200_48HS     "1440x3200_48HS"
#define PANEL_DISPLAY_MODE_1440x3200_48HS_96HS_TE_SW_SKIP_1      "1440x3200_48HS_96HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_1440x3200_48HS_96HS_TE_HW_SKIP_1      "1440x3200_48HS_96HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1440x3200_60NS     "1440x3200_60NS"
#define PANEL_DISPLAY_MODE_1440x3200_52NS     "1440x3200_52NS"
#define PANEL_DISPLAY_MODE_1440x3200_48NS     "1440x3200_48NS"
#define PANEL_DISPLAY_MODE_1440x3200_30NS     "1440x3200_30NS"
#define PANEL_DISPLAY_MODE_1440x2304_60HS     "1440x2304_60HS"
#define PANEL_DISPLAY_MODE_1080x2408_120HS    "1080x2408_120HS"
#define PANEL_DISPLAY_MODE_1080x2408_90HS     "1080x2408_90HS"
#define PANEL_DISPLAY_MODE_1080x2408_60HS     "1080x2408_60HS"
#define PANEL_DISPLAY_MODE_1080x2400_120HS    "1080x2400_120HS"
#define PANEL_DISPLAY_MODE_1080x2400_120HS_AID_4_CYCLE    "1080x2400_120HS_AID_4_CYCLE"
#define PANEL_DISPLAY_MODE_1080x2400_112HS    "1080x2400_112HS"
#define PANEL_DISPLAY_MODE_1080x2400_110HS    "1080x2400_110HS"
#define PANEL_DISPLAY_MODE_1080x2400_104HS    "1080x2400_104HS"
#define PANEL_DISPLAY_MODE_1080x2400_100HS    "1080x2400_100HS"
#define PANEL_DISPLAY_MODE_1080x2400_96HS     "1080x2400_96HS"
#define PANEL_DISPLAY_MODE_1080x2400_90HS     "1080x2400_90HS"
#define PANEL_DISPLAY_MODE_1080x2400_70HS     "1080x2400_70HS"
#define PANEL_DISPLAY_MODE_1080x2400_60HS     "1080x2400_60HS"
#define PANEL_DISPLAY_MODE_1080x2400_60HS_120HS_TE_SW_SKIP_1      "1080x2400_60HS_120HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2400_60HS_120HS_TE_HW_SKIP_1      "1080x2400_60HS_120HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2400_48HS     "1080x2400_48HS"
#define PANEL_DISPLAY_MODE_1080x2400_48HS_96HS_TE_SW_SKIP_1      "1080x2400_48HS_96HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2400_48HS_96HS_TE_HW_SKIP_1      "1080x2400_48HS_96HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2400_60NS     "1080x2400_60NS"
#define PANEL_DISPLAY_MODE_1080x2400_52NS     "1080x2400_52NS"
#define PANEL_DISPLAY_MODE_1080x2400_48NS     "1080x2400_48NS"
#define PANEL_DISPLAY_MODE_1080x2400_30NS     "1080x2400_30NS"
#define PANEL_DISPLAY_MODE_720x1600_120HS    "720x1600_120HS"
#define PANEL_DISPLAY_MODE_720x1600_120HS_AID_4_CYCLE    "720x1600_120HS_AID_4_CYCLE"
#define PANEL_DISPLAY_MODE_720x1600_112HS    "720x1600_112HS"
#define PANEL_DISPLAY_MODE_720x1600_110HS    "720x1600_110HS"
#define PANEL_DISPLAY_MODE_720x1600_104HS    "720x1600_104HS"
#define PANEL_DISPLAY_MODE_720x1600_100HS    "720x1600_100HS"
#define PANEL_DISPLAY_MODE_720x1600_96HS     "720x1600_96HS"
#define PANEL_DISPLAY_MODE_720x1600_90HS     "720x1600_90HS"
#define PANEL_DISPLAY_MODE_720x1600_70HS     "720x1600_70HS"
#define PANEL_DISPLAY_MODE_720x1600_60HS     "720x1600_60HS"
#define PANEL_DISPLAY_MODE_720x1600_60HS_120HS_TE_SW_SKIP_1      "720x1600_60HS_120HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_720x1600_60HS_120HS_TE_HW_SKIP_1      "720x1600_60HS_120HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_720x1600_48HS     "720x1600_48HS"
#define PANEL_DISPLAY_MODE_720x1600_48HS_96HS_TE_SW_SKIP_1      "720x1600_48HS_96HS_TE_SW_SKIP_1"
#define PANEL_DISPLAY_MODE_720x1600_48HS_96HS_TE_HW_SKIP_1      "720x1600_48HS_96HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_720x1600_60NS     "720x1600_60NS"
#define PANEL_DISPLAY_MODE_720x1600_52NS     "720x1600_52NS"
#define PANEL_DISPLAY_MODE_720x1600_48NS     "720x1600_48NS"
#define PANEL_DISPLAY_MODE_720x1600_30NS     "720x1600_30NS"

#define PANEL_DISPLAY_MODE_1440x3088_120HS    "1440x3088_120HS"
#define PANEL_DISPLAY_MODE_1440x3088_60HS_120HS_TE_HW_SKIP_1      "1440x3088_60HS_120HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1440x3088_30HS_120HS_TE_HW_SKIP_3      "1440x3088_30HS_120HS_TE_HW_SKIP_3"
#define PANEL_DISPLAY_MODE_1440x3088_24HS_120HS_TE_HW_SKIP_4      "1440x3088_24HS_120HS_TE_HW_SKIP_4"
#define PANEL_DISPLAY_MODE_1440x3088_10HS_120HS_TE_HW_SKIP_11      "1440x3088_10HS_120HS_TE_HW_SKIP_11"
#define PANEL_DISPLAY_MODE_1440x3088_96HS     "1440x3088_96HS"
#define PANEL_DISPLAY_MODE_1440x3088_48HS_96HS_TE_HW_SKIP_1      "1440x3088_48HS_96HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1440x3088_48HS     "1440x3088_48HS"
#define PANEL_DISPLAY_MODE_1440x3088_60NS     "1440x3088_60NS"
#define PANEL_DISPLAY_MODE_1440x3088_48NS     "1440x3088_48NS"
#define PANEL_DISPLAY_MODE_1440x3088_30NS     "1440x3088_30NS"
#define PANEL_DISPLAY_MODE_1080x2316_120HS    "1080x2316_120HS"
#define PANEL_DISPLAY_MODE_1080x2316_60HS_120HS_TE_HW_SKIP_1      "1080x2316_60HS_120HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2316_30HS_120HS_TE_HW_SKIP_3      "1080x2316_30HS_120HS_TE_HW_SKIP_3"
#define PANEL_DISPLAY_MODE_1080x2316_24HS_120HS_TE_HW_SKIP_4      "1080x2316_24HS_120HS_TE_HW_SKIP_4"
#define PANEL_DISPLAY_MODE_1080x2316_10HS_120HS_TE_HW_SKIP_11      "1080x2316_10HS_120HS_TE_HW_SKIP_11"
#define PANEL_DISPLAY_MODE_1080x2316_96HS     "1080x2316_96HS"
#define PANEL_DISPLAY_MODE_1080x2316_48HS_96HS_TE_HW_SKIP_1       "1080x2316_48HS_96HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_1080x2316_60NS     "1080x2316_60NS"
#define PANEL_DISPLAY_MODE_1080x2316_48NS     "1080x2316_48NS"
#define PANEL_DISPLAY_MODE_1080x2316_30NS     "1080x2316_30NS"
#define PANEL_DISPLAY_MODE_720x1544_120HS     "720x1544_120HS"
#define PANEL_DISPLAY_MODE_720x1544_60HS_120HS_TE_HW_SKIP_1       "720x1544_60HS_120HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_720x1544_30HS_120HS_TE_HW_SKIP_3       "720x1544_30HS_120HS_TE_HW_SKIP_3"
#define PANEL_DISPLAY_MODE_720x1544_24HS_120HS_TE_HW_SKIP_4       "720x1544_24HS_120HS_TE_HW_SKIP_4"
#define PANEL_DISPLAY_MODE_720x1544_10HS_120HS_TE_HW_SKIP_11       "720x1544_10HS_120HS_TE_HW_SKIP_11"
#define PANEL_DISPLAY_MODE_720x1544_96HS      "720x1544_96HS"
#define PANEL_DISPLAY_MODE_720x1544_48HS_96HS_TE_HW_SKIP_1        "720x1544_48HS_96HS_TE_HW_SKIP_1"
#define PANEL_DISPLAY_MODE_720x1544_60NS      "720x1544_60NS"
#define PANEL_DISPLAY_MODE_720x1544_48NS      "720x1544_48NS"
#define PANEL_DISPLAY_MODE_720x1544_30NS      "720x1544_30NS"

#define PANEL_DISPLAY_MODE_1600x2560_60HS    "1600x2560_60HS"
#define PANEL_DISPLAY_MODE_1600x2560_90HS    "1600x2560_90HS"
#define PANEL_DISPLAY_MODE_1600x2560_30NS	 "1600x2560_30NS"

#define PCTRL_NONE	0
#define PCTRL_DELAY_MSLEEP	1
#define PCTRL_DELAY_USLEEP	2
#define PCTRL_REGULATOR_ENABLE	3
#define PCTRL_REGULATOR_DISABLE	4
#define PCTRL_REGULATOR_SET_VOLTAGE	5
#define PCTRL_REGULATOR_SSD_CURRENT	6
#define PCTRL_GPIO_ENABLE	7
#define PCTRL_GPIO_DISABLE	8

#define REGULATOR_TYPE_NONE 0
#define REGULATOR_TYPE_PWR 1
#define REGULATOR_TYPE_SSD 2

#define PANEL_ID(x) x
#define PANEL_MASK(x) x

#endif /* _DT_BINDINGS_PANEL_DISPLAY_H */
