#include "pmucal_cp.h"
#include "pmucal_rae.h"

static void pmucal_cp_dump_sfrs(const char *when);

/**
 *  pmucal_cp_init - init cp.
 *		        exposed to PWRCAL interface.

 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_cp_init(void)
{
	int ret;

	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.init) {
		pr_err("%s there is no sequence element for cp-init.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	pmucal_cp_dump_sfrs("pre-cp-init");

	ret = pmucal_rae_handle_cp_seq(pmucal_cp_list.init,
				pmucal_cp_list.num_init);
	if (ret) {
		pr_err("%s %s: error on handling cp-init sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_cp_status - get cp status.
 *		        exposed to PWRCAL interface.

 *  Returns 1 when the cp is initialized, 0 when not.
 *  Otherwise, negative error code.
 */
int pmucal_cp_status(void)
{
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.status) {
		pr_err("%s there is no sequence element for cp-status.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	pmucal_rae_handle_cp_seq(pmucal_cp_list.status,
				pmucal_cp_list.num_status);

	if (pmucal_cp_list.status->value == 0x5)
		return 1;
	else
		return 0;
}

/**
 *  pmucal_cp_reset_assert - reset assert cp.
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_cp_reset_assert(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.reset_assert) {
		pr_err("%s there is no sequence element for cp-reset_assert.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_cp_seq(pmucal_cp_list.reset_assert,
				pmucal_cp_list.num_reset_assert);
	if (ret) {
		pr_err("%s %s: error on handling cp-reset_assert sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_cp_reset_release - reset_release cp.
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_cp_reset_release(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.reset_release) {
		pr_err("%s there is no sequence element for cp-reset_release.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_cp_seq(pmucal_cp_list.reset_release,
				pmucal_cp_list.num_reset_release);
	if (ret) {
		pr_err("%s %s: error on handling cp-reset_release sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_cp_active_clear - cp_active clear
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_cp_active_clear(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.cp_active_clear) {
		pr_err("%s there is no sequence element for cp_active_clear.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_cp_seq(pmucal_cp_list.cp_active_clear,
				pmucal_cp_list.num_cp_active_clear);
	if (ret) {
		pr_err("%s %s: error on handling cp_active_clear sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_cp_enable_dump_pc_no_pg - enable cp dump_pc_no_pg
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_cp_enable_dump_pc_no_pg(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.cp_enable_dump_pc_no_pg) {
		pr_err("%s there is no sequence element for cp_enable_dump_pc_no_pg.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_cp_seq(pmucal_cp_list.cp_enable_dump_pc_no_pg,
				pmucal_cp_list.num_cp_enable_dump_pc_no_pg);
	if (ret) {
		pr_err("%s %s: error on handling cp_enable_dump_pc_no_pg sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_cp_disable_dump_pc_no_pg - enable cp dump_pc_no_pg
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_cp_disable_dump_pc_no_pg(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.cp_disable_dump_pc_no_pg) {
		pr_err("%s there is no sequence element for cp_disable_dump_pc_no_pg.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_cp_seq(pmucal_cp_list.cp_disable_dump_pc_no_pg,
				pmucal_cp_list.num_cp_disable_dump_pc_no_pg);
	if (ret) {
		pr_err("%s %s: error on handling cp_disable_dump_pc_no_pg sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}
#define PMU_CP_CTRL_NS_OFFSET	0x30
#define PMU_CP_CTRL_S_OFFSET	0x34
#define SMC_ID		0x82000700
#define READ_CTRL	0x3
#define WRITE_CTRL	0x4

enum cp_control {
	CP_CTRL_S,
	CP_CTRL_NS,
};

int pmucal_is_cp_smc_regs(struct pmucal_seq *seq) {
	int use_smc = 0;
	if (seq->offset == PMU_CP_CTRL_S_OFFSET || seq->offset == PMU_CP_CTRL_NS_OFFSET)
		use_smc = 1;
	return use_smc;
}

static u32 pmucal_cp_smc_read_ctrl(int reg)
{
	u32 lo, hi, v;

	v = exynos_smc(SMC_ID, READ_CTRL, 0, reg);
	if (v & 0xffff)
		return 0xdead0000 | (v & 0xffff);
	lo = v >> 16;

	v = exynos_smc(SMC_ID, READ_CTRL, 1, reg);
	if (v & 0xffff)
		return 0xdead0000 | (v & 0xffff);
	hi = v >> 16;

	return (hi << 16) | lo;
}

static void pmucal_cp_dump_sfrs(const char *when)
{
	void __iomem *pmu;
	u32 off;

	if (!pmucal_cp_list.init || !pmucal_cp_list.num_init) {
		pr_info("%s%s: no cp-init seq; cannot resolve PMU base\n",
				PMUCAL_PREFIX, __func__);
		return;
	}

	/* every cp_init seq element is based at PMU PA 0x11860000 */
	pmu = pmucal_cp_list.init[0].base_va;
	if (!pmu) {
		pr_info("%s%s: PMU base_va not mapped\n", PMUCAL_PREFIX, __func__);
		return;
	}

	pr_info("%s cp-sfr-dump [%s]:\n", PMUCAL_PREFIX, when);
	pr_info("%s   CP_CTRL_NS              = 0x%08x (smc)\n",
			PMUCAL_PREFIX, pmucal_cp_smc_read_ctrl(CP_CTRL_NS));
	pr_info("%s   CP_CTRL_S               = 0x%08x (smc)\n",
			PMUCAL_PREFIX, pmucal_cp_smc_read_ctrl(CP_CTRL_S));
	pr_info("%s   CP_STAT          (0x038)= 0x%08x\n",
			PMUCAL_PREFIX, __raw_readl(pmu + 0x0038));
	pr_info("%s   CENTRAL_SEQ_CP   (0x284)= 0x%08x\n",
			PMUCAL_PREFIX, __raw_readl(pmu + 0x0284));
	pr_info("%s   RESET_SEQ_STATUS (0x504)= 0x%08x\n",
			PMUCAL_PREFIX, __raw_readl(pmu + 0x0504));
	for (off = 0x1320; off <= 0x1338; off += 4)
		pr_info("%s   SYS_PWR_CP       (0x%03x)= 0x%08x\n",
				PMUCAL_PREFIX, off, __raw_readl(pmu + off));
}

void pmucal_smc_read(struct pmucal_seq *seq, int update)
{
	u32 reg;
	u32 cp_ctrl;
	u32 cp_ctrl_low;
	u32 cp_ctrl_high;

	if (seq->offset == PMU_CP_CTRL_S_OFFSET) {
		reg = CP_CTRL_S;
	} else if (seq->offset == PMU_CP_CTRL_NS_OFFSET) {
		reg = CP_CTRL_NS;
	} else {
		pr_err("%s%s: ERR! it is not CP_CTRL_S, CP_CTRL_NS: 0x%08x\n", PMUCAL_PREFIX, __func__, seq->offset);
		return;
	}

	cp_ctrl = exynos_smc(SMC_ID, READ_CTRL, 0, reg);
	if (!(cp_ctrl & 0xffff)) {
		cp_ctrl >>= 16;
		cp_ctrl_low = cp_ctrl;
	} else {
		pr_err("%s%s: ERR! read Fail: %d\n", PMUCAL_PREFIX, __func__, cp_ctrl & 0xffff);
		return;
	}

	cp_ctrl = exynos_smc(SMC_ID, READ_CTRL, 1, reg);
	if (!(cp_ctrl & 0xffff)) {
		cp_ctrl >>= 16;
		cp_ctrl_high = cp_ctrl;
	} else {
		pr_err("%s%s: ERR! read Fail: %d\n", PMUCAL_PREFIX, __func__, cp_ctrl & 0xffff);
		return;
	}

	if (update)
		seq->value = (((cp_ctrl_high << 16) | cp_ctrl_low) & seq->mask);

	pr_info("%s%s\t%s = 0x%08x\n", PMUCAL_PREFIX, "smc_read",
			seq->sfr_name, ((cp_ctrl_high << 16) | cp_ctrl_low));
}

void pmucal_smc_write(struct pmucal_seq *seq)
{
	u32 reg;
	int ret = 0;
	u32 cp_ctrl;
	u32 cp_ctrl_low;
	u32 cp_ctrl_high;

	if (seq->offset == PMU_CP_CTRL_S_OFFSET) {
		reg = CP_CTRL_S;
	} else if (seq->offset == PMU_CP_CTRL_NS_OFFSET) {
		reg = CP_CTRL_NS;
	} else {
		pr_err("%s%s: ERR! it is not CP_CTRL_S, CP_CTRL_NS: 0x%08x\n", PMUCAL_PREFIX, __func__, seq->offset);
		return;
	}

	if (seq->mask == U32_MAX) {
		ret = exynos_smc(SMC_ID, WRITE_CTRL, seq->value, reg);
		if (ret > 0) {
			pr_err("%s%s: ERR! CP_CTRL Write Fail: %d\n", PMUCAL_PREFIX, __func__, ret);
			return;
		}
	} else {
		cp_ctrl = exynos_smc(SMC_ID, READ_CTRL, 0, reg);
		if (!(cp_ctrl & 0xffff)) {
			cp_ctrl >>= 16;
			cp_ctrl_low = cp_ctrl;
		} else {
			pr_err("%s%s: ERR! read Fail: %d\n", PMUCAL_PREFIX, __func__, cp_ctrl & 0xffff);
			return;
		}

		cp_ctrl = exynos_smc(SMC_ID, READ_CTRL, 1, reg);
		if (!(cp_ctrl & 0xffff)) {
			cp_ctrl >>= 16;
			cp_ctrl_high = cp_ctrl;
		} else {
			pr_err("%s%s: ERR! read Fail: %d\n", PMUCAL_PREFIX, __func__, cp_ctrl & 0xffff);
			return;
		}

		seq->value |= (((cp_ctrl_high << 16) | cp_ctrl_low) & ~seq->mask);

		ret = exynos_smc(SMC_ID, WRITE_CTRL, seq->value, reg);
		if (ret > 0) {
			pr_err("%s%s: ERR! CP_CTRL Write Fail: %d\n", PMUCAL_PREFIX, __func__, ret);
			return;
		}
	}

	pr_info("%s%s\t%s = 0x%08x\n", PMUCAL_PREFIX, "smc_write", seq->sfr_name, seq->value);
	pmucal_smc_read(seq, 0);
}

int pmucal_is_cp_regs(int reg) {
	int i;
	int is_cp_regs = 0;

	for (i = 0; i < pmucal_cp_list.num_init; i++) {
		if (reg == pmucal_cp_list.init[i].base_pa + pmucal_cp_list.init[i].offset) {
			is_cp_regs = 1;
			goto out;
		}
	}

	for (i = 0; i < pmucal_cp_list.num_reset_assert; i++) {
		if (reg == pmucal_cp_list.reset_assert[i].base_pa + pmucal_cp_list.reset_assert[i].offset) {
			is_cp_regs = 1;
			goto out;
		}
	}

	for (i = 0; i < pmucal_cp_list.num_reset_release; i++) {
		if (reg == pmucal_cp_list.reset_release[i].base_pa + pmucal_cp_list.reset_release[i].offset) {
			is_cp_regs = 1;
			goto out;
		}
	}

	for (i = 0; i < pmucal_cp_list.num_cp_active_clear; i++) {
		if (reg == pmucal_cp_list.cp_active_clear[i].base_pa + pmucal_cp_list.cp_active_clear[i].offset) {
			is_cp_regs = 1;
			goto out;
		}
	}

	for (i = 0; i < pmucal_cp_list.num_cp_reset_req_clear; i++) {
		if (reg == pmucal_cp_list.cp_reset_req_clear[i].base_pa + pmucal_cp_list.cp_reset_req_clear[i].offset) {
			is_cp_regs = 1;
			goto out;
		}
	}
out:
	return is_cp_regs;
}

/**
 *  pmucal_cp_reset_req_clear - cp_reset_req clear
 *		        exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int pmucal_cp_reset_req_clear(void)
{
	int ret;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list.cp_reset_req_clear) {
		pr_err("%s there is no sequence element for cp_reset_req_clear.\n",
				PMUCAL_PREFIX);
		return -ENOENT;
	}

	ret = pmucal_rae_handle_cp_seq(pmucal_cp_list.cp_reset_req_clear,
				pmucal_cp_list.num_cp_reset_req_clear);
	if (ret) {
		pr_err("%s %s: error on handling cp_reset_req_clear sequence.\n",
				PMUCAL_PREFIX, __func__);
		return ret;
	}

	return 0;
}

/**
 *  pmucal_cp_initialize - Initialize function of PMUCAL CP common logic.
 *		            exposed to PWRCAL interface.
 *
 *  Returns 0 on success. Otherwise, negative error code.
 */
int __init pmucal_cp_initialize(void)
{
	int ret = 0;
	pr_info("%s%s()\n", PMUCAL_PREFIX, __func__);

	if (!pmucal_cp_list_size) {
		pr_err("%s %s: there is no cp list. aborting init...\n",
				PMUCAL_PREFIX, __func__);
		return -ENOENT;
	}

	/* convert physical base address to virtual addr */
	ret = pmucal_rae_phy2virt(pmucal_cp_list.init,
				pmucal_cp_list.num_init);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting init...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_cp_list.status,
				pmucal_cp_list.num_status);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting status...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_cp_list.reset_assert,
				pmucal_cp_list.num_reset_assert);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting reset_assert...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_cp_list.reset_release,
				pmucal_cp_list.num_reset_release);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting reset_release...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_cp_list.cp_active_clear,
				pmucal_cp_list.num_cp_active_clear);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting cp_active_clear...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}

	ret = pmucal_rae_phy2virt(pmucal_cp_list.cp_reset_req_clear,
				pmucal_cp_list.num_cp_reset_req_clear);
	if (ret) {
		pr_err("%s %s: error on PA2VA conversion. aborting cp_reset_req_clear...\n",
				PMUCAL_PREFIX, __func__);
		goto out;
	}
out:
	return ret;
}
