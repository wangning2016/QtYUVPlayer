#ifndef CYUVPLAY_H
#define CYUVPLAY_H
#include <QObject>
#include <QWidget>
#include "comdefine.h"
#include <SDL2/SDL.h>

class CYUVPlay : public QObject
{
    Q_OBJECT
public:
    enum PlayStatus
    {
        UnInit,
        Stoped,
        Playing,
        Paused
    };

    explicit CYUVPlay(QObject *parent = nullptr);
    ~CYUVPlay();


    bool Preview(QWidget* widget);
    bool Play();
    bool Stop();
    bool Pause(bool bPuase = true);
    bool isPause();

    bool SetRenderWidget(QWidget *pWidget);

    void SetFilePath(QString filePath);
    void SetParmam(PlayParam param);
    void SetPlayMode(int mode = SHOW_YUV);

public:
    void PlayVideo();
    int GetYUVLen(int width,int height,int format);
    int GetRowPixel(int width,int height,int format);

private:
    PlayParam m_param;
    QString m_filePath;
    int m_mode;
    PlayStatus m_status;
    bool m_bStop;
    bool m_bPause;

    QWidget* m_renderWidget;
    SDL_Window* m_window;
    SDL_Renderer* m_render;
    SDL_Texture* m_texture;
    SDL_Thread* m_thread;
};

#endif // CYUVPLAY_H
