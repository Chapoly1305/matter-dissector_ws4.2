/*
 *  Copyright (c) 2023 Project CHIP Authors.
 *
 *  Use of this source code is governed by a BSD-style
 *  license that can be found in the LICENSE file or at
 *  https://opensource.org/license/bsd-3-clause
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include <Matter/Core/MatterCore.h>
#include <Matter/Support/CodeUtils.h>
#include <epan/packet.h>
#include <epan/prefs.h>
#include <epan/proto_data.h>
#include <epan/uat.h>
#include <glib.h>
#include <stdint.h>

#include "UserNodeIdPrefs.h"
#include "config.h"
#include "packet-matter.h"

uat_t *UserNodeIdPrefs::sUAT = NULL;
MessageNodeId *UserNodeIdPrefs::sNodeIdList = NULL;
guint UserNodeIdPrefs::sNodeIdCount = 0;

UAT_BUFFER_CB_DEF(MessageNodeId, nodeId, MessageNodeId, dataNodeId, dataNodeIdLen)

static bool MessageNodeId_nodeId_check_cb(void *rec, const char *ptr,
                                          unsigned len,
                                          const void *chk_data,
                                          const void *fld_data,
                                          char **err) {
  
  if (len != kNodeIdLength) {
     *err = g_strdup("Invalid node id length");
     return FALSE;
   }	
  return TRUE;
}


static void *MessageNodeId_copy_cb(void *dest, const void *orig, size_t len) {
  MessageNodeId *d = (MessageNodeId *)dest;
  const MessageNodeId *o = (const MessageNodeId *)orig;
  if (o->dataNodeIdLen) {
    d->dataNodeId = (unsigned char *)g_memdup2(o->dataNodeId, o->dataNodeIdLen);
    d->dataNodeIdLen = o->dataNodeIdLen;
    d->NodeId = ((uint64_t)(uint8_t)o->dataNodeId[0] << (7 * 8)) |
                ((uint64_t)(uint8_t)o->dataNodeId[1] << (6 * 8)) |
                ((uint64_t)(uint8_t)o->dataNodeId[2] << (5 * 8)) |
                ((uint64_t)(uint8_t)o->dataNodeId[3] << (4 * 8)) |
                ((uint64_t)(uint8_t)o->dataNodeId[4] << (3 * 8)) |
                ((uint64_t)(uint8_t)o->dataNodeId[5] << (2 * 8)) |
                ((uint64_t)(uint8_t)o->dataNodeId[6] << (1 * 8)) |
                (uint64_t)(uint8_t)o->dataNodeId[7];
  }
  return dest;
}

static bool MessageNodeId_update_cb(void *rec, char **err) {
  *err = NULL;
  return TRUE; 
}

static void MessageNodeId_free_cb(void *rec) {
  MessageNodeId *r = (MessageNodeId *)rec;

  g_free(r->dataNodeId);
}

static void MessageNodeId_post_update_cb(void) {
  // No action required.
}

void UserNodeIdPrefs::Init(module_t *prefs) {
#ifndef UAT_FLD_BUFFER_OTHER
#define UAT_FLD_BUFFER_OTHER(basename, field_name, title, desc)                \
  {                                                                            \
    #field_name, title, PT_TXTMOD_HEXBYTES,                                    \
        {basename##_##field_name##_check_cb, basename##_##field_name##_set_cb, \
         basename##_##field_name##_tostr_cb},                                  \
        {0, 0, 0}, 0, desc, FLDFILL                                            \
  }
#endif

  static uat_field_t keyDataUATFields[] = {
      UAT_FLD_BUFFER_OTHER(MessageNodeId, nodeId, "Node Id",
                           "Matter message node id"),
      UAT_END_FIELDS};

    sUAT = uat_new("Message Node Ids", sizeof(MessageNodeId),
               "matter_message_node_ids", TRUE, &sNodeIdList, &sNodeIdCount,
               UAT_AFFECTS_DISSECTION, NULL, MessageNodeId_copy_cb,
               MessageNodeId_update_cb, MessageNodeId_free_cb,
               MessageNodeId_post_update_cb, NULL, keyDataUATFields);

  prefs_register_uat_preference(prefs, "message_node_ids", "Message Node Ids",
                                "A table of node ids for Matter messages",
                                sUAT);

  char *err;
  uat_load(sUAT, nullptr, &err);
}
