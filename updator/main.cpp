#include<stdio.h>
#include <curl/curl.h>
#include <sstream>
#include <fstream>
#include <string>
 #include <memory>
#include "7zFile.h"
#include "LzmaDec.h"
#include "7ZItf.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/writer.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "../common/comm.h"
CURLcode Download(const char * url,const char * filename);
bool LoadConfig();
void WriteConfig(int32_t version);
 
EgoType Type;
int32_t Version;
std::string Url;
std::string getpath()
{
	  char exec_name [BUFSIZ];
	 readlink ("/proc/self/exe", exec_name, BUFSIZ);
	 int32_t len=strlen(exec_name);
     for(int i=len;i>=0;i--)
     {
         if(exec_name[i]=='/')
           {
               exec_name[i]=0;
               break;
           }
     }
	 return std::string(exec_name);
}
int main()
{
    int32_t version=-1;
    if(!LoadConfig()) return 0;
    char szurl[256];
    sprintf(szurl,"%s/iego.json",Url.c_str());
    
    if(Download(szurl,"iego.json")==CURLcode::CURLE_OK)
    {
        Json::Value root;
        Json::Reader jsonReader;
        std::ifstream ifile("./iego.json");
        if(jsonReader.parse(ifile,root))
        {
            
            version=root["car_ver"].asInt();
        }
        if(Version<version)
        {
          std::string file=root["car_file"].asString();

          char szurl[256];
          sprintf(szurl,"%s/%s",Url.c_str(),file.c_str());
          Download(szurl,file.c_str());
	      Decompress(file.c_str());
		  WriteConfig(version);
        
           
        }
		system("./EgoController");

    }

    printf("hello,world");
    return 0;
}
size_t DownloadCallback(void * pBuffer,size_t nSize,size_t nMemBytes,FILE * fp);
CURLcode Download(const char * url,const char * filename)
{
     FILE * fp=fopen(filename,"wb+");
     CURL* curl = curl_easy_init();
     
    if(curl == NULL)
    {
        //抛出错误异常
        throw std::runtime_error("create easy_handle fail");
    }
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &DownloadCallback);
    curl_easy_setopt(curl,CURLOPT_WRITEDATA,fp);
    curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);

    //开始进行请求
    CURLcode retCode=curl_easy_perform(curl);

    //清理curl_easy
    curl_easy_cleanup(curl);
    fclose(fp);
    return retCode;

    
}
bool  LoadConfig()
{
    Json::Value root;
    Json::Reader jsonReader;
    std::ifstream ifile("./ego.json");
    if(jsonReader.parse(ifile,root))
    {
        Url=root["url"].asString();
        auto str=root["type"].asString();
        
        Type=EgoType::Car;
        Version=root["version"].asInt();
        return true;

    }
    return false;
}
void WriteConfig(int32_t version)
{
	Json::Value root;
    Json::Reader jsonReader;
    std::ifstream ifile("./ego.json");
    if(jsonReader.parse(ifile,root))
	{
		root["version"]=version;
		Json::StreamWriterBuilder b;
		std::unique_ptr<Json::StreamWriter> sw(b.newStreamWriter());
		//sw.Write(root);
		std::ofstream ofile;
		ofile.open("./ego.json",std::ios::out);
		sw->write(root,&ofile);
		ofile.close();

	}
}
size_t DownloadCallback(void * pBuffer,size_t nSize,size_t nMemBytes,FILE * fp)
{
  
   
    size_t nWrite=fwrite(pBuffer,nSize,nMemBytes,fp);
  
    return nWrite;
}
