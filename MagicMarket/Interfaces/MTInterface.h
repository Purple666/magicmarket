#pragma once

#include <memory>
#include <string>

#include "Interfaces/UDP.h"

namespace Interface
{
	namespace MetaTrader
	{
		namespace Message
		{
			enum Type
			{
				bridgeUp = 1,
				bridgeDown = 2,
				bridgeTick = 3,
				bridgeAccountInfo = 4,
				bridgeOrders = 5,
				bridgeError = 6,
				newOrder = 7,
				closeOrder = 8,
				updateOrder = 9
			};
#pragma pack(push, 1)
			struct BridgeUp
			{
				char pair[7];
				int32_t timestamp;
			};

			struct BridgeDown
			{
				char pair[7];
				int32_t timestamp;
			};

			static_assert(sizeof(double) == 8, "Require double to be 64bit length.");
			struct Tick
			{
				char pair[7];
				double bid;
				double ask;
				int32_t timestamp;
			};

			struct AccountInfo
			{
				double leverage;
				double balance;
				double margin;
				double freeMargin;
			};

			struct Order
			{
				char pair[7];
				int32_t type;
				int32_t tickedID;
				double openPrice;
				double takeProfit;
				double stopLoss;
				int32_t timestampOpen;
				int32_t timestampExpire;
				double lots;
				double profit;
			};

			struct NewOrder
			{
				int32_t messageType = Type::newOrder;
				int32_t type;
				double orderPrice;
				double takeProfitPrice;
				double stopLossPrice;
				double lotSize;
			};

			struct CloseOrder
			{
				int32_t messageType = Type::closeOrder;
				int32_t ticketID;
			};

			struct UpdateOrder
			{
				int32_t messageType = Type::updateOrder;
				int32_t ticketID;
				double takeProfitPrice;
				double stopLossPrice;
			};
#pragma pack(pop)
		};
		
		class MTInterface
		{
		public:
			MTInterface();
			~MTInterface();

			void init(void *ini);
			void checkIncomingMessages();

			template<typename T> void send(const T &data);
		private:
			void setup();

			std::unique_ptr<::Interface::Internet::WSASession> session;
			std::unique_ptr<::Interface::Internet::UDPSocket> socket;
			int port;

			::Interface::Internet::UDPSocketReplyChannel marketExecutioner;
		};
	};
};

extern Interface::MetaTrader::MTInterface metatrader;
