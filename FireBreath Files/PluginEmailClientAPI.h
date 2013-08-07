/**********************************************************\

  Auto-generated PluginEmailClientAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "PluginEmailClient.h"

#ifndef H_PluginEmailClientAPI
#define H_PluginEmailClientAPI

class PluginEmailClientAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn PluginEmailClientAPI::PluginEmailClientAPI(const PluginEmailClientPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    PluginEmailClientAPI(const PluginEmailClientPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        registerMethod("echo",      make_method(this, &PluginEmailClientAPI::echo));
        registerMethod("testEvent", make_method(this, &PluginEmailClientAPI::testEvent));

        registerMethod("testMethod", make_method(this, &PluginEmailClientAPI::testMethod));

        registerMethod("chooseFile", make_method(this, &PluginEmailClientAPI::chooseFile));
        registerMethod("savePathSelect", make_method(this, &PluginEmailClientAPI::savePathSelect));

        registerMethod("testThreadNoneParam", make_method(this, &PluginEmailClientAPI::testThreadNoneParam));
        registerMethod("testThreadOneParam", make_method(this, &PluginEmailClientAPI::testThreadOneParam));
        registerMethod("testThreadMultiParam", make_method(this, &PluginEmailClientAPI::testThreadMultiParam));

        registerMethod("testSaveHtml", make_method(this, &PluginEmailClientAPI::testSaveHtml));

        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &PluginEmailClientAPI::get_testString,
                                       &PluginEmailClientAPI::set_testString));

        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &PluginEmailClientAPI::get_version));
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn PluginEmailClientAPI::~PluginEmailClientAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~PluginEmailClientAPI() {};

    PluginEmailClientPtr getPlugin();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);

    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));

    // Method test-event
    void testEvent();

    //
    //
    std::string testMethod(const std::string &sMessage);

    // Method choose file
    // std::string chooseFile();
    std::vector<std::string> chooseFile();

    //
    //
    int testThreadNoneParam();

    //
    //
    int testThreadOneParam();

    //
    //
    int testThreadMultiParam();

    //
    // Method save-path-select
    std::string savePathSelect();

    //
    //
    int NoneParamFunc();

    //
    //
    int OneParamFunc(int iParamOne);

    //
    //
    int MultiParamFunc(int iParamOne, int iParamTwo, int iParamThird);

    //
    //
    void testSaveHtml(const std::string& sUrl);

    //
    //
    void testCurlDownHtmlFunc(std::string sUrl, std::string strSaveFilePath);

    //
    //写文件函数
    static size_t MyWriteToFile(void *ptr, size_t size, size_t nmemb, void *stream);


private:
    PluginEmailClientWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;

    static FILE *pMyFile;
};

#endif // H_PluginEmailClientAPI

