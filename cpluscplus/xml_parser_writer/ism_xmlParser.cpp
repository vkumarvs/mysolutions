#include <iostream>
#include <fstream>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <Poco/AutoPtr.h>
#include <Poco/Exception.h>
#include <Poco/Util/XMLConfiguration.h>
#include "Poco/DOM/DOMParser.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/NodeIterator.h"
#include "Poco/DOM/NodeFilter.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/DOM/AutoPtr.h"
#include "Poco/SAX/InputSource.h"
#include "Poco/DOM/DOMWriter.h"
#include "Poco/XML/XMLWriter.h"
#include <sstream>
#include "Poco/SAX/InputSource.h"
#include "Poco/SAX/AttributesImpl.h"
using namespace std;
using Poco::Exception;
using Poco::AutoPtr;
using Poco::Util::XMLConfiguration;
/*
<head>
    <meta name="clientManifestRelativePath" content="drm_elephant.ismc" />
    <meta name="creator" content="Unified Streaming Platform (USP)" />
    <meta name="iss_playout" content="playready" />
    <meta name="hls_playout" content="sample_aes" />
    <meta name="hds_playout" content="false" />
    <meta name="mpd_playout" content="true" />
    <meta name="mpd_inline_drm" content="true" />
    <paramGroup id="hls">
      <meta name="key" content=":8275665585FBC4C95EC59FA8B925ED0F" />
      <meta name="key_iv" content="DA5797617DC0ABA90E59DD82C2531777" />
      <meta name="license_server_url" content="http://widevine-dash.ezdrm.com/proxy?pX=EZDRM_ACCOUNT_ID" />
    </paramGroup>
    <paramGroup id="iss">
      <meta name="key" content="000102030405060708090A0B0C0D0E0F:000102030405060708090A0B0C0D0E0F" />
    </paramGroup>
    <paramGroup id="widevine">
      <meta name="drm_specific_data" content="aDM0aDM0MjNrMDAyODVtbW1tZ2dnZzMyNDM0MmRmYWRm" />
      <meta name="key" content="000102030405060708090A0B0C0D0E0F:000102030405060708090A0B0C0D0E0F" />
    </paramGroup>
  </head>
*/

#if 1
void InsertDrmInfo(string &drmType) {
}
void createWidewine(Poco::AutoPtr<Poco::XML::Document> &pDoc)
{
#if 1
    for (Poco::XML::Node *p = pDoc->documentElement()->firstChild(); p; p = p->nextSibling()) {
        if (p->nodeType() != Poco::XML::Node::ELEMENT_NODE) {
            continue;
        }
        Poco::XML::Element *pe = dynamic_cast<Poco::XML::Element *>(p);
        if (!pe)
            continue;
        if (pe->nodeName() == "head")  {
            cout << "Head node present" <<endl;
            Poco::AutoPtr<Poco::XML::Element> headNode = pDoc->createElement("paramGroup");
            headNode->setAttribute("id","hls");

            Poco::AutoPtr<Poco::XML::Element> child;
            child  = pDoc->createElement("meta");
            child->setAttribute("name","key");
            child->setAttribute("content",":8275665585FBC4C95EC59FA8B925ED0F");
            headNode->appendChild(child);

            child  = pDoc->createElement("meta");
            child->setAttribute("name","key_iv");
            child->setAttribute("content","DA5797617DC0ABA90E59DD82C2531777");
            headNode->appendChild(child);

            child  = pDoc->createElement("meta");
            child->setAttribute("name","license_server_url");
            child->setAttribute("content","http://widevine-dash.ezdrm.com/proxy?pX=EZDRM_ACCOUNT_ID");
            headNode->appendChild(child);

            pe->appendChild(headNode);
            break;
        }
    }
#else
    AutoPtr<Element> pRoot = pDoc->createElement("root");
    pDoc->appendChild(pRoot);
    AutoPtr<Element> pChild1 = pDoc->createElement("child1");
    AutoPtr<Text> pText1 = pDoc->createTextNode("text1");
    pChild1->appendChild(pText1);
    pRoot->appendChild(pChild1);
    AutoPtr<Element> pChild2 = pDoc->createElement("child2");
    AutoPtr<Text> pText2 = pDoc->createTextNode("text2");
    pChild2->appendChild(pText2);
    pRoot->appendChild(pChild2);
#endif
}

void writeXMLDoc(Poco::AutoPtr<Poco::XML::Document> &pDoc)
{
    Poco::XML::DOMWriter writer;
    writer.setNewLine("\n");
    writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT|Poco::XML::XMLWriter::WRITE_XML_DECLARATION);
    //writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT);
#if 1
    writer.writeNode(std::cout, pDoc);
#else
    std::stringstream output;
    writer.writeNode(output, pDoc);
    cout << output.str() <<endl;
    //                             outputStr = output.str();
    //   string outputStr;
#endif
}
#endif
void xmleditor() {
    string accumulatedBodyStr;

#if 1
    ifstream in;
    in.open("elephant.ism");
    if ( ! in ) {
        cout << "Error: Can't open the file named data.txt.\n";
        exit(1);
    }
    string str;
    getline(in,str);  // Get the frist line from the file, if any.
    while ( in ) {  // Continue if the line was sucessfully read.
        accumulatedBodyStr.append(str);  // Process the line.
        accumulatedBodyStr.append("\n");  // Process the line.
        getline(in,str);   // Try to get another line.
    }
    //Poco::XML::InputSource src(accumulatedBodyStr);
#else
    std::ifstream in("elephant.ism");
    Poco::XML::InputSource src(in);
    //cout << accumulatedBodyStr <<endl;
#endif
    Poco::XML::DOMParser parser;
    // Disable namespaces, otherwise XMLWriter will add dummynamespaces
    // to the output XML file
    parser.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, false);
        Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parseString(accumulatedBodyStr);
      createWidewine(pDoc);
       writeXMLDoc(pDoc);

        //Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parse(&src);
#if 0
    try {
        Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parseString(accumulatedBodyStr);
        Poco::XML::NodeList* headNodes = pDoc->getElementsByTagName("head");
        string firstchildval="<paramGroup id=\"widevine\">\
                              <meta name=\"drm_specific_data\" content=\"aDM0aDM0MjNrMDAyODVtbW1tZ2dnZzMyNDM0MmRmYWRm\" />\
                              <meta name=\"key\" content=\"000102030405060708090A0B0C0D0E0F:000102030405060708090A0B0C0D0E0F\" />\
                              </paramGroup>"
                              if (headNodes) {
                                  //This is the case we need to modify the Base URL
                                  Poco::XML::NodeList* nodes = pDoc->getElementsByTagName("BaseURL");
                                  if (nodes) {
                                      for(unsigned int i=0;i<nodes->length();i++) {
                                          Poco::XML::Node* node = nodes->item(i);
                                          Poco::XML::Element* element = dynamic_cast<Poco::XML::Element*>(node);
                                          Poco::XML::Node* baseurlNode = element->firstChild();
                                          string baseurl = baseurlNode->nodeValue();

                                          //baseurlNode->setNodeValue(baseurl+baseurltokenparmstart+tad->streamingtokenparms);
                                      }
                                      nodes->release();
                                  } else {
                                      segnodes->release();
                                      return setResponseBodyandReturn(tad,pluginResponse,accumulatedBodyStr);
                                  }
                                  Poco::XML::DOMWriter writer;
                                  writer.setNewLine("\n");
                                  writer.setOptions(Poco::XML::XMLWriter::PRETTY_PRINT|Poco::XML::XMLWriter::WRITE_XML_DECLARATION);
                                  std::stringstream output;
                                  writer.writeNode(output, pDoc);
                                  outputStr = output.str();
                              }catch (Poco::Exception ex) {
                                  printf("Exception occured\n");
                              }
    }
#endif
}


int main() {
    xmleditor();
    return 0;
}
