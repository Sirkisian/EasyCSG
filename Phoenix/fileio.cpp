#include "fileio.hpp"

bool FileIO::Export::poped = false;
std::stack<std::basic_string<TCHAR>> FileIO::Export::xmlTags;

void FileIO::Export::push(std::basic_ostream<TCHAR> & out, std::basic_string<TCHAR> tag)
{
	FileIO::Export::poped = false;

	FileIO::Export::xmlTags.push(tag);

	out << std::endl << _T("<") << tag << _T(">");
}

std::basic_ostream<TCHAR> & FileIO::Export::pop(std::basic_ostream<TCHAR> & out)
{
	if(!FileIO::Export::xmlTags.empty())
	{
		std::basic_string<TCHAR> tag = FileIO::Export::xmlTags.top();

		FileIO::Export::xmlTags.pop();

		if(FileIO::Export::poped)
		{
			out << std::endl;
		}

		out << _T("</") << tag << _T(">");

		FileIO::Export::poped = true;
	}

	return out;
}

bool FileIO::Import::isTag(_IN_(std::basic_string<TCHAR> & tag), _IN_(rapidxml::xml_node<TCHAR>* node))
{
	if(node != nullptr)
	{
		return tag.compare(node->name()) == 0;
	}

	return false;
}