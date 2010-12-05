#include <iostream>
#include <sstream>
#include <string>

/*
* Return phrase if phrase doesn't contain delimiter
* if not, for ex, with delimiter=":", phrase="abc:def"
* StringSplit returns abc
* and phrase="def"
*/
std::string StringSplit(std::string & phrase, std::string delimiter){
	using namespace std;
	int cutAt = phrase.find(delimiter);
	if (phrase == "")
		return "";
	if( cutAt != std::string::npos ){
		if(cutAt > 0){
			std::string tmp = phrase.substr(0,cutAt);
			phrase = phrase.substr(cutAt+1);
			return tmp;
		}
	}else{
		std::string tmp = phrase;
		phrase = "";
		return tmp;
	}
}

/*
* Same as StringSplit but doesn't destroy phrase
*/
std::string getFirstOccurence(std::string phrase, std::string delimiter){
	using namespace std;
	int cutAt = phrase.find(delimiter);
	if (phrase == "")
		return "";
	if( cutAt != std::string::npos ){
		if(cutAt > 0){
			std::string tmp = phrase.substr(0,cutAt);
			return tmp;
		}
	}else{
		std::string tmp = phrase;
		return tmp;
	}
}

/* return s+a */
std::string joinString(std::string s,int a){
	std::ostringstream oss;
	oss << s;
	oss << a;
	return oss.str();
}

/* return s+a */
std::string joinString(std::string s,std::string a){
	std::ostringstream oss;
	oss << s;
	oss << a;
	return oss.str();
}

std::string joinString(int s,int a){
	std::ostringstream oss;
	oss << s;
	oss << a;
	return oss.str();
}