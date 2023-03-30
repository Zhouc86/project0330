#include <algorithm>
#include "sanitize_string.h"
bool ignoreInLabel(char c)
{
	return c == ' ' || c == ':' || c == '.' || c == '/' || c == '&';
}
std::string sanitizeLabel(const std::string &label)
{
	std::string out(label);

	// conceal labels that contain rtsp URL to prevent sensitive data leaks.
	//if (label.find("rtsp:") != std::string::npos)
	{
		std::hash<std::string> hash_fn;
		size_t hash = hash_fn(out);
		return std::to_string(hash);
	}

	out.erase(std::remove_if(out.begin(), out.end(), ignoreInLabel), out.end());
	return out;
}