/*
OVERHEAD_MAP.C

symbols in this file:
000CFB20 0010:
	_overhead_map_initialize (0000)
000CFB30 0010:
	_overhead_map_initialize_for_new_map (0000)
000CFB40 0010:
	_overhead_map_dispose_from_old_map (0000)
000CFB50 00e0:
	_overhead_map_post_rasterize (0000)
00270BD0 000c:
	_rdata_00270bd0 (0000)
00453CD0 0030:
	_bss_00453cd0 (0000)
*/

/* ---------- headers */

#include "cseries.h"

/* ---------- constants */

/* ---------- macros */

/* ---------- structures */

struct overhead_map_globals
{
	boolean enabled;
	byte alignment_pad;
	rectangle2d bitmap_bounds;
	boolean throttle_updates;
	boolean render_target_dirty;
	rectangle2d render_bounds;
	real_point2d viewport_size;
	volatile point2d bitmap_origin;
	long last_render_bounds_update_time;
	long reserved24;
	struct bitmap_data *bitmap;
	long reserved2c;
};

typedef char verify_overhead_map_globals_size[sizeof(struct overhead_map_globals) == 0x30 ? 1 : -1];
typedef char verify_overhead_map_enabled_offset[offsetof(struct overhead_map_globals, enabled) == 0x00 ? 1 : -1];
typedef char verify_overhead_map_bitmap_bounds_offset[offsetof(struct overhead_map_globals, bitmap_bounds) == 0x02 ? 1 : -1];
typedef char verify_overhead_map_throttle_updates_offset[offsetof(struct overhead_map_globals, throttle_updates) == 0x0A ? 1 : -1];
typedef char verify_overhead_map_render_target_dirty_offset[offsetof(struct overhead_map_globals, render_target_dirty) == 0x0B ? 1 : -1];
typedef char verify_overhead_map_render_bounds_offset[offsetof(struct overhead_map_globals, render_bounds) == 0x0C ? 1 : -1];
typedef char verify_overhead_map_viewport_size_offset[offsetof(struct overhead_map_globals, viewport_size) == 0x14 ? 1 : -1];
typedef char verify_overhead_map_bitmap_origin_offset[offsetof(struct overhead_map_globals, bitmap_origin) == 0x1C ? 1 : -1];
typedef char verify_overhead_map_last_update_offset[offsetof(struct overhead_map_globals, last_render_bounds_update_time) == 0x20 ? 1 : -1];
typedef char verify_overhead_map_bitmap_offset[offsetof(struct overhead_map_globals, bitmap) == 0x28 ? 1 : -1];

/* ---------- prototypes */

/* ---------- globals */

struct
{
	boolean bss_00453cd0;
	short data_853cd2;
	short data_853cd4;
	short data_853cd6;
	long data_853cd8;
	long data_853cdc;
	long data_853ce0;
	double data_853ce4;
	short data_853cec;
	short data_853cee;
	long data_853cf0;
	void* data_853cf8;
} overhead_map_globals;

/* ---------- public code */

void overhead_map_initialize(
	void)
{
	return;
}

void overhead_map_initialize_for_new_map(
	void)
{
	return;
}

void overhead_map_dispose_from_old_map(
	void)
{
	overhead_map_globals.bss_00453cd0 = FALSE;

	return;
}

void overhead_map_post_rasterize(
	void* a1,
	void* a2,
	double* a3)
{
	double* v1;
	void* v2;

	if (overhead_map_globals.bss_00453cd0)
	{
		overhead_map_globals.data_853ce4 = *a3;

		v1 = a3;
		v2 = overhead_map_globals.data_853cf8;
	}

	return;
}

/* ---------- private code */
