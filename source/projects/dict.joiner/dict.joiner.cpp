/// @file	
///	@ingroup 	minexamples
///	@copyright	Copyright (c) 2016, Cycling '74
/// @author		Timothy Place
///	@license	Usage of this file and its contents is governed by the MIT License

#include "c74_min.h"

using namespace c74::min;

class dict_joiner : public object<dict_joiner> {
public:
	
	MIN_DESCRIPTION { "Merge the content of two dictionaries." };
	MIN_TAGS		{ "dictionary" };
	MIN_AUTHOR		{ "Cycling '74" };
	MIN_RELATED		{ "pedro, dict.join" };

	inlet	left	{ this, "dictionary to combined with dictionary at right inlet",	"" };
	inlet	right	{ this, "dictionary to combined with dictionary at left inlet",	"dictionary" };
	outlet	output	{ this, "dictionary of entries combined from both inlets",		"dictionary" };
	

	argument<anything> name_arg { this, "dictionary-syntax", "Define an initial dictionary for joining." };

	dict_joiner(const atoms& args = {}) {
		if (!args.empty())
			dict_right = dict(args);
		cout << "Hello World" << endl;
	}
		
	
	message bang { this, "bang", "Resend the most recently combined dictionary",
		MIN_FUNCTION {
			output.send("dictionary", dict_merged.name());
			return {};
		}
	};
	
	
	message dictionary { this, "dictionary",
		"Dictionary from the second inlet is combined with the dictionary from the first inlet and a new dictionary is sent",
		MIN_FUNCTION {
			try {
				dict d = { args[0] };
				
				if (current_inlet() == 0) {
					dict_merged = dict_right;	// start with our stored dict contents
					dict_merged.copyunique(d);	// now merge in any keys that are not duplicated in the incoming dict
					bang();						// send the dictionary name out the outlet
					dict_merged.touch();		// notify anything listening remotely (e.g. dict.view objects) that we changed
				}
				else {
					dict_right = d;
				}
			}
			catch (std::runtime_error& e) {
				cerr << e.what() << endl;
			}
			return {};
		}
	};

private:
	dict	dict_right { symbol(true) };
	dict	dict_merged { symbol(true) };
};

MIN_EXTERNAL(dict_joiner);
