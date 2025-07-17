/*
 * 86Box    A hypervisor and IBM PC system emulator that specializes in
 *          running old operating systems and software designed for IBM
 *          PC systems and compatibles from 1981 through fairly recent
 *          system designs based on the PCI bus.
 *
 *          This file is part of the 86Box distribution.
 *
 *          Implementation of Socket 7 (Single Voltage) machines.
 *
 *
 *
 * Authors: Miran Grca, <mgrca8@gmail.com>
 *
 *          Copyright 2016-2020 Miran Grca.
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <86box/86box.h>
#include <86box/mem.h>
#include <86box/io.h>
#include <86box/rom.h>
#include <86box/pci.h>
#include <86box/device.h>
#include <86box/chipset.h>
#include <86box/hdc.h>
#include <86box/hdc_ide.h>
#include <86box/keyboard.h>
#include <86box/flash.h>
#include <86box/sio.h>
#include <86box/hwm.h>
#include <86box/video.h>
#include <86box/spd.h>
#include "cpu.h"
#include <86box/machine.h>
#include <86box/timer.h>
#include <86box/fdd.h>
#include <86box/fdc.h>
#include <86box/nvr.h>
#include <86box/plat_unused.h>
#include <86box/sound.h>
#include <86box/fdc_ext.h>

static void
machine_at_thor_gpio_init(void)
{
    uint32_t gpio = 0xffffe1cf;

    /* Register 0x0078 (Undocumented): */
    /* Bit 5: 0 = Multiplier. */
    /* Bit 4: 0 = Multiplier. */
    /*        1.5: 0, 0. */
    /*        3.0: 0, 1. */
    /*        2.0: 1, 0. */
    /*        2.5: 1, 1. */
    /* Bit 1: 0 = Error beep, 1 = No error. */
    if (cpu_dmulti <= 1.5)
        gpio |= 0xffff0000;
    else if ((cpu_dmulti > 1.5) && (cpu_dmulti <= 2.0))
        gpio |= 0xffff0020;
    else if ((cpu_dmulti > 2.0) && (cpu_dmulti <= 2.5))
        gpio |= 0xffff0030;
    else if (cpu_dmulti > 2.5)
        gpio |= 0xffff0010;

    /* Register 0x0079: */
    /* Bit 7: 0 = Clear password, 1 = Keep password. */
    /* Bit 6: 0 = NVRAM cleared by jumper, 1 = NVRAM normal. */
    /* Bit 5: 0 = CMOS Setup disabled, 1 = CMOS Setup enabled. */
    /* Bit 4: External CPU clock (Switch 8). */
    /* Bit 3: External CPU clock (Switch 7). */
    /*        50 MHz: Switch 7 = Off, Switch 8 = Off. */
    /*        60 MHz: Switch 7 = On, Switch 8 = Off. */
    /*        66 MHz: Switch 7 = Off, Switch 8 = On. */
    /* Bit 2: 0 = On-board audio absent, 1 = On-board audio present. */
    /* Bit 1: 0 = Soft-off capable power supply present, 1 = Soft-off capable power supply absent. */
    /* Bit 0: 0 = Reserved. */
    /* NOTE: A bit is read as 1 if switch is off, and as 0 if switch is on. */
    if (cpu_busspeed <= 50000000)
        gpio |= 0xffff0000;
    else if ((cpu_busspeed > 50000000) && (cpu_busspeed <= 60000000))
        gpio |= 0xffff0800;
    else if (cpu_busspeed > 60000000)
        gpio |= 0xffff1000;

    machine_set_gpio_default(gpio);
}

static void
machine_at_thor_common_init(const machine_t *model, int has_video)
{
    machine_at_common_init_ex(model, 2);
    machine_at_thor_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 2, 1);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 3, 2, 1);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);

    if (has_video && (gfxcard[0] == VID_INTERNAL))
        device_add(machine_get_vid_device(machine));

    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
}

static void
machine_at_p54tp4xe_common_init(const machine_t *model)
{
    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);
}

int
machine_at_mbp5nsp_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mbp5nsp/mb-p5n-sp-eval-rom-6330893893f81318384708.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_award_common_init(model);

    device_add(&i430nx_device);

    return ret;
}

int
machine_at_apchunterindustrial_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/apchunterindustrial/pnthi-07.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
	pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
	
	device_add(&i430fx_device);
	device_add(&keyboard_ps2_ami_device);
	device_add(&piix_device);
	device_add(&fdc37c665_device);
	device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_p54tp4xe_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p54tp4xe/t15i0302.awd",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_p54tp4xe_common_init(model);

    return ret;
}

int
machine_at_p54tp4xe_mr_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p54tp4xe/TRITON.BIO",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_p54tp4xe_common_init(model);

    return ret;
}

int
machine_at_presario8700_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/presario8700/PRED5T10.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

	machine_at_common_init_ex(model, 2);
	
    pci_init(PCI_CONFIG_TYPE_1 | FLAG_NO_BRIDGES);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
    pci_register_slot(0x02, PCI_CARD_VIDEO,       2, 0, 0, 0); /* Onboard */
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x04, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 01 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 02 */
    
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C931 | FDC37C93X_NORMAL));
    device_add(&intel_flash_bxb_device);
	spd_register(SPD_TYPE_EDO, 0x7, 256);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
	
	 if (fdc_current[0] == FDC_INTERNAL){
        fdd_set_turbo(0, 1);
        fdd_set_turbo(1, 1);
    }

    return ret;
}

int
machine_at_exp8551_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/exp8551/AMI20.BIO",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x14, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x11, PCI_CARD_NORMAL, 4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&w83787f_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_delloptigsplus_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/delloptigsplus/delloptigsplus.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 3, 2); /* Slot 02 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 2, 1); /* Slot 01 */
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 03 */
	pci_register_slot(0x10, PCI_CARD_VIDEO, 	  0, 0, 0, 0); /* Onboard */
    device_add(&i430fx_device);
    device_add(&piix_device);
	device_add(&dell_jumper_device);
    device_add(&keyboard_ps2_phoenix_pci_device);
    device_add(&pc87332_398_ide_fdcon_device);
    device_add(&intel_flash_bxt_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_delloptigmplus_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/delloptigmplus/delloptigmplus.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      4, 1, 3, 2); /* Slot 01 */
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 4, 2, 1); /* Slot 02 */
	pci_register_slot(0x10, PCI_CARD_VIDEO,       1, 3, 2, 4); /* Onboard  */
    device_add(&i430fx_device);
    device_add(&piix_device);
	device_add(&dell_jumper_device);
	device_add(&keyboard_ps2_phoenix_pci_device);
    device_add(&pc87332_398_ide_fdcon_device);
    device_add(&intel_flash_bxt_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    if (sound_card_current[0] == SOUND_INTERNAL)
        machine_snd = device_add(machine_get_snd_device(machine));

    return ret;
}

int
machine_at_ga586atep_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();
	
	machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
   
    device_add(&keyboard_at_ami_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&sst_flash_29ee010_device); 

    return ret;
}


static const device_config_t ga586atep_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ga586atep_aug95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50PG Revision 2.12 (08/22/1995)", .internal_name = "ga586atep_aug95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_aug95.bin", "" } },
            { .name = "Version 4.50PG Revision 2.16 (09/07/1995)", .internal_name = "ga586atep_sep95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_sep95.bin", "" } },
            { .name = "Version 4.50PG Revision 2.17 (10/02/1995)", .internal_name = "ga586atep_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_oct95.bin", "" } },
			{ .name = "Version 4.50PG Revision 2.19 (10/19/1995)", .internal_name = "ga586atep_oct95_2", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_oct95_2.bin", "" } },
			{ .name = "Version 4.50PG Revision 2.20 (11/21/1995)", .internal_name = "ga586atep_nov95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_nov95.bin", "" } },
			{ .name = "Version 4.50PG Revision 2.22 (11/28/1995)", .internal_name = "ga586atep_nov95_2", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_nov95_2.bin", "" } },
			{ .name = "Version 4.50PG Revision 2.25 (12/29/1995)", .internal_name = "ga586atep_dec95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_dec95.bin", "" } },
			{ .name = "Version 4.50PG Revision 2.30 (04/12/1996)", .internal_name = "ga586atep_apr96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_apr96.bin", "" } },
			{ .name = "Version 4.51PG Revision 2.31 (05/06/1998)", .internal_name = "ga586atep_may98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_may98.bin", "" } },
			{ .name = "MR BIOS Version 3.31 (05/07/1996)", .internal_name = "ga586atep_may96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586atep/ga586atep_may96.bin", "" } },			  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t ga586atep_device = {
    .name          = "Gigabyte GA-586ATE/P",
    .internal_name = "ga586atep",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ga586atep_config[0]
};

static void
machine_at_hpvectravexxx_gpio_init(void)
{
    uint32_t gpio = 0x40;

     if (cpu_busspeed <= 40000000)
      gpio |= 0x30;
        else if ((cpu_busspeed > 40000000) && (cpu_busspeed <= 50000000))
        gpio |= 0x00;
        else if ((cpu_busspeed > 50000000) && (cpu_busspeed <= 60000000))
         gpio |= 0x20;
        else if (cpu_busspeed > 60000000)
        gpio |= 0x10;

        if (cpu_dmulti <= 1.5)
        gpio |= 0x82;
        else if ((cpu_dmulti > 1.5) && (cpu_dmulti <= 2.0))
        gpio |= 0x02;
        else if ((cpu_dmulti > 2.0) && (cpu_dmulti <= 2.5))
        gpio |= 0x00;
        else if (cpu_dmulti > 2.5)
        gpio |= 0x80;

    machine_set_gpio_default(gpio);
}

int
machine_at_hpvectravexxx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hpvectravexxx/d3653.bin",
                            0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	machine_at_hpvectravexxx_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_VIDEO,       0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      3, 4, 1, 2);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_device);
	
	 if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_pwrmatep150166_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pwrmatep150166/B50TJ004.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x08, PCI_CARD_NORMAL,      0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
	pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 02 */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_ready9616_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ready9616/B50U5007.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x08, PCI_CARD_NORMAL,      0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
	pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 02 */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_readypowerplayer_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/readypowerplayer/B50NM00V.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x08, PCI_CARD_NORMAL,      0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
	pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 02 */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_simaticpcri25_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/simaticpcri25/RI45_V35.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 0, 0, 0); /* Onboard */
	pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
	pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 02 */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_sy5tc_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();
	
	machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL, 1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x12, PCI_CARD_NORMAL, 2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x13, PCI_CARD_NORMAL, 3, 4, 1, 2); /* Slot 03 */
    pci_register_slot(0x14, PCI_CARD_NORMAL, 4, 1, 2, 3); /* Slot 04 */
   
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_no_mirq_device);
    device_add(&um8669f_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}


static const device_config_t sy5tc_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "5tc_oct95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50PG Revision UA0 (10/26/1995)", .internal_name = "5tc_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_oct95.bin", "" } },
            { .name = "Version 4.50PG Revision MD2.00.01P (11/13/1995, MaxData OEM, 5TC2)", .internal_name = "5tc_nov95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_nov95.bin", "" } },
			{ .name = "Version 4.50PG Revision MD2.00.01P (11/13/1995, MaxData OEM, 5TS2)", .internal_name = "5tc_nov952", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_nov952.bin", "" } },
            { .name = "Version 4.51PG Revision 2.1 (01/20/1996, TCS)", .internal_name = "5tc_jan96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_jan96.bin", "" } },
			{ .name = "Version 4.51PG Revision 2.2 (01/27/1996, TCU)", .internal_name = "5tc_jan962", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_jan962.bin", "" } }, 
			{ .name = "Version 4.51PG Revision MD2.10P (02/07/1996, MaxData OEM, 5TCU0/5TCU2)", .internal_name = "5tc_feb96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_feb96.bin", "" } },
			{ .name = "Version 4.51PG Revision 2.3 (04/09/1996, TCU)", .internal_name = "5tc_apr96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_apr96.bin", "" } },  
			{ .name = "Version 4.51PG Revision SA0201 (02/01/1997, TCS)", .internal_name = "5tc_jan97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_jan97.bin", "" } },
			{ .name = "Version 4.51PG Revision 1015 (10/14/1997, TCS)", .internal_name = "5tc_oct97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5tc/5tc_oct97.bin", "" } }, 
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t sy5tc_device = {
    .name          = "Soyo SY-5TC",
    .internal_name = "5tc",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &sy5tc_config[0]
};

int
machine_at_sy5td2_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();
	
	machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL, 1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x12, PCI_CARD_NORMAL, 2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x13, PCI_CARD_NORMAL, 3, 4, 1, 2); /* Slot 03 */
    pci_register_slot(0x14, PCI_CARD_NORMAL, 4, 1, 2, 3); /* Slot 04 */
   
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_no_mirq_device);
    device_add(&pc87332_398_ide_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

static const device_config_t sy5td2_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "5td2",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50PG Revision A4 (07/12/1995)", .internal_name = "5td2", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/5td2/5TA4S.bin", "" } },
            { .name = "Version 4.50PG Revision A4i (07/17/1995)", .internal_name = "5td2_2", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5td2/5TA4I.bin", "" } },
            { .name = "Version 4.51PG Revision NA201 (02/01/1997)", .internal_name = "5td2_feb97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5td2/5TAN0201.bin", "" } },
			{ .name = "Version 4.51PG Revision TAN-0823 (08/22/1997)", .internal_name = "5td2_aug97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5td2/5tan0823.bin", "" } }, 
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t sy5td2_device = {
    .name          = "Soyo SY-5TD2 (MaxData OEM)",
    .internal_name = "5td2",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &sy5td2_config[0]
};

int
machine_at_pb5400d_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pb5400d/P54D203I.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */

    device_add(&umc_8890_device);
    device_add(&umc_8886af_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_gw2katx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/gw2katx/1003CN0T.BIO",
                                    "roms/machines/gw2katx/1003CN0T.BI1",
                                    0x20000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_thor_common_init(model, 0);

    return ret;
}

int
machine_at_thor_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/thor/1006cn0_.bio",
                                    "roms/machines/thor/1006cn0_.bi1",
                                    0x20000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_thor_common_init(model, 1);

    return ret;
}

int
machine_at_mrthor_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mrthor/mr_atx.bio",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_thor_common_init(model, 0);

    return ret;
}

static void
machine_at_endeavor_gpio_init(void)
{
    uint32_t gpio = 0xffffe0cf;
    uint16_t addr;

    /* Register 0x0078 (Undocumented): */
    /* Bit 5,4: Vibra 16S base address: 0 = 220h, 1 = 260h, 2 = 240h, 3 = 280h. */
    device_context(machine_get_snd_device(machine));
    addr = device_get_config_hex16("base");
    switch (addr) {
        case 0x0220:
            gpio |= 0xffff00cf;
            break;
        case 0x0240:
            gpio |= 0xffff00ef;
            break;
        case 0x0260:
            gpio |= 0xffff00df;
            break;
        case 0x0280:
            gpio |= 0xffff00ff;
            break;
    }
    device_context_restore();

    /* Register 0x0079: */
    /* Bit 7: 0 = Clear password, 1 = Keep password. */
    /* Bit 6: 0 = NVRAM cleared by jumper, 1 = NVRAM normal. */
    /* Bit 5: 0 = CMOS Setup disabled, 1 = CMOS Setup enabled. */
    /* Bit 4: External CPU clock (Switch 8). */
    /* Bit 3: External CPU clock (Switch 7). */
    /*        50 MHz: Switch 7 = Off, Switch 8 = Off. */
    /*        60 MHz: Switch 7 = On, Switch 8 = Off. */
    /*        66 MHz: Switch 7 = Off, Switch 8 = On. */
    /* Bit 2: 0 = On-board audio absent, 1 = On-board audio present. */
    /* Bit 1: 0 = Soft-off capable power supply present, 1 = Soft-off capable power supply absent. */
    /* Bit 0: 0 = 2x multiplier, 1 = 1.5x multiplier (Switch 6). */
    /* NOTE: A bit is read as 1 if switch is off, and as 0 if switch is on. */
    if (cpu_busspeed <= 50000000)
        gpio |= 0xffff0000;
    else if ((cpu_busspeed > 50000000) && (cpu_busspeed <= 60000000))
        gpio |= 0xffff0800;
    else if (cpu_busspeed > 60000000)
        gpio |= 0xffff1000;

    if (sound_card_current[0] == SOUND_INTERNAL)
        gpio |= 0xffff0400;

    if (cpu_dmulti <= 1.5)
        gpio |= 0xffff0100;
    else
        gpio |= 0xffff0000;

    machine_set_gpio_default(gpio);
}

uint32_t
machine_at_endeavor_gpio_handler(uint8_t write, uint32_t val)
{
    uint32_t ret = machine_get_gpio_default();

    if (write) {
        ret &= ((val & 0xffffffcf) | 0xffff0000);
        ret |= (val & 0x00000030);
        if (machine_snd != NULL)  switch ((val >> 4) & 0x03) {
            case 0x00:
                sb_vibra16s_onboard_relocate_base(0x0220, machine_snd);
                break;
            case 0x01:
                sb_vibra16s_onboard_relocate_base(0x0260, machine_snd);
                break;
            case 0x02:
                sb_vibra16s_onboard_relocate_base(0x0240, machine_snd);
                break;
            case 0x03:
                sb_vibra16s_onboard_relocate_base(0x0280, machine_snd);
                break;
        }
        machine_set_gpio(ret);
    } else
        ret = machine_get_gpio();

    return ret;
}

int
machine_at_endeavor_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/endeavor/1006cb0_.bio",
                                    "roms/machines/endeavor/1006cb0_.bi1",
                                    0x1d000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_endeavor_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    if (sound_card_current[0] == SOUND_INTERNAL)
        machine_snd = device_add(machine_get_snd_device(machine));

    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

int
machine_at_stimpy_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/stimpy/intel.bin",
							0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	machine_at_endeavor_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    if (sound_card_current[0] == SOUND_INTERNAL)
        machine_snd = device_add(machine_get_snd_device(machine));
	
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

int
machine_at_ms5119_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms5119/A37EB.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0d, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0e, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0f, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83787f_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

static void
machine_at_pb640_gpio_init(void)
{
    uint32_t gpio = 0xffffe6ff;

    /* Register 0x0079: */
    /* Bit 7: 0 = Clear password, 1 = Keep password. */
    /* Bit 6: 0 = NVRAM cleared by jumper, 1 = NVRAM normal. */
    /* Bit 5: 0 = CMOS Setup disabled, 1 = CMOS Setup enabled. */
    /* Bit 4: External CPU clock (Switch 8). */
    /* Bit 3: External CPU clock (Switch 7). */
    /*        50 MHz: Switch 7 = Off, Switch 8 = Off. */
    /*        60 MHz: Switch 7 = On, Switch 8 = Off. */
    /*        66 MHz: Switch 7 = Off, Switch 8 = On. */
    /* Bit 2: No Connect. */
    /* Bit 1: No Connect. */
    /* Bit 0: 2x multiplier, 1 = 1.5x multiplier (Switch 6). */
    /* NOTE: A bit is read as 1 if switch is off, and as 0 if switch is on. */
    if (cpu_busspeed <= 50000000)
        gpio |= 0xffff00ff;
    else if ((cpu_busspeed > 50000000) && (cpu_busspeed <= 60000000))
        gpio |= 0xffff08ff;
    else if (cpu_busspeed > 60000000)
        gpio |= 0xffff10ff;

    if (cpu_dmulti <= 1.5)
        gpio |= 0xffff01ff;
    else
        gpio |= 0xffff00ff;

    machine_set_gpio_default(gpio);
}

int
machine_at_pb640_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/pb640/1007CP0R.BIO",
                                    "roms/machines/pb640/1007CP0R.BI1", 0x1d000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_pb640_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 2, 1, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&i430fx_rev02_device);
    device_add(&piix_rev02_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

int
machine_at_mb500n_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mb500n/031396s.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x14, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x11, PCI_CARD_NORMAL, 4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_no_mirq_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_fmb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/fmb/P5IV183.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 2, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      4, 3, 2, 1);

    device_add(&i430fx_device);
    device_add(&piix_no_mirq_device);
    device_add(&keyboard_at_ami_device);
    device_add(&w83787f_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_acerm3a_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acerm3a/r01-b3.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x10, PCI_CARD_VIDEO,       4, 0, 0, 0);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C935 | FDC37C93X_NORMAL));

    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_ap53_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ap53/ap53r2c0.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_VIDEO,       1, 2, 3, 4);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c669_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_8500tuc_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/8500tuc/Tuc0221b.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&um8669f_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_hpruby_init(const machine_t *model)
{
    int ret;

	ret = bios_load_linear("roms/machines/hpruby/intel.bin",
							0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	machine_at_thor_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 2, 1, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_ms5141_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms5141/W5141MS_1.1_120396.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x02, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x03, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x04, PCI_CARD_NORMAL,      0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x10, PCI_CARD_SOUTHBRIDGE, 4, 1, 2, 3);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83877f_president_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_m550d_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m550d/M550_08DH.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      4, 1, 2, 3);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c669_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_d943_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();
	
	machine_at_common_init_ex(model, 2);
	device_add(&amstrad_megapc_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 	  0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 	  0, 0, 0, 0);
	pci_register_slot(0x08, PCI_CARD_VIDEO,	  	      4, 0, 0, 0);
	pci_register_slot(0x11, PCI_CARD_NORMAL,          3, 2, 4, 1);
	pci_register_slot(0x12, PCI_CARD_NORMAL,	      2, 1, 3, 4);
	pci_register_slot(0x13, PCI_CARD_NORMAL,     	  1, 3, 2, 4);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);
	spd_register(SPD_TYPE_EDO, 0x7, 256);
	
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    if (sound_card_current[0] == SOUND_INTERNAL)
        machine_snd = device_add(machine_get_snd_device(machine));

    return ret;
}

static const device_config_t d943_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d943_oct96",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.05 Revision 1.02.943 (10/28/1996)", .internal_name = "d943_oct96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d943/d943_oct96.bin", "" } },
            { .name = "Version 4.05 Revision 1.03.943 (12/12/1996)", .internal_name = "d943_dec96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d943/d943_dec96.bin", "" } },
			{ .name = "Version 4.05 Revision 1.05.943 (09/04/1997)", .internal_name = "d943_sept97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d943/d943_sept97.bin", "" } },
            { .name = "Version 4.05 Revision 1.06.943 (10/29/1997)", .internal_name = "d943_oct97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d943/d943_oct97.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t d943_device = {
    .name          = "Siemens-Nixdorf D943",
    .internal_name = "d943",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d943_config[0]
};

int
machine_at_p55t2s_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p55t2s/s6y08t.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&i430hx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_p5vxb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p5vxb/P5VXB10.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x05, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    device_add(&i430vx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83877f_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_pctheatre9xxx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pctheatre9xxx/PRED5T50.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	
    pci_init(PCI_CONFIG_TYPE_1 | FLAG_NO_BRIDGES);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
    pci_register_slot(0x02, PCI_CARD_VIDEO,       2, 0, 0, 0); /* Onboard */
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x04, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 01 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 02 */
    
    device_add(&i430vx_device);
    device_add(&piix3_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C931 | FDC37C93X_NORMAL));
    device_add(&intel_flash_bxb_device);
	spd_register(SPD_TYPE_EDO, 0x7, 256);
	
	 if (fdc_current[0] == FDC_INTERNAL){
        fdd_set_turbo(0, 1);
        fdd_set_turbo(1, 1);
    }

    return ret;
}

int
machine_at_md45600_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/md45600/PRED5T40.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	
    pci_init(PCI_CONFIG_TYPE_1 | FLAG_NO_BRIDGES);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
    pci_register_slot(0x02, PCI_CARD_VIDEO,       2, 0, 0, 0); /* Onboard */
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x04, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 01 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 02 */
    
    device_add(&i430vx_device);
    device_add(&piix3_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C931 | FDC37C93X_NORMAL));
    device_add(&intel_flash_bxb_device);
	spd_register(SPD_TYPE_EDO, 0x7, 256);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
	
	 if (fdc_current[0] == FDC_INTERNAL){
        fdd_set_turbo(0, 1);
        fdd_set_turbo(1, 1);
    }

    return ret;
}

int
machine_at_presario4760_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    machine_at_common_init_ex(model, 2);
	
    pci_init(PCI_CONFIG_TYPE_1 | FLAG_NO_BRIDGES);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
    pci_register_slot(0x02, PCI_CARD_VIDEO,       2, 0, 0, 0); /* Onboard */
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x04, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 01 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 02 */
    
    device_add(&i430vx_device);
    device_add(&piix3_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C931 | FDC37C93X_NORMAL));
    device_add(&intel_flash_bxb_device);
	spd_register(SPD_TYPE_EDO, 0x7, 256);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
	
	 if (fdc_current[0] == FDC_INTERNAL){
        fdd_set_turbo(0, 1);
        fdd_set_turbo(1, 1);
    }

    return ret;
}

static const device_config_t presario4760_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "presario4760_dec96",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "BIOS (12/19/1996)", .internal_name = "presario4760_dec96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/presario4760/presario4760_dec96.bin", "" } },
            { .name = "BIOS (06/23/1997)", .internal_name = "presario4760_jun97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/presario4760/presario4760_jun97.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t presario4760_device = {
    .name          = "Compaq Presario 4760",
    .internal_name = "presario4760",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &presario4760_config[0]
};
/*
int
machine_at_ergoprovx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ergoprovx/E452.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	device_add(&amstrad_megapc_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x05, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0C, PCI_CARD_SOUTHBRIDGE, 3, 4, 1, 2);
    device_add(&i430vx_device);
    device_add(&piix3_device);
	device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}
*/

int
machine_at_gw2kma_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined2("roms/machines/gw2kma/1007DQ0T.BIO",
                                     "roms/machines/gw2kma/1007DQ0T.BI1",
                                     "roms/machines/gw2kma/1007DQ0T.BI2",
                                     "roms/machines/gw2kma/1007DQ0T.BI3",
                                     "roms/machines/gw2kma/1007DQ0T.RCV",
                                     0x3a000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);

    if ((sound_card_current[0] == SOUND_INTERNAL) && machine_get_snd_device(machine)->available())
        machine_snd = device_add(machine_get_snd_device(machine));

    device_add(&i430vx_device);
    device_add(&piix3_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C932 | FDC37C93X_FR));
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

static const device_config_t ap5s_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "ap5s",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "04/22/96 1.20 4.50PG", .internal_name = "ap5s_450pg", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ap5s/ap5s120.bin", "" } },
            { .name = "11/13/96 1.50 4.51PG", .internal_name = "ap5s", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ap5s/AP5S150.BIN", "" } },
            { .name = "06/25/97 1.60 4.51PG", .internal_name = "ap5s_latest", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ap5s/ap5s160.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t ap5s_device = {
    .name          = "AOpen AP5S",
    .internal_name = "ap5s_device",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = ap5s_config
};

int
machine_at_ap5s_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);

    device_add(&sis_5511_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_fm562_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/fm562/PR11_US.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4);

    device_add(&sis_5511_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c669_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_ms5118_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();
	
	machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 1, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 2, 1, 4);

    device_add(&sis_5511_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&w83787f_ide_device);
    device_add(&winbond_flash_w29c010_device); 

    return ret;
}

static const device_config_t ms5118_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ms5118_sep95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "AMIBIOS Version AF78 (09/29/1995)", .internal_name = "ms5118_sep95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ms5118/ms5118_sep95.bin", "" } },
            { .name = "AwardBIOS Core Ver. 4.50PG Version MD2.00.01P (11/07/1995, MaxData OEM)", .internal_name = "ms5118_nov95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ms5118/ms5118_nov95.bin", "" } },
            { .name = "AwardBIOS Core Ver. 4.50PG Version WF73 (12/28/1995, with PS/2)", .internal_name = "ms5118_dec95ps2", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ms5118/ms5118_dec95ps2.bin", "" } },
			{ .name = "AwardBIOS Core Ver. 4.50PG Version WF73PS2 (12/28/1995, with PS/2)", .internal_name = "ms5118_dec95ps2", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ms5118/ms5118_dec95ps2.bin", "" } },
			{ .name = "AwardBIOS Core Ver. 4.50PG Version WF73 (12/28/1995, no PS/2)", .internal_name = "ms5118_dec95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ms5118/ms5118_dec95.bin", "" } },
			{ .name = "AwardBIOS Core Ver. 4.50PG Version MD2.00.04P (03/21/1996, MaxData OEM)", .internal_name = "ms5118_mar96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ms5118/ms5118_mar96.bin", "" } },
			{ .name = "AMIBIOS Version AF7D (04/18/1996)", .internal_name = "ms5118_apr96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ms5118/ms5118_apr96.bin", "" } }, 
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
	// clang-format on
};

const device_t ms5118_device = {
    .name          = "MSI MS-5118",
    .internal_name = "ms5118",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ms5118_config[0]
};

int
machine_at_ms5124_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms5124/AG77.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0xFE, 0xFF, 0, 0);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);

    device_add(&sis_5511_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&w83787f_88h_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_amis727_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/amis727/S727p.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0xFE, 0xFF, 0, 0);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);

    device_add(&sis_5511_device);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_vectra54_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/vectra54/GT0724.22",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_VIDEO,       0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      3, 4, 1, 2);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&s3_phoenix_trio64_onboard_pci_device);

    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C932 | FDC37C93X_NORMAL));
    device_add(&sst_flash_29ee010_device);

    return ret;
}

static const device_config_t c5sbm2_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "5sbm2",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "4.50GP (07/17/1995)", .internal_name = "5sbm2", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5sbm2/5SBM0717.BIN", "" } },
            { .name = "4.50PG (03/21/1996)", .internal_name = "5sbm2_450pg", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5sbm2/5SBM0326.BIN", "" } },
            { .name = "4.51PG (03/15/2000 Unicore Upgrade)", .internal_name = "5sbm2_451pg", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/5sbm2/2A5ICC3A.BIN", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t c5sbm2_device = {
    .name          = "Chaintech 5SBM/5SBM2 (M103)",
    .internal_name = "5sbm2_device",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = c5sbm2_config
};

int
machine_at_5sbm2_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&keyboard_at_ami_device);
    device_add(&sis_550x_device);
    device_add(&um8663af_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_pc140_6260_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pc140_6260/LYKT32A.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x14, PCI_CARD_VIDEO,       0, 0, 0, 0); /* Onboard video */

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5436_onboard_pci_device);

    device_add(&sis_5511_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c669_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}
