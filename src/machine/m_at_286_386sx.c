/*
 * 86Box    A hypervisor and IBM PC system emulator that specializes in
 *          running old operating systems and software designed for IBM
 *          PC systems and compatibles from 1981 through fairly recent
 *          system designs based on the PCI bus.
 *
 *          This file is part of the 86Box distribution.
 *
 *          Implementation of 286 and 386SX machines.
 *
 *
 *
 * Authors: Miran Grca, <mgrca8@gmail.com>
 *          EngiNerd <webmaster.crrc@yahoo.it>
 *
 *          Copyright 2016-2019 Miran Grca.
 *          Copyright 2020 EngiNerd.
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
#include <86box/rom.h>
#include <86box/fdd.h>
#include <86box/fdc.h>
#include <86box/fdc_ext.h>
#include <86box/hdc.h>
#include <86box/nvr.h>
#include <86box/port_6x.h>
#include <86box/sio.h>
#include <86box/serial.h>
#include <86box/video.h>
#include <86box/vid_cga.h>
#include <86box/flash.h>
#include <86box/machine.h>
#include <86box/sound.h>
#include <86box/snd_sn76489.h>

int
machine_at_mr286_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/mr286/V000B200-1",
                                "roms/machines/mr286/V000B200-2",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static void
machine_at_headland_common_init(int type)
{
    device_add(&keyboard_at_ami_device);

    if ((type != 2) && (fdc_current[0] == FDC_INTERNAL))
        device_add(&fdc_at_device);

    if (type == 2)
        device_add(&headland_ht18b_device);
    else if (type == 1)
        device_add(&headland_gc113_device);
    else
        device_add(&headland_gc10x_device);
}

int
machine_at_tg286m_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tg286m/ami.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);

    machine_at_headland_common_init(1);

    return ret;
}

int
machine_at_p3230_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p3230/p3230.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);

    machine_at_headland_common_init(1);

    return ret;
}

int
machine_at_p3238_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p3238/p3238.bin",
						   0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);
	
	mem_remap_top(384);
	
	device_add(&headland_gc113_device);
	 device_add(&keyboard_at_ami_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    

    return ret;
}

int
machine_at_ptm1630_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/ptm1630/PTM-1630 (L).bin",
								"roms/machines/ptm1630/PTM-1630 (H).bin",
								0x000f8000, 32768, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
	device_add(&ct_82c100_device);
	device_add(&keyboard_at_ami_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
    

    return ret;
}


int
machine_at_m216_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m216/2pcm001.bin",
						   0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	
	device_add(&headland_ht21c_d_device);
	device_add(&keyboard_at_ami_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
    

    return ret;
}

int
machine_at_ama932j_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ama932j/ami.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&oti067_ama932j_device);

    machine_at_headland_common_init(2);

    device_add(&ali5105_device);

    return ret;
}

int
machine_at_t1000rsx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/t1000rsx/tandy-1000rsx-1-10.00-5f70fba6020e2422950521.bin",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);

	device_add(&headland_ht21c_d_device);
	device_add(&keyboard_ps2_ami_device);
	device_add(&pssj_1e0_device);
	
	if (gfxcard[0] == VID_INTERNAL)
       device_add(&gd5402_onboard_device);
	
    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_mecerb64x_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mecerb64x/B640&B642.bin",
						    0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&keyboard_at_ami_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
	
	device_add(&headland_ht18a_device);
    return ret;
}

int
machine_at_npm162025_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/npm162025/NPM25-H0_LO.bin",
								"roms/machines/npm162025/NPM25-H0_HI.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&keyboard_at_ami_device);
	
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
	
	device_add(&headland_ht18b_device);
    return ret;
}



int
machine_at_quadt286_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/quadt286/QUADT89L.ROM",
                                "roms/machines/quadt286/QUADT89H.ROM",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&headland_gc10x_device);

    return ret;
}

int
machine_at_quadt386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/quadt386sx/QTC-SXM-EVEN-U3-05-07.BIN",
                                "roms/machines/quadt386sx/QTC-SXM-ODD-U3-05-07.BIN",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&headland_gc10x_device);

    return ret;
}

int
machine_at_cpu3216sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/cpu3216sx/3216SX-BIOS-LO.BIN",
                                "roms/machines/cpu3216sx/3216SX-BIOS-HI.BIN",
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
machine_at_p3349_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p3349/M27C1001_Magnavox2_unknown.BIN",
                                0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);
	
	mem_remap_top(384);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&headland_gc10x_device);

    return ret;
}

int
machine_at_neat_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dtk386/3cto001.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_init(model);

    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_friend386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/friend386sx/FTK_FRIEND_386SX_ENSX-1220-121589-K8_combined.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;


    machine_at_common_init(model);
	device_add(&keyboard_at_ami_device);
    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
	
	

    return ret;
}

int
machine_at_c386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/c386sx/386SX_L.bin",
								"roms/machines/c386sx/386SX_H.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;


    machine_at_common_init(model);
	device_add(&keyboard_at_ami_device);
    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
	
	

    return ret;
}

int
machine_at_dataexpert386sxneat_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/dataexpert386sxneat/dataexpert-386sx-EVEN.bin",
								"roms/machines/dataexpert386sxneat/dataexpert-386sx-ODD.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;


    machine_at_common_init(model);
	device_add(&keyboard_at_ami_device);
    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
	
	

    return ret;
}

int
machine_at_npm16_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/npm16/386-VIP-M345000even.BIN",
								"roms/machines/npm16/386-VIP-M345000odd.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;


    machine_at_common_init(model);
	device_add(&keyboard_at_ami_device);
    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
	
	

    return ret;
}

int
machine_at_neat_ami_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ami286/AMIC206.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_elt286b_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/elt286b/286-ELT-286B-160B L.BIN",
								"roms/machines/elt286b/286-ELT-286B-160B H.BIN",
								0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}


int
machine_at_elt286b_160eb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/elt286b_160eb/286-2061618 L.BIN",
								"roms/machines/elt286b_160eb/286-2061618 H.BIN",
								0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_tc16_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tc16/phoenix-286-plus-310-tulip-tc16-665cae0d7bddc223032572.BIN",
                           0x000f8000, 32768, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&neat_device);
    
	if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_nsc_device);
	

    device_add(&keyboard_at_device);

    return ret;
}

int
machine_at_auva286ct_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/auva286ct/286-VIPBAM-6-1 M215100-LO.BIN",
								"roms/machines/auva286ct/286-VIPBAM-6-1 M215100-HI.BIN",
								0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

// TODO
// Onboard Paradise PVGA1A-JK VGA Graphics
// Data Technology Corporation DTC7187 RLL Controller (Optional)
int
machine_at_ataripc4_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/ataripc4/AMI_PC4X_1.7_EVEN.BIN",
                                "roms/machines/ataripc4/AMI_PC4X_1.7_ODD.BIN",
#if 0
    ret = bios_load_interleaved("roms/machines/ataripc4/ami_pc4x_1.7_even.bin",
                                "roms/machines/ataripc4/ami_pc4x_1.7_odd.bin",
#endif
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_px286_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/px286/KENITEC.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&neat_device);

    return ret;
}

int
machine_at_micronics386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/micronics386/386-Micronics-09-00021-EVEN.BIN",
                                "roms/machines/micronics386/386-Micronics-09-00021-ODD.BIN",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_init(model);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static void
machine_at_scat_init(const machine_t *model, int is_v4, int is_ami)
{
    machine_at_common_init(model);

    if (machines[machine].bus_flags & MACHINE_BUS_PS2) {
        if (is_ami)
            device_add(&keyboard_ps2_ami_device);
        else
            device_add(&keyboard_ps2_device);
    } else {
        if (is_ami)
            device_add(&keyboard_at_ami_device);
        else
            device_add(&keyboard_at_device);
    }

    if (is_v4)
        device_add(&scat_4_device);
    else
        device_add(&scat_device);
}

static void
machine_at_scatsx_init(const machine_t *model)
{
    machine_at_common_init(model);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&scat_sx_device);
}

int
machine_at_award286_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/award286/award.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 0, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&ide_isa_device);

    return ret;
}

int
machine_at_gdc212m_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/gdc212m/gdc212m_72h.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 0, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&ide_isa_device);

    return ret;
}

int
machine_at_gw286ct_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/gw286ct/2ctc001.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    device_add(&f82c710_device);

    machine_at_scat_init(model, 1, 0);

    device_add(&ide_isa_device);

    return ret;
}

int
machine_at_senor_scat286_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/senor286/AMI-DSC2-1115-061390-K8.rom",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 0, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_tekat1_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tekat1/B700_241.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 0, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_super286c_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/super286c/hyundai_award286.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&neat_device);

    return ret;
}

int
machine_at_super286tr_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/super286tr/hyundai_award286.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 0, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_spc4200p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/spc4200p/u8.01",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 0, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&ide_isa_device);

    return ret;
}

int
machine_at_spc4216p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/spc4216p/7101.U8",
                                "roms/machines/spc4216p/AC64.U10",
                                0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 1, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_spc4620p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/spc4620p/31005h.u8",
                                "roms/machines/spc4620p/31005h.u10",
                                0x000f0000, 131072, 0x8000);

    if (bios_only || !ret)
        return ret;

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&ati28800k_spc4620p_device);

    machine_at_scat_init(model, 1, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&ide_isa_device);

    return ret;
}

int
machine_at_kmxc02_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/kmxc02/3ctm005.bin",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scatsx_init(model);

    return ret;
}

int
machine_at_tekat2_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/tekat2/T701_AT2_V220.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scatsx_init(model);

    return ret;
}

int
machine_at_deskmaster286_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/deskmaster286/SAMSUNG-DESKMASTER-28612-ROM.BIN",
                           0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scat_init(model, 0, 1);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
        
    device_add(&ide_isa_device);

    return ret;
}

int
machine_at_shuttle386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/shuttle386sx/386-Shuttle386SX-Even.BIN",
                                "roms/machines/shuttle386sx/386-Shuttle386SX-Odd.BIN",
                                0x000f0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&intel_82335_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_adi386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/adi386sx/3iip001l.bin",
                                "roms/machines/adi386sx/3iip001h.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);
    device_add(&amstrad_megapc_nvr_device); /* NVR that is initialized to all 0x00's. */

    device_add(&intel_82335_device);
    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_wd76c10_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/megapc/41651-bios lo.u18",
                                "roms/machines/megapc/211253-bios hi.u19",
                                0x000f0000, 65536, 0x08000);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&paradise_wd90c11_megapc_device);

    device_add(&keyboard_ps2_quadtel_device);

    device_add(&wd76c10_device);

    return ret;
}

int
machine_at_gw2kslimline386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/gw2kslimline386sx/BIOS_QUADTEL_3.05.06.bin",
                            0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&paradise_wd90c11_megapc_device);

    device_add(&keyboard_ps2_quadtel_device);

    device_add(&wd76c10_device);

    return ret;
}

int
machine_at_cmdsl386sx16_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/cmdsl386sx16/cbm-sl386sx-bios-lo-v1.04-390914-04.bin",
                                "roms/machines/cmdsl386sx16/cbm-sl386sx-bios-hi-v1.04-390915-04.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&keyboard_ps2_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&neat_device);
    /* Two serial ports - on the real hardware SL386SX-16, they are on the single UMC UM82C452. */
    device_add_inst(&ns16450_device, 1);
    device_add_inst(&ns16450_device, 2);

    return ret;
}

int
machine_at_if386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/if386sx/OKI_IF386SX_odd.bin",
                                "roms/machines/if386sx/OKI_IF386SX_even.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_phoenix_device);

    device_add(&neat_sx_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&if386jega_device);
    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

static void
machine_at_scamp_common_init(const machine_t *model, int is_ps2)
{
    machine_at_common_ide_init(model);

    if (is_ps2)
        device_add(&keyboard_ps2_ami_device);
    else
        device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&vlsi_scamp_device);
}

int
machine_at_cmdsl386sx25_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cmdsl386sx25/f000.rom",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&gd5402_onboard_device);

    machine_at_common_ide_init(model);

    device_add(&ali5105_device);  /* The FDC is part of the ALi M5105. */
    device_add(&vl82c113_device); /* The keyboard controller is part of the VL82c113. */

    device_add(&vlsi_scamp_device);

    return ret;
}

int
machine_at_dataexpert386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dataexpert386sx/5e9f20e5ef967717086346.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_scamp_common_init(model, 0);

    return ret;
}

int
machine_at_spc6033p_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/spc6033p/phoenix.BIN",
                           0x000f0000, 65536, 0x10000);

    if (bios_only || !ret)
        return ret;

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&ati28800k_spc6033p_device);

    machine_at_scamp_common_init(model, 1);

    return ret;
}

int
machine_at_dell386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/dell386sx/dell386sx.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;
	
	machine_at_common_ide_init(model);
	
	device_add(&pc87310_device);
	device_add(&keyboard_ps2_ami_device);
	device_add(&vlsi_scamp_device);
    

    return ret;
}

int
machine_at_svc386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/svc386sx/svc-386sx-am27c512dip28-6468c04f09d89320349795.BIN",
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

static void p386sx_setup(void);

int
machine_at_p386sx_init(const machine_t *model)

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "amibios_jul91", 0);
    }

    ret = bios_load_linear(fn, 0x000f0000, 65536, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init(model);

    p386sx_setup();  

    return ret;
}


static void p386sx_setup(void)
{
    device_add(&opti283_device);
    device_add(&keyboard_at_ami_device);
	
	 if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
}

static const device_config_t p386sx_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "amibios_jul91",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "AMIBIOS (07/1991)", .internal_name = "amibios_jul91", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/p386sx/amibios_jul91.bin", "" } },
            { .name = "Mr. BIOS 1.0A (11/18/1990)", .internal_name = "mrbios_nov90", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/p386sx/mrbios_nov90.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};



const device_t p386sx_device = {
    .name          = "QDI P386SX/25 PW",
    .internal_name = "p386sx",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &p386sx_config[0]
};

int
machine_at_awardsx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/awardsx/Unknown 386SX OPTi291 - Award (original).BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_init(model);

    device_add(&opti291_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_acer100t_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/acer100t/acer386.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_ps2_ide_init(model);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
    
    device_add(&ali1409_device);
    if (gfxcard[0] == VID_INTERNAL)
        device_add(&oti077_acer100t_device);   
     
    device_add(&ali5105_device);
    
    return ret;
}


int
machine_at_arb1374_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/arb1374/1374s.rom",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1217_device);
    device_add(&w83787f_ide_en_device);
    device_add(&keyboard_ps2_ami_device);

    return ret;
}

int
machine_at_sbc350a_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/sbc350a/350a.rom",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1217_device);
    device_add(&ide_isa_device);
    device_add(&fdc37c665_ide_pri_device);
    device_add(&keyboard_ps2_ami_device);

    return ret;
}

int
machine_at_flytech386_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/flytech386/FLYTECH.BIO",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1217_device);
    device_add(&w83787f_ide_en_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&tvga8900d_device);

    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_325ax_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/325ax/M27C512.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1217_device);
    device_add(&fdc_at_device);
    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_mr1217_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/mr1217/mrbios.BIN",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&ali1217_device);
    device_add(&fdc_at_device);
    device_add(&keyboard_at_ami_device);

    return ret;
}

int
machine_at_pja511m_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pja511m/2006915102435734.rom",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add_inst(&fdc37c669_device, 1);
    device_add_inst(&fdc37c669_device, 2);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&ali6117d_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

int
machine_at_prox1332_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/prox1332/D30B3AC1.BIN",
                           0x000e0000, 131072, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&fdc37c669_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&ali6117d_device);
    device_add(&sst_flash_29ee010_device);

    return ret;
}

/*
 * Current bugs:
 * - ctrl-alt-del produces an 8042 error
 */
int
machine_at_pc8_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/pc8/ncr_35117_u127_vers.4-2.bin",
                                "roms/machines/pc8/ncr_35116_u113_vers.4-2.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
    device_add(&keyboard_at_ncr_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_3302_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/3302/f000-flex_drive_test.bin",
                           0x000f0000, 65536, 0);

    if (ret) {
        ret &= bios_load_aux_linear("roms/machines/3302/f800-setup_ncr3.5-013190.bin",
                                    0x000f8000, 32768, 0);
    }

    if (bios_only || !ret)
        return ret;

    machine_at_common_ide_init(model);
    device_add(&neat_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    if (gfxcard[0] == VID_INTERNAL)
        device_add(&paradise_pvga1a_ncr3302_device);

    device_add(&keyboard_at_ncr_device);

    return ret;
}

/*
 * Current bugs:
 * - soft-reboot after saving CMOS settings/pressing ctrl-alt-del produces an 8042 error
 */
int
machine_at_pc916sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/pc916sx/ncr_386sx_u46-17_7.3.bin",
                                "roms/machines/pc916sx/ncr_386sx_u12-19_7.3.bin",
                                0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);

    device_add(&keyboard_at_ncr_device);
    mem_remap_top(384);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_ev1856_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/ev1856/AM27C256-EVEN.bin",
								"roms/machines/ev1856/AM27C256-ODD.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&ct_82c100_device);

    device_add(&keyboard_at_ami_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    return ret;
}

int
machine_at_pwrmatesx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/pwrmatesx/pwrmatesx.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&keyboard_at_device);
	device_add(&ide_isa_device);
	
	if (fdc_current[0] == FDC_INTERNAL) 
        device_add(&fdc_at_device); 
	

    return ret;
}

int
machine_at_d508_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/d508/d508.bin",
								0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init(model);
	device_add(&keyboard_at_siemens_device);

	
	if (fdc_current[0] == FDC_INTERNAL) 
        device_add(&fdc_at_device); 
	
	device_add(&intel_82335_device);

    return ret;
}

int
machine_at_d548_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/d548/even_low.rom",
								"roms/machines/d548/odd_high.rom",
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

static void tandon386sx_setup(void);

int
machine_at_tandon386sx_init(const machine_t *model)

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "tandon_v316", 0);
    }

    ret = bios_load_linear(fn, 0x000f0000, 65536, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init(model);

    tandon386sx_setup();  

    return ret;
}


static void tandon386sx_setup(void)
{
    device_add(&keyboard_at_device);
	
	mem_remap_top(384);
	
	 if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);
}

static const device_config_t tandon386sx_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "tandon_v316",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Tandon BIOS v3.16", .internal_name = "tandon_v316", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/tandon386sx/tandon_v316.bin", "" } },
            { .name = "Tandon BIOS v3.64", .internal_name = "mrbios_nov90", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 65536, .files = { "roms/machines/tandon386sx/tandon_v364.bin", "" } },
            { .files_no = 0 }
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
};



const device_t tandon386sx_device = {
    .name          = "TANDON CORPORATION 386SX PAC/TARGA 16MHZ",
    .internal_name = "tandon386sx",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &tandon386sx_config[0]
};

int
machine_at_octek386sx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_interleaved("roms/machines/octek386sx/Low.bin",
								"roms/machines/octek386sx/High.bin",
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

#ifdef USE_OLIVETTI
int
machine_at_m290_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m290/m290_pep3_1.25.bin",
                           0x000f0000, 65536, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 4);
    device_add(&keyboard_at_olivetti_device);
    device_add(&port_6x_olivetti_device);

    if (fdc_current[0] == FDC_INTERNAL)
        device_add(&fdc_at_device);

    device_add(&olivetti_eva_device);

    return ret;
}
#endif /* USE_OLIVETTI */
