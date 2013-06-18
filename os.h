// name: os.h
// version: 1.0
// description: gathers operating system information

// make sure definitions only occurred once
#ifndef OS_H
#define OS_H

	// windows detection
	#if defined(WIN32) || defined(__WIN32__) || defined(_WIN32) || defined(WIN64) || defined(_WIN64) || defined(__WINDOWS__) || defined(__CYGWIN__) || defined(__MINGW32__) || defined(__BORLANDC__) || defined(__TOS_WIN__)

		#define OS_WIN

	// unix detection
	#elif defined(unix) || defined(__unix__) || defined(__unix)

		#define OS_UNIX

	// android detection
	#elif defined(__ANDROID__)

		#define OS_ANDROID

	// unknown detection
	#else

		#define OS_NULL

	#endif

#endif
