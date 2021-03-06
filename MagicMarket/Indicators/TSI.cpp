#include "TSI.h"
#include "SMA.h"
#include "Moves.h"
#include "Market.h"
#include "Stock.h"
#include "TimePeriod.h"

namespace MM
{
	namespace Indicators
	{

		void TSI::reset()
		{
			tsi = std::numeric_limits<double>::quiet_NaN();
			momentumDoubleMA  = momentumDoubleMA_pushed = std::numeric_limits<double>::quiet_NaN();
			absMomentumDoubleMA = absMomentumDoubleMA_pushed = std::numeric_limits<double>::quiet_NaN();
			lastPushed = 0;
		}

		TSI::TSI(std::string currencyPair, int history, int seconds) :
			currencyPair(currencyPair),
			history(history),
			seconds(seconds)
		{
			moves = Indicators::get<Moves>(currencyPair, 2 * history, seconds);
		}

		void TSI::declareExports() const
		{
			exportVariable("TSI", std::bind(&TSI::getTSI, this), "period " + std::to_string(seconds) + ", memory " + std::to_string(history));
		}

		void TSI::update(const std::time_t &secondsSinceStart, const std::time_t &time)
		{
			const double &momentumMA    = moves->getMomentumMA();
			const double &absMomentumMA = moves->getAbsoluteMomentumMA();
			if (std::isnan(momentumMA) || std::isnan(absMomentumMA)) return;

			momentumDoubleMA    = Math::MA2(momentumDoubleMA_pushed, momentumMA, history);
			absMomentumDoubleMA = Math::MA2(absMomentumDoubleMA_pushed, absMomentumMA, history);
			
			if (time >= (lastPushed + seconds))
			{
				momentumDoubleMA_pushed = momentumDoubleMA;
				absMomentumDoubleMA_pushed = absMomentumDoubleMA;
			}

			if (absMomentumDoubleMA == 0.0)
			{
				tsi = std::numeric_limits<double>::quiet_NaN();
				return;
			}

			tsi = 100.0 * momentumDoubleMA / absMomentumDoubleMA;
			assert(tsi >= -101.0);
			assert(tsi <= 101.0);
			tsi = Math::clamp(tsi, -100.0, 100.0);
		}
	};
};