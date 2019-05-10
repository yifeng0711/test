//---------------------------------------------------------------------------
#ifndef MC_RapidXMLH
#define MC_RapidXMLH
//---------------------------------------------------------------------------
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include <string>
#include <stdio.h>
//---------------------------------------------------------------------------
template <class Ch=char>
class MC_XMLDocument : public rapidxml::xml_document<Ch>
{
public:
	enum XMLSaveCharCode{UTF8, GBK};
	
public:
	bool							Load			(Ch* vXmlContent, int vSize);
	bool							LoadFromFile	(const char* vFilePath);
	bool							SaveToFile		(const char* vFilePath, XMLSaveCharCode vSaveCharCode=GBK);
	Ch*								XmlPtr			(void); 

	void							SetValue		(rapidxml::xml_base<Ch>* vBase, Ch* vVal);
	void							SetName			(rapidxml::xml_base<Ch>* vBase, Ch* vName);

	rapidxml::xml_node<Ch>*			AllocNode		(rapidxml::node_type type, const Ch *name=0, const Ch *value=0, std::size_t name_size=0, std::size_t value_size=0);
	rapidxml::xml_attribute<Ch>*	AllocAttr		(const Ch *name = 0, const Ch *value = 0,std::size_t name_size = 0, std::size_t value_size = 0);
	bool							AppendNode		(rapidxml::xml_node<Ch> *vChild);
};
//---------------------------------------------------------------------------
template <class Ch>
bool MC_XMLDocument<Ch>::Load(Ch* vXmlContent, int vSize)
{
	if(0 == vXmlContent)
		return false;
		
	bool tRet = true;
	try
		{
		Ch* tBuf = (Ch*)allocate_aligned(vSize + 1);
		memcpy(tBuf, vXmlContent, vSize); 
		tBuf[vSize] = 0;                           
		parse<0>(tBuf);
		}
	catch(...)
		{
		tRet = false;
		}

	return tRet;
}
//---------------------------------------------------------------------------
template <class Ch>
bool MC_XMLDocument<Ch>::LoadFromFile(const char* vFilePath)
{
	if(0 == vFilePath)		
		return false;
			
	bool tRet = true;
	try
		{
		std::basic_ifstream<Ch> tIStream(vFilePath, std::ios::binary);
		tIStream.unsetf(std::ios::skipws);

		tIStream.seekg(0, std::ios::end);
		std::size_t tSize = tIStream.tellg();
		tIStream.seekg(0);   

		Ch* tBuf = (Ch*)allocate_aligned(tSize + 1);; 
		tIStream.read(tBuf, (std::streamsize)(tSize));
		tBuf[tSize] = 0;
		parse<0>(tBuf);
		
		}
	catch(...)
		{
		tRet = false;
		}
		
	return tRet;
}
//---------------------------------------------------------------------------
template <class Ch>
bool MC_XMLDocument<Ch>::SaveToFile(const char* vFilePath, XMLSaveCharCode vSaveCharCode)
{
	bool tRet = true;
	try
		{
		std::basic_ofstream<Ch> tOStream(vFilePath, std::ios::binary);
        if(UTF8 == vSaveCharCode)
            tOStream<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";

        if(GBK == vSaveCharCode)
            tOStream<<"<?xml version=\"1.0\" encoding=\"gbk\"?>\n";
			
		rapidxml::print(std::ostream_iterator<Ch>(tOStream), *(rapidxml::xml_document<Ch>*)this);
	    }
	catch(...)
		{
		tRet = false;
		}
		
	return tRet;
}
//---------------------------------------------------------------------------
template <class Ch>
Ch* MC_XMLDocument<Ch>::XmlPtr(void)
{
	try
		{
		std::vector<Ch> tXmlData;
		rapidxml::print(std::back_inserter(tXmlData), *(rapidxml::xml_document<Ch>*)this);
		tXmlData.push_back((Ch)0);

		static std::string tResult;
		tResult = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
		tResult += (Ch*)&tXmlData.front();
		return (char*)tResult.c_str();
		}
	catch(...)
		{
		return 0;
		}
}
//---------------------------------------------------------------------------
template <class Ch>
void MC_XMLDocument<Ch>::SetValue(rapidxml::xml_base<Ch>* vBase, Ch* vVal)
{
	vBase->value(allocate_string(vVal));
}
//---------------------------------------------------------------------------
template <class Ch>
void MC_XMLDocument<Ch>::SetName(rapidxml::xml_base<Ch>* vBase, Ch* vName)
{
	vBase->name(allocate_string(vName));
}
//---------------------------------------------------------------------------
template <class Ch>
rapidxml::xml_node<Ch>* MC_XMLDocument<Ch>::AllocNode(rapidxml::node_type type, const Ch *name, const Ch *value, std::size_t name_size, std::size_t value_size)
{
	Ch* tName	= 0;
	Ch* tValue 	= 0;
	if(0 != name)
		tName = allocate_string(name, name_size);
		
	if(0 != value)
		tValue = allocate_string(value, value_size);

	return allocate_node(type, tName, tValue, name_size, value_size);
}
//---------------------------------------------------------------------------
template <class Ch>
rapidxml::xml_attribute<Ch>* MC_XMLDocument<Ch>::AllocAttr(const Ch* name, const Ch* value, std::size_t name_size, std::size_t value_size)
{
	Ch* tName	= 0;
	Ch* tValue 	= 0;
	if(0 != name)
		tName = allocate_string(name, name_size);
	if(0 != value)
		tValue = allocate_string(value, value_size);

	return allocate_attribute(tName, tValue, name_size, value_size);		
}
//---------------------------------------------------------------------------
template <class Ch>
bool MC_XMLDocument<Ch>::AppendNode(rapidxml::xml_node<Ch> *vChild)
{
	if(0 == vChild)
		return false;
	
	xml_node<Ch>* tNode = (xml_node<Ch>*)allocate_aligned(sizeof(xml_node<Ch>));
	if(0 == tNode)
		return false;

	memcpy(tNode, vChild, sizeof(xml_node<Ch>));
	append_node(tNode);
	return true;
}
//---------------------------------------------------------------------------
#endif
