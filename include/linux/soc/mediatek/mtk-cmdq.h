/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2018 MediaTek Inc.
 *
 */

#ifndef __MTK_CMDQ_H__
#define __MTK_CMDQ_H__

#include <linux/mailbox_client.h>
#include <linux/mailbox/mtk-cmdq-mailbox.h>
#include <linux/timer.h>

struct cmdq_pkt;

struct cmdq_client_reg {
	u8 subsys;
	u16 offset;
	u16 size;
};

/**
 * cmdq_dev_get_client_reg() - parse cmdq client reg from the device
 *			       node of CMDQ client
 * @dev:	device of CMDQ mailbox client
 * @client_reg: CMDQ client reg pointer
 * @idx:	the index of desired reg
 *
 * Return: 0 for success; else the error code is returned
 *
 * Help CMDQ client parsing the cmdq client reg
 * from the device node of CMDQ client.
 */
int cmdq_dev_get_client_reg(struct device *dev,
			    struct cmdq_client_reg *client_reg, int idx);

/**
 * cmdq_pkt_create() - create a CMDQ packet
 * @chan:	the mailbox channel
 * @size:	required CMDQ buffer size
 *
 * Return: CMDQ packet pointer
 */
struct cmdq_pkt *cmdq_pkt_create(struct mbox_chan *chan, size_t size);

/**
 * cmdq_pkt_destroy() - destroy the CMDQ packet
 * @chan:	the mailbox channel
 * @pkt:	the CMDQ packet
 */
void cmdq_pkt_destroy(struct mbox_chan *chan, struct cmdq_pkt *pkt);

/**
 * cmdq_pkt_write() - append write command to the CMDQ packet
 * @pkt:	the CMDQ packet
 * @subsys:	the CMDQ sub system code
 * @offset:	register offset from CMDQ sub system
 * @value:	the specified target register value
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_write(struct cmdq_pkt *pkt, u8 subsys, u16 offset, u32 value);

/**
 * cmdq_pkt_write_mask() - append write command with mask to the CMDQ packet
 * @pkt:	the CMDQ packet
 * @subsys:	the CMDQ sub system code
 * @offset:	register offset from CMDQ sub system
 * @value:	the specified target register value
 * @mask:	the specified target register mask
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_write_mask(struct cmdq_pkt *pkt, u8 subsys,
			u16 offset, u32 value, u32 mask);

/**
 * cmdq_pkt_wfe() - append wait for event command to the CMDQ packet
 * @pkt:	the CMDQ packet
 * @event:	the desired event type to "wait and CLEAR"
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_wfe(struct cmdq_pkt *pkt, u16 event);

/**
 * cmdq_pkt_clear_event() - append clear event command to the CMDQ packet
 * @pkt:	the CMDQ packet
 * @event:	the desired event to be cleared
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_clear_event(struct cmdq_pkt *pkt, u16 event);

/**
 * cmdq_pkt_set_event() - append set event command to the CMDQ packet
 * @pkt:	the CMDQ packet
 * @event:	the desired event to be set
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_set_event(struct cmdq_pkt *pkt, u16 event);

/**
 * cmdq_pkt_poll() - Append polling command to the CMDQ packet, ask GCE to
 *		     execute an instruction that wait for a specified
 *		     hardware register to check for the value w/o mask.
 *		     All GCE hardware threads will be blocked by this
 *		     instruction.
 * @pkt:	the CMDQ packet
 * @subsys:	the CMDQ sub system code
 * @offset:	register offset from CMDQ sub system
 * @value:	the specified target register value
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_poll(struct cmdq_pkt *pkt, u8 subsys,
		  u16 offset, u32 value);

/**
 * cmdq_pkt_poll_mask() - Append polling command to the CMDQ packet, ask GCE to
 *		          execute an instruction that wait for a specified
 *		          hardware register to check for the value w/ mask.
 *		          All GCE hardware threads will be blocked by this
 *		          instruction.
 * @pkt:	the CMDQ packet
 * @subsys:	the CMDQ sub system code
 * @offset:	register offset from CMDQ sub system
 * @value:	the specified target register value
 * @mask:	the specified target register mask
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_poll_mask(struct cmdq_pkt *pkt, u8 subsys,
		       u16 offset, u32 value, u32 mask);

/**
 * cmdq_pkt_assign() - Append logic assign command to the CMDQ packet, ask GCE
 *		       to execute an instruction that set a constant value into
 *		       internal register and use as value, mask or address in
 *		       read/write instruction.
 * @pkt:	the CMDQ packet
 * @reg_idx:	the CMDQ internal register ID
 * @value:	the specified value
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_assign(struct cmdq_pkt *pkt, u16 reg_idx, u32 value);

/**
 * cmdq_pkt_finalize() - Append EOC and jump command to pkt.
 * @pkt:	the CMDQ packet
 *
 * Return: 0 for success; else the error code is returned
 */
int cmdq_pkt_finalize(struct cmdq_pkt *pkt);

/**
 * cmdq_pkt_flush_async() - trigger CMDQ to asynchronously execute the CMDQ
 *                          packet and call back at the end of done packet
 * @chan:	the mailbox channel
 * @pkt:	the CMDQ packet
 *
 * Return: 0 for success; else the error code is returned
 *
 * Trigger CMDQ to asynchronously execute the CMDQ packet and call back
 * at the end of done packet. Note that this is an ASYNC function. When the
 * function returned, it may or may not be finished.
 */
int cmdq_pkt_flush_async(struct mbox_chan *chan, struct cmdq_pkt *pkt);

#endif	/* __MTK_CMDQ_H__ */
