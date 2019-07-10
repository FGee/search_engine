#include "../inc/webpageLibProducer.h"
#include "../inc/configuration.h"
#include "../inc/dirFileGet.h"
#include "../inc/tinyxml2/tinyxml2.h"
#include "../inc/macro.h"
#include "../inc/webPage.h"

#include <iostream>

using namespace tinyxml2;

namespace wd
{

WebpageLibProducer::WebpageLibProducer()
    : _offsetMap(), _docID(0)
{
    cout << "WebpageLibProducer()" << endl;
}

WebpageLibProducer::~WebpageLibProducer()
{
    cout << "~WebpageLibProducer()" << endl;
}

void WebpageLibProducer::producePageLib()
{
    auto configMap = Configuration::getInstanceConfig()->getConfigMap();
    string rssRawPath = configMap["rss_raw_path"];

    DirFileGet dirFileGet(rssRawPath.c_str(), "^[a-zA-Z0-9._]+\\.[x|X][m|M][l|L]$");
    const auto& filePathVec = dirFileGet.filePathVecGet();
    
    string webPageLib = configMap["webpage_lib"];
    ofstream ofs;
    ofs.open(webPageLib, ios::out);
    if (ofs.fail()) { ERROR_EXIT("webpage lib open error"); }
    
    for (const auto filePath : filePathVec) {
        processSingle(ofs, filePath.c_str());
    }

    ofs.close();
    saveOffsetLib();
}

void WebpageLibProducer::processSingle(ofstream& ofs, const char* pathName) 
{
    cout << ">>>> pathName: " << pathName << endl;
    XMLDocument doc;
    if (0 != doc.LoadFile(pathName)) { ERROR_EXIT("xml file load fail"); }

    XMLElement* root = doc.RootElement();

    XMLElement* pChannel = root->FirstChildElement("channel");

    if (nullptr == pChannel) { ERROR_EXIT("get first child element error"); }
    
    XMLElement* pItem = pChannel->FirstChildElement("item");
    while (pItem) {
        WebPage tmpWeb;

        XMLElement* pItemChild = pItem->FirstChildElement();

        while (pItemChild) {
            if (string("url") == string(pItemChild->Name())) {
                if (pItemChild->GetText()) {
                    tmpWeb._url = pItemChild->GetText();
                }
            } else if (string("link") == string(pItemChild->Name())) {
                if (pItemChild->GetText()) {
                    tmpWeb._url = pItemChild->GetText();
                }
            } else if (string("title") == string(pItemChild->Name())) {
                if (pItemChild->GetText()) {
                    tmpWeb._title = pItemChild->GetText();
                }
            } else if (string("content:encoded") == string(pItemChild->Name())) {
                if (pItemChild->GetText()) {
                    tmpWeb._content = deleteHtmlMark(pItemChild->GetText());
                }
            } else if (string("description") == string(pItemChild->Name())) {
                if (pItemChild->GetText()) {
                    tmpWeb._content = deleteHtmlMark(pItemChild->GetText());
                }
            }
            pItemChild = pItemChild->NextSiblingElement();
        }
        if (string() == tmpWeb._url) {
            pItem = pItem->NextSiblingElement();
            continue;
        }
        if (string() == tmpWeb._title) {
            string line;
            istringstream iss(tmpWeb._content);
            getline(iss, line);
            tmpWeb._title = line;
        }
        tmpWeb._docid = _docID;

        tmpWeb._simHash = 0;
        
        string txt = tmpWeb.toDoc();
        pair<long, long> newPair;
        newPair.first = ofs.tellp();
        
        ofs << txt;
        
        newPair.second = txt.size();

        _offsetMap.insert(make_pair(_docID, newPair));
        
        ++_docID;

        pItem = pItem->NextSiblingElement();
    }
}

string WebpageLibProducer::deleteHtmlMark(string content)
{
    regex r("<(\\S*?)[^>]*>.*?|<.*? />|&[a-zA-Z]+?;");
    return regex_replace(content, r, " ");
}

void WebpageLibProducer::saveOffsetLib()
{
    auto configMap = Configuration::getInstanceConfig()->getConfigMap();
    string offsetLibPath = configMap["offset_lib"];
    
    ofstream ofs;
    ofs.open(offsetLibPath, ios::out);

    if (ofs.fail()) { ERROR_EXIT("offset lib open error"); }

    for (auto & elem : _offsetMap) {
        ofs << elem.first << " "
            << elem.second.first << " "
            << elem.second.second
            << endl;
    }

    ofs.close();
}

}
