#include <gtk/gtk.h>
#include <curl/curl.h>
#include <vector>
#include <iostream>

using namespace std;

#define _FPTR(x) void *(*x)(void *)

class MyProgressBar
{

public:
	MyProgressBar(void) :
	    myProgressBarWin(NULL),
	    sUrl("")
	{
        gtk_init(0,0);
        gdk_threads_init();
	}

	MyProgressBar(int argc, char *argv[]) :
	    myProgressBarWin(NULL),
	    sUrl("")
	{
	    gtk_init(&argc,&argv);
	    gdk_threads_init();
	}

	~MyProgressBar(void){}

private:
	GtkWidget            *myProgressBarWin;
	GtkWidget            *myFrame1, *myFrame2;
	GtkAdjustment       *adj;
	string 		        sUrl;

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

        gtk_window_set_position(GTK_WINDOW(myProgressBarWin), GTK_WIN_POS_CENTER_ALWAYS);

		gtk_widget_show_all(myProgressBarWin);
	}

	void SetUrl(string sUrl)
	{
        	this->sUrl = sUrl;
	}

	void CreateCurlThrd(_FPTR(curlFunc))
	{
		if (!g_thread_create(curlFunc, (void *)sUrl.c_str(), FALSE, NULL) != 0)
    			g_warning("can't create the progress thread!");
	}


	//这个函数是为了符合CURLOPT_PROGRESSFUNCTION而构造的
	//显示文件传输进度，t代表文件大小，d代表传输已经完成部分
	static int my_progress_func(  GtkWidget *bar,
					     double dltotal, /* dltotal */
					     double dlnow, /* dlnow */
					     double ultotal,
					     double ulnow)
	{
	  printf("%d / %d (%g %%)\n", ulnow, ultotal, ulnow*100.0/ultotal);
	  gdk_threads_enter();
	  gtk_progress_set_value(GTK_PROGRESS(bar), ulnow*100.0/ultotal);
	  if(100 <= ulnow*100.0/ultotal)
	  {
	      cout<< "Done!" <<endl;
	      GtkMainLoopQuit();
	  }
	  gdk_threads_leave();
	  return 0;
	}

	void GtkMainLoop()
	{
	    gtk_main();
	}

private:

    static void GtkMainLoopQuit()
    {
        gtk_main_quit();
    }

};


GtkWidget   *MyProgressBar::myProgressBar = NULL;
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
  CURLMcode res;
  FILE *outfile;
  gchar *url = (gchar *)ptr;


  CURLM *multi_handle;
  int still_running;

  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";

  curl_formadd(  &formpost
                  , &lastptr
                  , CURLFORM_COPYNAME, "person"
                  , CURLFORM_COPYCONTENTS, "LiuJun"
                  , CURLFORM_END);
  /* Fill in the file upload field. This makes libcurl load data from
     the given file name when curl_easy_perform() is called. */
  curl_formadd( &formpost,
                   &lastptr,
                   CURLFORM_COPYNAME, "file",
                   CURLFORM_FILE, "curl.docx",
                   CURLFORM_END);

  /* Fill in the filename field */
  /*curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename",
               CURLFORM_COPYCONTENTS, "postit2.c",
               CURLFORM_END);*/

  /* Fill in the submit field too, even if this is rarely needed */
  curl_formadd( &formpost,
                   &lastptr,
                   CURLFORM_COPYNAME, "submit",
                   CURLFORM_COPYCONTENTS, "send",
                   CURLFORM_END);

  curl = curl_easy_init();
  multi_handle = curl_multi_init();

  headerlist = curl_slist_append(headerlist, buf);

  if(curl && multi_handle)
  {
    outfile = fopen("curl.docx", "r");

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_READDATA, outfile);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_write_func);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, my_read_func);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, MyProgressBar::my_progress_func);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, MyProgressBar::myProgressBar);

    curl_multi_add_handle(multi_handle, curl);

    res = curl_multi_perform(multi_handle, &still_running);

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

      curl_multi_timeout(multi_handle, &curl_timeo);
      if(curl_timeo >= 0)
      {
        timeout.tv_sec = curl_timeo / 1000;
        if(timeout.tv_sec > 1)
          timeout.tv_sec = 1;
        else
          timeout.tv_usec = (curl_timeo % 1000) * 1000;
      }

      /* get file descriptors from the transfers */
      curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);

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
        curl_multi_perform(multi_handle, &still_running);
        printf("running: %d!\n", still_running);
        break;
      }
    } while(still_running);

    fclose(outfile);

    curl_multi_cleanup(multi_handle);

    /* always cleanup */
    curl_easy_cleanup(curl);

    /* then cleanup the formpost chain */
    curl_formfree(formpost);

    /* free slist */
    curl_slist_free_all (headerlist);


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

	mMyProgressBar.GtkMainLoop();

	return 0;
}


