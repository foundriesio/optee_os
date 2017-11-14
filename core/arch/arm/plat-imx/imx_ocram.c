// SPDX-License-Identifier: BSD-2-Clause
/*
 * Copyright 2017 NXP
 *
 */

#include <kernel/panic.h>
#include <initcall.h>
#include <trace.h>
#include <mm/core_memprot.h>
#include <string.h>
#include <io.h>

#include <imx.h>
#include <imx_pm.h>

paddr_t iram_tlb_phys_addr = -1UL;

#ifdef CFG_MX7
static const paddr_t phys_addr_imx7[] = {
	AIPS1_BASE, AIPS2_BASE, AIPS3_BASE, 0
};
#endif

#if defined(CFG_MX7)
static void init_tz_ocram(void)
{
	/* Configure the Secure OCRAM granularity */
	vaddr_t  iomux_base;
	uint32_t val;
	uint32_t lock;

	iomux_base = (vaddr_t)phys_to_virt(IOMUXC_GPR_BASE, MEM_AREA_IO_SEC);

	val = read32(iomux_base + IOMUX_GPRx_OFFSET(IOMUX_GPR_OCRAM_ID));

	/* Configure the OCRAM Retention to start at offset 0 */
	val &= ~BM_IOMUX_GPR_OCRAM_S_TZ_ADDR;
	val |= IOMUX_GPR_OCRAM_S_TZ_ENABLE;

	lock = BM_IOMUX_GPR_OCRAM_S_TZ_ADDR | IOMUX_GPR_OCRAM_S_TZ_ENABLE;

	write32(val, (iomux_base + IOMUX_GPRx_OFFSET(IOMUX_GPR_OCRAM_ID)));

	/* Then lock configuration */
	write32(IOMUX_GPR_OCRAM_LOCK(lock) | val,
			(iomux_base + IOMUX_GPRx_OFFSET(IOMUX_GPR_OCRAM_ID)));
}

static TEE_Result init_ocram(void)
{
	struct tee_mmap_region map;
	const paddr_t *phys_addr;
	size_t        size_area;
	void          *iram_tlb_vaddr;

	DMSG("IRAM TLB phys addr = 0x%X", (uint32_t)iram_tlb_phys_addr);

	/* iram tlb already initialized */
	if (iram_tlb_phys_addr != (-1UL)) {
		return TEE_SUCCESS;
	}

	/* Initialize the Secure OCRAM */
	init_tz_ocram();

#ifdef CFG_MX7
	iram_tlb_phys_addr = TRUSTZONE_OCRAM_START + IRAM_TBL_OFFSET;
	phys_addr = phys_addr_imx7;
	size_area = AIPS1_SIZE; /* 4M for AIPS1/2/3 */
#endif

	iram_tlb_vaddr = phys_to_virt(iram_tlb_phys_addr,
					  MEM_AREA_TEE_COHERENT);

	/* 16KB */
	DMSG("%x %x\n", (uint32_t)iram_tlb_phys_addr,
			(uint32_t)iram_tlb_vaddr);
	memset(iram_tlb_vaddr, 0, 16 * 1024);

	do {
		map.pa = *phys_addr;
		map.va = (vaddr_t)phys_to_virt(map.pa, MEM_AREA_IO_SEC);
		map.region_size = CORE_MMU_DEVICE_SIZE;
		map.size = size_area;
		map.type = MEM_AREA_IO_SEC;
		map.attr = TEE_MATTR_VALID_BLOCK | TEE_MATTR_PRW |
				TEE_MATTR_GLOBAL | TEE_MATTR_SECURE |
				(TEE_MATTR_CACHE_NONCACHE <<
					TEE_MATTR_CACHE_SHIFT);
		map_memarea_sections(&map, (uint32_t *)iram_tlb_vaddr);
	} while (*(++phys_addr));

#ifdef CFG_MX7
	/* Note IRAM_S_BASE is not 1M aligned, so take care */
	map.pa = ROUNDDOWN(IRAM_S_BASE, CORE_MMU_DEVICE_SIZE);
	map.va = (vaddr_t)phys_to_virt(map.pa, MEM_AREA_TEE_COHERENT);
	map.region_size = CORE_MMU_DEVICE_SIZE;
	map.size = CORE_MMU_DEVICE_SIZE;
	map.type = MEM_AREA_TEE_COHERENT;
	map.attr = TEE_MATTR_VALID_BLOCK | TEE_MATTR_PRW | TEE_MATTR_GLOBAL |
				TEE_MATTR_SECURE | TEE_MATTR_PX;
	map_memarea_sections(&map, (uint32_t *)iram_tlb_vaddr);

	map.pa = GIC_BASE;
	map.va = (vaddr_t)phys_to_virt((paddr_t)GIC_BASE, MEM_AREA_IO_SEC);
	map.region_size = CORE_MMU_DEVICE_SIZE;
	map.size = CORE_MMU_DEVICE_SIZE;
	map.type = MEM_AREA_TEE_COHERENT;
	map.attr = TEE_MATTR_VALID_BLOCK | TEE_MATTR_PRW | TEE_MATTR_GLOBAL |
				TEE_MATTR_SECURE | TEE_MATTR_PX;
	map_memarea_sections(&map, (uint32_t *)iram_tlb_vaddr);


	/*
	 * Note: DRAM space is not mapped, DRAM is in auto-selfrefresh,
	 * If map DRAM in to MMU, mmu will access DRAM which
	 * hang system.
	 */
#endif

	return TEE_SUCCESS;
}
#else
static TEE_Result init_ocram(void)
{

	return TEE_SUCCESS;
}
#endif

service_init(init_ocram);
