/*
 * 86Box    A hypervisor and IBM PC system emulator that specializes in
 *          running old operating systems and software designed for IBM
 *          PC systems and compatibles from 1981 through fairly recent
 *          system designs based on the PCI bus.
 *
 *          This file is part of the 86Box distribution.
 *
 *          Implementation of Socket 5 machines.
 *
 *
 *
 * Authors: Miran Grca, <mgrca8@gmail.com>
 *
 *          Copyright 2016-2019 Miran Grca.
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
#include <86box/fdc_ext.h>
#include <86box/hdc.h>
#include <86box/hdc_ide.h>
#include "cpu.h"
#include <86box/timer.h>
#include <86box/fdd.h>
#include <86box/fdc.h>
#include <86box/keyboard.h>
#include <86box/flash.h>
#include <86box/nvr.h>
#include <86box/scsi_ncr53c8xx.h>
#include <86box/sio.h>
#include <86box/video.h>
#include <86box/machine.h>
#include <86box/sound.h>
#include <86box/apm.h>

int
machine_at_plato_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/plato/1016ax1_.bio",
                                    "roms/machines/plato/1016ax1_.bi1",
                                    0x1d000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_premiere_common_init(model, PCI_CAN_SWITCH_TYPE);

    device_add(&i430nx_device);

    return ret;
}

int
machine_at_dellplato_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear_inverted(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    machine_at_common_init_ex(model, 2);

    device_add(&amstrad_megapc_nvr_device);
    device_add(&ide_pci_2ch_device);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_CAN_SWITCH_TYPE);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_IDE,         0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      3, 2, 1, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 1, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	device_add(&i430nx_device);
    device_add(&keyboard_ps2_phoenix_device);
    device_add(&sio_zb_device);
    device_add(&ide_rz1000_pci_single_channel_device);
    device_add(&fdc37c665_ide_sec_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

static const device_config_t dellplato_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "a01",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "A01", .internal_name = "a01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dellplato/a01.rom", "" } },
            { .name = "A02", .internal_name = "a02", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dellplato/a02.rom", "" } },
            { .name = "A03", .internal_name = "a03", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dellplato/a03.rom", "" } },
			{ .name = "A04", .internal_name = "a04", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dellplato/a04.rom", "" } },
			{ .name = "A05", .internal_name = "a05", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dellplato/a05.rom", "" } },
			{ .name = "A06", .internal_name = "a06", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dellplato/a06.rom", "" } },
			{ .name = "S45", .internal_name = "s45", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dellplato/s45.rom", "" } },	
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t dellplato_device = {
    .name          = "Dell Dimension XPS Pxxx",
    .internal_name = "dellplato",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &dellplato_config[0]
};

int
machine_at_d842_init(const machine_t *model)

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

    device_add(&ide_pci_2ch_device);
    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x03, PCI_CARD_VIDEO,       4, 0, 0, 0); /* Onboard */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 02 */

    device_add(&keyboard_ps2_pci_device);
    device_add(&i430nx_device);
    device_add(&sio_zb_device);
    device_add(&fdc37c665_device);
	device_add(&amd_am28f010_flash_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static const device_config_t d842_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d842",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 1.03 Revision 1.03.842 (11/24/1994)", .internal_name = "d842", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d842/d842.bin", "" } },
            { .name = "Version 4.04 Revision 1.05.842 (03/15/1996)", .internal_name = "d842_mar96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d842/d842_mar96.bin", "" } },
            { .name = "Version 4.04 Revision 1.06.842 (04/03/1998)", .internal_name = "d842_apr98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d842/d842_apr98.bin", "" } },
			{ .name = "Version 4.04 Revision 1.07.842 (06/02/1998)", .internal_name = "d842_jun98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d842/d842_jun98.bin", "" } },
			{ .name = "Version 1.03 Revision 1.09.842 (07/08/1996)", .internal_name = "d842_jul96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d842/d842_jul96.bin", "" } },
			{ .name = "Version 1.03 Revision 1.10.842 (06/04/1998)", .internal_name = "d842_jun98_1", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d842/d842_jun98_1.bin", "" } },			  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t d842_device = {
    .name          = "Siemens-Nixdorf D842",
    .internal_name = "d842",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d842_config[0]
};

int
machine_at_ambradp90_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/ambradp90/1002AX1P.BIO",
                                    "roms/machines/ambradp90/1002AX1P.BI1",
                                    0x1d000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_premiere_common_init(model, PCI_CAN_SWITCH_TYPE);

    device_add(&i430nx_device);

    return ret;
}

int
machine_at_p54np4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p54np4/asus-642accdebcb75833703472.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ide_vlb_2ch_device);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_CAN_SWITCH_TYPE);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      1, 2, 3, 4); /* 06 = Slot 1 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      2, 3, 4, 1); /* 05 = Slot 2 */
    pci_register_slot(0x04, PCI_CARD_NORMAL,      3, 4, 1, 2); /* 04 = Slot 3 */
    pci_register_slot(0x03, PCI_CARD_NORMAL,      4, 1, 2, 3); /* 03 = Slot 4 */
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&i430nx_device);
    device_add(&sio_zb_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c665_ide_pri_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_pn2000_init(const machine_t *model)

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

    device_add(&ide_cmd640_pci_device);
	pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x01, PCI_CARD_IDE, 		  0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      4, 1, 2, 3); /* 06 = Slot 1 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      3, 2, 4, 1); /* 05 = Slot 2 */
    pci_register_slot(0x04, PCI_CARD_NORMAL,      2, 3, 4, 1); /* 04 = Slot 3 */
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 4, 3, 2); /* 03 = Slot 4 */
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);

    device_add(&keyboard_ps2_pci_device);
    device_add(&i430nx_device);
    device_add(&sio_zb_device);
    device_add(&pc87332_398_device);
	device_add(&intel_flash_bxt_device);
    device_add(&apm_pci_device);

    return ret;
}


static const device_config_t pn2000_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "pn2000_dec94",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Core Version 4.50G Version 1.54 (12/14/1994)", .internal_name = "pn2000_dec94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/pn2000/pn2000_dec94.bin", "" } },
            { .name = "Core Version 4.50G Version 1.56 (03/27/1995)", .internal_name = "pn2000_mar95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/pn2000/pn2000_mar95.bin", "" } },	  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t pn2000_device = {
    .name          = "FIC PN-2000",
    .internal_name = "pn2000",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &pn2000_config[0]
};

int
machine_at_586ip_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/586ip/IP.20",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_award_common_init(model);

    device_add(&i430nx_device);

    return ret;
}

int
machine_at_m54pi_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m54pi/BIOS.N19",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
    
    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 03 */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430nx_device);
    device_add(&sio_zb_device);
    device_add(&fdc37c665_device);
    device_add(&ide_cmd640_pci_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_tek932_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tek932/B932_019.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
    
    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_CAN_SWITCH_TYPE);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 3, 2, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430nx_device);
    device_add(&sio_zb_device);
    device_add(&ide_vlb_device);
    device_add(&fdc37c665_ide_pri_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

int
machine_at_coralmax3_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/coralmax3/003102.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
    
    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 03 */
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430nx_device);
    device_add(&sio_zb_device);
    device_add(&fdc37c665_device);
    device_add(&ide_rz1000_pci_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_acerv30_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acerv30/V30R01N9.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&keyboard_ps2_acer_pci_device);
    device_add(&fdc37c665_device);

    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_acerv30l_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acerv30l/V30L_1F4.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 3, 4, 1);
	pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&keyboard_ps2_acer_pci_device);
    device_add(&fdc37c665_device);

    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_apollo_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/apollo/S728P.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ami_1995_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87332_398_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

static void
machine_at_dimpxxxc_gpio_init(void)
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

static const device_config_t dimpxxxc_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "dimpxxxc_jul95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "Version A01 (03/31/1995, released 07/05/1995)", .internal_name = "dimpxxxc_jul95", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dimpxxxc/dimpxxxc_jul95.rom", "" } },
            { .name = "Version A02 (05/30/1995, released 08/01/1995)", .internal_name = "dimpxxxc_aug95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dimpxxxc/dimpxxxc_aug95.rom", "" } },
            { .name = "Version A04 (09/22/1995, released 10/24/1995)", .internal_name = "dimpxxxc_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dimpxxxc/dimpxxxc_oct95.rom", "" } },
			{ .name = "Version A05 (12/13/1995, released 12/28/1995)", .internal_name = "dimpxxxc_dec95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dimpxxxc/dimpxxxc_dec95.rom", "" } },
			{ .name = "Version A06 (05/13/1996, released 05/22/1996)", .internal_name = "dimpxxxc_may96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dimpxxxc/dimpxxxc_may96.rom", "" } },	
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t dimpxxxc_device = {
    .name          = "Dell Dimension XPS Pxxxc",
    .internal_name = "dimpxxxc",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = dimpxxxc_config
};

int
machine_at_dimpxxxc_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios"), 0);
    ret = bios_load_linear_inverted(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	machine_at_dimpxxxc_gpio_init();

	pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);

    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

static void
machine_at_dimpxxxt_gpio_init(void)
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

static const device_config_t dimpxxxt_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "dimpxxxt_feb96",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "Version A02 (10/27/1995, released 02/22/1996)", .internal_name = "dimpxxxt_feb96", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dimpxxxt/dimpxxxt_feb96.rom", "" } },
            { .name = "Version A03 (12/15/1995, released 02/22/1996)", .internal_name = "dimpxxxt_feb96_1", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/dimpxxxt/dimpxxxt_feb96_1.rom", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t dimpxxxt_device = {
    .name          = "Dell Dimension XPS Pxxxt",
    .internal_name = "dimpxxxt",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = dimpxxxt_config
};

int
machine_at_dimpxxxt_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios"), 0);
    ret = bios_load_linear_inverted(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	machine_at_dimpxxxt_gpio_init();

	pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);

    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	 if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static const device_config_t optiglplus_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "optiglplus_sep95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "Version A02 (09/20/1995)", .internal_name = "optiglplus_sep95", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optiglplus/optiglplus_sep95.rom", "" } },
            { .name = "Version A03 (10/03/1995, released 10/19/1995)", .internal_name = "optiglplus_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optiglplus/optiglplus_oct95.rom", "" } },
            { .name = "Version A06 (10/30/1995, released 11/03/1995)", .internal_name = "optiglplus_nov95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optiglplus/optiglplus_nov95.rom", "" } },
            { .files_no = 0 }
		},
	},	
    {
        .name = "dell_model_select",
        .description = "Machine type",
        .type = CONFIG_SELECTION,
        .default_int = 2,  // z. B. GXMT als Standard
        .selection = {
            { .description = "GL+",  .value = 0 },
            { .description = "GMT",  .value = 1 },
            { .description = "GXMT", .value = 2 },
            { .description = "" }  // Terminator
        }
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t optiglplus_device = {
    .name          = "Dell OptiPlex GL/GL+",
    .internal_name = "optiglplus",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = optiglplus_config
};

int
machine_at_optiglplus_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;
	int model_select;
	model_select = machine_get_config_int("dell_model_select");

switch (model_select) {
    case 0:
        device_add(&dell_jumper_gl_device);
        break;
    case 1:
        device_add(&dell_jumper_gmt_device);
        break;
    case 2:
        device_add(&dell_jumper_gxmt_device);
        break;
}
	

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 4, 2, 1);
    pci_register_slot(0x10, PCI_CARD_VIDEO,       0, 0, 0, 0);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87332_398_device);
    device_add(&intel_flash_bxt_device);
	
	 if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static const device_config_t optigxlgxm_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "optigxlgxm_dec95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "Version A10 (12/28/1995)", .internal_name = "optigxlgxm_dec95", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optigxlgxm/optigxlgxm_dec95.rom", "" } },
            { .name = "Version A11 (01/12/1996)", .internal_name = "optigxlgxm_jan96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optigxlgxm/optigxlgxm_jan96.rom", "" } },
            { .name = "Version A15 (02/15/1996)", .internal_name = "optigxlgxm_feb96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optigxlgxm/optigxlgxm_feb96.rom", "" } },
			{ .name = "Version A18 (07/29/1996)", .internal_name = "optigxlgxm_jul96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optigxlgxm/optigxlgxm_jul96.rom", "" } },
			{ .name = "Version A19 (09/19/1996)", .internal_name = "optigxlgxm_sep96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/optigxlgxm/optigxlgxm_sep96.rom", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t optigxlgxm_device = {
    .name          = "Dell OptiPlex GXL/GXM",
    .internal_name = "optigxlgxm",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = optigxlgxm_config
};

int
machine_at_optigxlgxm_init(const machine_t *model)
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
	device_add(&amstrad_megapc_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 4, 2, 1);
    pci_register_slot(0x10, PCI_CARD_VIDEO,       0, 0, 0, 0);
    device_add(&keyboard_ps2_phoenix_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
	device_add(&dell_jumper_device);
    device_add(&pc87332_device);
    device_add(&intel_flash_bxt_device);
	
	 if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    if (sound_card_current[0] == SOUND_INTERNAL)
        machine_snd = device_add(machine_get_snd_device(machine));

    return ret;
}

static void
machine_at_aladdin_gpio_init(void)
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
machine_at_aladdin_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/aladdin/intel.bin",
							0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_aladdin_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static void
machine_at_zappa_gpio_init(void)
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
machine_at_zappa_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/zappa/1006bs0_.bio",
                                    "roms/machines/zappa/1006bs0_.bi1",
                                    0x20000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_zappa_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

static void
machine_at_morrison32_hp_gpio_init(void)
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
machine_at_morrison32_hp_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/morrison32_hp/1011BT0L.BIO",
									"roms/machines/morrison32_hp/1011BT0L.BI1",
									0x20000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_morrison32_hp_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static void
machine_at_advt8100p_gpio_init(void)
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
machine_at_advt8100p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/advt8100p/1004BT0Q.BIO",
									"roms/machines/advt8100p/1004BT0Q.BI1",
									0x20000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_advt8100p_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static void
machine_at_morrison64_gpio_init(void)
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
machine_at_morrison64_init(const machine_t *model)
{
    int ret;

   ret = bios_load_linear_combined("roms/machines/morrison64/1006CA2L.BIO",
								   "roms/machines/morrison64/1006CA2L.BI1",
									0x20000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_morrison64_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static void
machine_at_morrisonmc_gpio_init(void)
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
machine_at_morrisonmc_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/morrisonmc/1002CH0L.BIO",
									"roms/machines/morrisonmc/1002CH0L.BI1",
									0x20000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_morrisonmc_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_pc330_65x6_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000c0000, 262144, 0);
    device_context_restore();

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_morrison64_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

static const device_config_t pc330_65x6_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "pc330_65x6_sep95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "LPKT49A (09/14/1995)", .internal_name = "pc330_65x6_sep95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_sep95.bin", "" } },
            { .name = "LPKT51A (10/05/1995)", .internal_name = "pc330_65x6_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_oct95.bin", "" } },
            { .name = "LPKT52A (10/13/1995)", .internal_name = "pc330_65x6_oct952", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_oct952.bin", "" } },
			{ .name = "LPKT53A (10/26/1995)", .internal_name = "pc330_65x6_oct953", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_oct953.bin", "" } },
			{ .name = "LPKT56A (12/20/1995)", .internal_name = "pc330_65x6_dec95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_dec95.bin", "" } },
			{ .name = "LPKT59A (04/24/1996)", .internal_name = "pc330_65x6_apr96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_apr96.bin", "" } },
			{ .name = "LPKT60A (11/15/1996)", .internal_name = "pc330_65x6_nov96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_nov96.bin", "" } },
			{ .name = "LPKT63A (02/04/1998)", .internal_name = "pc330_65x6_feb98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/pc330_65x6/pc330_65x6_feb98.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};


const device_t pc330_65x6_device = {
    .name          = "IBM PC330/350 type 65x6 (Intel Morrison64 OEM)",
    .internal_name = "pc330_65x6",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &pc330_65x6_config[0]
};

static void
machine_at_monaco_gpio_init(void)
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
machine_at_monaco_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/monaco/intel.bin",
                            0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    machine_at_monaco_gpio_init();

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       4, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&keyboard_ps2_intel_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87306_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

int
machine_at_ap5200if_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ap5200if/Lanner Electronics AP-5200IF BIOS (SST29EE010).bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);
	pci_register_slot(0x14, PCI_CARD_NORMAL,      2, 3, 4, 1);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_m54hi_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m54hi/M54HI_06.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 3, 4, 1);
	pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_powermatev_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/powermatev/BIOS.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 3, 4, 1);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_ready7022_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ready7022/B50MC00C.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C932 | FDC37C93X_NORMAL));
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_hawk_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hawk/HAWK.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ami_1994_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_tg_ami_pci_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}


int
machine_at_d858_init(const machine_t *model)

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
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
	pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 02 */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i430fx_rev02_device);
    device_add(&piix_rev02_device);
    device_add(&fdc37c665_device);
	device_add(&intel_flash_bxt_device);
	
	
	if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&sb_vibra16s_onboard_device); 

    return ret;
}


static const device_config_t d858_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d858",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.04 Revision 1.00.858 (04/25/1995)", .internal_name = "d858", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d858/d858.bin", "" } },
            { .name = "Version 4.04 Revision 1.07.858 (08/06/1996)", .internal_name = "d858_aug96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d858/d858aug96.bin", "" } },
            { .name = "Version 4.05 Revision 2.00.858 (09/24/1997)", .internal_name = "d858_sept97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d858/d858_sept97.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};


const device_t d858_device = {
    .name          = "Siemens-Nixdorf Scenic 5H/PCI",
    .internal_name = "scenic5h_pci",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d858_config[0]
};

int
machine_at_pt2000_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ficpt2000/PT2000_v1.01.BIN",
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
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    /* Should be VIA, but we do not emulate that yet. */
    device_add(&keyboard_ps2_holtek_device);
    device_add(&i430fx_device);
    device_add(&piix_device);
    device_add(&pc87332_398_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_mb1575pct_init(const machine_t *model)

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
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&opti5x7_pci_device);
    device_add(&opti822_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}


static const device_config_t mb1575pct_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "mb1575pct",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50G Revision 1500 PCT-A (08/31/1994)", .internal_name = "mb1575pct", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb1575pct/mb1575pct.bin", "" } },
            { .name = "Version 4.50G Revision 1500 PCT-A (08/31/1994, K6 Mod)", .internal_name = "mb1575pct_k6", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb1575pct/mb1575pct_k6.bin", "" } },
            { .name = "Version 4.50G Revision 1500 PCT-A (08/31/1994, K6 Mod + PCI Fix)", .internal_name = "mb1575pct_k6_pci", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb1575pct/mb1575pct_k6_pci.bin", "" } },
			{ .name = "Version 4.50G Revision 1500 PCT-B (11/23/1994, patched PCI steering table)", .internal_name = "mb1575pct_patch", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb1575pct/mb1575pct_patch.bin", "" } }, 
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};


const device_t mb1575pct_device = {
    .name          = "Biostar MB-1575PCT",
    .internal_name = "mb1575pct",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &mb1575pct_config[0]
};

int
machine_at_pat54pv_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pat54pv/PAT54PV.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti5x7_device);
    device_add(&keyboard_ps2_intel_ami_pci_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_cat3010_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cat3010/CAT3010.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

	device_add(&ide_isa_device);
    machine_at_common_init(model);

    device_add(&opti5x7_device);
    device_add(&keyboard_ps2_ami_pci_device);
	device_add(&fdc37c665_ide_pri_device);
    return ret;
}

int
machine_at_hot543_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hot543/543_R21.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&opti5x7_pci_device);
    device_add(&opti822_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_ncselp90_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ncselp90/elegancep90.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&opti5x7_pci_device);
    device_add(&opti822_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&ide_opti611_vlb_device);
    device_add(&fdc37c665_ide_sec_device);
    device_add(&ide_vlb_2ch_device);

    return ret;
}

int
machine_at_advt4075p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/advt4075p/AST404.B06",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x0D, PCI_CARD_VIDEO, 	  0, 0, 0, 0); /* Onboard mach64 CT */
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    device_add(&sis_85c50x_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&um8663af_ide_device);
    device_add(&sst_flash_29ee010_device);
	
	if (sound_card_current[0] == SOUND_INTERNAL)
		device_add(&sb_vibra16s_onboard_device);

    return ret;
}

int
machine_at_p54sp4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p54sp4/SI5I0204.AWD",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sp4_common_init(model);

    return ret;
}

int
machine_at_sq588_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sq588/sq588b03.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    /* Correct: 0D (01), 0F (02), 11 (03), 13 (04) */
    pci_register_slot(0x02, PCI_CARD_IDE,         1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);
    device_add(&sis_85c50x_device);
    device_add(&ide_cmd640_pci_single_channel_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c665_ide_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_action8xxx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/action8xxx/SPITFIRE.19X",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x03, PCI_CARD_VIDEO,       0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_IDE,         0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    device_add(&sis_85c50x_device);
    device_add(&ide_cmd640_pci_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c665_ide_device);
    device_add(&sst_flash_29ee010_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_p54sps_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p54sps/35s106.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x08, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 4, 1, 2, 3);
    device_add(&sis_85c50x_device);
    device_add(&ide_pci_2ch_device);
    device_add(&keyboard_at_ami_device);
    device_add(&w83787f_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_m54li_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m54li/M54LI.007",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_IDE, 		  0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 	  1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 	  2, 3, 4, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL, 	  3, 4, 1, 2);
    device_add(&sis_550x_85c503_device);
    device_add(&ide_w83769f_pci_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_ms5109_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms5109/A778.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ami_1994_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_IDE, 0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 1, 3, 2, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 2, 1, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL, 3, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 4, 1, 2, 3);
    device_add(&sis_550x_85c503_device);
    device_add(&ide_w83769f_pci_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&w83787f_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_torino_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_inverted("roms/machines/torino/PER113.ROM",
                                    0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ami_1994_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO, 0, 0, 0, 0);
    pci_register_slot(0x03, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL, 4, 1, 2, 3);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 3, 4, 1, 2);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&sis_550x_85c503_device);
    device_add(&ide_um8673f_device);
    device_add(&keyboard_ps2_tg_ami_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_ami_device);

    return ret;
}

int
machine_at_fm520_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/fm520/sst29ee010-67c22d072e8e6320773303.bin",
                            0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0B, PCI_CARD_VIDEO, 	  0, 0, 0, 0); /* SiS 6205 */
    pci_register_slot(0x11, PCI_CARD_NORMAL, 	  1, 3, 2, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 3, 4);
    device_add(&sis_5511_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c665_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_hot539_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hot539/539_R17.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
	return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x15, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x16, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0E, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 4, 1, 2, 3);
    device_add(&umc_8890_device);
    device_add(&umc_8886af_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&um8663af_device);

    return ret;
}

int
machine_at_mb8500urc_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mb8500urc/8500urc.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
	return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,	  1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0D, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
    device_add(&umc_8890_device);
    device_add(&umc_8886af_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&um8663af_device);

    return ret;
}

int
machine_at_presario7100_init(const machine_t *model)

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
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x13, PCI_CARD_VIDEO,       0, 0, 0, 0); /* Onboard */
    device_add(&umc_8890_device);
    device_add(&umc_8886af_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&um8663af_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&s3_phoenix_trio64_onboard_pci_device);
	
	if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ess_1688_device);  

    return ret;
}

static const device_config_t presario7100_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "presario7100_oct95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.04 Revision 1.01 (10/04/1995)", .internal_name = "presario7100_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/presario7100/presario7100_oct95.bin", "" } },
            { .name = "Version 4.04 Revision 1.40 (11/09/1995)", .internal_name = "presario7100_nov95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/presario7100/presario7100_nov95.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t presario7100_device = {
    .name          = "Compaq Presario 71xx",
    .internal_name = "presario7100",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &presario7100_config[0]
};

int
machine_at_ga586am_init(const machine_t *model)

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
	pci_register_slot(0x12, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0F, PCI_CARD_SOUTHBRIDGE, 3, 4, 1, 2); /* Onboard */
	pci_register_slot(0x10, PCI_CARD_VIDEO,       4, 1, 2, 3); /* Onboard */
    device_add(&umc_8890_device);
    device_add(&umc_8886bf_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&um8663af_device);

    return ret;
}

static const device_config_t ga586am_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ga586am_sept95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50PG Revision A (09/25/1995)", .internal_name = "ga586am_sept95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586am/ga586am_sept95.bin", "" } },
            { .name = "Version 4.50PG Revision 1.0 (12/07/1995)", .internal_name = "ga586am_dec95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586am/ga586am_dec95.bin", "" } },
			 { .name = "Version 4.50PG Revision 1.0A (12/07/1995, unlocked)", .internal_name = "ga586am_dec952", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga586am/ga586am_dec952.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t ga586am_device = {
    .name          = "Gigabyte GA-586AM",
    .internal_name = "ga586am",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ga586am_config[0]
};
