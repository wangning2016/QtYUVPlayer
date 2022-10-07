#include "cyuvplay.h"
#include <QPalette>
#include <QWidget>
#include <QDebug>
#include <QFile>

int refreshVideo(void* data)
{
    CYUVPlay* pThis = (CYUVPlay*)data;
    if(pThis)
        pThis->PlayVideo();

    return 0;
}

CYUVPlay::CYUVPlay(QObject *parent) : QObject(parent)
{
    m_mode = SHOW_YUV;
    m_status = UnInit;
    m_bStop = false;
    m_bPause = false;
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        qInfo("SDL init err");
        return;
    }
    m_renderWidget = nullptr;
    m_window = nullptr;
    m_render = nullptr;
    m_texture= nullptr;
    m_thread = nullptr;
    m_status = Stoped;
}

CYUVPlay::~CYUVPlay()
{
    Stop();
    if(m_render)
    {
        SDL_DestroyRenderer(m_render);
        m_render = nullptr;
    }
    if(m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
}


bool CYUVPlay::Preview(QWidget* widget)
{
   SDL_Window* win =  SDL_CreateWindowFrom((void *)widget->winId());
   if(win == NULL)
       return false;
   SDL_Renderer* render = SDL_CreateRenderer(win,-1,SDL_RENDERER_SOFTWARE);
   if(render == NULL)
       return false;

   SDL_Texture* texture = SDL_CreateTexture(render,m_param.avPixformat,SDL_TEXTUREACCESS_STREAMING
                                         ,m_param.widht,m_param.height);
   if(texture == NULL)
       return false;

   FILE* fd = fopen(m_filePath.toStdString().c_str(),"rb");
   if(fd == NULL)
       return false;

   int maxBuffsize = 10*4096*4096*3;
   char* videoBuff = new char[maxBuffsize];
   if(videoBuff == NULL)
   {
       fclose(fd);
       return false;
   }
   int videoBuffLen = 0;
   SDL_Rect rect;
   while (1)
   {
       int yuvLen = GetYUVLen(m_param.widht,m_param.height,m_param.avPixformat);
       if(yuvLen == 0)
           break;

       videoBuffLen = fread(videoBuff,1,yuvLen,fd);
       if(videoBuffLen <= 0)
       {
           break;
       }
       int rowPixel = GetRowPixel(m_param.widht,m_param.height,m_param.avPixformat);
       if(rowPixel == 0)
       {
           break;
       }
       SDL_SetWindowSize(win,widget->width(),widget->height());
       SDL_UpdateTexture(texture,NULL,videoBuff,rowPixel);
       rect.x = 0;
       rect.y = 0;
       rect.w = widget->width();
       rect.h = widget->height();
       SDL_RenderClear(render);
       SDL_RenderCopy(render,texture,NULL,&rect);
       SDL_RenderPresent(render);

       break;
   }
   SDL_DestroyRenderer(render);
   SDL_DestroyTexture(texture);
   fclose(fd);
   delete []videoBuff;
   return true;
}

bool CYUVPlay::Play()
{
    Stop();
    if(m_filePath.isEmpty() || m_window == nullptr || m_render == nullptr)
    {
        return false;
    }
    if(m_thread == nullptr)
    {
        m_thread = SDL_CreateThread(refreshVideo,NULL,this);
        m_bStop = false;
    }
    return true;
}

bool CYUVPlay::Stop()
{
    m_bStop = true;
    if(m_thread)
    {
        SDL_WaitThread(m_thread,NULL);
    }
    m_thread = nullptr;
    return true;
}

bool CYUVPlay::Pause(bool bPuase)
{
    m_bPause = bPuase;
    return true;
}

bool CYUVPlay::isPause()
{
    return m_bPause;
}

bool CYUVPlay::SetRenderWidget(QWidget *pWidget)
{
    m_renderWidget = pWidget;
    if(m_window == nullptr)
    {
        m_window = SDL_CreateWindowFrom((void *)pWidget->winId());
    }
    if(m_window == nullptr)
        return false;

    if(m_render == nullptr)
    {
        m_render = SDL_CreateRenderer(m_window,-1,SDL_RENDERER_SOFTWARE);
    }
    if(m_render == nullptr)
    {
        qInfo("SDL_CreateRender error=%s",SDL_GetError());
        return false;
    }
    if(m_param.avPixformat > 0 && m_param.height > 0 && m_param.widht > 0)
    {
        if(m_texture)
        {
            SDL_DestroyTexture(m_texture);
            m_texture = nullptr;
        }
        m_texture = SDL_CreateTexture(m_render,m_param.avPixformat,SDL_TEXTUREACCESS_STREAMING
                                      ,m_param.widht,m_param.height);
    }
    return true;
}


void CYUVPlay::SetFilePath(QString filePath)
{
    m_filePath = filePath;
}

void CYUVPlay::SetParmam(PlayParam param)
{
    if(param.height <=0 || param.height > 4096 || param.widht <=0 || param.widht > 4096)
        return;

    m_param = param;
    if(m_renderWidget)
    {
        SetRenderWidget(m_renderWidget);
    }

}

void CYUVPlay::SetPlayMode(int mode)
{
    m_mode = mode;
}

void CYUVPlay::PlayVideo()
{
    if(m_param.frameRate <=0 || m_param.height <= 0|| m_param.widht <= 0)
        return;

    if(m_render == NULL || m_texture == NULL)
        return;

    FILE* fd = fopen(m_filePath.toStdString().c_str(),"rb");
    if(fd == NULL)
        return;
    int maxBuffsize = 4096*4096*3;
    char* videoBuff = new char[maxBuffsize];
    if(videoBuff == NULL)
    {
        fclose(fd);
        return;
    }
    int videoBuffLen = 0;
    SDL_Rect rect;
    int waitTime = (1000/(float)m_param.frameRate);
    while (!m_bStop)
    {
        if(m_bPause)
        {
            SDL_Delay(waitTime);
            continue;
        }
        int yuvLen = GetYUVLen(m_param.widht,m_param.height,m_param.avPixformat);
        if(yuvLen == 0)
            break;

        videoBuffLen = fread(videoBuff,1,yuvLen,fd);
        if(videoBuffLen <= 0)
        {
            break;
        }
        int rowPixel = GetRowPixel(m_param.widht,m_param.height,m_param.avPixformat);
        if(rowPixel == 0)
        {
            break;
        }
        SDL_SetWindowSize(m_window,m_renderWidget->width(),m_renderWidget->height());
        SDL_UpdateTexture(m_texture,NULL,videoBuff,rowPixel);
        rect.x = 0;
        rect.y = 0;
        rect.w = m_renderWidget->width();
        rect.h = m_renderWidget->height();
        SDL_RenderClear(m_render);
        SDL_RenderCopy(m_render,m_texture,NULL,&rect);
        SDL_RenderPresent(m_render);
        SDL_Delay(waitTime);
    }
    m_bStop = true;
    fclose(fd);
    delete []videoBuff;
}

int CYUVPlay::GetYUVLen(int width, int height, int format)
{
    if(width <= 0 || height <=0)
        return 0;
    int yuvLen = 0;
    if(format == SDL_PIXELFORMAT_IYUV)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_RGB24)
    {
        yuvLen = width*height*3;
    }
    else if(format == SDL_PIXELFORMAT_BGR24)
    {
        yuvLen = width*height*3;
    }
    else if(format == SDL_PIXELFORMAT_RGBA32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_ARGB32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_BGRA32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_ABGR32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_YV12)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_YUY2)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_UYVY)
    {
        yuvLen = width*height*2;
    }
    else if(format == SDL_PIXELFORMAT_YVYU)
    {
        yuvLen = width*height*2;
    }
    else if(format == SDL_PIXELFORMAT_NV12)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_NV21)
    {
        yuvLen = width*height + width*height/2;
    }
    return yuvLen;
}

int CYUVPlay::GetRowPixel(int width, int height, int format)
{
    int rowPixel = 0;
    if(format == SDL_PIXELFORMAT_IYUV)
    {
        return width;
    }
    else if(format == SDL_PIXELFORMAT_RGB24 || format == SDL_PIXELFORMAT_BGR24)
    {
        rowPixel = width*3;
    }
    else if(format == SDL_PIXELFORMAT_RGBA32 || format == SDL_PIXELFORMAT_ARGB32 || format == SDL_PIXELFORMAT_BGRA32 || format == SDL_PIXELFORMAT_ABGR32)
    {
        rowPixel = width*4;
    }
    else if(format == SDL_PIXELFORMAT_YV12)
    {
        return width;
    }
    else if(format == SDL_PIXELFORMAT_YUY2)
    {
        rowPixel = width + width/2;
    }
    else if(format == SDL_PIXELFORMAT_UYVY)
    {
        rowPixel = width*2;
    }
    else if(format == SDL_PIXELFORMAT_YVYU)
    {
        rowPixel = width*2;
    }
    else if(format == SDL_PIXELFORMAT_NV12)
    {
        rowPixel = width*2;
    }
    else if(format == SDL_PIXELFORMAT_NV21)
    {
        rowPixel = width*2;
    }
    return rowPixel;
}

