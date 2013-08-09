#pragma once
#include <gtk/gtk.h>
#include <gtk/gtkprogressbar.h>
#include <curl/curl.h>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

#define _FPTR(x) void *(*x)(void *)

class ProgressBar_Base
{

public:
	ProgressBar_Base(void) :
        m_ptrProgressMainFrame(NULL),
        m_ptrProgressBarFrame(NULL),
        m_ptrAdjustment(NULL),
	    m_strUrl("")
	{
        gtk_init(0,0);
        gdk_threads_init();
	}

	~ProgressBar_Base(void)
	{
	    m_ptrProgressBar = NULL;
	    m_ptrProgressBarWindow = NULL;
	    m_ptrProgressMainFrame = NULL;
	    m_ptrProgressBarFrame = NULL;
	    m_ptrAdjustment = NULL;
    }

private:
	GtkWidget            *m_ptrProgressMainFrame, *m_ptrProgressBarFrame;
	GtkAdjustment       *m_ptrAdjustment;
	string 		        m_strUrl;

public:
    static GtkWidget 	*m_ptrProgressBar;

	static GtkWidget    *m_ptrProgressBarWindow;
public:

	void CreateProgressBar()
	{
	    //创建主窗口
		m_ptrProgressBarWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        //创建对话框主框架，并插入主窗口中。
        m_ptrProgressMainFrame = gtk_frame_new(NULL);
	  	gtk_frame_set_shadow_type(GTK_FRAME(m_ptrProgressMainFrame), GTK_SHADOW_OUT);
		gtk_container_add(GTK_CONTAINER(m_ptrProgressBarWindow), m_ptrProgressMainFrame);

        //创建进度条框架，并插入主框架中
		m_ptrProgressBarFrame = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(m_ptrProgressBarFrame), GTK_SHADOW_IN);
		gtk_container_add(GTK_CONTAINER(m_ptrProgressMainFrame), m_ptrProgressBarFrame);

        //设置进度条框架长度
		gtk_container_set_border_width(GTK_CONTAINER(m_ptrProgressBarFrame), 5);

        //设置进度条控制范围
		m_ptrAdjustment = (GtkAdjustment*)gtk_adjustment_new(0, 0, 100, 0, 0, 0);
		m_ptrProgressBar = gtk_progress_bar_new_with_adjustment(m_ptrAdjustment);

		gtk_container_add(GTK_CONTAINER(m_ptrProgressBarFrame), m_ptrProgressBar);

        //设置主窗口始终居中
        gtk_window_set_position(GTK_WINDOW(m_ptrProgressBarWindow), GTK_WIN_POS_CENTER_ALWAYS);

        //显示进度条主窗口
		gtk_widget_show_all(m_ptrProgressBarWindow);
	}

    //
    //
	void SetUrl(string m_strUrl)
	{
	    //设置链接
        this->m_strUrl = m_strUrl;
	}

    //
    //
	void CreateCurlThrd(_FPTR(curlFunc))
	{
		if (!g_thread_create(curlFunc, (void *)m_strUrl.c_str(), FALSE, NULL) != 0)
    			g_warning("can't create the progress thread!");
	}

    //
    //
	void GtkMainLoop()
	{
        //进度条主窗口进行循环
        //当调用gtk_main_quit()时将退出循环
	    gtk_main();
	}
    //
    //
    static void GtkMainLoopQuit()
    {
        //退出窗口循环
        gtk_widget_hide(m_ptrProgressBarWindow);
    }

};

class ProgressBar_Upload :
    public ProgressBar_Base
{
public:
    ProgressBar_Upload(void){}
    ~ProgressBar_Upload(void){}

public:

	//这个函数是为了符合CURLOPT_PROGRESSFUNCTION而构造的
	//显示文件传输进度，t代表文件大小，d代表传输已经完成部分
	static int ProgressControlFunc(  GtkWidget *ptrProgressBar,
                                         double     download_total,
                                         double     download_now,
                                         double     upload_total,
                                         double     upload_now)
	{
        //printf("%d / %d (%g %%)\n", upload_now, upload_total, upload_now*100.0/upload_total);
        gdk_threads_enter();
        //GTK_FRAME(ptrProgressbar);
        gtk_progress_bar_set_fraction((GtkProgressBar*)ptrProgressBar, upload_now/upload_total);
        gdk_threads_leave();
        return 0;
	}

    //这个函数是为了符合CURLOPT_READFUNCTION而构造的
    //数据上传时使用
    static size_t file_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        return fread(ptr, size, nmemb, stream);
    }
};

class ProgressBar_Download :
    public ProgressBar_Base
{
public:
    ProgressBar_Download(void){}
    ~ProgressBar_Download(void){}

public:

    //这个函数是为了符合CURLOPT_PROGRESSFUNCTION而构造的
	//显示文件传输进度，t代表文件大小，d代表传输已经完成部分
	static int ProgressControlFunc(  GtkWidget *ptrProgressBar,
                                             double     download_total,
                                             double     download_now,
                                             double     upload_total,
                                             double     upload_now)
	{
        //printf("%d / %d (%g %%)\n", download_now, download_total, download_now*100.0/download_total);
        std::ofstream testDownloadLog("//root//result.txt", ios::app);

        testDownloadLog << "Get into ProgressControlFunc() ." <<endl;
        testDownloadLog << "download_now : " << download_now << endl;
        testDownloadLog << "download_total : " << download_total << endl;
        gdk_threads_enter();
        gtk_progress_bar_set_fraction((GtkProgressBar*)ptrProgressBar, download_now/download_total);
        gdk_threads_leave();
        return 0;
	}

	////这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
    //完成数据保存功能
    static size_t file_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
    {
        return fwrite(ptr, size, nmemb, stream);
    }
};

GtkWidget   *ProgressBar_Base::m_ptrProgressBar = NULL;
GtkWidget   *ProgressBar_Base::m_ptrProgressBarWindow = NULL;




