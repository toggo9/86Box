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
#include <86box/io.h>
#include <86box/windows.h>
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
#include <86box/sound.h>
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
	
	device_add(&intel_82335_device);

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
    int ret;

    ret = bios_load_linear("roms/machines/netplex486/netplex486.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&sis_85c460_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5428_onboard_device);

    /* TODO: Phoenix MultiKey KBC */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&ide_isa_2ch_device);
    device_add(&fdc37c651_device);

    return ret;
}

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
machine_at_gs386ct_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/gs386ct/386-Goldstar-E.bin",
                                "roms/machines/gs386ct/386-Goldstar-O.bin",
								0x000e0000, 131072, 0);

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
        device_add(&ht216_32_pb410a_device);

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

    machine_at_common_ide_init(model);

    device_add(&vl82c480_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5428_onboard_device);

    device_add(&keyboard_ps2_ami_device);
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

    machine_at_common_init(model);

    device_add(&vl82c480_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5428_onboard_device);

    device_add(&keyboard_ps2_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c651_device);
    
    return ret;
}

int
machine_at_acera1g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acera1g/4alo001.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&ali1429g_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5428_onboard_device);

    device_add(&keyboard_ps2_acer_pci_device);

    device_add(&ali5105_device);
    device_add(&ide_ali5213_device);

    return ret;
}

int
machine_at_acerv10_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acerv10/ALL.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&sis_85c461_device);
    device_add(&keyboard_ps2_acer_pci_device);
    device_add(&ide_isa_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_gw2k486dxlb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/gw2k486dxlb/LB01.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&sis_85c461_device);
    device_add(&keyboard_at_ami_device);
    device_add(&ide_isa_2ch_device);

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

    device_add(&sis_85c461_device);
    device_add(&keyboard_ps2_device);
	device_add(&pc87311_ide_device);

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
        device_add(&s3_86c805_onboard_vlb_device);

    /* TODO: Phoenix MultiKey KBC */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&ide_isa_2ch_device);
    device_add(&fdc37c663_ide_device);


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

    device_add(&opti495_device);

    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static void
machine_at_opti495_ami_common_init(const machine_t *model)
{
    machine_at_common_init(model);

    device_add(&opti495_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
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


static void d819_setup(void);




int
machine_at_d819_init(const machine_t *model)

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "v334819", 0);
    }

    ret = bios_load_linear(fn, 0x000e0000, 131072, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init(model);

    d819_setup();  

    return ret;
}

// Definition of the setup function for D819
static void d819_setup(void)
{
    device_add(&vl82c480_device); 

    if (gfxcard[0] == VID_INTERNAL) {
        device_add(&s3_phoenix_86c805_vlb_device);
    }
    
    device_add(&keyboard_ps2_device); 
    device_add(&ide_isa_device);       
    device_add(&fdc37c651_device);    
}

static const device_config_t d819_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Revisions",
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
    { .available = NULL },
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

    machine_at_common_init(model);

    device_add(&vl82c480_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&s3_phoenix_86c805_vlb_device);

    device_add(&keyboard_ps2_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c651_device);
    
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

    machine_at_common_init(model);

    device_add(&vl82c486_device);

    device_add(&keyboard_ps2_device);

    device_add(&ide_isa_device);
    device_add(&fdc37c651_device);
    
    return ret;
}


int
machine_at_pb450_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pb450/OPTI802.bin",
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

int
machine_at_mvi486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mvi486/MVI627.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&opti895_device);
    device_add(&keyboard_at_device);
    device_add(&pc87311_ide_device);

    return ret;
}



int
machine_at_d756_init(const machine_t *model) /* Floppy issues */
{
    int ret;

    ret = bios_load_linear("roms/machines/d756/d756.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
	if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5426_onboard_device);

    device_add(&opti895_device);
	device_add(&ide_isa_device);
    device_add(&keyboard_ps2_device);
    device_add(&fdc37c651_device);

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
machine_at_vli486sv2g_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/vli486sv2g/0402.001",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_sis_85c471_common_init(model);
    device_add(&keyboard_at_ami_device);

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
	device_add(&sis_85c471_device);
    device_add(&fdc37c665_device);
	device_add(&keyboard_ps2_ami_pci_device);

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
    device_add(&keyboard_ps2_ami_device);
    device_add(&sio_device);
    device_add(&intel_flash_bxt_ami_device);

    device_add(&i420tx_device);
    device_add(&ncr53c810_onboard_pci_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

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
machine_at_xps486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/xps486/xps486.rom",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_ami_device);
	
	device_add(&sis_85c461_device);
	device_add(&fdc37c665_device);
    device_add(&sst_flash_29ee010_device);
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

    machine_at_common_init(model);
    device_add(&ide_pci_device);

    pci_init(PCI_CONFIG_TYPE_2 | PCI_NO_IRQ_STEERING);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_IDE,         0, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      3, 2, 1, 4);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      2, 1, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 3, 2, 4);
    pci_register_slot(0x02, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    device_add(&keyboard_ps2_pci_device);
    device_add(&sio_device);
    device_add(&fdc37c663_device);
    device_add(&intel_flash_bxt_ami_device);

    device_add(&i420tx_device);

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

    machine_at_common_init(model);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x05, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      1, 2, 1, 2);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      2, 1, 2, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 1, 2, 1);
    device_add(&keyboard_ps2_intel_ami_pci_device);
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
    device_add(&keyboard_ps2_ami_pci_device); /* Uses the AMIKEY KBC */
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

    device_add(&ali1489_device);
    device_add(&fdc37c665_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&tgui9440_onboard_pci_device);

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
    int ret;

    ret = bios_load_linear("roms/machines/ecs486/8810AIO.32J",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

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


static void d882_setup(void);

int
machine_at_d882_init(const machine_t *model) /* Windows 95 hangs with APM, BIOS also hangs at APM if soft reset occurs */

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "d882", 0);
    }

    ret = bios_load_linear(fn, 0x000e0000, 262144, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init_ex(model, 2);
	device_add(&amstrad_megapc_nvr_device);

    d882_setup();  

    return ret;
}

static void d882_setup(void)
{
	pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x10, PCI_CARD_NORTHBRIDGE, 	  0, 0, 0, 0); /* Onboard */
    pci_register_slot(0x12, PCI_CARD_SOUTHBRIDGE, 	  1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x13, PCI_CARD_NORMAL,          4, 0, 0, 0); /* Onboard */
    pci_register_slot(0x1C, PCI_CARD_NORMAL,      	  1, 3, 2, 4); /* Slot 1 */
    pci_register_slot(0x1E, PCI_CARD_NORMAL,      	  2, 1, 3, 4); /* Slot 2 */
	device_add(&umc_hb4_device);
    device_add(&umc_8886f_device);
    device_add(&ide_cmd640_pci_legacy_only_device);
	device_add(&pc87332_device);
    device_add(&intel_flash_bxt_device);
    device_add(&keyboard_ps2_ami_device);
}

static const device_config_t d882_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Revisions",
        .type = CONFIG_BIOS,
        .default_string = "d882_jul95",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Version 4.04 Revision 1.05.882.12 (07/14/1995)", .internal_name = "d882_jul95", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 131072, .files = { "roms/machines/d882/d882_jul95.bin", "" } },
            { .name = "Version 4.04 Revision 1.06.882 (01/26/1996)", .internal_name = "d882_jan96", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/d882/d882_jan96.bin", "" } }, /* Broken, reports invalid system configuration data */
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
    { .available = NULL },
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &d882_config[0]
};

int
machine_at_hot433a_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/hot433/433AUS33.ROM",
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
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1);

    device_add(&umc_hb4_device);
    device_add(&umc_8886bf_device);
    device_add(&um8669f_device);
    device_add(&winbond_flash_w29c010_device);
    device_add(&keyboard_at_ami_device);

    return ret;
}

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
        device_add(&tgui9440_onboard_pci_device);

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
        device_add(&gd5430_onboard_pci_device);

    return ret;
}

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
	pci_register_slot(0x13, PCI_CARD_NORMAL,		0, 0, 0, 0); /* Onboard */

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

    machine_at_common_init(model);
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
machine_at_genoa486_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/genoa486/AMI486.BIO",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&compaq_genoa_device);
    device_add(&port_92_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_asus486c_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/asus486c/asus-isa-486c-401a0-040591-657e2c17a0218417632602.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

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

