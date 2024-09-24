#ifndef SLOGE_HPP
#define SLOGE_HPP

/**************************************************************************
 * This file extends the logging methods and provides a single include
 * to use for logging in files. To log, include this file and use the
 * SLOG::log function.
 *
 * You may also extend LogLevels by creating a LogLevelsE class.
 *
 * Example:
 * SLOG::log( LogLevels::INFO, LogMethodsE::INFO, "example", __LINE__, __FILE__ );
**************************************************************************/

#include "SLOG.hpp"
#include "LogLevels.hpp"
#include "LogMethods.hpp"
#include "LogMethodStd.hpp"


class LogMethodsE : public LogMethods
{
	public:
		static const LogMethodStdout INFO;
		static const LogMethodStdout WARN;
		static const LogMethodStderr ERROR;

};

#endif // SLOGE_HPP
