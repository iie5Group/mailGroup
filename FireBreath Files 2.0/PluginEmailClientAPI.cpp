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
void *CurlUploadThreadFunc(void *ptrUrl);
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
    vector<string> myVectorFileList = mMyFileChoose.GetFilesPathList();

    return myVectorFileList;
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
     std::ofstream testDownloadLog("//root//test//loopresult.txt");


    mMyFileSave.SelectSavePath();

    //strFilePath = mMyFileSave.GetSavePath();

    strUpOrDownloadFilePath = mMyFileSave.GetSavePath();
    if(strUpOrDownloadFilePath == "")
        return;
    ProgressBar_Download mProgressBar_Download;

    testDownloadLog << "Get into ProgressControlFunc() ." <<endl;

    mProgressBar_Download.CreateProgressBar();
    mProgressBar_Download.SetUrl(sUrl);
    mProgressBar_Download.CreateCurlThrd(&CurlDownloadThreadFunc);

    //mProgressBar_Download.GtkMainLoop();
    //gtk_main();

    testDownloadLog << "download_now "  << endl;
    testDownloadLog << "download_total " << endl;
    //boost::thread mHtmlDownloadThrd(boost::bind(&PluginEmailClientAPI::testCurlDownHtmlFunc, this, sUrl, strSaveFilePath));
}


//
//下载网页函数
//@param    std::string sUrl    输入需要下载的网页地址
//@return   void
void PluginEmailClientAPI::testUploadFile(const std::string& sUrl)
{
    //若链接为空则返回
    if(sUrl == "")
        return;
    MyFileChoose mMyFileChoose;

    mMyFileChoose.SelectFiles();

    vector<string> myVectorFileList = mMyFileChoose.GetFilesPathList();

    if(myVectorFileList.empty())
        return;

    strUpOrDownloadFilePath = *(myVectorFileList.begin());

    ProgressBar_Upload mProgressBar_Upload;

    cout << "Create Bar!" << endl;


	mProgressBar_Upload.CreateProgressBar();

	cout << "Done!" << endl;

	cout << "Set Url!" << endl;

	mProgressBar_Upload.SetUrl(sUrl);

    cout << "Done!" << endl;

    cout << "Create Process Thread!" << endl;

	mProgressBar_Upload.CreateCurlThrd(&CurlUploadThreadFunc);

	cout << "All Done!" << endl;

	mProgressBar_Upload.GtkMainLoop();
}


void *CurlDownloadThreadFunc(void *ptrUrl)
{
    CURL *ptrCurl;
    CURLcode resResultCode;
    FILE *ptrWriteFile;
    gchar *strUrl = (gchar *)ptrUrl;
    std::ofstream mOfstream("//root//test//curlresult.txt");

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
        mOfstream << "resResultCode : " << resResultCode <<endl;
        fclose(ptrWriteFile);
        /* always cleanup */
        curl_easy_cleanup(ptrCurl);
    }

    g_usleep(1000000);

    mOfstream << "gtk_widget_destroy " <<endl;

    gtk_widget_hide(ProgressBar_Base::m_ptrProgressBarWindow);

    return NULL;
}

void *CurlUploadThreadFunc(void *ptrUrl)
{
  CURL *ptrCurl;
  CURLMcode resResultCode;
  FILE *ptrReadFile;
  //gchar *strUrl = (gchar *)ptrUrl;
  gchar *strUrl = "http://192.168.1.115:8080/xgs/AnalyServlet?person=fsadfa";

  CURLM *ptrMultiHandle;
  int stillRunning;

  struct curl_httppost *htmlFormPost   = NULL;
  struct curl_httppost *ptrLast         = NULL;
  struct curl_slist     *headerList     = NULL;

  static const char buf[]= "Expect:";

  curl_formadd(  &htmlFormPost
                  , &ptrLast
                  , CURLFORM_COPYNAME, "person"
                  , CURLFORM_COPYCONTENTS, "TestUpload"
                  , CURLFORM_END);
  /* Fill in the file upload field. This makes libcurl load data from
     the given file name when curl_easy_perform() is called. */
  curl_formadd( &htmlFormPost,
                   &ptrLast,
                   CURLFORM_COPYNAME, "file",
                   CURLFORM_FILE, strUpOrDownloadFilePath.c_str(),
                   CURLFORM_END);

  curl_formadd( &htmlFormPost,
                   &ptrLast,
                   CURLFORM_COPYNAME, "submit",
                   CURLFORM_COPYCONTENTS, "send",
                   CURLFORM_END);

  ptrCurl = curl_easy_init();
  ptrMultiHandle = curl_multi_init();

  headerList = curl_slist_append(headerList, buf);
  if(ptrCurl && ptrMultiHandle)
  {
    ptrReadFile = fopen(strUpOrDownloadFilePath.c_str(), "r");

    curl_easy_setopt(ptrCurl, CURLOPT_URL, strUrl);
    curl_easy_setopt(ptrCurl, CURLOPT_READDATA, ptrReadFile);
    //curl_easy_setopt(ptrCurl, CURLOPT_WRITEFUNCTION, my_write_func);
    curl_easy_setopt(ptrCurl, CURLOPT_READFUNCTION, ProgressBar_Upload::file_read_func);

    curl_easy_setopt(ptrCurl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(ptrCurl, CURLOPT_HTTPHEADER, headerList);
    curl_easy_setopt(ptrCurl, CURLOPT_HTTPPOST, htmlFormPost);

    curl_easy_setopt(ptrCurl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(ptrCurl, CURLOPT_PROGRESSFUNCTION, ProgressBar_Upload::ProgressControlFunc);
    curl_easy_setopt(ptrCurl, CURLOPT_PROGRESSDATA, ProgressBar_Base::m_ptrProgressBar);
    curl_multi_add_handle(ptrMultiHandle,ptrCurl);

    resResultCode = curl_multi_perform(ptrMultiHandle, &stillRunning);
    //resResultCode = curl_easy_perform(ptrCurl);
    do
    {
      struct timeval timeout;
      int rc; /* select() return code */

      fd_set fdread;
      fd_set fdwrite;
      fd_set fdexcep;
      int maxfd = -1;

      long curl_timeo = -1;

      FD_ZERO(&fdread);
      FD_ZERO(&fdwrite);
      FD_ZERO(&fdexcep);

      /* set a suitable timeout to play around with */
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;

      curl_multi_timeout(ptrMultiHandle, &curl_timeo);
      if(curl_timeo >= 0)
      {
        timeout.tv_sec = curl_timeo / 1000;
        if(timeout.tv_sec > 1)
          timeout.tv_sec = 1;
        else
          timeout.tv_usec = (curl_timeo % 1000) * 1000;
      }

      /* get file descriptors from the transfers */
      curl_multi_fdset(ptrMultiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);

      /* In a real-world program you OF COURSE check the return code of the
         function calls.  On success, the value of maxfd is guaranteed to be
         greater or equal than -1.  We call select(maxfd + 1, ...), specially in
         case of (maxfd == -1), we call select(0, ...), which is basically equal
         to sleep. */

      rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);

      switch(rc) {
      case -1:
        /* select error */
        break;
      case 0:
      default:
        /* timeout or readable/writable sockets */
        printf("perform!\n");
        curl_multi_perform(ptrMultiHandle, &stillRunning);
        printf("running: %d!\n", stillRunning);
        break;
      }
    } while(stillRunning);

    fclose(ptrReadFile);

    curl_multi_cleanup(ptrMultiHandle);

    /* always cleanup */
    curl_easy_cleanup(ptrCurl);

    /* then cleanup the formpost chain */
    curl_formfree(htmlFormPost);

    /* free slist */
    curl_slist_free_all (headerList);


  }
    ProgressBar_Base::GtkMainLoopQuit();

    return NULL;
}

