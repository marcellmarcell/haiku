/*
 * Copyright 2008, Haiku Inc. All rights reserved.
 * Distributed under the terms of the MIT License.
 *
 * Authors:
 *		Michael Lotz <mmlr@mlotz.ch>
 */
#ifndef _USB_DISK_H_
#define _USB_DISK_H_


#include <lock.h>
#include <USB3.h>
#include <usb/USB_massbulk.h>


#define MAX_LOGICAL_UNIT_NUMBER		15
#define ATAPI_COMMAND_LENGTH		12

#define SYNC_SUPPORT_RELOAD			5

typedef struct device_lun_s device_lun;

// holds common information about an attached device (pointed to by luns)
typedef struct disk_device_s {
	usb_device	device;
	uint32		device_number;
	bool		removed;
	uint32		open_count;
	mutex		lock;
	void *		link;

	// device state
	usb_pipe	bulk_in;
	usb_pipe	bulk_out;
	uint8		interface;
	uint32		current_tag;
	uint8		sync_support;
	bool		tur_supported;
	bool		is_atapi;

	// used to store callback information
	sem_id		notify;
	status_t	status;
	size_t		actual_length;

	// logical units of this device
	uint8		lun_count;
	device_lun **luns;
} disk_device;


// represents a logical unit on the pointed to device - this gets published
struct device_lun_s {
	disk_device *device;
	char		name[32];
	uint8		logical_unit_number;
	bool		should_sync;

	// device information through read capacity/inquiry
	bool		media_present;
	bool		media_changed;
	uint32		block_count;
	uint32		block_size;
	uint8		device_type;
	bool		removable;
	bool		write_protected;

	char		vendor_name[8];
	char		product_name[16];
	char		product_revision[4];
};


#endif // _USB_DISK_H_
