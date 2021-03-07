#include "gd32f150_pmu.h"

typedef struct
{
    uint32_t ctl;
    uint32_t cs;
} pmu_peripheral;

volatile pmu_peripheral * const pmu   = (pmu_peripheral *)0x40007000;

void pmu_enable_backupd_write()
{
    uint32_t pmu_ctl;

    pmu_ctl = pmu->ctl;
    pmu_ctl |= (1 << 8);
    pmu->ctl = pmu_ctl;
}