#include "SMA.h"
#include "Market.h"
#include "Stock.h"
#include "TimePeriod.h"

namespace MM
{
	namespace Indicators
	{

		SMA::SMA(std::string currencyPair, int history, int seconds) :
			currencyPair(currencyPair),
			history(history),
			seconds(seconds)
		{
			sma     = std::numeric_limits<double>::quiet_NaN();
			sma2    = std::numeric_limits<double>::quiet_NaN();
			sma2abs = std::numeric_limits<double>::quiet_NaN();
		}


		SMA::~SMA()
		{
		}

		void SMA::declareExports() const
		{
			exportVariable("SMA", getSMA, "period " + std::to_string(seconds) + ", memory " + std::to_string(history));
			exportVariable("SMA2", getSMA2, "period " + std::to_string(seconds) + ", memory " + std::to_string(history));
			exportVariable("SMA2Abs", getSMA2Abs, "period " + std::to_string(seconds) + ", memory " + std::to_string(history));
		}

		void SMA::update(const std::time_t &secondsSinceStart, const std::time_t &time)
		{
			Stock *stock = market.getStock(currencyPair);
			if (stock == nullptr) return;
			MM::TimePeriod now = stock->getTimePeriod(time - seconds, time);
			const PossibleDecimal price = now.getClose();
			if (!price.get()) return;

			sma     = Math::MA (sma, *price, history);
			sma2    = Math::MA2(sma2, *price, history);
			sma2abs = Math::MA2(sma2abs, std::abs(*price), history);
		}
	};
};