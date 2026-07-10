#include <switch.h>
#include <SDL2/SDL.h>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavformat/avio.h>
}

// Связываем программу с внутренними переменными твоего видеофайла
extern const uint8_t skibidi_mp4[];
extern const uint32_t skibidi_mp4_size;

// Структура для чтения памяти
struct BufferData {
    const uint8_t *ptr;
    size_t size;
};

// Функция передачи байтов видеофайла в декодер FFmpeg
int read_packet(void *opaque, uint8_t *buf, int buf_size) {
    struct BufferData *bd = (struct BufferData *)opaque;
    buf_size = FFMIN(buf_size, bd->size);
    if (!buf_size) return AVERROR_EOF;
    memcpy(buf, bd->ptr, buf_size);
    bd->ptr  += buf_size;
    bd->size -= buf_size;
    return buf_size;
}

int main(int argc, char** argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) return -1;
    
    SDL_Window* window = SDL_CreateWindow("Skibidi Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Подключаем наш массив данных из памяти
    struct BufferData bd = { skibidi_mp4, skibidi_mp4_size };
    size_t avio_ctx_buffer_size = 4096;
    uint8_t *avio_ctx_buffer = (uint8_t *)av_malloc(avio_ctx_buffer_size);
    
    AVFormatContext* pFormatCtx = avformat_alloc_context();
    AVIOContext *avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, &bd, &read_packet, NULL, NULL);
    pFormatCtx->pb = avio_ctx;

    // Открываем видеопоток из памяти устройства
    if (avformat_open_input(&pFormatCtx, NULL, NULL, NULL) != 0) goto cleanup;
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0) goto cleanup;

    int videoStream = -1;
    for (unsigned int i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
            break;
        }
    }
    if (videoStream == -1) goto cleanup;

    const AVCodec* pCodec = avcodec_find_decoder(pFormatCtx->streams[videoStream]->codecpar->codec_id);
    AVCodecContext* pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoStream]->codecpar);
    avcodec_open2(pCodecCtx, pCodec, NULL);

    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_YV12, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);

    AVPacket* packet = avpacket_alloc();
    AVFrame* pFrame = avframe_alloc();

    while (appletMainLoop()) {
        hidScanInput();
        if (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_PLUS) break; // Выход на Плюс (+)

        if (av_read_frame(pFormatCtx, packet) >= 0) {
            if (packet->stream_index == videoStream) {
                if (avcodec_send_packet(pCodecCtx, packet) == 0) {
                    while (avcodec_receive_frame(pCodecCtx, pFrame) == 0) {
                        SDL_UpdateYUVTexture(texture, NULL, 
                            pFrame->data, pFrame->linesize,
                            pFrame->data, pFrame->linesize,
                            pFrame->data, pFrame->linesize);

                        SDL_RenderClear(renderer);
                        SDL_RenderCopy(renderer, texture, NULL, NULL);
                        SDL_RenderPresent(renderer);
                        SDL_Delay(33); // Удерживаем фреймрейт ~30fps
                    }
                }
            }
            av_packet_unref(packet);
        } else {
            // Если видео доиграло до конца — сбрасываем указатели в начало памяти (зацикливаем)
            bd.ptr = skibidi_mp4;
            bd.size = skibidi_mp4_size;
            av_seek_frame(pFormatCtx, videoStream, 0, AVSEEK_FLAG_BACKWARD);
        }
    }

cleanup:
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

