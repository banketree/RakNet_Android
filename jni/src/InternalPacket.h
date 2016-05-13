/*
 *  Copyright (c) 2014, Oculus VR, Inc.
 *  All rights reserved.
 *
 *  This source code is licensed under the BSD-style license found in the
 *  LICENSE file in the root directory of this source tree. An additional grant 
 *  of patent rights can be found in the PATENTS file in the same directory.
 *
 */

/// \file
/// \brief \b [Internal] A class which stores a user message, and all information associated with sending and receiving that message.
///

#ifndef __INTERNAL_PACKET_H
#define __INTERNAL_PACKET_H

#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "RakMemoryOverride.h"
#include "RakNetDefines.h"
#include "NativeTypes.h"
#include "RakNetDefines.h"
#if USE_SLIDING_WINDOW_CONGESTION_CONTROL!=1
#include "CCRakNetUDT.h"
#else
#include "CCRakNetSlidingWindow.h"
#endif

//内部包
namespace RakNet {

typedef uint16_t SplitPacketIdType; //id标识类型
typedef uint32_t SplitPacketIndexType; //序列类型

/// This is the counter used for holding packet numbers, so we can detect duplicate packets.  It should be large enough that if the variables
/// Internally assumed to be 4 bytes, but written as 3 bytes in ReliabilityLayer::WriteToBitStreamFromInternalPacket
typedef uint24_t MessageNumberType; //保持分组数的计数器，可以检测到重复的数据包。

/// This is the counter used for holding ordered packet numbers, so we can detect out-of-order packets.  It should be large enough that if the variables
/// were to wrap, the newly wrapped values would no longer be in use.  Warning: Too large of a value wastes bandwidth!
typedef MessageNumberType OrderingIndexType;//用来保持有序的包数的计数器，可以检测出数据包的顺序。

typedef RakNet::TimeUS RemoteSystemTimeType;

struct InternalPacketFixedSizeTransmissionHeader
{
	/// A unique numerical identifier given to this user message. Used to identify reliable messages on the network
	MessageNumberType reliableMessageNumber; //唯一标识 号码

	///The ID used as identification for ordering messages. Also included in sequenced messages
	OrderingIndexType orderingIndex; //排序消息识别

	// Used only with sequenced messages
	OrderingIndexType sequencingIndex;//用于排序消息

	///What ordering channel this packet is on, if the reliability type uses ordering channels
	unsigned char orderingChannel; //顺序通道

	///The ID of the split packet, if we have split packets.  This is the maximum number of split messages we can send simultaneously per connection.
	SplitPacketIdType splitPacketId; //分包标识

	///If this is a split packet, the index into the array of subsplit packets
	SplitPacketIndexType splitPacketIndex;//分包序列

	///The size of the array of subsplit packets
	SplitPacketIndexType splitPacketCount; //分包总数

	///How many bits long the data is
	BitSize_t dataBitLength; //数据长度

	///What type of reliability algorithm to use with this packet
	PacketReliability reliability;//可靠性算法
	// Not endian safe
	// unsigned char priority : 3;
	// unsigned char reliability : 5;
};

/// Used in InternalPacket when pointing to sharedDataBlock, rather than allocating itself
struct InternalPacketRefCountedData //引用计数
{
	unsigned char *sharedDataBlock;
	unsigned int refCount;
};

/// Holds a user message, and related information
/// Don't use a constructor or destructor, due to the memory pool I am using
struct InternalPacket : public InternalPacketFixedSizeTransmissionHeader //内部数据包
{
	/// Identifies the order in which this number was sent. Used locally
	MessageNumberType messageInternalOrder; //唯一标识

	/// Has this message number been assigned yet?  We don't assign until the message is actually sent.
	/// This fixes a bug where pre-determining message numbers and then sending a message on a different channel creates a huge gap.
	/// This causes performance problems and causes those messages to timeout.
	bool messageNumberAssigned; //是否被分配标识

	/// Was this packet number used this update to track windowing drops or increases?  Each packet number is only used once per update.
//	bool allowWindowUpdate;
	///When this packet was created
	RakNet::TimeUS creationTime; //创建时间

	///The resendNext time to take action on this packet
	RakNet::TimeUS nextActionTime; //下个动作时间

	// For debugging
	RakNet::TimeUS retransmissionTime; //丢失时间

	// Size of the header when encoded into a bitstream
	BitSize_t headerLength; //头长度信息

	/// Buffer is a pointer to the actual data, assuming this packet has data at all
	unsigned char *data; //数据内容

	/// How to alloc and delete the data member
	enum AllocationScheme //定义申请内容方式
	{
		/// Data is allocated using rakMalloc. Just free it
		NORMAL, //正常情况

		/// data points to a larger block of data, where the larger block is reference counted. internalPacketRefCountedData is used in this case
		REF_COUNTED, //引用计数
	
		/// If allocation scheme is STACK, data points to stackData and should not be deallocated
		/// This is only used when sending. Received packets are deallocated in RakPeer
		STACK //栈模式
	} allocationScheme;
	InternalPacketRefCountedData *refCountedData; //引用计数

	/// How many attempts we made at sending this message
	unsigned char timesSent; //发送时间

	/// The priority level of this packet
	PacketPriority priority; //优先级

	/// If the reliability type requires a receipt, then return this number with it
	uint32_t sendReceiptSerial; //可靠性统计

	// Used for the resend queue
	// Linked list implementation so I can remove from the list via a pointer, without finding it in the list
	InternalPacket *resendPrev, *resendNext,*unreliablePrev,*unreliableNext; //发送队列

	unsigned char stackData[128]; //堆栈数据
};

} // namespace RakNet

#endif

