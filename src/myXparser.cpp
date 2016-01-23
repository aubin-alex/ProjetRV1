/*
 * myXparser.cpp
 *
 *  Created on: Mar 7, 2014
 *      Author: fanzhang
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

#include <iostream>
#include <sstream>

#include "myXparser.h"

using namespace xercesc;
using namespace std;

myXparser::myXparser() {
	try {
		XMLPlatformUtils::Initialize();
	} catch (XMLException& e) {
		char* message = XMLString::transcode(e.getMessage());
		std::cerr << "XML toolkit initialization error: " << message << endl;
		XMLString::release(&message);
	}
	m_ConfigFileParser = new XercesDOMParser;
}

myXparser::~myXparser() {
	// Free memory
	delete m_ConfigFileParser;

	try {
		XMLPlatformUtils::Terminate();  // Terminate after release of memory
	} catch (xercesc::XMLException& e) {
		char* message = xercesc::XMLString::transcode(e.getMessage());

		cerr << "XML toolkit torn-down error: " << message << endl;
		XMLString::release(&message);
	}
}

void myXparser::readXMLFile(string& configFile) {
	// Test to see if the file is ok.
	//struct stat fileStatus;

	errno = 0;
	/*if (stat(configFile.c_str(), &fileStatus) == -1) // ==0 ok; ==-1 error
			{
		if ( errno == ENOENT)      // errno declared by include file errno.h
			throw(std::runtime_error(
					"Path file_name does not exist, or path is an empty string."));
		else if ( errno == ENOTDIR)
			throw(std::runtime_error(
					"A component of the path is not a directory."));
		else if ( errno == ELOOP)
			throw(std::runtime_error(
					"Too many symbolic links encountered while traversing the path."));
		else if ( errno == EACCES)
			throw(std::runtime_error("Permission denied."));
		else if ( errno == ENAMETOOLONG)
			throw(std::runtime_error("File can not be read\n"));
	}*/

	// Configure DOM parser.
	m_ConfigFileParser->setValidationScheme(XercesDOMParser::Val_Never);
	m_ConfigFileParser->setDoNamespaces(false);
	m_ConfigFileParser->setDoSchema(false);
	m_ConfigFileParser->setLoadExternalDTD(false);

	try {
		m_ConfigFileParser->parse(configFile.c_str());

	} catch (xercesc::XMLException& e) {
		char* message = xercesc::XMLString::transcode(e.getMessage());
		ostringstream errBuf;
		errBuf << "Error parsing file: " << message << flush;
		XMLString::release(&message);
	}
}

DOMDocument* myXparser::getParseredDoc() {
	return m_ConfigFileParser->getDocument();
}

