#include "Statistics.h"

#include "Market.h"
#include <SimpleIni.h>
#include "VirtualMarket.h"

#include <sstream>

MM::Statistics statistics;

namespace MM
{

	Statistics::Statistics()
	{
	}


	Statistics::~Statistics()
	{
	}

	void Statistics::init(void *_ini)
	{
		const CSimpleIniA &ini = *(CSimpleIniA*)_ini;
		loggingActive = ini.GetBoolValue("Statistics", "Enabled", true);
		config.outputFilename = ini.GetValue("Statistics", "OutputFilename", "saves/variables.csv");
		config.delimiter = ini.GetValue("Statistics", "Delimiter", "\t");
		if (config.delimiter == "tab")
			config.delimiter = "\t";
		else if (config.delimiter == "space")
			config.delimiter = " ";
	}

	void Statistics::addVariable(const Variable &var)
	{
		const int duplicateCount = std::count_if(variables.begin(), variables.end(), [&](const Variable &other) { return other.originalName == var.name; }); 

		variables.push_back(var);

		if (duplicateCount > 0)
		{
			std::ostringstream os;
			os << var.name << "_" << (duplicateCount + 1);
			variables.back().name = os.str();
		}
	}

	void Statistics::log()
	{
		if (!loggingActive) return;

		// try to open for the very first time?
		if (!outputStream.is_open())
		{
			// log variable descriptions
			std::fstream descriptionStream(config.outputFilename + ".info", std::ios_base::out);
			// open real stream
			outputStream.open(config.outputFilename, std::ios_base::out);
			if (!outputStream.good()) return;

			// print header row
			outputStream << "time" << config.delimiter << "snapshot index";
			for (const Variable &var : variables)
			{
				outputStream << config.delimiter << var.name;

				descriptionStream << var.name << config.delimiter << var.description << std::endl;
			}
		}

		// get all observations and log them
		int lastVirtualMarketSnapshotIndex = -1;
		if (market.isVirtual())
			lastVirtualMarketSnapshotIndex = virtualMarket->getLastSnapshotIndex();
		outputStream << market.getLastTickTime() << config.delimiter << lastVirtualMarketSnapshotIndex;

		for (const Variable &var : variables)
		{
			outputStream << config.delimiter << var.get();
		}
		// flush file to allow killing the application without losses
		outputStream << std::endl << std::flush;
	}
};