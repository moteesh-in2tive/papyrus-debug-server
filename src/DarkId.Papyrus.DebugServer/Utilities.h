#pragma once

#include <string>
#include <sstream>
#include <boost/algorithm/string.hpp>

namespace DarkId::Papyrus::DebugServer
{
	template<typename... Args>
	std::string StringFormat(const char* fmt, Args... args)
	{
		const size_t size = snprintf(nullptr, 0, fmt, args...);
		std::string buf;
		buf.reserve(size + 1);
		buf.resize(size);
		snprintf(&buf[0], size + 1, fmt, args...);
		return buf;
	}

	template <typename T>
	T ByteSwap(T val)
	{
		T retVal;
		const auto pVal = reinterpret_cast<char*>(& val);
		const auto pRetVal = reinterpret_cast<char*>(& retVal);
		const int size = sizeof(T);
		for (auto i = 0; i < size; i++)
		{
			pRetVal[size - 1 - i] = pVal[i];
		}

		return retVal;
	}

	inline std::vector<std::string> Split(const std::string& s, const std::string& delimiter) {
		size_t posStart = 0, posEnd;
		const auto delimLen = delimiter.length();
		std::vector<std::string> res;

		while ((posEnd = s.find(delimiter, posStart)) != std::string::npos) {
			auto token = s.substr(posStart, posEnd - posStart);
			posStart = posEnd + delimLen;
			res.push_back(token);
		}

		res.push_back(s.substr(posStart));
		return res;
	}

	inline std::string Join(const std::vector<std::string>& elements, const char* const separator)
	{
		switch (elements.size())
		{
		case 0:
			return "";
		case 1:
			return elements[0];
		default:
			std::ostringstream os;
			std::copy(elements.begin(), elements.end() - 1, std::ostream_iterator<std::string>(os, separator));
			os << *elements.rbegin();
			return os.str();
		}
	}

	inline bool ParseInt(const std::string& str, int* value, std::size_t* pos = nullptr, const int base = 10)
	{
		try
		{
			*value = std::stoi(str, pos, base);
		}
		catch (void*)
		{
			return false;
		}

		return true;
	}

	inline bool CaseInsensitiveEquals(std::string a, std::string b)
	{
		boost::algorithm::to_lower(a);
		boost::algorithm::to_lower(b);

		return a == b;
	}

	inline std::string DemangleName(std::string name)
	{
		if (name.front() == ':')
		{
			return name.substr(2, name.length() - 6);
		}

		return name;
	}
}
