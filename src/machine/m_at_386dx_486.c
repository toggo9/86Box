/*
 * 86Box    A hypervisor and IBM PC system emulator that specializes in
 *          running old operating systems and software designed for IBM
 *          PC systems and compatibles from 1981 through fairly recent
 *          system designs based on the PCI bus.
 *
 *          This file is part of the 86Box distribution.
 *
 *          Implementation of 386DX and 486 machines.
 *
 *
 *
 * Authors: Miran Grca, <mgrca8@gmail.com>
 *
 *          Copyright 2016-2020 Miran Grca.
 */
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#define HAVE_STDARG_H
#include <86box/86box.h>
#include "cpu.h"
#include <86box/timer.h>
#include <86box/io.h>
#include <86box/device.h>
#include <86box/chipset.h>
#include <86box/keyboard.h>
#include <86box/mem.h>
#include <86box/nvr.h>
#include <86box/pci.h>
#include <86box/dma.h>
#include <86box/fdd.h>
#include <86box/fdc.h>
#include <86box/fdc_ext.h>
#include <86box/gameport.h>
#include <86box/pic.h>
#include <86box/pit.h>
#include <86box/rom.h>
#include <86box/serial.h>
#include <86box/sio.h>
#include <86box/hdc.h>
#include <86box/port_6x.h>
#include <86box/port_92.h>
#include <86box/video.h>
#include <86box/flash.h>
#include <86box/scsi_ncr53c8xx.h>
#include <86box/hwm.h>
#include <86box/machine.h>
#include <86box/plat_unused.h>
#include <86box/sound.h>

/* 386DX */
int
machine_at_acc386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acc386/acc386.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&acc2168_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_p386dx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p386dx/386_Peacock-P386DX-40-ZA224147.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sl82c461_device);
    device_add(&keyboard_at_ami_device);
	device_add(&w83787f_ide_en_device);

    return ret;
}

int
machine_at_asus386_3364k_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/asus386_3364k/am27c512dip28-64b53c26be3d8160533563.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&isa486c_device);
    device_add(&port_92_key_device);
    device_add(&rabbit_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}


int
machine_at_asus386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/asus386/ASUS_ISA-386C_BIOS.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&rabbit_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_msi3113_7_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/msi3113-7/Mainboard_MSI_3113-7.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&rabbit_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_gemini386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/gemini386/g3-bios-v1-10-g17-2-micronics-09-00086-02-27c512-628b8da0bd68d651412403.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti495sx_device);
    device_add(&keyboard_at_phoenix_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_mpfpc_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/mpfpc/386-bios-u41-v2-20-63794fd125f2b832441578.bin",
								"roms/machines/mpfpc/386-bios-u43-v2-20-63794fd115194441387087.bin",
								0x000f8000, 32768, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_acer1100_386dx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/acer1100_386dx/Acer 1100-16-SE 386Bios V2.25 - U9.bin",
								"roms/machines/acer1100_386dx/Acer 1100-16-SE 386Bios V2.25 - U10.bin",
								0x000f8000, 32768, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_intel301_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/intel301/U52_D27256@DIP28_EVEN.bin",
								"roms/machines/intel301/U53_D27256@DIP28_ODD.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);
	mem_remap_top(384);
    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_intel302_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/intel302/456720-001_8904.bin",
								"roms/machines/intel302/456721-001_8904.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);
	mem_remap_top(384);
    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_intel303_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/intel303/ROM3DX.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);
	mem_remap_top(384);
    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_tandy4000_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tandy4000/BIOS Tandy 4000 v1.03.01.bin",
                           0x000f8000, 32768, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_apcpredatoriplus_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/apcpredatoriplus/PRED1PLUS.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sl82c461_device);
    device_add(&keyboard_ps2_ami_device);
	device_add(&ide_isa_device);
	device_add(&fdc37c651_ide_device);

    return ret;
}

int
machine_at_dtk461_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dtk461/DTK.BIO",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sl82c461_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_dt423s4l_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000f0000, 65536, 0);
    device_context_restore();
	
	machine_at_common_init(model);
	
	device_add(&sl82c461_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
	
    return ret;
}

static const device_config_t dt423s4l_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "dt423s4l_ami",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "AMIBIOS (07/07/1991)", .internal_name = "dt423s4l_ami", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/dt423s4l/dt423s4l_ami.bin", "" } },
            { .name = "Mr. BIOS 1.45 (04/18/1994)", .internal_name = "dt423s4l_mrbios", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/dt423s4l/dt423s4l_mrbios.bin", "" } },
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};

const device_t dt423s4l_device = {
    .name          = "DD & TT Enterprise DT-423S4L",
    .internal_name = "dt423s4l",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &dt423s4l_config[0]
};

int
machine_at_pck486dx_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000f0000, 65536, 0);
    device_context_restore();
	
	machine_at_common_init(model);

    device_add(&sl82c461_device);
    device_add(&keyboard_at_ami_device);
	device_add(&fdc37c651_ide_device);
	device_add(&ide_isa_device);
    

    return ret;
}

static const device_config_t pck486dx_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "pck486dx_13",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "AMIBIOS Core Version 121291 BIOS Version 1.3 (10/01/1992)", .internal_name = "pck486dx_13", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/pck486dx/PCK_P486DX_1.3.bin", "" } },
            { .name = "AMIBIOS Core Version 060692 BIOS Version 2.0 (11/30/1992)", .internal_name = "pck486dx_20", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/pck486dx/PCK486DX.bin", "" } },
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};

const device_t pck486dx_device = {
    .name          = "Peacock PCK486 DX",
    .internal_name = "pck486dx",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &pck486dx_config[0]
};

int
machine_at_d525_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/d525/BIOSDUMP.ROM",
							0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&keyboard_at_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_wysepc386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/wysepc386/ROM_socket LOW (10E)_labelled HX4 v3.62_D27256_DIP28.bin",
								"roms/machines/wysepc386/ROM_socket HIGH (10F)_labelled HX1 v3.62_HN27256G_DIP28.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&keyboard_ps2_device);
	
	mem_remap_top(384);
	
	device_add_inst(&ns16450_device, 1);
    device_add_inst(&ns16450_device, 2);

	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_mic386p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/mic386p/EVEN LOW.BIN",
							    "roms/machines/mic386p/ODD HIGH.BIN",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&keyboard_at_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static void
machine_at_sis401_common_init(const machine_t *model)
{
    machine_at_common_init(model);
    device_add(&sis_85c401_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
}

int
machine_at_sis401_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sis401/SIS401-2.AMI",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis401_common_init(model);

    return ret;
}

int
machine_at_isa486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/isa486/ISA-486.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis401_common_init(model);

    return ret;
}

int
machine_at_av4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/av4/amibios_486dx_isa_bios_aa4025963.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sis_85c460_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_netplex486_init(const machine_t *model)

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
	
	machine_at_common_init(model);
	
	device_add(&sis_85c461_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
    else {
        for (uint16_t i = 0; i < 32768; i++)
            rom[i] = mem_readb_phys(0x000c0000 + i);
    }
    mem_mapping_set_addr(&bios_mapping, 0x0c0000, 0x40000);
    mem_mapping_set_exec(&bios_mapping, rom);
	
    device_add(&keyboard_ps2_phoenix_pci_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c661_ide_device);
   
    return ret;
}

static const device_config_t netplex486_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "netplex486_sep93",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Phoenix ROM BIOS PLUS 1.10 Revision A01 (09/02/1993)", .internal_name = "netplex486_sep93", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/netplex486/netplex486_sep93.bin", "" } },
            { .name = "Phoenix ROM BIOS PLUS 1.10 Revision A02 (03/24/1994)", .internal_name = "netplex486_mar94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/netplex486/netplex486_mar94.bin", "" } },
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};

const device_t netplex486_device = {
    .name          = "Dell System 4XX/P NetPlex",
    .internal_name = "netplex486",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &netplex486_config[0]
};

int
machine_at_valuepoint433_init(const machine_t *model) // hangs without the PS/2 mouse
{
    int ret;

    ret = bios_load_linear("roms/machines/valuepoint433/$IMAGEP.FLH",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);
    device_add(&sis_85c461_device);
    if (gfxcard[0] == VID_INTERNAL)
        device_add(&et4000w32_onboard_device);

    device_add(&keyboard_ps2_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_pkm0037s_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pkm0037s/0037S.001.03.1.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&sis_85c461_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_a3805_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/a3805/AT27C256@DIP28_even.bin",
                                "roms/machines/a3805/AT27C256@DIP28_odd.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_unitron_u5908_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/unitron_u5908/EVEN.bin",
                                "roms/machines/unitron_u5908/ODD.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_ecs386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/ecs386/AMI BIOS for ECS-386_32 motherboard - L chip.bin",
                                "roms/machines/ecs386/AMI BIOS for ECS-386_32 motherboard - H chip.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_ch38620b_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/ch38620b/EC&T-1133-040990-K8_LOW_EVEN.bin",
                                "roms/machines/ch38620b/EC&T-1133-040990-K8_HIGH_ODD.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_pc60iii_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/pc60iii/cbm-pc60-bios-lo_u52-v1.0-454300-001.bin",
                                "roms/machines/pc60iii/cbm-pc60-bios-lo-u53-v1.0-454301-01.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);
	

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_gs386ct_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/gs386ct/386-Goldstar-E.bin",
                                "roms/machines/gs386ct/386-Goldstar-O.bin",
								0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	device_add(&amstrad_megapc_nvr_device);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_phoenix_device);

    return ret;
}

int
machine_at_msi3102_3_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/msi3102-3/BIOS_3102-3_386_387_CHIPS_L_EVEN.bin",
                                "roms/machines/msi3102-3/BIOS_3102-3_386_387_CHIPS_H_ODD.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_msi3102_7_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/msi3102-7/EVEN.bin",
                                "roms/machines/msi3102-7/ODD.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_mxa386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mxa386/MXA.BIN",
							0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_trump386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/trump386/27C256@DIP28_L.bin",
                                "roms/machines/trump386/27C256@DIP28_H.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_spc6000a_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/spc6000a/3c80.u27",
                                "roms/machines/spc6000a/9f80.u26",
                                0x000f8000, 32768, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 1);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_pg750_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/pg750/pg750_4.0_386_l.bin",
                                "roms/machines/pg750/pg750_4.0_386_h.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_asuscache386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/asuscache386/mainboard-asus-cache386-33-rev21-low-even-659048247505f355495242.bin",
                                "roms/machines/asuscache386/mainboard-asus-cache386-33-rev21-high-odd-6590482460bf2329957475.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs8230_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_device);

    return ret;
}

int
machine_at_ecs386v_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ecs386v/PANDA_386V.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
    return ret;

    machine_at_common_init(model);
    device_add(&ali1429_device);
    device_add(&keyboard_ps2_intel_ami_pci_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_rycleopardlx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/rycleopardlx/486-RYC-Leopard-LX.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti283_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_486vchd_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486vchd/486-4386-VC-HD.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&via_vt82c49x_device);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_gdc425g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/gdc425g/gdc425g.bio",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5428_onboard_device);

    device_add(&via_vt82c49x_device);
    device_add(&keyboard_ps2_device);
    device_add(&fdc37c651_device);
	device_add(&ide_um8673f_device);

    return ret;
}

int
machine_at_cs4031_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cs4031/CHIPS_1.AMI",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs4031_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_advantage486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/advantage486/ast6000_am27c010.bin",
						   0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&cs4031_device);
    device_add(&keyboard_ps2_ami_device);
	device_add(&ide_isa_device);
	device_add(&fdc37c651_device);
	device_add(&intel_flash_bxt_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_ch486s_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ch486s/CH-3486-33G_AMI_060691.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti493_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_cpc2000_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cpc2000/cpc-2000le-67b2074c2ec45690670918.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti493_device);
    device_add(&keyboard_at_phoenix_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_precisionv486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/precisionv486/dell-486sx-65590549071bc198028914.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti493_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_pwrmate486opti_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pwrmate486opti/01_02_18.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti493_device);
	device_add(&ide_isa_device);
	device_add(&fdc37c651_device);
    device_add(&keyboard_ps2_phoenix_device);

    return ret;
}

int
machine_at_pb410a_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pb410a/pb410a.080337.4abf.u25.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_ibm_common_ide_init(model);

    device_add(&keyboard_ps2_device);

    device_add(&acc3221_device);
    device_add(&acc2168_device);

    device_add(&phoenix_486_jumper_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_vect486vl_init(const machine_t *model) // has HDC problems
{
    int ret;

    ret = bios_load_linear("roms/machines/vect486vl/aa0500.ami",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c480_device);

    device_add(&vl82c113_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c651_ide_device);

    return ret;
}

int
machine_at_d824_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/d824/fts-biosupdated824noflashbiosepromv320-320334-160.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c480_device);

    /*
       Technically, it should be the VL82C114 but we do not have
       a proper datasheet of it that tells us the registers.
     */
    device_add(&vl82c113_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c651_device);
    
    return ret;
}

int
machine_at_tuliptc38_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tuliptc38/TULIP1.BIN",
                           0x000f0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c486_device);
    device_add(&tulip_jumper_device);

    device_add(&vl82c113_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c651_ide_device);

    if (gfxcard[0] == VID_INTERNAL) {
        bios_load_aux_linear("roms/machines/tuliptc38/VBIOS.BIN",
                             0x000c0000, 32768, 0);

        device_add(machine_get_vid_device(machine));
    } else  for (uint16_t i = 0; i < 32768; i++)
        rom[i] = mem_readb_phys(0x000c0000 + i);

    mem_mapping_set_addr(&bios_mapping, 0x0c0000, 0x40000);
    mem_mapping_set_exec(&bios_mapping, rom);

    return ret;
}

int
machine_at_martin_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/martin/NONSCSI.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c480_device);
    device_add(&vl82c113_device);

    device_add(&ide_vlb_device);
    device_add(&fdc37c651_ide_device);

    device_add(&intel_flash_bxt_device);
    
    return ret;
}

int
machine_at_acera1g_init(const machine_t *model)

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

    device_add(&ali1429g_device);
    device_add(&keyboard_ps2_acer_pci_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
	
	device_add_params(&pc87310_device, (void *) (PC87310_ALI));
	device_add(&ide_ali5213_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static const device_config_t acera1g_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "acera1g_mar94",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Core Version V1.2 Version R4.0 (03/15/1994)", .internal_name = "acera1g_mar94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/acera1g/acera1g_mar94.bin", "" } },
            { .name = "Core Version V2.0 Version R01-E0 (07/20/1994)", .internal_name = "acera1g_jul94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/acera1g/acera1g_jul94.bin", "" } },
			{ .name = "Core Version V2.0 Version R02-A3 (12/12/1994)", .internal_name = "acera1g_dec94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/acera1g/acera1g_dec94.bin", "" } },
			{ .name = "Core Version V2.0 Version R02-F0 (10/05/1995)", .internal_name = "acera1g_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/acera1g/acera1g_oct95.bin", "" } },  	
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};

const device_t acera1g_device = {
    .name          = "Acer A1G",
    .internal_name = "acera1g",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &acera1g_config[0]
};

int
machine_at_acerv10_init(const machine_t *model)

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

    device_add(&sis_85c461_device);
    device_add(&keyboard_ps2_acer_pci_device);
    device_add(&ide_isa_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static const device_config_t acerv10_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "acerv10_oct93",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Core Version V1.2 Version R1.5 (10/18/1993)", .internal_name = "acerv10_oct93", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/acerv10/acerv10_oct93.bin", "" } },
            { .name = "Core Version V1.2 Version R2.0 (12/07/1993)", .internal_name = "acerv10_dec93", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/acerv10/acerv10_dec93.bin", "" } },
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};

const device_t acerv10_device = {
    .name          = "Acer V10",
    .internal_name = "acerv10",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &acerv10_config[0]
};

int
machine_at_geminivlbus_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000f0000, 65536, 0);
    device_context_restore();
	
	machine_at_common_init(model);

    device_add(&opti495sx_device);
    device_add(&pc87311_ide_device);
    device_add(&keyboard_at_phoenix_device); 

    return ret;
}

static const device_config_t geminivlbus_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "geminivlbus_aug92",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 486 PLUS 0.10 Revision G22-2LB (08/19/1992)", .internal_name = "geminivlbus_aug92", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/geminivlbus/geminivlbus_aug92.bin", "" } },
            { .name = "Version 486 PLUS 0.10 Revision GLB01 (11/04/1992, Gateway 2000 OEM)", .internal_name = "geminivlbus_nov92", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/geminivlbus/geminivlbus_nov92.bin", "" } },
			{ .name = "Version 486 PLUS 0.10 Revision GLB05 (02/08/1993, Gateway 2000 OEM)", .internal_name = "geminivlbus_feb93", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/geminivlbus/geminivlbus_feb93.bin", "" } },
			{ .name = "Version 486 PLUS 0.10 Revision G26-2LB (05/26/1993)", .internal_name = "geminivlbus_may93", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/geminivlbus/geminivlbus_may93.bin", "" } },
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};

const device_t geminivlbus_device = {
    .name          = "Micronics Gemini VL-BUS",
    .internal_name = "geminivlbus",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &geminivlbus_config[0]
};

int
machine_at_gemini33_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/gemini33/U23 - G4 LO -- V.0.10 -- G17-2.bin",
								"roms/machines/gemini33/U21 - G4 HI -- V.0.10 -- G17-2.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti495sx_device);
    device_add(&keyboard_at_phoenix_device);
	device_add(&intel_flash_bxt_device);
	
	 if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);


    return ret;
}

int
machine_at_jx30g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/jx30g/Micronics_GJX30G-04P_09-24-93.BIN",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti495sx_device);
    device_add(&keyboard_ps2_phoenix_device);
	device_add(&intel_flash_bxt_device);
	
	 if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);


    return ret;
}

int
machine_at_classic_e_series_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/classic_e_series/classic_e_series.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti495sx_device);
	device_add(&keyboard_ps2_phoenix_device);
    device_add(&pc87311_ide_device);
	device_add(&intel_flash_bxt_device);
    
    return ret;
}

int
machine_at_valueplusdl_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/valueplusdl/ICL_Valueplus_BIOS_1.05JA.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
	
    machine_at_common_init(model);

    device_add(&via_vt82c49x_device);
    device_add(&keyboard_ps2_ami_device);
	device_add(&fdc37c651_device);

    return ret;
}

int
machine_at_evoiv_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/evoiv/alrevo-63d7d9e0c8206403830081.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&vl82c480_device);
	device_add(&vl82c113_device);
    device_add(&fdc37c663_device);
    
    return ret;
}

int
machine_at_lp486ms_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/lp486ms/compuadd-lp486ms-6757107ea96a1986417891.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c480_device);
	device_add(&vl82c113_device);
	
	device_add(&phoenix_486_jumper_pci_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&ide_isa_device);
    device_add(&fdc37c661_ide_device);
    
    return ret;
}

int
machine_at_prolineamt_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/prolineamt/bios93.10.27.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c486_device);
    device_add(&vl82c113_device);
	device_add(&ide_isa_device);
    device_add(&fdc37c661_ide_device);
	device_add(&amd_am28f010_flash_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
    
    return ret;
}

int
machine_at_prolinea466_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/prolinea466/COMPAQ_PROLINEA_07-29-94.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c486_device);
    device_add(&vl82c113_device);
	device_add(&ide_isa_device);
	device_add(&fdc37c665_ide_device);
	device_add(&amd_am28f010_flash_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
    
    return ret;
}

int
machine_at_ms4130_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms4130/MS4130.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&sis_85c461_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_auva486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/auva486/CAM33-40-50-SG0.rom",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&sis_85c461_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_decpclpv_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/decpclpv/bios.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&sis_85c461_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&keyboard_ps2_phoenix_pci_device);

    device_add(&ide_isa_2ch_device);
    device_add(&fdc37c663_ide_device);

    return ret;
}

int
machine_at_dell466np_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dell466np/466np.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sis_85c461_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
    else {
        for (uint16_t i = 0; i < 32768; i++)
            rom[i] = mem_readb_phys(0x000c0000 + i);
    }
    mem_mapping_set_addr(&bios_mapping, 0x0c0000, 0x40000);
    mem_mapping_set_exec(&bios_mapping, rom);

    device_add(&keyboard_ps2_phoenix_pci_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c661_ide_device);

    return ret;
}

static void
machine_at_ali1429_common_init(const machine_t *model, int is_green)
{
    machine_at_common_init(model);

    if (is_green)
        device_add(&ali1429g_device);
    else
        device_add(&ali1429_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
}

int
machine_at_ali1429_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ali1429/ami486.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_ali1429_common_init(model, 0);

    return ret;
}

int
machine_at_olivettim4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/olivettim4/trout09.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
	device_add(&port_6x_olivetti_device);
	
	device_add(&et6000_device);
	
	device_add(&keyboard_ps2_olivetti_device);
	
	device_add_params(&pc87310_device, (void *) (PC87310_IDE));

    return ret;
}

int
machine_at_winbios1429_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/win486/ali1429g.amw",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_ali1429_common_init(model, 1);

    return ret;
}

int
machine_at_opti495_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/award495/opt495s.awa",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti495sx_device);

    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static void
machine_at_opti495_ami_common_init(const machine_t *model)
{
    machine_at_common_init(model);

    device_add(&opti495sx_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
}

int
machine_at_acervi9_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acervi9/Acer_Vi9.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_opti495_ami_common_init(model);

    return ret;
}

int
machine_at_opti495_ami_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ami495/opt495sx.ami",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_opti495_ami_common_init(model);

    return ret;
}

int
machine_at_fic4386vio_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/fic4386vio/4386V.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&via_vt82c49x_device);
    device_add(&keyboard_at_device);
	device_add(&fdc37c661_device);
	device_add(&ide_isa_device);

    return ret;
}

int
machine_at_opti495_mr_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mr495/opt495sx.mr",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_opti495_ami_common_init(model);

    return ret;
}

int
machine_at_exp4349_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/exp4349/biosdump.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1429g_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
    return ret;
}

static void
machine_at_403tg_common_init(const machine_t *model, int nvr_hack)
{
    if (nvr_hack) {
        machine_at_common_init_ex(model, 2);
        device_add(&ami_1994_nvr_device);
    } else
        machine_at_common_init(model);

    device_add(&opti895_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
}

int
machine_at_403tg_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/403tg/403TG.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_403tg_common_init(model, 0);

    return ret;
}

int
machine_at_403tg_d_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/403tg_d/J403TGRevD.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_403tg_common_init(model, 1);

    return ret;
}

int
machine_at_403tg_d_mr_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/403tg_d/MRBiosOPT895.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_403tg_common_init(model, 0);

    return ret;
}

int
machine_at_d819_init(const machine_t *model)

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
	
	device_add(&vl82c480_device);
    device_add(&vl82c113_device);	

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
	
    device_add(&ide_vlb_device);       
    device_add(&fdc37c665_ide_pri_device); 

    return ret;
}

static const device_config_t d819_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "v325819",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 3.25.819 (08/19/1994)", .internal_name = "v325819", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d819/d819.bin", "" } },
			{ .name = "Version 3.31.819 (01/11/1995)", .internal_name = "v331819", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d819/d819jan95.bin", "" } },
            { .name = "Version 3.32.819 (08/09/1996)", .internal_name = "v332819", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d819/d819aug96.bin", "" } },
            { .name = "Version 3.34.819 (02/06/1998)", .internal_name = "v334819", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d819/lastest.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};


// Device instance for D819
const device_t d819_device = {
    .name          = "Siemens-Nixdorf PCD-4H/VL",
    .internal_name = "pcd4h_vl",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d819_config[0]
};

int
machine_at_d802a_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/d802a/fts-biosupdated802noflashbiosepromv320-320334-152.bin-5f63d7470ef38003821803.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c480_device);
	device_add(&vl82c113_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
 
    device_add(&ide_isa_device);
    device_add(&fdc37c651_ide_device);
    
    return ret;
}

int
machine_at_dim486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dim486/DELL.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	
    device_add(&vl82c480_device);
	device_add(&vl82c113_device);

    device_add(&ide_isa_device);
    device_add(&pc87311_ide_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
    
    return ret;
}

int
machine_at_tekat4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tekat4/b703-eb-61e09ec51e0b6814876356.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c486_device);
	device_add(&vl82c113_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c651_device);
    
    return ret;
}

int
machine_at_sensation_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sensation/P1033PCD_01.10.01_11-11-92_E687_Sensation_1_BIOS.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c486_device);
	device_add(&vl82c113_device);
	device_add(&fdc37c651_device);


    device_add(&ide_isa_device);
    

    
    return ret;
}

int
machine_at_sensationii_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sensationii/TANDY_SENSATION_2_011004_10051993.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&vl82c486_device);
	device_add(&vl82c113_device);
	device_add(&fdc37c651_ide_device);

    device_add(&ide_isa_device);

	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    
    return ret;
}

int
machine_at_fic486gac2_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/fic486gac2/bios.bin-5f28285c93b72279880948.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ide_vlb_device);

    device_add(&via_vt82c49x_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c661_ide_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_fic486giovt2_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000f0000, 65536, 0);
    device_context_restore();
	
	machine_at_common_init_ex(model, 2);
	device_add(&amstrad_megapc_nvr_device);

    device_add(&ide_vlb_2ch_device);
    device_add(&via_vt82c49x_device); /* actually uses a VT82C486, but not implemented yet. */
    device_add(&keyboard_at_ami_device);
    device_add(&w83787f_device);

    return ret;
}

static const device_config_t fic486giovt2_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "fic486giovt2_jul94",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50G Revision 3.26G (07/06/1994)", .internal_name = "fic486giovt2_jul94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/fic486giovt2/fic486giovt2_jul94.bin", "" } },
            { .name = "Version 1.03 Revision VBS1.04 (08/30/1994, Highscreen OEM)", .internal_name = "fic486giovt2_aug94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/fic486giovt2/fic486giovt2_aug94.bin", "" } },
			{ .name = "Version 1.03 Revision 3.07G (11/02/1994)", .internal_name = "fic486giovt2_nov94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/fic486giovt2/fic486giovt2_nov94.bin", "" } },
			{ .name = "Version 1.03 Revision VBS1.061h (01/20/1995, Highscreen OEM)", .internal_name = "fic486giovt2_jan95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/fic486giovt2/fic486giovt2_jan95.bin", "" } },
			{ .name = "Version 1.03 Revision 3.274 (01/08/1997, text corruption bug)", .internal_name = "fic486giovt2_jan97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/fic486giovt2/fic486giovt2_jan97.bin", "" } },  
            { .name = "Version 1.03 Revision 3.276 (07/17/1997, Beta)", .internal_name = "fic486giovt2_jul97", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/fic486giovt2/fic486giovt2_jul97.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};



const device_t fic486giovt2_device = {
    .name          = "FIC 486-GIO-VT2",
    .internal_name = "fic486giovt2",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &fic486giovt2_config[0]
};

int
machine_at_hyundaisolomon486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hyundaisolomon486/AMIBIOS_Hyundai_Solomon_486SX33HMz_ORG.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ide_isa_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5428_onboard_device);

    device_add(&opti895_device);
    device_add(&opti601_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_device);

    return ret;
}

int
machine_at_ih4077d_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ih4077d/r1-01-mx27c512-at-dip28-66ddfed8cf07c059580779.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ide_isa_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5428_onboard_device);

    device_add(&opti895_device);
    device_add(&opti602_device);
    device_add(&opti822_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_device);

    return ret;
}

int
machine_at_globalyst550_init(const machine_t *model)
{
	
	int ret;

    ret = bios_load_linear("roms/machines/globalyst550/3246-5fa2819a96b67074231532.bin",
                           0x000e0000, 131072, 0);
	
	if (bios_only || !ret)
        return ret;					   
						   
    machine_at_common_init_ex(model, 2);
    device_add(&ide_vlb_2ch_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      5, 6, 7, 8);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&opti602_device);
    device_add(&opti802g_device);
    device_add(&opti822_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);

}

int
machine_at_ibmaptiva486c2_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ibmaptiva486c2/CSI_CAT28F010_2168C2_11H2337_CS8700.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	device_add(&ide_vlb_2ch_device);
    
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5430_onboard_vlb_device);

    device_add(&opti802g_device);
    device_add(&opti602_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_ide_device);
	device_add(&ide_opti611_vlb_device);

    return ret;
}

int
machine_at_ibmpc330_type6571_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ibmpc330_type6571/11h2236-1994-ibm-corp-m28f101-6798054e7382c844008182.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
	device_add(&ide_vlb_2ch_device);
    
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5430_onboard_vlb_device);

    device_add(&opti802g_device);
    device_add(&opti602_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_ide_device);
	device_add(&ide_opti611_vlb_device);
	device_add(&intel_flash_bxt_device);

    return ret;
}

static const device_config_t pb450_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "pb450a",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "PCI 1.0A", .internal_name = "pb450a" /*"pci10a"*/, .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/pb450/OPTI802.bin", "" } },
            { .name = "PNP 1.1A", .internal_name = "pnp11a", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/pb450/PNP11A.bin", "" } },
            { .name = "P4HS20 (Micro Firmware/Phoenix 4.05)", .internal_name = "p4hs20", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/pb450/p4hs20.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t pb450_device = {
    .name          = "Packard Bell PB450",
    .internal_name = "pb450_device",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = pb450_config
};

int
machine_at_pb450_init(const machine_t *model)
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
    device_add(&ide_vlb_2ch_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      5, 6, 7, 8);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&opti895_device);
    device_add(&opti602_device);
    device_add(&opti822_device);
    device_add(&keyboard_ps2_phoenix_device);
    device_add(&fdc37c665_ide_device);
    device_add(&ide_opti611_vlb_sec_device);
    device_add(&intel_flash_bxt_device);
    device_add(&phoenix_486_jumper_pci_device);

    return ret;
}

static void
machine_at_pc330_6573_common_init(const machine_t *model)
{
    machine_at_common_init_ex(model, 2);
    device_add(&ide_vlb_2ch_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE,  0,  0,  0,  0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,       1,  2,  3,  4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,       5,  6,  7,  8);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,       9, 10, 11, 12);
    /* This is a guess because the BIOS always gives it a video BIOS
       and never gives it an IRQ, so it is impossible to known for
       certain until we obtain PCI readouts from the real machine. */
    pci_register_slot(0x0E, PCI_CARD_VIDEO,       13, 14, 15, 16);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&opti602_device);
    device_add(&opti802g_device);
    device_add(&opti822_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&fdc37c665_ide_device);
    device_add(&ide_opti611_vlb_device);
    device_add(&intel_flash_bxt_device);
}

int
machine_at_aptiva510_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/aptiva510/$IMAGES.USF",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_pc330_6573_common_init(model);

    return ret;
}

int
machine_at_pc330_6573_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pc330_6573/$IMAGES.USF",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_pc330_6573_common_init(model);

    return ret;
}

static const device_config_t ibmvaluepointperformance_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "ibmvaluepointperformance",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "LDJT67A (English)", .internal_name = "ibmvaluepointperformance", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGE.FLH", "" } },
            { .name = "LDJT67A (German)", .internal_name = "ibmvaluepointperformance_gr", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGEGERMAN.FLH", "" } },
            { .name = "LDJT67A (Italian)", .internal_name = "ibmvaluepointperformance_it", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGEITALIAN.FLH", "" } },
			{ .name = "LDJT67A (Belgian French)", .internal_name = "ibmvaluepointperformance_bf", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGEBF.FLH", "" } },
			{ .name = "LDJT67A (Danish)", .internal_name = "ibmvaluepointperformance_dk", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGEDANISH.FLH", "" } },
			{ .name = "LDJT67A (Dutch)", .internal_name = "ibmvaluepointperformance_nl", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGEDUTCH.FLH", "" } },
			{ .name = "LDJT67A (Polish)", .internal_name = "ibmvaluepointperformance_pl", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGEPOLISH.FLH", "" } },
			{ .name = "LDJT67A (Suomi)", .internal_name = "ibmvaluepointperformance_fi", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGESUOMI.FLH", "" } },
			{ .name = "LDJT67A (Swiss German)", .internal_name = "ibmvaluepointperformance_sg", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ibmvaluepointperformance/$IMAGESWISSGERMAN.FLH", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t ibmvaluepointperformance_device = {
    .name          = "IBM ValuePoint Performance Series (6472, 6482, 6484)",
    .internal_name = "ibmvaluepointperformance",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = ibmvaluepointperformance_config
};

int
machine_at_ibmvaluepointperformance_init(const machine_t *model)
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
	
	device_add(&ide_opti611_vlb_device);
    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE,  0,  0,  0,  0);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,       1,  2,  3,  4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,       5,  6,  7,  8);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,       9, 10, 11, 12);
	/*pci_register_slot(0x0E, PCI_CARD_VIDEO,       13, 14, 15, 16); */
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&opti601_device);
    device_add(&opti802g_device);
    device_add(&opti822_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&pc87332_device);
    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_mvi486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mvi486/MVI627.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti498_device);
    device_add(&keyboard_at_device);
    device_add(&pc87311_ide_device);

    return ret;
}

int
machine_at_decpclp_init(const machine_t *model)

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

    device_add(&opti495sx_device);
    device_add(&keyboard_ps2_phoenix_device);
    device_add(&pc87311_ide_device);  

    return ret;
}


static const device_config_t decpclp_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "decpclp_v108",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "PhoenixBIOS A486 Revision 1.08", .internal_name = "decpclp_v108", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/decpclp/decpclp_v108.bin", "" } },
			{ .name = "PhoenixBIOS A486 Revision 1.20", .internal_name = "decpclp_v120", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/decpclp/decpclp_v120.bin", "" } },
			{ .name = "PhoenixBIOS A486 Revision 1.22", .internal_name = "decpclp_v122", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/decpclp/decpclp_v122.bin", "" } },
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};


const device_t decpclp_device = {
    .name          = "DEC DECpc LP",
    .internal_name = "decpc_lp",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &decpclp_config[0]
};

int
machine_at_c747_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/c747/486-C747 Tandon.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti495sx_device);
    device_add(&keyboard_at_device);
    device_add(&um8663af_ide_device);

    return ret;
}

int
machine_at_d756_init(const machine_t *model)

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

     if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
	
	device_add(&ide_isa_device);
    device_add(&opti498_device);
    device_add(&keyboard_ps2_device);
    device_add(&fdc37c651_device);
    return ret;
}

static const device_config_t d756_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d756_nov93",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
			{ .name = "Version 3.20 Revision 3.10 (09/20/1993)", .internal_name = "d756_sep93", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d756/d756_sep93.bin", "" } },
            { .name = "Version 3.20 Revision 3.12 (11/25/1993)", .internal_name = "d756_nov93", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d756/d756_nov93.bin", "" } },
			{ .name = "Version 3.16.756 (01/22/1998)", .internal_name = "d756_jan98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d756/d756_jan98.bin", "" } },    
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};


const device_t d756_device = {
    .name          = "Siemens Nixdorf PCD-4Lsx/Ldx/PCD-4H (D756)",
    .internal_name = "d756",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d756_config[0]
};

int
machine_at_svc486bb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/svc486bb/486-Azza_4SGS-3VL-AM27C512DIP28.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti495sx_device);
    device_add(&keyboard_at_device);
    device_add(&ide_isa_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static void
machine_at_sis_85c471_common_init(const machine_t *model)
{
    machine_at_common_init(model);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&sis_85c471_device);
}

int
machine_at_ami471_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ami471/SIS471BE.AMI",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis_85c471_common_init(model);
    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_advantage40xxd_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/advantage40xxd/AST101.09A",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sis_85c471_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&keyboard_ps2_phoenix_device);
    device_add(&um82c863f_ide_device);

    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_vli486sv2g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/vli486sv2g/0402.001",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis_85c471_common_init(model);
    device_add(&keyboard_ps2_ami_device);

    return ret;
}

int
machine_at_dtk486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dtk486/4siw005.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis_85c471_common_init(model);
    device_add(&keyboard_at_device);

    return ret;
}

int
machine_at_px471_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/px471/SIS471A1.PHO",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis_85c471_common_init(model);
    device_add(&ide_vlb_device);
    device_add(&keyboard_at_device);

    return ret;
}

int
machine_at_win471_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/win471/486-SiS_AC0360136.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis_85c471_common_init(model);
    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_decpclpxplus_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/decpclpxplus/BGLPXG04.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
	
	machine_at_common_init(model);
    device_add(&sis_85c461_device);
    device_add(&ide_isa_device);
    device_add(&fdc37c665_ide_pri_device);
    device_add(&keyboard_ps2_phoenix_device);

    return ret;
}

int
machine_at_pci400ca_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pci400ca/486-AA008851.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SCSI,        1, 2, 3, 4);
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x04, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x05, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_at_ami_device);
    device_add(&sio_device);
    device_add(&intel_flash_bxt_ami_device);

    device_add(&i420tx_device);
    device_add(&ncr53c810_onboard_pci_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_xps486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/xps486/xps486.rom",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_ps2_phoenix_device);
	
	device_add(&sis_85c461_device);
	device_add(&fdc37c665_ide_device);
    device_add(&sst_flash_29ee010_device);
	device_add(&ide_opti611_vlb_device);
	return ret;
}

int
machine_at_jx30g04p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/jx30g04p/Gjx30.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_ps2_phoenix_device);
	
	device_add(&sis_85c471_device);
	device_add(&fdc37c665_device);
	device_add(&ide_cmd640_vlb_device);
	device_add(&intel_flash_bxt_device);
	return ret;
}

int
machine_at_jx30vlbus_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/jx30vlbus/JX30-6.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_ps2_phoenix_device);
	
	device_add(&sis_85c461_device);
	device_add(&fdc37c665_ide_device);
	device_add(&ide_opti611_vlb_device);
	return ret;
}

int
machine_at_jx30gcvlbus_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/jx30gcvlbus/GC-02.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_ps2_phoenix_device);
	
	device_add(&sis_85c461_device);
	device_add(&fdc37c665_ide_device);
	device_add(&ide_opti611_vlb_device);
	return ret;
}

int
machine_at_mpower4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_inverted("roms/machines/mpower4/MPW4-09.bin",
									0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_ps2_phoenix_device);
	
	device_add(&sis_85c461_device);
	device_add(&fdc37c665_device);
	device_add(&ide_cmd640_vlb_device);
	device_add(&intel_flash_bxt_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));
	
	return ret;
}

int
machine_at_lpm30_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/lpm30/n28f001bx-t-at-plcc32-miconics-lpm30-672d0f4956595929959378.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_ps2_phoenix_device);
	
	device_add(&sis_85c471_device);
	device_add(&fdc37c665_device);
	device_add(&ide_cmd640_vlb_device);
	device_add(&intel_flash_bxt_device);
	return ret;
}

int
machine_at_vi15g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/vi15g/vi15gr23.rom",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis_85c471_common_init(model);
    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_greenb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/greenb/4gpv31-ami-1993-8273517.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&contaq_82c597_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_4gpv5_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/4gpv5/4GPV5.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&contaq_82c596a_device);

    device_add(&keyboard_at_device);

    return ret;
}

static void
machine_at_sis_85c496_common_init(UNUSED(const machine_t *model))
{
    device_add(&ide_pci_2ch_device);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);

    pci_set_irq_routing(PCI_INTA, PCI_IRQ_DISABLED);
    pci_set_irq_routing(PCI_INTB, PCI_IRQ_DISABLED);
    pci_set_irq_routing(PCI_INTC, PCI_IRQ_DISABLED);
    pci_set_irq_routing(PCI_INTD, PCI_IRQ_DISABLED);
}

int
machine_at_r418_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/r418/r418i.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_device);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x07, PCI_CARD_NORMAL, 4, 1, 2, 3);

    device_add(&fdc37c665_device);
    device_add(&keyboard_ps2_pci_device);

    return ret;
}

int
machine_at_m4li_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m4li/M4LI.04S",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_device);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x07, PCI_CARD_NORMAL, 4, 1, 2, 3);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 3, 4, 1, 2);

    device_add(&fdc37c665_device);
    device_add(&keyboard_ps2_pci_device);

    return ret;
}

int
machine_at_ls486e_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ls486e/LS486E RevC.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_ls486e_device);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x06, PCI_CARD_NORMAL, 4, 1, 2, 3);

    device_add(&fdc37c665_device);
    device_add(&keyboard_ps2_ami_pci_device);

    return ret;
}

int
machine_at_4dps_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/4dps/4DPS172G.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_device);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0E, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x07, PCI_CARD_NORMAL, 4, 1, 2, 3);

    device_add(&w83787f_device);
    device_add(&keyboard_ps2_ami_device);

    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_ms4144_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms4144/ms-4144-1.4.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_ls486e_device);
    pci_register_slot(0x03, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 3, 4, 1, 2);

    device_add(&w83787f_device);
    device_add(&keyboard_at_ami_device);

    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_acerp3_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acerp3/Acer Mate 600 P3 BIOS U13 V2.0R02-J3 ACR8DE00-S00-950911-R02-J3.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_device);
    pci_register_slot(0x09, PCI_CARD_VIDEO,  0, 0, 0, 0);
    pci_register_slot(0x0A, PCI_CARD_IDE, 	 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL, 2, 3, 4, 1);
	pci_register_slot(0x14, PCI_CARD_NORMAL, 1, 2, 3, 4);

    device_add(&fdc37c665_ide_device);
    device_add(&keyboard_ps2_acer_pci_device);
	device_add(&ide_cmd640_pci_legacy_only_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5434_onboard_pci_device);

    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_486sp3c_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486sp3c/SI4I0306.AWD",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_device);
    pci_register_slot(0x0C, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL, 3, 4, 1, 2);

    device_add(&fdc37c665_device);
    device_add(&keyboard_ps2_ami_pci_device);

    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_4saw2_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/4saw2/4saw0911.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    machine_at_sis_85c496_common_init(model);
    device_add(&sis_85c496_device);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0F, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x11, PCI_CARD_NORMAL, 4, 1, 2, 3);

    device_add(&w83787f_device);
    device_add(&keyboard_ps2_pci_device);

    device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_alfredo_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/alfredo/1010AQ0_.BIO",
                                    "roms/machines/alfredo/1010AQ0_.BI1", 0x1c000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&amstrad_megapc_nvr_device);
    device_add(&ide_pci_device);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_IDE,         0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      3, 2, 1, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 1, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_phoenix_device);
    device_add(&sio_device);
    device_add(&fdc37c663_device);
    device_add(&intel_flash_bxt_ami_device);

    device_add(&i420tx_device);

    return ret;
}

int
machine_at_entrada_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/entrada/03_AZ0.BIO",
                                    "roms/machines/entrada/03_AZ0.BI1", 0x1c000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&amstrad_megapc_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
	pci_register_slot(0x08, PCI_CARD_VIDEO,       2, 1, 1, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      2, 3, 2, 3);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 2, 3, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 1, 2, 1);
    device_add(&keyboard_ps2_phoenix_device);
    device_add(&intel_flash_bxt_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&i420ex_device);
    device_add(&i82091aa_device);

    return ret;
}

int
machine_at_ninja_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_combined("roms/machines/ninja/1008AY0_.BIO",
                                    "roms/machines/ninja/1008AY0_.BI1", 0x1c000, 128);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&amstrad_megapc_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 2, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 1, 2, 1);
    device_add(&keyboard_ps2_phoenix_device);
    device_add(&intel_flash_bxt_ami_device);

    device_add(&i420ex_device);
    device_add(&i82091aa_device);

    return ret;
}

int
machine_at_bat4ip3e_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/bat4ip3e/404C.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_IDE,   0xfe, 0xff, 0, 0);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 1, 2);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 1, 2, 1);
    pci_register_slot(0x0a, PCI_CARD_NORMAL,      1, 2, 1, 2);

    device_add(&phoenix_486_jumper_pci_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&i420ex_device);
    device_add(&ide_cmd640_pci_device);
    device_add(&fdc37c665_device);

    return ret;
}

int
machine_at_486pi_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486pi/486pi.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_NORMAL,      1, 2, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 2, 1);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 1, 2);

    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&fdc37c665_device);
    device_add(&i420ex_device);

    return ret;
}

int
machine_at_sb486p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sb486p/amiboot.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      1, 2, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 2, 1);

    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&i82091aa_device);
    device_add(&i420ex_device);

    return ret;
}

int
machine_at_486sp3_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486sp3/awsi2737.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ide_isa_device);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SCSI,        1, 2, 3, 4); /* 01 = SCSI */
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4); /* 03 = Slot 1 */
    pci_register_slot(0x04, PCI_CARD_NORMAL,      2, 3, 4, 1); /* 04 = Slot 2 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      3, 4, 1, 2); /* 05 = Slot 3 */
    pci_register_slot(0x06, PCI_CARD_NORMAL,      4, 1, 2, 3); /* 06 = Slot 4 */
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_at_ami_device); /* Uses the AMIKEY KBC */
    device_add(&sio_device);
    device_add(&fdc37c663_ide_device);
    device_add(&sst_flash_29ee010_device);

    device_add(&i420tx_device);
    device_add(&ncr53c810_onboard_pci_device);

    return ret;
}

int
machine_at_amis76_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_inverted("roms/machines/s76p/S76P.ROM",
                                    0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    // pci_register_slot(0x01, PCI_CARD_IDE,         1, 2, 3 ,4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sio_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_ami_device);

    device_add(&i420tx_device);
    // device_add(&ide_cmd640_pci_device); /* is this actually cmd640? is it single channel? */
    device_add(&ide_pci_device);

    return ret;
}

int
machine_at_m4p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear_inverted("roms/machines/m4p/M4P-05.bin",
									0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ide_pci_device);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_at_ami_device); /* Uses the AMIKEY KBC */
    device_add(&sio_device);
    device_add(&fdc37c665_ide_sec_device);
    device_add(&intel_flash_bxt_ami_device);

    device_add(&i420tx_device);

    return ret;
}

int
machine_at_pci400cb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pci400cb/032295.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ami_1994_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      4, 3, 2, 1); /* 0F = Slot 1 */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2); /* 0E = Slot 2 */
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1); /* 0D = Slot 3 */
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* 0C = Slot 4 */
    device_add(&keyboard_ps2_ami_pci_device);                  /* Assume AMI Megakey 1993 standalone ('P')
                                                                  because of the Tekram machine below. */

    device_add(&ims8848_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_g486ip_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/g486ip/G486IP.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&ami_1992_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2); /* 03 = Slot 1 */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1); /* 04 = Slot 2 */
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4); /* 05 = Slot 3 */
    device_add(&keyboard_ps2_ami_pci_device);                  /* AMI Megakey 1993 stanalone ('P') */

    device_add(&ims8848_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_d800_init(const machine_t *model)

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
	pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x03, PCI_CARD_VIDEO,       4, 0, 0, 0); /* Onboard */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 3, 2, 4); /* Slot 01 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 1, 3, 4); /* Slot 02 */
    device_add(&i420zx_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&sio_zb_device);
    device_add(&fdc37c665_device);
	device_add(&amd_am28f010_flash_device);

    return ret;
}


static const device_config_t d800_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d800_mar95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 1.03 Revision 1.04.800 (08/10/1994)", .internal_name = "d800_aug94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d800/d800_aug94.bin", "" } },
            { .name = "Version 1.03 Revision 1.09.800 (03/30/1995)", .internal_name = "d800_mar95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d800/d800.bin", "" } },
			{ .name = "Version 1.03 Revision 1.10.800 (05/28/1996)", .internal_name = "d800_may96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d800/d800_may96.bin", "" } },
			{ .name = "Version 1.03 Revision 1.11.800 (11/25/1996)", .internal_name = "d800_nov96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d800/d800_nov96.bin", "" } },
            { .name = "Version 1.03 Revision 1.12.800 (06/02/1998)", .internal_name = "d800_jun98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d800/d800_jun98.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};

// Device instance for D800
const device_t d800_device = {
    .name          = "Siemens-Nixdorf PCD-4H/PCI",
    .internal_name = "pcd4h_pci",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d800_config[0]
};

int
machine_at_486sp3g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486sp3g/PCI-I-486SP3G_0306.001 (Beta).bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ide_isa_device);

    pci_init(PCI_CONFIG_TYPE_2);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SCSI,        1, 2, 3, 4); /* 01 = SCSI */
    pci_register_slot(0x06, PCI_CARD_NORMAL,      1, 2, 3, 4); /* 06 = Slot 1 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      2, 3, 4, 1); /* 05 = Slot 2 */
    pci_register_slot(0x04, PCI_CARD_NORMAL,      3, 4, 1, 2); /* 04 = Slot 3 */
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_ami_pci_device);                  /* Uses the AMIKEY KBC */
    device_add(&sio_zb_device);
    device_add(&pc87332_398_ide_device);
    device_add(&sst_flash_29ee010_device);

    device_add(&i420zx_device);
    device_add(&ncr53c810_onboard_pci_device);

    return ret;
}

static const device_config_t sb486pv_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "sb486pv",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "AMI 062594 (0108)", .internal_name = "sb486pv", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/sb486pv/41-0108-062594-SATURN2.rom", "" } },
            { .name = "AMI 062594 (0301)", .internal_name = "sb486pv_94", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/sb486pv/0301-062594-SATURN2.rom", "" } },
            { .name = "AMI 071595 (1301)", .internal_name = "sb486pv_95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/sb486pv/amiboot.rom", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t sb486pv_device = {
    .name          = "ICS SB486PV",
    .internal_name = "sb486pv_device",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = sb486pv_config
};

int
machine_at_sb486pv_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios"), 0);
    if (!strcmp(fn, "roms/machines/sb486pv/amiboot.rom"))
        ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    else
        ret = bios_load_linear_inverted(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_2);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0e, PCI_CARD_IDE,         0, 0, 0, 0);
    pci_register_slot(0x0f, PCI_CARD_VIDEO,       1, 2, 3, 4);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sio_zb_device);
    device_add(&ide_rz1000_pci_single_channel_device);
    device_add(&i82091aa_26e_device);
    if (!strcmp(fn, "roms/machines/sb486pv/amiboot.rom"))
        device_add(&intel_flash_bxt_device);
    else
        device_add(&intel_flash_bxt_ami_device);

    device_add(&i420zx_device);

    return ret;
}

int
machine_at_486ap4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486ap4/0205.002",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    /* Excluded: 5, 6, 7, 8 */
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4); /* 09 = Slot 1 */
    pci_register_slot(0x0a, PCI_CARD_NORMAL,      2, 3, 4, 1); /* 0a = Slot 2 */
    pci_register_slot(0x0b, PCI_CARD_NORMAL,      3, 4, 1, 2); /* 0b = Slot 3 */
    pci_register_slot(0x0c, PCI_CARD_NORMAL,      4, 1, 2, 3); /* 0c = Slot 4 */
    device_add(&keyboard_ps2_ami_pci_device);                  /* Uses the AMIKEY KBC */

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&i420ex_device);

    return ret;
}

int
machine_at_g486vpa_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/g486vpa/3.BIN",
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

    device_add(&via_vt82c49x_pci_ide_device);
    device_add(&via_vt82c505_device);
    device_add(&pc87332_398_ide_sec_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_486vipio2_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486vipio2/1175G701.BIN",
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

    device_add(&via_vt82c49x_pci_ide_device);
    device_add(&via_vt82c505_device);
    device_add(&w83787f_ide_sec_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_abpb4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/abpb4/486-AB-PB4.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CAN_SWITCH_TYPE);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x04, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x05, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&ali1489_device);
    device_add(&w83787f_device);
    device_add(&keyboard_at_device);
#if 0
    device_add(&intel_flash_bxt_device);
#endif
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_win486pci_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/win486pci/v1hj3.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x04, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x05, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&ali1489_device);
    device_add(&prime3b_device);
    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_ga5486al_init(const machine_t *model)

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
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 	  0, 0, 0, 0); /* Onboard */
    pci_register_slot(0x03, PCI_CARD_NORMAL, 	  	  1, 2, 3, 4); /* Slot 1 */
	pci_register_slot(0x04, PCI_CARD_VIDEO,           2, 3, 4, 1); /* Slot 2 */
    pci_register_slot(0x05, PCI_CARD_NORMAL,      	  3, 4, 1, 2); /* Slot 3 */
	device_add(&ali1489_device);
	device_add(&um8663bf_ide_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_at_ami_device);

    return ret;
}

static const device_config_t ga5486al_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ga5486al_jan96",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Core Version 4.50PG BIOS Version 1.06 (01/17/1996)", .internal_name = "ga5486al_jan96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga5486al/ga5486al_jan96.bin", "" } },
            { .name = "Core Version 4.50PG BIOS Version 1.07 (05/20/1996)", .internal_name = "ga5486al_may96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga5486al/ga5486al_may96.bin", "" } },
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
	// clang-format on
};

const device_t ga5486al_device = {
    .name          = "Gigabyte GA-5486AL",
    .internal_name = "ga5486al",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ga5486al_config[0]
};

int
machine_at_ms4145_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms4145/AG56S.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x03, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x04, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x05, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      4, 1, 2, 3);

    device_add(&ali1489_device);
    device_add(&w83787f_device);
    device_add(&keyboard_at_ami_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_sbc490_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sbc490/07159589.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_VIDEO,       4, 1, 2, 3);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&ali1489_device);
    device_add(&fdc37c665_device);
    device_add(&keyboard_ps2_ami_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_tf486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tf486/tf486v10.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);

    device_add(&ali1489_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_at_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_arb1476_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/arb1476/w1476b.v21",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);

    device_add(&ali1489_device);
    device_add(&fdc37c669_device);
    device_add(&keyboard_ps2_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_itoxstar_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/itoxstar/STARA.ROM",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x0B, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x1F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    device_add(&w83977f_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&stpc_client_device);
    device_add(&sst_flash_29ee020_device);
    device_add(&w83781d_device);    /* fans: Chassis, CPU, unused; temperatures: Chassis, CPU, unused */
    hwm_values.fans[2]         = 0; /* unused */
    hwm_values.temperatures[2] = 0; /* unused */
    hwm_values.voltages[0]     = 0; /* Vcore unused */

    return ret;
}

int
machine_at_arb1423c_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/arb1423c/A1423C.v12",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x0B, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x1F, PCI_CARD_NORMAL,      1, 0, 0, 0);
    pci_register_slot(0x1E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x1D, PCI_CARD_NORMAL,      3, 4, 1, 2);
    device_add(&w83977f_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&stpc_consumer2_device);
    device_add(&winbond_flash_w29c020_device);

    return ret;
}

int
machine_at_arb1479_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/arb1479/1479A.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x0B, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x1F, PCI_CARD_NORMAL,      1, 0, 0, 0);
    pci_register_slot(0x1E, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x1D, PCI_CARD_NORMAL,      3, 4, 1, 2);
    device_add(&w83977f_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&stpc_consumer2_device);
    device_add(&winbond_flash_w29c020_device);

    return ret;
}

int
machine_at_iach488_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/iach488/FH48800B.980",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x0B, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&w83977f_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&stpc_consumer2_device);
    device_add(&sst_flash_29ee020_device);

    return ret;
}

int
machine_at_pcm9340_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pcm9340/9340v110.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x0B, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x1D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x1E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x1F, PCI_CARD_NORMAL,      2, 3, 4, 1);
    device_add_inst(&w83977f_device, 1);
    device_add_inst(&w83977f_device, 2);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&stpc_elite_device);
    device_add(&sst_flash_29ee020_device);

    return ret;
}

int
machine_at_pcm5330_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pcm5330/5330_13b.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x0B, PCI_CARD_NORTHBRIDGE,     0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_SOUTHBRIDGE,     0, 0, 0, 0);
    pci_register_slot(0x0D, PCI_CARD_SOUTHBRIDGE_IDE, 0, 0, 0, 0);
    pci_register_slot(0x0E, PCI_CARD_SOUTHBRIDGE_USB, 1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,          1, 2, 3, 4);
    device_add(&stpc_serial_device);
    device_add(&w83977f_370_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&stpc_atlas_device);
    device_add(&sst_flash_29ee020_device);

    return ret;
}

int
machine_at_ecs486_init(const machine_t *model)

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
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_IDE,         0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&umc_hb4_device);
    device_add(&umc_8886f_device);
    device_add(&ide_cmd640_pci_legacy_only_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_ps2_ami_device);

    return ret;
}

static const device_config_t ecs486_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ecs486_sep94ph",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "PhoenixBIOS 4.03 Version 1.4 (09/21/1994)", .internal_name = "ecs486_sep94ph", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_sep94ph.bin", "" } },
            { .name = "AwardBIOS 4.50G Version 1.0 (09/27/1994))", .internal_name = "ecs486_sep94aw", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_sep94aw.bin", "" } },
            { .name = "AwardBIOS 4.50G Version VBS1.06h (01/13/1995, Highscreen OEM)", .internal_name = "ecs486_jan95aw", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_jan95aw.bin", "" } },
			{ .name = "PhoenixBIOS 4.04 Version 2.1 (01/27/1995)", .internal_name = "ecs486_jan95ph", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_jan95ph.bin", "" } },
            { .name = "AwardBIOS 4.50PG Version VBS1.10H (06/23/1995, Highscreen OEM)", .internal_name = "ecs486_jun95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_jun95.bin", "" } },
            { .name = "PhoenixBIOS 4.04 Version 3.1 (08/15/1995)", .internal_name = "ecs486_aug95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_aug95.bin", "" } },
			{ .name = "AwardBIOS 4.50PG Version VBS1.22PH (10/30/1995, Highscreen OEM)", .internal_name = "ecs486_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_oct95.bin", "" } },
            { .name = "AwardBIOS 4.50PG Version VBS1.23PH (01/08/1996, Highscreen OEM)", .internal_name = "ecs486_jan96aw", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_jan96aw.bin", "" } },
            { .name = "PhoenixBIOS 4.04 Version 3.2f (01/08/1996)", .internal_name = "ecs486_jan96ph", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_jan96ph.bin", "" } },
			{ .name = "PhoenixBIOS 4.04 Version 3.2H (06/27/1996)", .internal_name = "ecs486_jun96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/ecs486_jun96.bin", "" } },
            { .name = "PhoenixBIOS 4.04 Version 3.2J (12/11/1996)", .internal_name = "ecs486_lastest", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ecs486/8810AIO.32J", "" } }, 
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
	// clang-format on
};

const device_t ecs486_device = {
    .name          = "ECS Elite UM8810P-AIO",
    .internal_name = "ecs486",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ecs486_config[0]
};

static const device_config_t hot433a_config[] = {
    // clang-format off
    {
        .name = "bios",
        .description = "BIOS Version",
        .type = CONFIG_BIOS,
        .default_string = "hot433a",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 },
        .bios = {
            { .name = "AMI", .internal_name = "hot433a", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/hot433/433AUS33.ROM", "" } },
            { .name = "Award (eSupport update)", .internal_name = "hot433a_award", .bios_type = BIOS_NORMAL, 
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/hot433/2A4X5H21.BIN", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};

const device_t hot433a_device = {
    .name          = "Shuttle HOT-433A",
    .internal_name = "hot433a_device",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available     = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = hot433a_config
};

int
machine_at_hot433a_init(const machine_t *model)
{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    int is_award = !strcmp(device_get_config_bios("bios"), "hot433a_award");
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    if (is_award)
        device_add(&amstrad_megapc_nvr_device);
    else
        device_add(&ami_1994_nvr_device);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);

    device_add(&umc_hb4_device);
    device_add(&umc_8886bf_device);
    if (is_award)
        device_add(&um8663af_device);
    else
        device_add(&um8669f_device);
    device_add(&winbond_flash_w29c010_device);
    if (is_award)
        device_add(&keyboard_ps2_ami_device);
    else
        device_add(&keyboard_at_ami_device);

    pic_toggle_latch(is_award);

    return ret;
}

int
machine_at_d882_init(const machine_t *model)

{
    int ret = 0;
    const char* fn;

    /* No ROMs available */
    if (!device_available(model->device))
        return ret;

    device_context(model->device);
    fn = device_get_bios_file(machine_get_device(machine), device_get_config_bios("bios_versions"), 0);
    ret = bios_load_linear(fn, 0x000e0000, 262144, 0);
    device_context_restore();
	
	machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 	  0, 0, 0, 0); /* Onboard */
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 	  1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x13, PCI_CARD_VIDEO,           4, 0, 0, 0); /* Onboard */
    pci_register_slot(0x1C, PCI_CARD_NORMAL,      	  1, 3, 2, 4); /* Slot 1 */
    pci_register_slot(0x1E, PCI_CARD_NORMAL,      	  2, 1, 3, 4); /* Slot 2 */
	device_add(&umc_hb4_device);
    device_add(&umc_8886f_device);
    device_add(&ide_cmd640_pci_legacy_only_device);
	device_add(&pc87332_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_ps2_pci_device);

    return ret;
}


static const device_config_t d882_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d882_jul95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.04 Revision 1.05.882.12 (07/14/1995)", .internal_name = "d882_jul95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d882/d882_jul95.bin", "" } },
            { .name = "Version 4.04 Revision 1.06.882 (01/26/1996)", .internal_name = "d882_jan96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/d882/d882_jan96.bin", "" } },
            { .name = "Version 4.04 Revision 1.09.882.01 (06/02/1998)", .internal_name = "d882_jun98", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d882/d882_jun98.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
	// clang-format on
};

const device_t d882_device = {
    .name          = "Siemens-Nixdorf Scenic 4H/PCI",
    .internal_name = "d882",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d882_config[0]
};

int
machine_at_presario7100486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/presario7100486/SST29EE010.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,		1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0D, PCI_CARD_NORMAL,		4, 1, 2, 3); /* Slot 02 */
	pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE,	0, 0, 0, 0);
	pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE,	1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x13, PCI_CARD_VIDEO,			0, 0, 0, 0); /* Onboard */

    device_add(&umc_hb4_device);
    device_add(&umc_8886af_device);
    device_add(&um8663af_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_ps2_ami_pci_device);
   
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5430_onboard_pci_device);
	
	if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ess_1688_device);

    return ret;
}

int
machine_at_mb8425uuda_init(const machine_t *model)

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
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x1B, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x1C, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x1D, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */

    device_add(&umc_hb4_device);
    device_add(&umc_8886bf_device);  
    device_add(&um8663bf_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_at_ami_device);

    return ret;
}

static const device_config_t mb8425uuda_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "mb8425uuda_nov95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50PG Revision UUD951108A (11/07/1995)", .internal_name = "mb8425uuda_nov95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb8425uuda/mb8425uuda_nov95.bin", "" } },
            { .name = "Version 4.51PG Revision UUD951222S (12/05/1995)", .internal_name = "mb8425uuda_dec95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb8425uuda/mb8425uuda_dec95.bin", "" } },
			{ .name = "Version 4.50PG Revision UUD960111S (01/11/1996)", .internal_name = "mb8425uuda_jan96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb8425uuda/mb8425uuda_jan96.bin", "" } },
			{ .name = "Version 4.50PG Revision UUD960326S (03/26/1996)", .internal_name = "mb8425uuda_mar96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb8425uuda/mb8425uuda_mar96.bin", "" } },
            { .name = "Version 4.51PG Revision UUD960520S (05/20/1996)", .internal_name = "mb8425uuda_may96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mb8425uuda/mb8425uuda_may96.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};


const device_t mb8425uuda_device = {
    .name          = "Biostar MB-8425UUD-A",
    .internal_name = "mb8425uuda",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &mb8425uuda_config[0]
};

int
machine_at_atc1415_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/atc1415/1415V330.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&umc_hb4_device);
    device_add(&umc_8886bf_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_actionpc2600_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/actionpc2600/action2600.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 3);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_VIDEO,       0, 0, 0, 0);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);

    device_add(&umc_hb4_device);
    device_add(&umc_8886bf_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_ps2_tg_ami_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_actiontower8400_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/actiontower8400/V31C.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x15, PCI_CARD_VIDEO,       0, 0, 0, 0);
    pci_register_slot(0x16, PCI_CARD_IDE,         0, 0, 0, 0);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      2, 3, 4, 1);

    device_add(&umc_hb4_device);
    device_add(&umc_8886f_device);
    device_add(&fdc37c665_device);
    device_add(&ide_cmd640_pci_device);
    device_add(&intel_flash_bxt_device); // The ActionPC 2600 has this so I'm gonna assume this does too.
    device_add(&keyboard_ps2_ami_pci_device);
    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_ga486am_init(const machine_t *model)

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
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x16, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 04 */

    device_add(&umc_hb4_device);
    device_add(&umc_8886af_device);  
    device_add(&um8663af_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_at_ami_device);  

    return ret;
}


static const device_config_t ga486am_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ga486am_feb95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.50G Revision A (02/20/1995)", .internal_name = "ga486am_feb95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga486am/ga486am_feb95.bin", "" } },
            { .name = "Version 4.50PG Revision B (06/22/1995)", .internal_name = "ga486am_jun95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga486am/ga486am_jun95.bin", "" } },
			{ .name = "Core Version 4.50PG Version Oct 3 Revision A (10/03/1995)", .internal_name = "ga486am_oct95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga486am/ga486am_oct95.bin", "" } },
			{ .name = "Core Version 4.50PG Version Nov 1 Revision A (11/01/1995)", .internal_name = "ga486am_nov95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga486am/ga486am_nov95.bin", "" } },
            { .name = "Core Version 4.50PG Version Nov 7 Revision A (11/07/1995)", .internal_name = "ga486am_nov952", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/ga486am/ga486am_nov952.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};


const device_t ga486am_device = {
    .name          = "Gigabyte GA-486AM",
    .internal_name = "ga486am",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ga486am_config[0]
};

int
machine_at_m919_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m919/9190914s.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&umc_hb4_device);
    device_add(&umc_8886af_device);    /* AF is correct - the BIOS does IDE writes to ports 108h and 109h. */
    device_add(&um8663bf_device);
    device_add(&sst_flash_29ee010_device);
    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_mp4p4u885g_init(const machine_t *model)

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
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 	  0, 0, 0, 0); /* Onboard */
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 	  0, 0, 0, 0); /* Onboard */
	pci_register_slot(0x0F, PCI_CARD_VIDEO,           4, 0, 0, 0); /* Onboard */
    pci_register_slot(0x13, PCI_CARD_NORMAL,      	  2, 3, 4, 1); /* Slot 1 */
    pci_register_slot(0x14, PCI_CARD_NORMAL,      	  1, 2, 3, 4); /* Slot 2 */
	pci_register_slot(0x19, PCI_CARD_NORMAL,      	  4, 1, 2, 3); /* Slot 2 */
	device_add(&umc_hb4_device);
    device_add(&umc_8886f_device);
    device_add(&ide_isa_2ch_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_at_ami_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device); 

    return ret;
}

static const device_config_t mp4p4u885g_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "mp4p4u885g_dec94",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.04 Revision 2.1 (12/09/1994)", .internal_name = "mp4p4u885g_dec94", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mp4p4u885g/mp4p4u885g_dec94.bin", "" } },
            { .name = "Version 4.04 Revision 2.3 (01/04/1996)", .internal_name = "mp4p4u885g_jan96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/mp4p4u885g/mp4p4u885g_jan96.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
	// clang-format on
};

const device_t mp4p4u885g_device = {
    .name          = "QDI MP4-P4U885G ",
    .internal_name = "mp4p4u885g",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &mp4p4u885g_config[0]
};

int
machine_at_spc7700plw_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/spc7700plw/77LW13FH.P24",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      3, 4, 1, 2);

    device_add(&umc_hb4_device);
    device_add(&umc_8886af_device);
    device_add(&fdc37c665_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_ps2_ami_device);

    return ret;
}

int
machine_at_acerv20486aio_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acerv20486aio/ACERV20.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ali1429g_device);
	
	pci_init(FLAG_MECHANISM_1 | FLAG_MECHANISM_2 | PCI_ALWAYS_EXPOSE_DEV0);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);

    pci_register_slot(0x03, PCI_CARD_NORMAL,      3, 2, 1, 4);
    pci_register_slot(0x04, PCI_CARD_NORMAL,      4, 0, 0, 0);
    pci_register_slot(0x05, PCI_CARD_NORMAL,      0, 0, 0, 0);

    device_add(&ali1435_device);
    device_add(&sst_flash_29ee010_device);

    device_add(&keyboard_ps2_acer_pci_device);

    device_add_params(&pc87310_device, (void *) (PC87310_ALI));
	device_add(&ide_ali5213_device);

    return ret;
}

int
machine_at_ms4134_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ms4134/4alm001.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);

    device_add(&ali1429g_device);

    device_add(&fdc37c665_ide_pri_device);

    pci_init(FLAG_MECHANISM_1 | FLAG_MECHANISM_2 | PCI_ALWAYS_EXPOSE_DEV0);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);

    pci_register_slot(0x0B, PCI_CARD_SCSI, 4, 1, 2, 3);
    pci_register_slot(0x08, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0A, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x10, PCI_CARD_NORMAL, 1, 2, 3, 4);

    device_add(&ali1435_device);
    device_add(&sst_flash_29ee010_device);

    device_add(&keyboard_ps2_ami_device);

    return ret;
}

int
machine_at_tg486gp_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tg486gp/tg486gp.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);

    device_add(&ali1429g_device);

    device_add(&fdc37c665_ide_pri_device);

    pci_init(FLAG_MECHANISM_1 | FLAG_MECHANISM_2 | PCI_ALWAYS_EXPOSE_DEV0);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);

    pci_register_slot(0x0F, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0D, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x10, PCI_CARD_NORMAL, 1, 2, 3, 4);

    device_add(&ali1435_device);
    device_add(&sst_flash_29ee010_device);

    device_add(&keyboard_ps2_tg_ami_device);

    return ret;
}

int
machine_at_tg486g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tg486g/tg486g.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&amstrad_megapc_nvr_device);
    device_add(&sis_85c471_device);
    device_add(&ide_isa_device);
    device_add(&fdc37c651_ide_device);
    device_add(&keyboard_ps2_tg_ami_pci_device);

    if (gfxcard[0] != VID_INTERNAL) {
        for (uint16_t i = 0; i < 32768; i++)
            rom[i] = mem_readb_phys(0x000c0000 + i);
    }
    mem_mapping_set_addr(&bios_mapping, 0x0c0000, 0x40000);
    mem_mapping_set_exec(&bios_mapping, rom);

    return ret;
}

int
machine_at_dvent4xx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dvent4xx/Venturis466_BIOS.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sis_85c471_device);
    device_add(&ide_cmd640_vlb_pri_device);
    device_add(&fdc37c665_ide_device);
    device_add(&keyboard_ps2_phoenix_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    return ret;
}

int
machine_at_micronicsjx30wb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/micronicsjx30wb/JX30WB-2.BIN",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&sis_85c471_device);
    device_add(&ide_cmd640_vlb_device);
    device_add(&fdc37c665_ide_device);
    device_add(&keyboard_ps2_ami_device);
	device_add(&intel_flash_bxt_device);

    return ret;
}

int
machine_at_acera1_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acera1/a1-r26-649899719899b635096390.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ali1429_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    device_add(&keyboard_ps2_acer_pci_device);

    device_add_params(&pc87310_device, (void *) (PC87310_ALI));

    return ret;
}

int
machine_at_acera1gx2_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acera1gx2/Acer.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ali1429_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5430_onboard_vlb_device);

    device_add(&keyboard_ps2_acer_pci_device);

    device_add(&fdc37c665_device);
    device_add(&ide_ali5213_device);

    return ret;
}

int
machine_at_magiabl2463_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/magiabl2463/Magitronics_BL-2463-DragonBIOS.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1429_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_ecsal486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ecsal486/ECS_AL486.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1429g_device);
    device_add(&keyboard_ps2_ami_pci_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_ap4100aa_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ap4100aa/M27C512DIP28.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    device_add(&ami_1994_nvr_device);
    device_add(&ali1429g_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&ide_vlb_device);
    device_add(&um8663bf_device);

    return ret;
}

int
machine_at_atc1762_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/atc1762/atc1762.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ali1429g_device);
    device_add(&keyboard_ps2_ami_pci_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_unitron_u5917b_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/unitron_u5917b/Unitron_U5917B-V2_phoenix_bios_v4_03_01.BIN",
						    0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti381_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_tam2533_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/tam2533/TAM33O0EVEN.BIN",
								"roms/machines/tam2533/TAM33O0ODD.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti381_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_ch38625c_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ch38625c/REF1116.BIN",
							0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&opti381_device);
	device_add(&keyboard_at_ami_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_dataexpert386wb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dataexpert386wb/st0386-wb-ver2-0-618f078c738cb397184464.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti391_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_m321_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m321/pcchips-m321-rev2.7_2-6015e43dc9a1c900996853.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti391_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_isa486c_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/isa486c/asus-isa-486c-401a0-040591-657e2c17a0218417632602.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&isa486c_device);
    device_add(&port_92_key_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_genoa486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/genoa486/AMI486.BIO",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&compaq_genoa_device);
    device_add(&port_92_key_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_pkm3300_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/pkm3300/U72_27256_DTK-PKM-3300.bin",
								"roms/machines/pkm3300/U71_27256_DTK-PKM-3300.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
    device_add(&port_92_key_device);
	
    device_add(&keyboard_at_ami_device);
	
	mem_remap_top(384);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_d620_init(const machine_t *model)

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
	
	device_add(&compaq_genoa_device);
    device_add(&port_92_key_device);

    device_add(&keyboard_at_siemens_device);
	
	mem_remap_top(384);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static const device_config_t d620_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "d620_nov90",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 3.10 Revision 2.02 (11/29/1990)", .internal_name = "d620_nov90", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d620/d620_nov90.bin", "" } },
			{ .name = "Version 3.10 Revision 2.06 (05/28/1992)", .internal_name = "d620_may92", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d620/d620_may92.bin", "" } },    
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};


const device_t d620_device = {
    .name          = "Siemens D620",
    .internal_name = "d620",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d620_config[0]
};

int
machine_at_ga486l_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ga486l/ga-486l_bios.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&opti381_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_cobalt_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cobalt/Cobalt_2.3.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
	
	machine_at_common_init(model);

    device_add(&opti499_device);
	device_add(&ide_opti611_vlb_device);
	device_add(&ide_isa_sec_device);
	device_add(&fdc37c665_device);
	
    device_add(&keyboard_ps2_ami_device);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(machine_get_vid_device(machine));

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}


int
machine_at_cougar_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cougar/COUGRMRB.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ide_vlb_device);

    device_add(&opti499_device);
    device_add(&fdc37c665_ide_pri_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_486ocvplus_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/486ocvplus/OCV-128K.ROM",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti499_device);

    device_add(&keyboard_ps2_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

