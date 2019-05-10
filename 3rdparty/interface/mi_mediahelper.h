//---------------------------------------------------------------------------
#ifndef MI_ZAPMEDIAHELPER_H
#define MI_ZAPMEDIAHELPER_H
//---------------------------------------------------------------------------
#define ModName_Mediahelper  "zap_mediahelper.dll"
//---------------------------------------------------------------------------
//int返回值说明
#define		MY_ERROR		-1			//函数错误
#define		MY_GENERAL		0			//非高清
#define		MY_HIGH			1			//高清
//---------------------------------------------------------------------------
typedef struct _MetaInfo
{
	int duration;			//时长
	int bitrate;			//码率
	int fps;				//帧率
	int width;				//宽
	int height;				//高 分辨率为：width*height
	char vcodec[64];		//视频流编码
}MetaInfo;
//---------------------------------------------------------------------------
struct izapmediahelper
{
	virtual bool	__cdecl MakeViedoFirstPhoto		(const char * vViedoPath, const char * vPhotoPath)	= 0;
	virtual int		__cdecl GetQuality				(const char * vViedoPath)							= 0;
	virtual void	__cdecl GetMediaMetaInfo		(const char * vVideoPath,MetaInfo* vMetaInfo)		= 0; 
};
//---------------------------------------------------------------------------
#endif 
//---------------------------------------------------------------------------
