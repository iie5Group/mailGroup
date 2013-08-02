#/**********************************************************\ 
#
# Auto-Generated Plugin Configuration file
# for MailPlugin
#
#\**********************************************************/

set(PLUGIN_NAME "hello")
set(PLUGIN_PREFIX "MPL")
set(COMPANY_NAME "xxx")

# ActiveX constants:
set(FBTYPELIB_NAME helloLib)
set(FBTYPELIB_DESC "hello 1.0 Type Library")
set(IFBControl_DESC "hello Control Interface")
set(FBControl_DESC "hello Control Class")
set(IFBComJavascriptObject_DESC "hello IComJavascriptObject Interface")
set(FBComJavascriptObject_DESC "hello ComJavascriptObject Class")
set(IFBComEventSource_DESC "hello IFBComEventSource Interface")
set(AXVERSION_NUM "1")

# NOTE: THESE GUIDS *MUST* BE UNIQUE TO YOUR PLUGIN/ACTIVEX CONTROL!  YES, ALL OF THEM!
set(FBTYPELIB_GUID 3e453cd4-1990-57b7-bfc2-fee9fbad80a2)
set(IFBControl_GUID d4c94e6b-c4d8-5458-965f-f0552192b79a)
set(FBControl_GUID 0b35a8eb-d456-530d-9dbf-0dc5fd251c50)
set(IFBComJavascriptObject_GUID 79f0c9f1-1319-5871-bbee-6d3c7205ca41)
set(FBComJavascriptObject_GUID bd0509b3-2129-5175-9bb9-8a0a85abf8ef)
set(IFBComEventSource_GUID 80227d07-9ad7-548a-bddc-00d8fc932939)
if ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID b0104aa9-a0ed-50ca-ac2c-0c97f1a745a2)
else ( FB_PLATFORM_ARCH_32 )
    set(FBControl_WixUpgradeCode_GUID 20850abb-d08f-5303-afdf-a2dfaaf31a11)
endif ( FB_PLATFORM_ARCH_32 )

# these are the pieces that are relevant to using it from Javascript
set(ACTIVEX_PROGID "xxx.hello")
set(MOZILLA_PLUGINID "xxx.com/hello")

# strings
set(FBSTRING_CompanyName "x")
set(FBSTRING_PluginDescription "mail plugin")
set(FBSTRING_PLUGIN_VERSION "1.0.0.0")
set(FBSTRING_LegalCopyright "Copyright 2013 x")
set(FBSTRING_PluginFileName "np${PLUGIN_NAME}.dll")
set(FBSTRING_ProductName "MailPlugin")
set(FBSTRING_FileExtents "")
if ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "MailPlugin")  # No 32bit postfix to maintain backward compatability.
else ( FB_PLATFORM_ARCH_32 )
    set(FBSTRING_PluginName "MailPlugin_${FB_PLATFORM_ARCH_NAME}")
endif ( FB_PLATFORM_ARCH_32 )
set(FBSTRING_MIMEType "application/x-hello")

# Uncomment this next line if you're not planning on your plugin doing
# any drawing:

#set (FB_GUI_DISABLED 1)

# Mac plugin settings. If your plugin does not draw, set these all to 0
set(FBMAC_USE_QUICKDRAW 0)
set(FBMAC_USE_CARBON 1)
set(FBMAC_USE_COCOA 1)
set(FBMAC_USE_COREGRAPHICS 1)
set(FBMAC_USE_COREANIMATION 0)
set(FBMAC_USE_INVALIDATINGCOREANIMATION 0)

# If you want to register per-machine on Windows, uncomment this line
#set (FB_ATLREG_MACHINEWIDE 1)
