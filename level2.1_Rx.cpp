#include <iostream>
#include <string>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

// UDP通信用ヘッダ
// #include <sys/sock.h> // linux
#include <WinSock2.h>    // windows
#include <Windows.h>
#include <Windowsx.h>

// Windows.hをインクルードしたい場合は、Windows.ｈよりもWinsock2.hを先にインクルード
// なんならWindows.ｈだけでも、Winsock2.hだけでも動く。

// UDP通信用ライブラリ
#pragma comment(lib, "ws2_32.lib")
// inet_addr()関数で警告が出る場合は以下で警告を無効化
#pragma warning(disable:4996)

// バッファのサイズ
#define BUFF_SIZE 8


int main()
{
    // ソケット通信winsockの立ち上げ
    // wsaDataはエラー取得時に使用
    WSAData wsaData;

    // MAKEWORD(2, 0)はwinsockのバージョン2.0
    int err = WSAStartup(MAKEWORD(2, 0), &wsaData);
    if (err != 0)
    {
        switch (err)
        {
        case WSASYSNOTREADY:
            std::cout << "WSASYSNOTREADY" << std::endl;
            break;
        case WSAVERNOTSUPPORTED:
            std::cout << "WSAVERNOTSUPPORTED" << std::endl;
            break;
        case WSAEINPROGRESS:
            std::cout << "WSAEINPROGRESS" << std::endl;
            break;
        case WSAEPROCLIM:
            std::cout << "WSAEPROCLIM" << std::endl;
            break;
        case WSAEFAULT:
            std::cout << "WSAEFAULT" << std::endl;
            break;
        }
    }

    // socket作成 
    // socketを通してデータのやり取りを行う
    // socket(アドレスファミリ, ソケットタイプ, プロトコル)
    // AF_INETはIPv4、SOCK_DGRAMはUDP通信
    // 0はプロトコルファミリーの種類ごとに一つのプロトコルのみをサポート
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == INVALID_SOCKET)
    {
        std::cout << "socket failed" << std::endl;
    }

    // アドレス等を格納
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;     // IPv4
    addr.sin_port = htons(50008);  // 通信ポート番号設定
    addr.sin_addr.S_un.S_addr = INADDR_ANY;  // INADDR_ANYはすべてのアドレスからのパケットを受信

    // バインド
    // アドレス等の情報をsocketに登録
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));

    // ブロッキング、ノンブロッキングの設定
    //（任意、必須コードではない（ソケットの初期設定はブロッキングモードなため）
    // val = 0 : ブロッキングモード データが受信されるまで待機
    // val = 1 : ノンブロッキング データが受信されなくても次の処理へ
    // u_long val = 1;
    // ioctlsocket(sock, FIONBIO, &val);

    // タイムアウト時間の設定
    // struct timeval tv;
    // tv.tv_sec = 0;
    // tv.tv_usec = 10;
    // if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) 
    // {
    //     perror("Error");
    // }

    // 座標
    int old_x, old_y, new_x, new_y;

    // パソコンサイズの黒画面
    cv::Mat img(cv::Size(1920, 1080), CV_8UC3, cv::Scalar(0, 0, 0));

    // 表示するウィンドウに名前を付ける
    cv::namedWindow("Level2.1_Rx", cv::WINDOW_NORMAL);

    // フルスクリーン表示
    // cv::setWindowProperty("Level2.1_Rx", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    while (1)
    {
        cv::imshow("Level2.1_Rx", img);

        // 受信用のバッファ
        char old_xx[BUFF_SIZE];
        char old_yy[BUFF_SIZE];
        char new_xx[BUFF_SIZE];
        char new_yy[BUFF_SIZE];

        // データ受信
        // recv(ソケット, 受信データ, データのバイト数, フラグ);
        // memset(メモリのポインタ, 値, サイズ);
        recv(sock, old_xx, BUFF_SIZE, 0);
        old_x = atoi(old_xx);

        recv(sock, old_yy, BUFF_SIZE, 0);
        old_y = atoi(old_yy);

        recv(sock, new_xx, BUFF_SIZE, 0);
        new_x = atoi(new_xx);

        recv(sock, new_yy, BUFF_SIZE, 0);
        new_y = atoi(new_yy);

        // 受信した座標を基に線を描写
        cv::line(img, cv::Point(old_x, old_y), cv::Point(new_x, new_y), cv::Scalar(0, 255, 0), 10, cv::LINE_AA);
    }

    // ウィンドウを閉じる
    cv::destroyAllWindows();

    // socketの破棄
    closesocket(sock);

    // すべてのスレッドのwinsockを終了
    WSACleanup();

    return 0;
}
