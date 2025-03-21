/*
 * 86Box    A hypervisor and IBM PC system emulator that specializes in
 *          running old operating systems and software designed for IBM
 *          PC systems and compatibles from 1981 through fairly recent
 *          system designs based on the PCI bus.
 *
 *          This file is part of the 86Box distribution.
 *
 *          Implementation of Socket 370(PGA370) machines.
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
#include <86box/clock.h>
#include <86box/sound.h>
#include <86box/snd_ac97.h>


int
machine_at_ergoproed_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ergoproed/ergoproed.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Onboard */
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 03 */
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
    device_add(&i440lx_device);
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
machine_at_s370slm_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/s370slm/3LM1202.rom",
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
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440lx_device);
    device_add(&piix4e_device);
    device_add(&w83977tf_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
    device_add(&w83781d_device);    /* fans: CPU, Fan 2, Chassis; temperatures: unused, CPU, unused */
    hwm_values.temperatures[0] = 0; /* unused */
    hwm_values.temperatures[2] = 0; /* unused */

    return ret;
}

int
machine_at_prosignias31x_bx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/prosignias31x_bx/p6bxt-ap-092600.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0a, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0b, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x0c, PCI_CARD_NORMAL, 4, 1, 2, 3);
    pci_register_slot(0x0d, PCI_CARD_SOUND,  4, 3, 2, 1); /* assumed */
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE, 1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&winbond_flash_w29c020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
    device_add(&gl520sm_2d_device);  /* fans: CPU, Chassis; temperature: System */
    hwm_values.temperatures[0] += 2; /* System offset */
    hwm_values.temperatures[1] += 2; /* CPU offset */
    hwm_values.voltages[0] = 3300;   /* Vcore and 3.3V are swapped */
    hwm_values.voltages[2] = hwm_get_vcore();

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&cmi8738_onboard_device);

    return ret;
}

int
machine_at_s1857_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/s1857/BX57200A.ROM",
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
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&w83977ef_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    if (sound_card_current[0] == SOUND_INTERNAL) {
        device_add(machine_get_snd_device(machine));
        device_add(&cs4297_device); /* no good pictures, but the marking looks like CS4297 from a distance */
    }

    return ret;
}

int
machine_at_p6bap_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p6bap/bapa14a.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0a, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0b, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x0c, PCI_CARD_NORMAL, 4, 1, 2, 3);
    pci_register_slot(0x0d, PCI_CARD_NORMAL, 4, 3, 2, 1);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE, 1, 2, 3, 4);
    device_add(&via_apro133a_device);  /* Rebranded as ET82C693A */
    device_add(&via_vt82c596b_device); /* Rebranded as ET82C696B */
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&cmi8738_onboard_device);

    return ret;
}

int
machine_at_p6bat_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/p6bat/bata+56.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL, 1, 2, 3, 4);
    pci_register_slot(0x0a, PCI_CARD_NORMAL, 2, 3, 4, 1);
    pci_register_slot(0x0b, PCI_CARD_NORMAL, 3, 4, 1, 2);
    pci_register_slot(0x0c, PCI_CARD_NORMAL, 4, 1, 2, 3);
    pci_register_slot(0x0d, PCI_CARD_NORMAL, 4, 3, 2, 1);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE, 1, 2, 3, 4);
    device_add(&via_apro133_device);
    device_add(&via_vt82c596b_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&cmi8738_onboard_device);

    return ret;
}

int
machine_at_cubx_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cubx/1008cu.004",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x07, PCI_CARD_IDE,         2, 3, 4, 1);
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
    device_add(&as99127f_device); /* fans: Chassis, CPU, Power; temperatures: MB, JTPWR, CPU */

    return ret;
}

int
machine_at_atc7020bxii_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/atc7020bxii/7020s102.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0E, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&slc90e66_device);
    device_add(&keyboard_ps2_pci_device);
    device_add(&w83977ef_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 256);

    return ret;
}

int
machine_at_m773_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m773/010504s.rom",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x0C, PCI_CARD_SOUND,       4, 3, 0, 0);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&slc90e66_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&it8671f_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 256);
    device_add(&gl520sm_2d_device);  /* fans: CPU, Chassis; temperature: System */
    hwm_values.temperatures[0] += 2; /* System offset */
    hwm_values.temperatures[1] += 2; /* CPU offset */
    hwm_values.voltages[0] = 3300;   /* Vcore and 3.3V are swapped */
    hwm_values.voltages[2] = hwm_get_vcore();

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&cmi8738_onboard_device);

    return ret;
}

int
machine_at_ambx133_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/ambx133/mkbx2vg2.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add(&w83977ef_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);
    device_add(&gl518sm_2d_device); /* fans: CPUFAN1, CPUFAN2; temperature: CPU */
    hwm_values.fans[1] += 500;
    hwm_values.temperatures[0] += 4;                         /* CPU offset */
    hwm_values.voltages[1] = RESISTOR_DIVIDER(12000, 10, 2); /* different 12V divider in BIOS (10K/2K?) */

    return ret;
}

int
machine_at_awo671r_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/awo671r/a08139c.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440bx_device);
    device_add(&piix4e_device);
    device_add_inst(&w83977ef_device, 1);
    device_add_inst(&w83977ef_device, 2);
    device_add(&keyboard_ps2_pci_device);
    device_add(&sst_flash_39sf020_device);
    if (gfxcard[0] == VID_INTERNAL) {
        device_add(&chips_69000_onboard_device);
    }
    spd_register(SPD_TYPE_SDRAM, 0x3, 256);

    return ret;
}

int
machine_at_63a1_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/63a1/63a-q3.bin",
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
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Integrated Sound? */
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&i440zx_device);
    device_add(&piix4e_device);
    device_add(&w83977tf_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&intel_flash_bxt_device);
    spd_register(SPD_TYPE_SDRAM, 0x3, 256);

    return ret;
}

int
machine_at_apas3_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/apas3/V0218SAG.BIN",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x13, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro_device);
    device_add(&via_vt82c586b_device);
    device_add(&fdc37c669_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 256);

    return ret;
}

int
machine_at_cuv4xls_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cuv4xls/1005LS.001",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 4, 1, 2, 3);
    pci_register_slot(0x05, PCI_CARD_SOUND,       3, 0, 0, 0);
    pci_register_slot(0x06, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x07, PCI_CARD_NORMAL,      2, 3, 0, 0);
    pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x14, PCI_CARD_NORMAL,      4, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686b_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 1024);
    device_add(&as99127f_device); /* fans: Chassis, CPU, Power; temperatures: MB, JTPWR, CPU */

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&cmi8738_onboard_device);

    return ret;
}

int
machine_at_cuv4xm_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/cuv4xm/1006fsc.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x05, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Onboard */
	pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686a_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0xF, 1024);
    device_add(&as99127f_device); /* fans: Chassis, CPU, Power; temperatures: MB, JTPWR, CPU */

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&cmi8738_onboard_device);

    return ret;
}

int
machine_at_6via90ap_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/6via90ap/90ap10.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 1, 2, 3, 4);
    pci_register_slot(0x09, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x0A, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x0B, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x0C, PCI_CARD_NORMAL,      4, 1, 2, 3);
    pci_register_slot(0x0D, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686b_device); /* fans: CPU1, CPU2; temperatures: CPU, System, unused */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    hwm_values.temperatures[0] += 2; /* CPU offset */
    hwm_values.temperatures[1] += 2; /* System offset */
    hwm_values.temperatures[2] = 0;  /* unused */

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&alc100_device); /* ALC100P identified on similar Acorp boards (694TA, 6VIA90A1) */

    return ret;
}

int
machine_at_7sbb_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/7sbb/sbb12aa2.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1 | FLAG_TRC_CONTROLS_CPURST);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_SOUTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      1, 2, 3, 4);
    pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1);
    pci_register_slot(0x11, PCI_CARD_NORMAL,      3, 4, 1, 2);
    pci_register_slot(0x02, PCI_CARD_AGPBRIDGE,   0, 0, 0, 0);
    device_add(&sis_5600_device);
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(&it8661f_device);
    device_add(&sst_flash_29ee020_device); /* assumed */

    return ret;
}

static void ms6323_setup(void);

int
machine_at_ms6323_init(const machine_t *model)

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "ms6323_apr00", 0);
    }

    ret = bios_load_linear(fn, 0x000c0000, 262144, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init_ex(model, 2);

    ms6323_setup();  

    return ret;
}

static void ms6323_setup(void)
{
    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 0, 0, 0, 0);
    pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4);
    pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 01 */
    pci_register_slot(0x10, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 02 */
    pci_register_slot(0x0B, PCI_CARD_SOUND,       3, 0, 0, 0);
    pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686b_device); /* fans: CPU1, CPU2; temperatures: CPU, System, unused */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    hwm_values.temperatures[0] += 2; /* CPU offset */
    hwm_values.temperatures[1] += 2; /* System offset */
    hwm_values.temperatures[2] = 0;  /* unused */

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ct5880_onboard_device);
   
}

static const device_config_t ms6323_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ms6323_apr00",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "AwardBIOS v6.00PG Version 1.0 (04/13/2000)", .internal_name = "ms6323_apr00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6323/ms6323_apr00.bin", "" } },
            { .name = "FSC OEM BIOS (06/30/2000)", .internal_name = "ms6323_jun00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6323/ms6323_jun00.bin", "" } },
			{ .name = "FSC OEM BIOS (01/12/2001)", .internal_name = "ms6323_jan01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6323/ms6323_jan01.bin", "" } },
            { .name = "Daewoo OEM BIOS (07/20/2001)", .internal_name = "ms6323_jul01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6323/ms6323_jul01.bin", "" } },
			{ .name = "PB OEM BIOS Version 1.2", .internal_name = "ms6323pb_v12", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6323/ms6323pb_v12.bin", "" } },
			{ .name = "PB OEM BIOS Version 1.4", .internal_name = "ms6323pb_v14", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6323/ms6323pb_v14.bin", "" } },
			{ .name = "PB OEM BIOS Version 1.5", .internal_name = "ms6323pb_v15", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6323/ms6323pb_v15.bin", "" } }, 			  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t ms6323_device = {
    .name          = "MSI MS-6323",
    .internal_name = "ms6323",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ms6323_config[0]
};

int
machine_at_m6vct_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m6vct/VCT0911F.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 05 */
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686b_device); /* fans: CPU1, CPU2; temperatures: CPU, System, unused */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
	spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    hwm_values.temperatures[0] += 2; /* CPU offset */
    hwm_values.temperatures[1] += 2; /* System offset */
    hwm_values.temperatures[2] = 0;  /* unused */
	
	 if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ad1881_device);
	
    

    return ret;
}

static void ms6318_setup(void);

int
machine_at_ms6318_init(const machine_t *model)

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "ms6318_feb01", 0);
    }

    ret = bios_load_linear(fn, 0x000c0000, 262144, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init_ex(model, 2);

    ms6318_setup();  

    return ret;
}

static void ms6318_setup(void)
{
    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4); /* Onboard */
	pci_register_slot(0x0C, PCI_CARD_SOUND,       3, 4, 1, 2); /* Slot 04 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 03 */
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686b_device); /* fans: CPU1, CPU2; temperatures: CPU, System, unused */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    hwm_values.temperatures[0] += 2; /* CPU offset */
    hwm_values.temperatures[1] += 2; /* System offset */
    hwm_values.temperatures[2] = 0;  /* unused */

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ct5880_onboard_device);
   
}

static const device_config_t ms6318_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "ms6318_feb01",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Retail BIOS (02/05/2001)", .internal_name = "ms6318_feb01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6318/ms6318_feb01.bin", "" } },
            { .name = "Retail BIOS (08/07/2001)", .internal_name = "ms6318_aug01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6318/ms6318_aug01.bin", "" } },
			{ .name = "FSC OEM BIOS (10/26/2001)", .internal_name = "fsc_oct01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6318/fsc_oct01.bin", "" } },
            { .name = "Medion OEM BIOS (03/18/2002)", .internal_name = "md6318_mar02", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6318/md6318_mar02.bin", "" } },
			{ .name = "Elonex OEM BIOS (03/25/2002)", .internal_name = "elonex_mar02", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/ms6318/elonex_mar02.bin", "" } },	  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t ms6318_device = {
    .name          = "MSI MS-6318",
    .internal_name = "ms6318",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &ms6318_config[0]
};

int
machine_at_m6vcg_init(const machine_t *model)
{
    int ret;

    ret = bios_load_linear("roms/machines/m6vcg/euro850.bin",
                           0x000c0000, 262144, 0);

    if (bios_only || !ret)
        return ret;

    machine_at_common_init_ex(model, 2);

    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4); /* Onboard */
	pci_register_slot(0x08, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x09, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 03 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 04 */
	pci_register_slot(0x0C, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 05 */
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686a_device); /* fans: CPU1, CPU2; temperatures: CPU, System, unused */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    hwm_values.temperatures[0] += 2; /* CPU offset */
    hwm_values.temperatures[1] += 2; /* System offset */
    hwm_values.temperatures[2] = 0;  /* unused */

    return ret;
}

static void md2000_setup(void);

int
machine_at_md2000_init(const machine_t *model)

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "md2000_jul00", 0);
    }

    ret = bios_load_linear(fn, 0x000c0000, 262144, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init_ex(model, 2);

    md2000_setup();  

    return ret;
}

static void md2000_setup(void)
{
    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4); /* Onboard */
	pci_register_slot(0x07, PCI_CARD_SOUTHBRIDGE, 0, 0, 3, 4); /* Onboard */
	pci_register_slot(0x0C, PCI_CARD_SOUND,       3, 4, 1, 2); /* Slot 04 */
	pci_register_slot(0x0E, PCI_CARD_NORMAL,      1, 2, 3, 4); /* Slot 01 */
	pci_register_slot(0x0F, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 02 */
	pci_register_slot(0x10, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 03 */
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686b_device); /* fans: CPU1, CPU2; temperatures: CPU, System, unused */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    hwm_values.temperatures[0] += 2; /* CPU offset */
    hwm_values.temperatures[1] += 2; /* System offset */
    hwm_values.temperatures[2] = 0;  /* unused */

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ct5880_onboard_device);
   
}

static const device_config_t md2000_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "md2000_jul00",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Medion OEM BIOS (07/19/2000)", .internal_name = "md2000_jul00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/md2000/md2000_jul00.bin", "" } },
            { .name = "Medion OEM BIOS (09/28/2000)", .internal_name = "md2000_sep00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/md2000/md2000_sep00.bin", "" } },
			{ .name = "HP OEM BIOS (09/28/2000)", .internal_name = "hpmed_sep00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/md2000/hpmed_sep00.bin", "" } },
            { .name = "Medion OEM BIOS (06/13/2001)", .internal_name = "md2000_jun01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/md2000/md2000_jun01.bin", "" } },
			{ .name = "HP OEM BIOS (06/13/2001)", .internal_name = "hpmed_jun01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/md2000/hpmed_jun01.bin", "" } },	  
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t md2000_device = {
    .name          = "Medion MD2000",
    .internal_name = "md2000",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &md2000_config[0]
};

static void cuv4xcm_setup(void);

int
machine_at_cuv4xcm_init(const machine_t *model)

{
    int ret;
    const char *fn;

    if (!device_available(model->device)) {
        return 0;
    }

    device_context(model->device);
    fn = device_get_bios_file(model->device, device_get_config_bios("bios_versions"), 0);

    if (!fn) {
        fn = device_get_bios_file(model->device, "md2001_sep00", 0);
    }

    ret = bios_load_linear(fn, 0x000c0000, 262144, 0);
    device_context_restore();

    if (bios_only || !ret) {
        return ret;
    }
	
	machine_at_common_init_ex(model, 2);

    cuv4xcm_setup();  

    return ret;
}

static void cuv4xcm_setup(void)
{
    pci_init(PCI_CONFIG_TYPE_1);
    pci_register_slot(0x00, PCI_CARD_NORTHBRIDGE, 1, 2, 3, 4);
	pci_register_slot(0x01, PCI_CARD_AGPBRIDGE,   1, 2, 3, 4);
	pci_register_slot(0x04, PCI_CARD_SOUTHBRIDGE, 4, 1, 2, 3); /* Onboard */
	pci_register_slot(0x05, PCI_CARD_SOUND,       4, 0, 0, 0); /* Onboard */
	pci_register_slot(0x09, PCI_CARD_NORMAL,      4, 1, 2, 3); /* Slot 03 */
	pci_register_slot(0x0A, PCI_CARD_NORMAL,      3, 4, 1, 2); /* Slot 02 */
	pci_register_slot(0x0B, PCI_CARD_NORMAL,      2, 3, 4, 1); /* Slot 01 */
    device_add(&via_apro133a_device);
    device_add(&via_vt82c686b_device); /* fans: CPU1, CPU2; temperatures: CPU, System, unused */
    device_add(&keyboard_ps2_ami_pci_device);
    device_add(ics9xxx_get(ICS9250_18));
    device_add(&sst_flash_39sf020_device);
    spd_register(SPD_TYPE_SDRAM, 0x7, 1024);
    hwm_values.temperatures[0] += 2; /* CPU offset */
    hwm_values.temperatures[1] += 2; /* System offset */
    hwm_values.temperatures[2] = 0;  /* unused */

    if (sound_card_current[0] == SOUND_INTERNAL)
        device_add(&ct5880_onboard_device);
   
}

static const device_config_t cuv4xcm_config[] = {
    // clang-format off
    {
        .name = "bios_versions",
        .description = "BIOS Versions",
        .type = CONFIG_BIOS,
        .default_string = "md2001_sep00",
        .default_int = 0,
        .file_filter = "",
        .spinner = { 0 }, /*W1*/
        .bios = {
            { .name = "Medion OEM BIOS (09/20/2000)", .internal_name = "md2001_sep00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/cuv4xcm/md2001_sep00.bin", "" } },
            { .name = "Retail BIOS (09/27/2000)", .internal_name = "cuv4xcm_sep00", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/cuv4xcm/cuv4xcm_sep00.bin", "" } },
			{ .name = "Medion OEM BIOS (06/12/2001, beta)", .internal_name = "md2001_jun01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/cuv4xcm/md2001_jun01.bin", "" } },
            { .name = "Medion OEM BIOS (12/05/2001)", .internal_name = "md2001_dec01", .bios_type = BIOS_NORMAL,
              .files_no = 1, .local = 0, .size = 262144, .files = { "roms/machines/cuv4xcm/md2001_dec01.bin", "" } },
            
        },
    },
    { .name = "", .description = "", .type = CONFIG_END }
    // clang-format on
};



const device_t cuv4xcm_device = {
    .name          = "ASUS CUV4X-CM (Medion)",
    .internal_name = "cuv4xcm",
    .flags         = 0,
    .local         = 0,
    .init          = NULL,
    .close         = NULL,
    .reset         = NULL,
    .available = NULL,
    .speed_changed = NULL,
    .force_redraw  = NULL,
    .config        = &cuv4xcm_config[0]
};

