/*
 *  myXparser.h
 *
 *  Created on: Aug 30, 2013
 *  Author: fzhang
 */

#ifndef MYXPARSER_H_
#define MYXPARSER_H_

#include <xercesc/dom/DOM.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/util/XMLUni.hpp>

using namespace xercesc;

// Error codes
enum {
	ERROR_ARGS = 1, ERROR_XERCES_INIT, ERROR_PARSE, ERROR_EMPTY_DOCUMENT
};

class myXparser {
public:
	myXparser();
	~myXparser();
	void readXMLFile(std::string&);
	DOMDocument* getParseredDoc();

private:
	XercesDOMParser* m_ConfigFileParser;
};

#endif /* MYXPARSER_H_ */
