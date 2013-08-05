#include <gtk/gtk.h>
#include <curl/curl.h>
#include <vector>
#include <iostream>

using namespace std;

class MyProgressBar
{

public:
	MyProgressBar(void) :
	    myProgressBarWin(NULL)
	  , sUrl("")
	{
        gtk_init(0,0);
        gdk_threads_init();
	}

	MyProgressBar(int argc, char *argv[]) :
	    myProgressBarWin(NULL)
	  , sUrl("")
	{
	    gtk_init(&argc,&argv);
	    gdk_threads_init();
	}

	~MyProgressBar(void){}

private:
	GtkWidget 		*myProgressBarWin, *myFrame1, *myFrame2;
	GtkAdjustment 	*adj;
	string 		sUrl;

public:
    static GtkWidget 		*myProgressBar;

public:
	void CreateProgressBar()
	{
		myProgressBarWin = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	      myFrame1 = gtk_frame_new(NULL);
	  	gtk_frame_set_shadow_type(GTK_FRAME(myFrame1), GTK_SHADOW_OUT);
		gtk_container_add(GTK_CONTAINER(myProgressBarWin), myFrame1);

		myFrame2 = gtk_frame_new(NULL);
		gtk_frame_set_shadow_type(GTK_FRAME(myFrame2), GTK_SHADOW_IN);
		gtk_container_add(GTK_CONTAINER(myFrame1), myFrame2);

		gtk_container_set_border_width(GTK_CONTAINER(myFrame2), 5);

		adj = (GtkAdjustment*)gtk_adjustment_new(0, 0, 100, 0, 0, 0);

		myProgressBar = gtk_progress_bar_new_with_adjustment(adj);

		gtk_container_add(GTK_CONTAINER(myFrame2), myProgressBar);

		gtk_widget_show_all(myProgressBarWin);
	}

	void SetUrl(string sUrl)
	{
        	this->sUrl = sUrl;
	}

	void CreateCurlThrd(void *(*curlFunc)(void *))
	{
		if (!g_thread_create(curlFunc, (void *)sUrl.c_str(), FALSE, NULL) != 0)
    			g_warning("can't create the thread");
	}


	//这个函数是为了符合CURLOPT_PROGRESSFUNCTION而构造的
	//显示文件传输进度，t代表文件大小，d代表传输已经完成部分
	static int my_progress_func(  GtkWidget *bar,
					     double t, /* dltotal */
					     double d, /* dlnow */
					     double ultotal,
					     double ulnow)
	{
	  printf("%d / %d (%g %%)\n", d, t, d*100.0/t);
	  gdk_threads_enter();
	  gtk_progress_set_value(GTK_PROGRESS(bar), d*100.0/t);
	  gdk_threads_leave();
	  return 0;
	}

};


GtkWidget *MyProgressBar::myProgressBar = NULL;

////这个函数是为了符合CURLOPT_WRITEFUNCTION而构造的
//完成数据保存功能
size_t my_write_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return fwrite(ptr, size, nmemb, stream);
}
//这个函数是为了符合CURLOPT_READFUNCTION而构造的
//数据上传时使用
size_t my_read_func(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
  return fread(ptr, size, nmemb, stream);
}

void *my_thread(void *ptr)
{
  CURL *curl;
  CURLcode res;
  FILE *outfile;
  gchar *url = (gchar *)ptr;

  curl = curl_easy_init();
  if(curl)
  {
    outfile = fopen("test.curl", "w");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, outfile);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, my_read_func);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, MyProgressBar::my_progress_func);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, MyProgressBar::myProgressBar);

    res = curl_easy_perform(curl);

    fclose(outfile);
    /* always cleanup */
    curl_easy_cleanup(curl);
  }

  return NULL;
}


int main(int argc, char *argv[])
{
	string strUrl = argv[1];

	cout << strUrl << endl;

	MyProgressBar mMyProgressBar;

    	cout << "Create Bar!" << endl;

	mMyProgressBar.CreateProgressBar();

	cout << "Done!" << endl;

	cout << "Set Url!" << endl;

	mMyProgressBar.SetUrl(strUrl);

   	cout << "Done!" << endl;

    	cout << "Create Process Thread!" << endl;

	mMyProgressBar.CreateCurlThrd(&my_thread);

	cout << "All Done!" << endl;

	gtk_main();

	return 0;
}

