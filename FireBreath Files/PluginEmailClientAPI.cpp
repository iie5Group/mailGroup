/**********************************************************\

  Auto-generated PluginEmailClientAPI.cpp

\**********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include <boost/thread/thread.hpp>
#include "myFileChoose.h"
#include "myFileSave.h"
#include "progressBar.h"
#include <fstream>

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <curl/easy.h>

#include "PluginEmailClientAPI.h"

FILE *PluginEmailClientAPI::pMyFile = NULL;
std::string  strUpOrDownloadFilePath = "";
void *CurlDownloadThreadFunc(void *ptrUrl);
///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant PluginEmailClientAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant PluginEmailClientAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn PluginEmailClientPtr PluginEmailClientAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
PluginEmailClientPtr PluginEmailClientAPI::getPlugin()
{
    PluginEmailClientPtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string PluginEmailClientAPI::get_testString()
{
    return m_testString;
}

void PluginEmailClientAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string PluginEmailClientAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void PluginEmailClientAPI::testEvent()
{
    fire_test();
}

//
//
std::string PluginEmailClientAPI::testMethod(const std::string &sMessage)
{
    return sMessage;
}

//
//
int PluginEmailClientAPI::testThreadNoneParam()
{
    int iThrdResult = 0;

    boost::packaged_task<int> pt(boost::bind(&PluginEmailClientAPI::NoneParamFunc, this));
    boost::unique_future<int> fi = pt.get_future();

    boost::thread task(boost::move(pt));                //

    fi.wait();

    iThrdResult = fi.get();

    return iThrdResult;
}

//
//
int PluginEmailClientAPI::testThreadOneParam()
{
    int iThrdResult = 0;
    int a = 1234;
    boost::packaged_task<int> pt(boost::bind(&PluginEmailClientAPI::OneParamFunc, this, a));
    boost::unique_future<int> fi = pt.get_future();

    boost::thread task(boost::move(pt));                //

    fi.wait();

    iThrdResult = fi.get();

    return iThrdResult;
}

//
//
int PluginEmailClientAPI::testThreadMultiParam()
{
    int iThrdResult = 0;
    int a = 1234, b = 4321, c = 3;
    boost::packaged_task<int> pt(boost::bind(&PluginEmailClientAPI::MultiParamFunc, this, a, b, c));
    boost::unique_future<int> fi = pt.get_future();

    boost::thread task(boost::move(pt));                //

    fi.wait();

    iThrdResult = fi.get();

    return iThrdResult;
}

//
//
int PluginEmailClientAPI::NoneParamFunc()
{
    return 1234*4321;
}

//
//
int PluginEmailClientAPI::OneParamFunc(int iParamOne)
{
    return iParamOne*4321;
}

//
//
int PluginEmailClientAPI::MultiParamFunc(int iParamOne, int iParamTwo, int iParamThird)
{
    return iParamOne*iParamTwo*iParamThird;
}

//
//选择文件函数
//@param    void
//@return   vector<std::string> 返回选择的文件路径列表
std::vector<std::string> PluginEmailClientAPI::chooseFile()
{
    MyFileChoose mMyFileChoose;

    mMyFileChoose.SelectFiles();
    vector<string> myVector = mMyFileChoose.GetFilesPathList();

    return myVector;
}

//
//选择文件函数
//@param    void
//@return   std::string 返回保存的文件路径
//@describe 函数调用GTK选择保存对话框，用户选择保存路径后，函数将路径返回。
//------------------------------------------------------------------------------------------------------------------
std::string PluginEmailClientAPI::savePathSelect()
{
    MyFileSave mMyFileSave;                   //GTK选择保存对话框封装对象
    std::string strTemp = "";

    mMyFileSave.SelectSavePath();            //弹出选择保存路径对话框
    strTemp = mMyFileSave.GetSavePath();   //获取选择的路径

    return strTemp;                             //返回路径
}

//
//下载网页函数
//@param    std::string sUrl    输入需要下载的网页地址
//@return   void
void PluginEmailClientAPI::testSaveHtml(const std::string& sUrl)
{
    //若链接为空则返回
    if(sUrl == "")
        return;

    MyFileSave mMyFileSave;
    //std::string strSaveFilePath = "";

    mMyFileSave.SelectSavePath();

    //strFilePath = mMyFileSave.GetSavePath();

    strUpOrDownloadFilePath = mMyFileSave.GetSavePath();

    ProgressBar_Download mProgressBar_Download;

    mProgressBar_Download.CreateProgressBar();
    mProgressBar_Download.SetUrl(sUrl);
    mProgressBar_Download.CreateCurlThrd(&CurlDownloadThreadFunc);
    mProgressBar_Download.GtkMainLoop();

    //boost::thread mHtmlDownloadThrd(boost::bind(&PluginEmailClientAPI::testCurlDownHtmlFunc, this, sUrl, strSaveFilePath));
}

//
//下载网页线程实体函数
//@param    std::string sUrl
//@return   void
//
void PluginEmailClientAPI::testCurlDownHtmlFunc(std::string sUrl, std::string strSaveFilePath)
{
    CURL *cCurl;
    std::ofstream mOfstream("\\root\\result.txt");

    if(strSaveFilePath == "")
        return;

    curl_global_init(CURL_GLOBAL_ALL);

    cCurl = curl_easy_init();

    curl_easy_setopt(cCurl, CURLOPT_URL, sUrl.c_str());

    if((pMyFile=  fopen(strSaveFilePath.c_str(), "w")) == NULL)
    {
        curl_easy_cleanup(cCurl);
        mOfstream << "Error: Can't open file!" << endl;
        return;
    }

    mOfstream << "Start write file!" << endl;
    curl_easy_setopt(cCurl, CURLOPT_WRITEFUNCTION, MyWriteToFile);

    mOfstream << "Hooked write file function!" << endl;
    curl_easy_perform(cCurl);

    curl_easy_cleanup(cCurl);

    mOfstream << "End of Thread!" << endl;

    fclose(pMyFile);

    return;

}


//
//curl写文件回调函数
//@param    void    *ptr
//          size_t  size
//          size_t  nmemb
//          void    *stream
//@return   size_t              本次写入数据的长度
//
size_t PluginEmailClientAPI::MyWriteToFile(void *ptr, size_t size, size_t nmemb, void *stream)
{
    int iWrittenSize = fwrite(ptr, size, nmemb, (FILE *)pMyFile);
    return iWrittenSize;
}

void *CurlDownloadThreadFunc(void *ptrUrl)
{
  CURL *ptrCurl;
  CURLcode resResultCode;
  FILE *ptrWriteFile;
  gchar *strUrl = (gchar *)ptrUrl;

  ptrCurl = curl_easy_init();
  if(ptrCurl)
  {
    ptrWriteFile = fopen(strUpOrDownloadFilePath.c_str(), "w");

    curl_easy_setopt(ptrCurl, CURLOPT_URL, strUrl);
    curl_easy_setopt(ptrCurl, CURLOPT_WRITEDATA, ptrWriteFile);
    curl_easy_setopt(ptrCurl, CURLOPT_WRITEFUNCTION, ProgressBar_Download::file_write_func);
    //curl_easy_setopt(ptrCurl, CURLOPT_READFUNCTION, my_read_func);
    curl_easy_setopt(ptrCurl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(ptrCurl, CURLOPT_PROGRESSFUNCTION, ProgressBar_Download::ProgressControlFunc);
    curl_easy_setopt(ptrCurl, CURLOPT_PROGRESSDATA, ProgressBar_Base::m_ptrProgressBar);

    resResultCode = curl_easy_perform(ptrCurl);

    fclose(ptrWriteFile);
    /* always cleanup */
    curl_easy_cleanup(ptrCurl);
  }

  return NULL;
}

