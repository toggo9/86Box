/*
 * 86Box    A hypervisor and IBM PC system emulator that specializes in
 *          running old operating systems and software designed for IBM
 *          PC systems and compatibles from 1981 through fairly recent
 *          system designs based on the PCI bus.
 *
 *          This file is part of the 86Box distribution.
 *
 *          Implementation of Slot 1 machines.
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
#include <86box/device.h>
#include <86box/pci.h>
#include <86box/chipset.h>
#include <86box/hdc.h>
#include <86box/hdc_ide.h>
#include <86box/keyboard.h>
#include <86box/flash.h>
#include <86box/sio.h>
#include <86box/hwm.h>
#include <86box/spd.h>
#include <86box/video.h>
#include "cpu.h"
#include <86box/machine.h>
#include <86box/sound.h>
#include <86box/clock.h>
#include <86box/snd_ac97.h>

int
machine_at_acerv62x_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acerv62x/v62xc0s1.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 5, 0, 0, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1);
    device_add(&i440fx_device);
    device_add(&piix3_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C935 | FDC37C93X_APM));
    device_add(&sst_flash_29ee020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 128);

    return ret;
}

int
machine_at_p65up5_cpknd_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p65up5/NDKN0218.AWD",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_p65up5_common_init(model, &i440fx_device);

    return ret;
}

int
machine_at_kn97_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/kn97/0116I.001",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    device_add(&i440fx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&w83877f_device);
    device_add(&intel_flash_bxt_device);
    device_add(&lm78_device); /* fans: Chassis, CPU, Power; temperature: MB */
    for (uint8_t i = 0; i < 3; i++)
        hwm_values.fans[i] *= 2; /* BIOS reports fans with the wrong divisor for some reason */

    return ret;
}

int
machine_at_ms6109_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms6109/W609MS10.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      4, 1, 2, 3);
    device_add(&i440fx_device);
    device_add(&piix3_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&w83877f_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_tahoeii_init(const machine_t *model)

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
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 01 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 02 */
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 03 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 04 */
    device_add(&i440fx_device);
    device_add(&piix3_ioapic_device);
	device_add(&ioapic_device);
    device_add(&fdc37c669_device);
    device_add(&keyboard_ps2_ami_pci_device);
	device_add(ics9xxx_get(ICS9150_08));
    device_add(&sst_flash_29ee010_device);
	device_add(&ioapic_device);

    return ret;
}

static const device_config_t tahoeii_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "tahoe_award",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "AwardBIOS v4.51PG Version Beta (04/22/1997)", .internal_name = "tahoe_award", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/tahoeii/P2AWDP01.BIN", "" } },
            { .name = "AMIBIOS Version 1.00", .internal_name = "ami_100", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/tahoeii/2tynet02.rom", "" } },
            { .name = "AMIBIOS Version 1.04", .internal_name = "ami_104", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/tahoeii/P2AMDP10.rom", "" } },
			{ .name = "AMIBIOS Version 2.00", .internal_name = "ami_200", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/tahoeii/P2AMDP20.rom", "" } },
			{ .name = "AMIBIOS Version 2.02", .internal_name = "ami_202", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/tahoeii/TAHOD202.002", "" } },	
			{ .name = "AMIBIOS Version 2.08", .internal_name = "ami_208", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/tahoeii/p2amdp28.rom", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t tahoeii_device = {
    .name          = "Tyan S1682D (Tahoe II)",
    .internal_name = "tahoeii",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &tahoeii_config[0]
};

int
machine_at_sbc775_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sbc775/775v110-66ce6f0e7ad1e689716485.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_lx6_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/lx6/LX6C_PZ.B00",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440lx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&sst_flash_29ee010_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);

    return ret;
}

int
machine_at_andover_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/andover/GW2KBOOT.ROM",
                           0x00080000, 524288, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x10, PCI_CARD_SOUND,       2, 0, 0, 0);
	pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
	pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1);
	pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);
	pci_register_slot(0x14, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 0, 0, 0);
    device_add(&i440lx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c67x_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
	
	if (sound_card_current[0] == SOUND_INTERNAL)
        machine_snd = device_add(machine_get_snd_device(machine));

    return ret;
}

int
machine_at_optiplexgxa_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/optiplexgxa/DELL.ROM",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NETWORK,     4, 0, 0, 0);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 2, 1);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 1, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_BRIDGE,      0, 0, 0, 0);

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(machine_get_snd_device(machine));

    device_add(&i440lx_device);
    device_add(&piix4_device);
	device_add(&ioapic_device);
    machine_at_optiplex_21152_init();
    device_add_params(&pc87307_device, (void *) (PCX730X_PHOENIX_42 | PCX7307_PC87307));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    return ret;
}

int
machine_at_vl601sni_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/vl601sni/627ic14.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
	pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1);
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440lx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&sst_flash_29ee010_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);

    return ret;
}

int
machine_at_spitfire_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/spitfire/SPIHM.02",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x08, PCI_CARD_VIDEO,       1, 2, 0, 0);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440lx_device);
    device_add(&piix4e_device);
    device_add_params(&fdc37c93x_device, (void *) (FDC37C935 | FDC37C93X_NORMAL | FDC37C93X_NO_NVR));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&lm78_device); /* no reporting in BIOS */

    return ret;
}

int
machine_at_ms6111_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms6111/A611MS11.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4);
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
	pci_register_slot(0x10, PCI_CARD_NORMAL,      3, 4, 1, 2);
	pci_register_slot(0x12, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&sst_flash_29ee010_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);

    return ret;
}


int
machine_at_tigercat_init(const machine_t *model)

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
	
	machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x10, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add(&fdc37c67x_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
	spd_register(SPD_TYPE_SDRAM, 0xF, 256);
	device_add(&lm78_device);
    hwm_values.voltages[1]     = 1500; /* IN1 (unknown purpose, assumed Vtt) */
    hwm_values.fans[1]         = 3500;    /* unused */
    hwm_values.fans[2]         = 3300;    /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */

    return ret;
}

static const device_config_t tigercat_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "tigercat_oct97",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.0 rel. 6.0 Rev. NS02 (10/29/1997)", .internal_name = "tigercat_oct97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/tigercat/tigercat_oct97.rom", "" } },
            { .name = "Version 4.0 rel 6.0 Rev. NS03 (01/23/1998)", .internal_name = "tigercat_jan98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/tigercat/tigercat_jan98.rom", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t tigercat_device = {
    .name          = "Micronics Tigercat",
    .internal_name = "tigercat",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &tigercat_config[0]
};

int
machine_at_legendv_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/legendv/L5_V18SL.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Onboard */
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 03 */
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_PHOENIX_42 | PC87309_PC87309));
    device_add(&winbond_flash_w29c020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&lm78_device);     /* fans: CPU, unused, unused; temperatures: unused, CPU, unused */
    hwm_values.temperatures[0] = 0;    /* unused */
    hwm_values.voltages[1]     = 1500; /* CPUVTT */

    return ret;
}

int
machine_at_lightninglx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/lightninglx/161526sw.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
    pci_register_slot(0x06, PCI_CARD_NORMAL,      4, 0, 0, 0); /* Video? */
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add_params(&pc87307_device, (void *) (PCX730X_PHOENIX_42 | PCX730X_15C | PCX7307_PC87307));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&lm78_device);     /* fans: CPU, unused, unused; temperatures: unused, CPU, unused */
    hwm_values.temperatures[0] = 0;    /* unused */
    hwm_values.voltages[1]     = 1500; /* CPUVTT */

    return ret;
}


int
machine_at_kl97a_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/kl97a/bios.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0E, PCI_CARD_VIDEO,       1, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_PHOENIX_42 | PC87309_PC87309));
    device_add(&winbond_flash_w29c020_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&mach64vt2_device);
	
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&as99127f_device);
    device_add(ics9xxx_get(ICS9250_08));
	hwm_values.temperatures[0] = 0;  /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */
    hwm_values.temperatures[2] = 0;  /* unused */
    hwm_values.fans[1]         = 0;  /* unused */
    hwm_values.fans[2]         = 0;  /* unused */

    return ret;
}

int
machine_at_p2l98xv_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p2l98xv/bios.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0E, PCI_CARD_VIDEO,       1, 0, 0, 0); /* Onboard */
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_PHOENIX_42 | PC87309_PC87309));
    device_add(&winbond_flash_w29c020_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&mach64vt2_device);
	
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&as99127f_device);
    device_add(ics9xxx_get(ICS9250_08));
	hwm_values.temperatures[0] = 0;  /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */
    hwm_values.temperatures[2] = 0;  /* unused */
    hwm_values.fans[1]         = 0;  /* unused */
    hwm_values.fans[2]         = 0;  /* unused */

    return ret;
}

int
machine_at_kl6011_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/kl6011/627ib13.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x09, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL, 	  4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 05 */
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x03, 256);
    device_add(&w83781d_device);       /* fans: CPU, CHS, PS; temperatures: unused, CPU, System */
    hwm_values.temperatures[0] = 0;    /* unused */
    hwm_values.voltages[1]     = 1500; /* CPUVTT */

    return ret;
}

int
machine_at_brio8xxx440lx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/brio8xxx440lx/QHL0700.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x14, PCI_CARD_VIDEO, 	  1, 2, 3, 4); /* Onboard */
    device_add(&i440lx_device);
    device_add(&piix4_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c67x_device);
    device_add(&winbond_flash_w29c020_device);
    spd_register(SPD_TYPE_SDRAM, 0x03, 256);
    device_add(&w83781d_device);       /* fans: CPU, CHS, PS; temperatures: unused, CPU, System */
    hwm_values.temperatures[0] = 0;    /* unused */
    hwm_values.voltages[1]     = 1500; /* CPUVTT */
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_ma30d_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ma30d/BIOS.ROM",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
#ifdef UNKNOWN_SLOT
    pci_register_slot(0x0A, PCI_CARD_NETWORK,     2, 3, 4, 1); /* ???? device - GPIO? */
#endif
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   3, 0, 0, 0);
    device_add(&i440lx_device);
    device_add(&piix4e_device);
    device_add(&nec_mate_unk_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c67x_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    return ret;
}

int
machine_at_optiplexe1_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/optiplexe1/DELL.ROM",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 01 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 	  3, 4, 2, 1); /* Slot 02 */
	pci_register_slot(0x11, PCI_CARD_NORMAL, 	  4, 0, 0, 0); /* Onboard ATi Rage IIc AGP */
    device_add(&i440ex_device);
    device_add(&piix4_device);
	device_add(ics9xxx_get(ICS9150_08));
    device_add_params(&pc87309_device, (void *) (PCX730X_15C | PCX730X_PHOENIX_42 | PC87309_PC87309));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x03, 256);
    return ret;
}

int
machine_at_vl603sni_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/vl603sni/114iq126.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
	pci_register_slot(0x08, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE, 1, 2, 3, 4);
    device_add(&i440ex_device);
    device_add(&piix4_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&it8671f_device);
    device_add(&sst_flash_29ee010_device);
    spd_register(SPD_TYPE_SDRAM, 0x03, 256);
    device_add(&w83781d_device);       /* fans: CPU, CHS, PS; temperatures: unused, CPU, System */
    hwm_values.temperatures[0] = 0;    /* unused */
    hwm_values.voltages[1]     = 1500; /* CPUVTT */

    return ret;
}

int
machine_at_ergoproe366_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ergoproe366/6EMMP_J1.123",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
	pci_register_slot(0x08, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE, 1, 2, 3, 4);
    device_add(&i440ex_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&it8671f_device);
    device_add(&sst_flash_29ee020_device);
    spd_register(SPD_TYPE_SDRAM, 0x03, 256);
    device_add(&w83781d_device);       /* fans: CPU, CHS, PS; temperatures: unused, CPU, System */
    hwm_values.temperatures[0] = 0;    /* unused */
    hwm_values.voltages[1]     = 1500; /* CPUVTT */

    return ret;
}

int
machine_at_p6i440e2_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p6i440e2/E2_v14sl.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE, 1, 2, 3, 4);
    device_add(&i440ex_device);
    device_add(&piix4_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&sst_flash_29ee010_device);
    spd_register(SPD_TYPE_SDRAM, 0x03, 256);
    device_add(&w83781d_device);       /* fans: CPU, CHS, PS; temperatures: unused, CPU, System */
    hwm_values.temperatures[0] = 0;    /* unused */
    hwm_values.voltages[1]     = 1500; /* CPUVTT */

    return ret;
}

int
machine_at_sbc770_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sbc770/770r1.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      2, 3, 4, 1);
	pci_register_slot(0x0B, PCI_CARD_VIDEO,       0, 0, 0, 0); /* Video */
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&w83781d_device);     /* fans: Chassis, CPU, Power; temperatures: MB, unused, CPU */
    hwm_values.temperatures[1] = 0;  /* unused */
    hwm_values.temperatures[2] -= 3; /* CPU offset */
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_p2bls_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p2bls/1014ls.003",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x06, PCI_CARD_SCSI,        4, 1, 2, 3);
    pci_register_slot(0x07, PCI_CARD_NETWORK,     3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977ef_device);
#if 0
    device_add(ics9xxx_get(ICS9150_08)); /* setting proper speeds requires some interaction with the AS97127F ASIC */
#endif
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&w83781d_device);     /* fans: Chassis, CPU, Power; temperatures: MB, unused, CPU */
    hwm_values.temperatures[1] = 0;  /* unused */
    hwm_values.temperatures[2] -= 3; /* CPU offset */

    return ret;
}

int
machine_at_lgibmx7g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/lgibmx7g/ms6119.331",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&winbond_flash_w29c020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    return ret;
}

int
machine_at_p3bf_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p3bf/1008f.004",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977ef_device);
    device_add(ics9xxx_get(ICS9250_08));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&as99127f_device);                    /* fans: Chassis, CPU, Power; temperatures: MB, JTPWR, CPU */
    hwm_values.voltages[4] = hwm_values.voltages[5]; /* +12V reading not in line with other boards; appears to be close to the -12V reading */

    return ret;
}

int
machine_at_bf6_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/bf6/Beh_70.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 1, 4, 3);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 3, 1, 2);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&w83977ef_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    return ret;
}

int
machine_at_bx6_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/bx6/BX6_EG.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&w83977f_device);
    device_add(&sst_flash_29ee010_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);

    return ret;
}

int
machine_at_ax6bc_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ax6bc/AX6BC_R2.59.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&sst_flash_29ee020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
    device_add(&gl518sm_2d_device); /* fans: System, CPU; temperature: CPU; no reporting in BIOS */

    return ret;
}

int
machine_at_atc6310bxii_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/atc6310bxii/6310s102.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&slc90e66_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&w83977ef_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    return ret;
}

int
machine_at_m003_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m003/M003BP43.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      4, 1, 2, 3);
	pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
	device_add(ics9xxx_get(ICS9150_08));
    device_add(&it8671f_device);
    device_add(&sst_flash_29ee020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);

    return ret;
}

int
machine_at_ergoproxd_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ergoproxd/M65V121.ROM",
                           0x00080000, 524288, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_VIDEO,       3, 0, 0, 0); /* Onboard */
	pci_register_slot(0x09, PCI_CARD_SCSI,        4, 0, 0, 0); /* Onboard */
	pci_register_slot(0x10, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x14, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x16, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_phoenix_device);
    device_add(&fdc37m60x_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&w83781d_device);     /* fans: CPU, unused, unused; temperatures: unused, CPU, unused */
    hwm_values.temperatures[0] = 0;  /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */
    hwm_values.temperatures[2] = 0;  /* unused */
    hwm_values.fans[1]         = 0;  /* unused */
    hwm_values.fans[2]         = 0;  /* unused */

    return ret;
}

int
machine_at_686bx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/686bx/6BX.F2a",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977tf_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&w83781d_device);     /* fans: CPU, unused, unused; temperatures: unused, CPU, unused */
    hwm_values.temperatures[0] = 0;  /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */
    hwm_values.temperatures[2] = 0;  /* unused */
    hwm_values.fans[1]         = 0;  /* unused */
    hwm_values.fans[2]         = 0;  /* unused */

    return ret;
}

int
machine_at_kbi6130compaq_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/kbi6130compaq/mz4225.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x09, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_SOUND, 	  4, 1, 2, 3); /* Slot 04 */
	device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977ef_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
	
	if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&es1371_onboard_device);
	
    device_add(&w83782d_device);     /* fans: CPU, unused, unused; temperatures: unused, CPU, unused */

    return ret;
}

int
machine_at_hpbrio845xx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hpbrio845xx/qhv0501.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
	pci_register_slot(0x09, PCI_CARD_NORMAL, 	  4, 1, 2, 3); /* Slot 01 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 02 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 03 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 04 */
	device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_PHOENIX_42 | PC87309_PC87309));
    device_add(&winbond_flash_w29c020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
	
    device_add(&as99127f_device); /* fans: Chassis, CPU, Power; temperatures: MB, JTPWR, CPU */
    device_add(ics9xxx_get(ICS9250_08));

    return ret;
}

int
machine_at_idotexplora_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/idotexplora/65005E.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
	pci_register_slot(0x09, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0F, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 02 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 03 */
	pci_register_slot(0x12, PCI_CARD_SOUND, 	  3, 4, 1, 2); /* Slot 04 */
	device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&it8671f_device);
    device_add(&winbond_flash_w29c020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
	
    device_add(&lm78_device);
    hwm_values.voltages[1]     = 1500; /* IN1 (unknown purpose, assumed Vtt) */
    hwm_values.fans[1]         = 3500;    /* unused */
    hwm_values.fans[2]         = 3300;    /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */

    return ret;
}

int
machine_at_p6sba_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p6sba/SBAB21.ROM",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&w83977tf_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
    device_add(&w83781d_device);    /* fans: CPU1, CPU2, Thermal Control; temperatures: unused, CPU1, CPU2? */
    hwm_values.fans[1]         = 0; /* no CPU2 fan */
    hwm_values.temperatures[0] = 0; /* unused */
    hwm_values.temperatures[2] = 0; /* CPU2? */
    /* no CPU2 voltage */

    return ret;
}

int
machine_at_s1846_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/s1846/bx46200f.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_SOUND,       1, 0, 0, 0);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_AMI | PC87309_PC87309));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    if (sound_card_current[0] == SOUND_INTERNAL) {
        device_add(machine_get_snd_device(machine));
        device_add(&ct1297_device); /* no good pictures, but the marking looks like CT1297 from a distance */
    }

    return ret;
}

int
machine_at_tp90_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tp90/1846TP90.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_SOUND,       1, 0, 0, 0);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_AMI | PC87309_PC87309));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    if (sound_card_current[0] == SOUND_INTERNAL) {
        device_add(machine_get_snd_device(machine));
        device_add(&ct1297_device); /* no good pictures, but the marking looks like CT1297 from a distance */
    }

    return ret;
}

int
machine_at_d1161_init(const machine_t *model)

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
	
	machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 0, 0); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x10, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 05 */
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_AMI | PC87309_PC87309));
	device_add(ics9xxx_get(ICS9248_39));
    device_add(&sst_flash_29ee020_device);
	device_add(&ioapic_device);
	spd_register(SPD_TYPE_SDRAM, 0xF, 256);
	device_add(&lm78_device);
	device_add(&w83781d_device);
    hwm_values.voltages[1]     = 1500; /* IN1 (unknown purpose, assumed Vtt) */
    hwm_values.fans[1]         = 3500;    /* unused */
    hwm_values.fans[3]         = 3300;    /* unused */
    hwm_values.temperatures[0] -= 0;  /* unused */
    hwm_values.temperatures[2] -= 3; /* CPU offset */

    return ret;
}


static const device_config_t d1161_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d1161",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 1.16a Rev. 1.01.1161 (05/04/1999)", .internal_name = "d1161", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/d1161/amiboot.rom", "" } },
            { .name = "Version 2.00.00 Rev. 2.10.1161 (12/23/1999)", .internal_name = "d1161_dec99", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/d1161/amiboot_dec99.rom", "" } },
            { .name = "Version 2.00.01 Rev. 2.20.1161 (03/24/2000)", .internal_name = "d1161_mar00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/d1161/amiboot_mar00.rom", "" } },
			{ .name = "Version 2.00.03 Rev. 2.30.1161 (08/11/2000)", .internal_name = "d1161_aug00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/d1161/amiboot_aug00.rom", "" } },		  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t d1161_device = {
    .name          = "Siemens Celsius 620 (Tyan S1832DL OEM)",
    .internal_name = "d1161",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d1161_config[0]
};

int
machine_at_pcvl_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pcvl/bios.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
	pci_register_slot(0x03, PCI_CARD_NORMAL,      2, 3, 4, 1);
	pci_register_slot(0x05, PCI_CARD_NORMAL,      4, 1, 2, 3);
	pci_register_slot(0x06, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977f_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);
    device_add(&w83782d_device);     /* fans: CPU, unused, unused; temperatures: unused, CPU, unused */

    return ret;
}

int
machine_at_phantom_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/phantom/164309SW.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      3, 0, 0, 0); /* Video? */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
    pci_register_slot(0x10, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
    pci_register_slot(0x12, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   0, 0, 0, 0);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
	device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c67x_device);
	device_add(ics9xxx_get(ICS9248_39));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
	device_add(&lm78_device);
    hwm_values.voltages[1]     = 1500; /* IN1 (unknown purpose, assumed Vtt) */
    hwm_values.fans[1]         = 3500;    /* unused */
    hwm_values.fans[2]         = 3300;    /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */
    return ret;
}

int
machine_at_s1833d_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/s1833d/0AANG025.ROM",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 0, 0);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add_params(&pc87309_device, (void *) (PCX730X_AMI | PC87309_PC87309));
	device_add(ics9xxx_get(ICS9248_39));
    device_add(&sst_flash_29ee020_device);
	device_add(&ioapic_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
	device_add(&lm75_1_4a_device);
	device_add(&lm75_1_4a_device);
	device_add(&lm78_device);
    hwm_values.voltages[1]     = 1500; /* IN1 (unknown purpose, assumed Vtt) */
    hwm_values.fans[1]         = 3500;    /* unused */
    hwm_values.fans[2]         = 3300;    /* unused */
    hwm_values.temperatures[1] += 4; /* CPU offset */
    return ret;
}

int
machine_at_ficka6130_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ficka6130/qa4163.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro_device);
    device_add(&via_vt82c596a_device);
    device_add(&w83877tf_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_29ee020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    return ret;
}

int
machine_at_ficvb601v_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ficvb601v/Qf6132s.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x09, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL, 	  4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 05 */
    device_add(&via_apro_device);
    device_add(&via_vt82c596a_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
	device_add(&w83782d_device);     /* fans: Chassis, CPU, Power; temperatures: MB, unused, CPU */
    hwm_values.temperatures[1] = 0;  /* unused */
    hwm_values.temperatures[2] -= 3; /* CPU offset */

    return ret;
}

int
machine_at_med9901via_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/med9901via/med9901via.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,	  1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x14, PCI_CARD_SOUND,       3, 4, 1, 2);  /* Slot 04 */
    device_add(&via_apro_device);
    device_add(&via_vt82c596a_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 512);
	
	
    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&es1373_onboard_device);
	
    return ret;
}

int
machine_at_p3v133_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p3v133/1003.002",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro133_device);
    device_add(&via_vt82c596b_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9248_39));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 512);
    device_add(&w83781d_device);     /* fans: Chassis, CPU, Power; temperatures: MB, unused, CPU */
    hwm_values.temperatures[1] = 0;  /* unused */
    hwm_values.temperatures[2] -= 3; /* CPU offset */

    return ret;
}

int
machine_at_ficka31_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ficka31/Ka31-411.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x09, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
    device_add(&via_apro133_device);
    device_add(&via_vt82c686a_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9248_39));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 512);
    device_add(&w83781d_device);     /* fans: Chassis, CPU, Power; temperatures: MB, unused, CPU */
    hwm_values.temperatures[1] = 0;  /* unused */
    hwm_values.temperatures[2] -= 3; /* CPU offset */

    return ret;
}

int
machine_at_ms6199vacompaq_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms6199vacompaq/ms-6199va-compaq-v1-5-674742eb0150d985198167.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
	pci_register_slot(0x0C, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 06 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 02 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 03 */
	pci_register_slot(0x12, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 04 */
	pci_register_slot(0x13, PCI_CARD_NORMAL, 	  2, 1, 4, 3); /* Slot 01 */
	pci_register_slot(0x14, PCI_CARD_NORMAL, 	  2, 1, 4, 3); /* Slot 05 */
    device_add(&via_apro133_device);
    device_add(&via_vt82c596b_device);
	device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9248_39));
    device_add(&sst_flash_29ee020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 512);
    device_add(&w83781d_device);     /* fans: Chassis, CPU, Power; temperatures: MB, unused, CPU */
    hwm_values.temperatures[1] = 0;  /* unused */
    hwm_values.temperatures[2] -= 3; /* CPU offset */

    return ret;
}

int
machine_at_s61_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("vendor"), 0);
    ret = bios_load_linear(fn, 0x000c0000, 262144, 0);
    device_context_restore();

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686a_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
	
	if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ad1881_device);

    return ret;
}

static const device_config_t s61_config[] = {
    // clang-format off
    {
        .name = "vendor",
        .description = "Vendor",
        .type = CONFIG_BIOS,
        .default_string = "apower",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "AcerPower", .internal_name = "apower", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/s61/S6101F0.bin", "" } },
            { .name = "Aspire", .internal_name = "aspire", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/s61/S6103F2C.bin", "" } },
			{ .name = "AOpen", .internal_name = "aopen", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/s61/mx64100.bin", "" } },		  			  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t s61_device = {
    .name          = "Acer S61",
    .internal_name = "s61",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &s61_config[0]
};

int
machine_at_p3v4x_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p3v4x/1006.004",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro133a_device);
    device_add(&via_vt82c596b_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 512);
    device_add(&as99127f_device); /* fans: Chassis, CPU, Power; temperatures: MB, JTPWR, CPU */

    return ret;
}

int
machine_at_gt694va_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/gt694va/21071100.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_SOUND,       4, 1, 2, 3); /* assumed */
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro133a_device);
    device_add(&via_vt82c596b_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    device_add(&w83782d_device);       /* fans: CPU, unused, unused; temperatures: System, CPU1, unused */
    hwm_values.voltages[1]     = 1500; /* IN1 (unknown purpose, assumed Vtt) */
    hwm_values.fans[0]         = 4500; /* BIOS does not display <4411 RPM */
    hwm_values.fans[1]         = 0;    /* unused */
    hwm_values.fans[2]         = 0;    /* unused */
    hwm_values.temperatures[2] = 0;    /* unused */

    if (sound_card_current[0] == SOUND_INTERNAL) {
        device_add(machine_get_snd_device(machine));
        device_add(&cs4297_device); /* no good pictures, but the marking looks like CS4297 from a distance */
    }

    return ret;
}

int
machine_at_m6vbe_init(const machine_t *model)

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
	
	machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	/*pci_register_slot(0x0B, PCI_CARD_NORMAL,    4, 1, 2, 3); Assumed Slot for the ESS Solo-1 Onboard Sound Card. Board only has 3 PCI Slots */
    device_add(&via_apro133_device);
    device_add(&via_vt82c596b_device);
    device_add(&fdc37m60x_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 384); 

    return ret;
}

static const device_config_t m6vbe_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "tbird_apr00",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
			{ .name = "Version 4.51PG Rev. VBE0226B (02/23/1999)", .internal_name = "m6vbe_feb99", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/m6vbe/m6vbe_feb99.bin", "" } },
			{ .name = "Version 4.51PG Rev. VBE0706A (07/06/1999)", .internal_name = "m6vbe_jul99", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/m6vbe/m6vbe_jul99.bin", "" } },
			{ .name = "Version 4.51PG Rev. VBE-A 1206A (11/18/1999, released 12/06/1999)", .internal_name = "m6vbe_dec99", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/m6vbe/m6vbe_dec99.bin", "" } },
			{ .name = "Version 4.51PG Rev. VBE-A 0323B (03/23/2000)", .internal_name = "m6vbe_mar00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/m6vbe/m6vbe_mar00.bin", "" } },  			  
            { .name = "Version 4.51PG Rev. VBE-A 0407A (11/29/1999, released 04/07/2000)", .internal_name = "tbird_apr00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/m6vbe/tbird_apr00.bin", "" } },
            { .name = "Version 4.51PG Rev. VBE0727 FSC (07/27/2000)", .internal_name = "tbird_jul00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/m6vbe/tbird_jul00.bin", "" } },	  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t m6vbe_device = {
    .name          = "Biostar M6VBE",
    .internal_name = "m6vbe",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &m6vbe_config[0]
};

int
machine_at_vei8_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/vei8/QHW1001.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&fdc37m60x_370_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_08));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 512);
    device_add(&as99127f_device); /* fans: Chassis, CPU, Power; temperatures: MB, JTPWR, CPU */

    return ret;
}

static void
machine_at_ms6168_common_init(const machine_t *model)
{
    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x14, PCI_CARD_SOUND,       3, 4, 1, 2);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440zx_device);
    device_add(&piix4e_device);
    device_add(&w83977ef_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&voodoo_3_2000_agp_onboard_8m_device);

    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 256);

    if (sound_card_current[0] == SOUND_INTERNAL) {
        device_add(machine_get_snd_device(machine));
        device_add(&cs4297_device);
    }
}

int
machine_at_borapro_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/borapro/MS6168V2.50",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_ms6168_common_init(model);

    return ret;
}

int
machine_at_ergoproet_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ergoproet/6zma_13m.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,	  1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x14, PCI_CARD_SOUND,       3, 4, 1, 2); /* Slot 04 */
    device_add(&i440zx_device);
    device_add(&piix4e_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 512);
	
	
    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&es1373_onboard_device);
	
    return ret;
}

int
machine_at_med9901_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/med9901/med9901.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,	  1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 	  1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x14, PCI_CARD_SOUND,       3, 4, 1, 2); /* Slot 04 */
    device_add(&i440zx_device);
    device_add(&piix4e_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 512);
	
	
    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&es1373_onboard_device);
	
    return ret;
}

int
machine_at_hn440zx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hn440zx/164845sw.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,	  1, 2, 3, 4);
	pci_register_slot(0x06, PCI_CARD_NORMAL, 	  3, 0, 0, 0); /* probably ESS Solo-1 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 	  4, 1, 2, 3); /* Slot 01 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 03 */
	pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 04 */
    device_add(&i440zx_device);
    device_add(&piix4e_device);
    device_add(&fdc37c67x_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 512);
	device_add(&lm78_device);      /* fans: Thermal, CPU, Chassis; temperature: unused */
    device_add(&lm75_1_4a_device); /* temperature: CPU */
	
	
    return ret;
}

int
machine_at_in440_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/in440/164845sw.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
	pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,	  1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x06, PCI_CARD_NORMAL, 	  3, 0, 0, 0); /* probably ESS Solo-1 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL, 	  4, 1, 2, 3); /* Slot 01 */
	pci_register_slot(0x10, PCI_CARD_NORMAL, 	  3, 4, 1, 2); /* Slot 02 */
	pci_register_slot(0x12, PCI_CARD_NORMAL, 	  2, 3, 4, 1); /* Slot 03 */
	pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 04 */
    device_add(&i440zx_device);
    device_add(&piix4e_device);
    device_add(&fdc37c67x_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 512);
	device_add(&lm78_device); /* fans: Chassis, CPU, Power; temperature: MB */
    for (uint8_t i = 0; i < 3; i++)
    hwm_values.fans[i] *= 2; /* BIOS reports fans with the wrong divisor for some reason */
	
	
    return ret;
}

int
machine_at_ms6168_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms6168/w6168ims.130",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_ms6168_common_init(model);

    return ret;
}

int
machine_at_m729_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m729/M729NEW.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE, 1, 2, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_SOUTHBRIDGE_IDE, 1, 2, 3, 4);
    pci_register_slot(0x03, PCI_CARD_SOUTHBRIDGE_PMU, 1, 2, 3, 4);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE_USB, 1, 2, 3, 4);
    pci_register_slot(0x14, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x10, PCI_CARD_NORMAL, 3, 4, 1, 2);
    device_add(&ali1621_device);
    device_add(&ali1543c_device); /* +0 */
    device_add(&winbond_flash_w29c010_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 512);

    return ret;
}

int
machine_at_p6f99_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p6f99/99-8.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_SOUND,       2, 3, 4, 1);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x02, PCI_CARD_AGPBRIDGE,   0, 0, 0, 0);
    device_add(&sis_5600_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&it8661f_device);
    device_add(&winbond_flash_w29c020_device);

    if (sound_card_current[0] == SOUND_INTERNAL) {
        device_add(machine_get_snd_device(machine));
        device_add(&ct1297_device);
    }

    return ret;
}

int
machine_at_m747_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m747/990521.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x02, PCI_CARD_AGPBRIDGE,   0, 0, 0, 0);
    device_add(&sis_5600_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&it8661f_device);
    device_add(&winbond_flash_w29c020_device);

    return ret;
}
