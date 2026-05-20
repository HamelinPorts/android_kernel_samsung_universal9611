// SPDX-License-Identifier: GPL-2.0
/*
 * Reader for the 6.12 ramcon log written to the Samsung debug-snapshot
 * "log_cachedump" sub-region (8 MiB at 0xF9810000 on Exynos 9611).
 *
 * The 6.12 kernel under bring-up (LineageOS 23.2 / kernel 6.12 rebase)
 * routes its earlycon-ram writer to that physical region instead of
 * the default log_kernel.  log_cachedump is declared persist=true in
 * 4.14's dss_items[], which means dbg_snapshot_init does NOT memset
 * it on recovery boot, and the 4.14 kernel doesn't write to it during
 * normal boot (only on panic for cache state).  Result: 6.12's content
 * survives the SoC reset + recovery boot untouched.
 *
 * On 4.14 recovery this driver:
 *   - snapshots the entire 8 MiB region into a vmalloc'd buffer at
 *     dbg_snapshot_init time (called by debug-snapshot.c right after
 *     sec_debug_save_last_kmsg)
 *   - exposes the snapshot as /proc/last_cachedump_kmsg
 *
 * Userspace reads /proc/last_cachedump_kmsg the same way it reads
 * /proc/last_kmsg.  Content is the raw bytes from the ramcon ring,
 * which is plain ASCII printk-style output written by 6.12.
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/vmalloc.h>
#include <asm/cacheflush.h>	/* __flush_dcache_area */

#define LOG_CACHEDUMP_PHYS	0xF9810000UL
#define LOG_CACHEDUMP_SIZE	0x800000UL	/* 8 MiB — see dt-bindings/.../debug-snapshot-table.h */

static char *last_cachedump_buffer;
static size_t last_cachedump_size;

void sec_debug_save_cachedump_log(void)
{
	void *base;

	/* memremap (not ioremap): log_cachedump is reserved normal-DRAM,
	 * not MMIO.  ioremap on arm64 refuses to map RAM and returns NULL. */
	base = memremap(LOG_CACHEDUMP_PHYS, LOG_CACHEDUMP_SIZE, MEMREMAP_WB);
	if (!base) {
		pr_err("sec_debug_cachedump: memremap 0x%lx failed\n",
		       LOG_CACHEDUMP_PHYS);
		return;
	}

	/* vmalloc (not kmalloc): LOG_CACHEDUMP_SIZE is 8 MiB which exceeds
	 * KMALLOC_MAX_SIZE on this kernel.  vmalloc is fine — read-only
	 * single-reader via procfs, no performance pressure. */
	last_cachedump_buffer = vmalloc(LOG_CACHEDUMP_SIZE);
	if (last_cachedump_buffer) {
		memcpy(last_cachedump_buffer, base, LOG_CACHEDUMP_SIZE);
		last_cachedump_size = LOG_CACHEDUMP_SIZE;
		pr_info("sec_debug_cachedump: snapshot OK (%zu bytes)\n",
			last_cachedump_size);
	} else {
		pr_err("sec_debug_cachedump: vmalloc %zu failed\n",
		       (size_t)LOG_CACHEDUMP_SIZE);
	}
	memunmap(base);
}

static ssize_t last_cachedump_kmsg_read(struct file *file, char __user *buf,
					size_t len, loff_t *offset)
{
	loff_t pos = *offset;
	ssize_t count;

	if (!last_cachedump_buffer || pos >= last_cachedump_size)
		return 0;

	count = min(len, (size_t)(last_cachedump_size - pos));
	if (copy_to_user(buf, last_cachedump_buffer + pos, count))
		return -EFAULT;

	*offset += count;
	return count;
}

/*
 * Writing ANY content to /proc/last_cachedump_kmsg clears the 8 MiB
 * DRAM-backed log_cachedump region AND the userspace-visible vmalloc
 * snapshot.  Independent of clear_pmu_inform4 — clearing the log
 * surface does not touch the bouncer flag.
 *
 * The DRAM region is mapped WB-cacheable; __flush_dcache_area after
 * memset guarantees DRAM commit before the write() returns, so a
 * subsequent panic + warm reset sees zeroes from offset 0 (matters
 * because ramcon_setup only clears the first 256 KiB of the region —
 * the upper 7.75 MiB is cleared by ramcon_upgrade_init at a later
 * initcall, which an early-panic boot never reaches).
 */
static ssize_t last_cachedump_kmsg_write(struct file *file,
					 const char __user *buf,
					 size_t count, loff_t *ppos)
{
	void *base;

	base = memremap(LOG_CACHEDUMP_PHYS, LOG_CACHEDUMP_SIZE, MEMREMAP_WB);
	if (!base) {
		pr_err("sec_debug_cachedump: memremap 0x%lx failed on clear\n",
		       LOG_CACHEDUMP_PHYS);
		return -EIO;
	}
	memset(base, 0, LOG_CACHEDUMP_SIZE);
	__flush_dcache_area(base, LOG_CACHEDUMP_SIZE);
	memunmap(base);

	if (last_cachedump_buffer)
		memset(last_cachedump_buffer, 0, last_cachedump_size);

	pr_info("sec_debug_cachedump: cleared 8 MiB log_cachedump (%zu bytes from userspace)\n",
		count);
	return count;
}

static const struct file_operations last_cachedump_kmsg_fops = {
	.owner = THIS_MODULE,
	.read = last_cachedump_kmsg_read,
	.write = last_cachedump_kmsg_write,
};

static int __init last_cachedump_kmsg_proc_init(void)
{
	struct proc_dir_entry *entry;

	if (!last_cachedump_buffer) {
		pr_info("sec_debug_cachedump: no snapshot — skipping proc node\n");
		return 0;
	}
	/* Mode 0660 so root can write (clear); group ownership is set by
	 * vendor init.rc (chgrp system) like the other sec_debug nodes. */
	entry = proc_create("last_cachedump_kmsg", 0660, NULL,
			    &last_cachedump_kmsg_fops);
	if (entry)
		proc_set_size(entry, last_cachedump_size);
	return 0;
}
late_initcall(last_cachedump_kmsg_proc_init);
