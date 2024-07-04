/*
 * @Author       : wilbur
 * @Date         : 2024-03-29 13:17:12
 * @LastEditTime : 2024-03-29 20:19:55
 * @LastEditors  : wilbur
 * @Description  : Please enter file description
 * Copyright (c) 2024, All Rights Reserved.
 */
/*
 *  Copyright (c) 2023 Project CHIP Authors.
 *
 *  Use of this source code is governed by a BSD-style
 *  license that can be found in the LICENSE file or at
 *  https://opensource.org/license/bsd-3-clause
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef USERNODEIDPREFS_H_
#define USERNODEIDPREFS_H_

enum { kNodeIdLength = 8 };

class MessageNodeId {
 public:
  char *dataNodeId;
  guint dataNodeIdLen;
  uint64_t NodeId;
  MessageNodeId *nextId;
};

class UserNodeIdPrefs {
 public:
  static void Init(module_t *prefs);
  static guint GetNodeIdCount() { return sNodeIdCount; }
  static MessageNodeId *GetNodeId(size_t index) {
    return (index < sNodeIdCount) ? &sNodeIdList[index] : 0;
  }

 private:
  static uat_t *sUAT;
  static MessageNodeId *sNodeIdList;
  static guint sNodeIdCount;
};

#endif /* USERNODEIDPREFS_H_ */
