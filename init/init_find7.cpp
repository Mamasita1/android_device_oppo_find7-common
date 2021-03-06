/*
   Copyright (c) 2014, The CyanogenMod Project

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <fcntl.h>
#include <linux/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

static void import_kernel_nv(char *name, int for_emulator)
{
    char *value = strchr(name, '=');
    int name_len = strlen(name);

    if (value == 0) return;
    *value++ = 0;
    if (name_len == 0) return;

    if (!strcmp(name,"oppo.rf_version")) {
        property_set("ro.oppo.rf_version", value);
    } else if (!strcmp(name,"oppo.pcb_version")) {
        property_set("ro.oppo.pcb_version", value);
        if (!strcmp(value, "20") ||
                !strcmp(value, "21") ||
                !strcmp(value, "22") ||
                !strcmp(value, "23")) {
            property_set("ro.sf.lcd_density", "640");
            property_set("ro.oppo.device", "find7s");
        } else {
            property_set("ro.sf.lcd_density", "480");
            property_set("ro.oppo.device", "find7a");
        }
    }
}

static bool has_unified_layout()
{
  return ( access("/dev/block/platform/msm_sdcc.1/by-name/sdcard", F_OK ) == -1 );
}

static bool has_lvm()
{
  return ( access("/dev/lvpool/userdata", F_OK ) == 0 );
}

static void set_oppo_layout()
{
    if (has_lvm()||has_unified_layout()) {
        property_set("ro.crypto.fuse_sdcard", "true");
    } else {
        property_set("ro.vold.primary_physical", "1");
    }
}

void init_msm_properties(unsigned long msm_id, unsigned long msm_ver, char *board_type)
{
    import_kernel_cmdline(0, import_kernel_nv);
    set_oppo_layout();
}
