/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */
//自动更新、不停
#ifndef __AUTOPATCHER_PATCH_CONTEXT_H
#define __AUTOPATCHER_PATCH_CONTEXT_H

enum PatchContext
{
	PC_HASH_1_WITH_PATCH, //hash值
	PC_HASH_2_WITH_PATCH,
	PC_WRITE_FILE, //写入文件
	PC_ERROR_FILE_WRITE_FAILURE, //写入文件失败
	PC_ERROR_PATCH_TARGET_MISSING, //目标文件丢失
	PC_ERROR_PATCH_APPLICATION_FAILURE, //失败
	PC_ERROR_PATCH_RESULT_CHECKSUM_FAILURE, //检测失败
	PC_NOTICE_WILL_COPY_ON_RESTART, //通知重新拷贝
	PC_NOTICE_FILE_DOWNLOADED, //通知文件下载
	PC_NOTICE_FILE_DOWNLOADED_PATCH, //通知文件下载更新
};

#endif
