#include "resource/locale.h"
#include "threads.h"
#include "str_util.h"
#include "util/format.h"
#include "main/logging.h"
#include <stdarg.h>

#include "textfrog.h"

extern textfrog tfg ;

namespace resource {

void Locale::clear() {
	foreach(it, localizations)
		delete it->second;
	localizations.clear();
	hashLocalizations.clear();
}

void Locale::load(const std::string& filename) {
	DataReader datafile(filename);
	while(datafile++) {
		if(!datafile.value.empty() && datafile.value[0] == '\"')
			datafile.value = datafile.value.substr(1, datafile.value.size() - 2);

		if(isIdentifier(datafile.key)) {
			std::string* str = new std::string(unescape(datafile.value));
			localizations[datafile.key] = str;

			std::string withHash = "#";
			withHash += datafile.key;
			hashLocalizations[withHash] = str;
		}
		else {
			error("Locale key '%s' is not a valid identifier", datafile.key.c_str());
		}
	}
}
void	tfglog(  std::string& text, int typ){
	 tfg_call2 ( tfg , "logx" , "SL" , text .c_str(), typ ) ;
}
std::string Locale::localize(const std::string& text, bool requireHash, bool doUnescape, bool doFormat) {
	// tfg_call ( tfg , "hello" , 0 ) ;
	bool hasHash = !text.empty() && text[0] == '#';
	if(requireHash && !hasHash)
		return doUnescape ? unescape(text) : text;

	auto pos = doFormat ? text.find(':') : std::string::npos;
	if(pos == std::string::npos) {
		if(hasHash) {
			auto it = hashLocalizations.find(text);
			if(it == hashLocalizations.end())
				return doUnescape ? unescape(text) : text;
			tfglog(*it->second,1);
			// tfg_call2 ( tfg , "log1" , "S" , *it->second->c_str() ) ;
			return *it->second;
		}
		else {
			auto it = localizations.find(text);
			if(it == localizations.end())
				return doUnescape ? unescape(text) : text;
			tfglog(*it->second,2);
			//tfg_call2 ( tfg , "log1" , "S" , *it->second->c_str() ) ;
			return *it->second;
		}
	}
	else {
		std::string result;
		std::vector<std::string> arguments;
		split(text, arguments, ':', false, true);

		if(hasHash) {
			auto it = hashLocalizations.find(arguments[0]);
			if(it == hashLocalizations.end())
				return doUnescape ? unescape(arguments[0]) : arguments[0];
			result = *it->second;
		}
		else {
			auto it = localizations.find(arguments[0]);
			if(it == localizations.end())
				return doUnescape ? unescape(arguments[0]) : arguments[0];
			result = *it->second;
		}

		FormatArg args[16];
		unsigned argCnt = arguments.size();
		for(unsigned i = 1; i < argCnt; ++i) {
			args[i-1].type = FormatArg::Arg_string;
			args[i-1].s = &arguments[i];
		}

		std::string output;
		format(output, result.c_str(), argCnt-1, args);
		//tfg_call2 ( tfg , "log2" , "S" , output.c_str() ) ;
		tfglog(output,3);
		return output;
	}
}

Locale::~Locale() {
	clear();
}

};
