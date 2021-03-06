#include "ATR.h"
#include "Market.h"
#include "Stock.h"
#include "TimePeriod.h"

namespace MM
{
	namespace Indicators
	{

		void ATR::reset()
		{
			value = std::numeric_limits<double>::quiet_NaN();
		}

		ATR::ATR(std::string currencyPair, int history, int seconds) :
			currencyPair(currencyPair),
			history(history),
			seconds(seconds)
		{
		}


		ATR::~ATR()
		{
		}

		void ATR::declareExports() const
		{
			exportVariable("ATR", std::bind(&ATR::getATRMA, this), "period " + std::to_string(seconds) + ", memory " + std::to_string(history));
		}

		double ATR::getTrueRange(MM::Stock *stock, const std::time_t &time, const int &duration)
		{
			MM::TimePeriod period = stock->getTimePeriod(time - duration, time);
			const MM::PossibleDecimal high = period.getHigh();
			const MM::PossibleDecimal low  = period.getLow();
			if (!high.get() || !low.get()) return std::numeric_limits<double>::quiet_NaN();

			// in theory, we need to adjust this by the last period's close;
			// this should not be necessary, though, because we don't have overnight gaps etc..
			return *high.get() - *low.get();
		}


		void ATR::update(const std::time_t &secondsSinceStart, const std::time_t &time)
		{
			Stock *stock = market.getStock(currencyPair);
			if (stock == nullptr) return;

			// need to initialize the value?
			if (std::isnan(value))
			{
				value = 0.0f;
				for (int p = 0; p < history; ++p)
				{
					value += getTrueRange(stock, time - seconds * p, seconds);
				}
				value /= static_cast<float>(history);
				return;
			}

			const double trueRange = getTrueRange(stock, time, seconds);
			const double historyDouble = static_cast<double>(history);
			value = (value * (historyDouble - 1.0) + trueRange) / historyDouble;
		}
	};
};