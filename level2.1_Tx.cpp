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

// 右クリックの状態（押された状態:true, 離した状態:false）
bool is_clicked = false;

// グローバル座標
int old_x = 0, old_y = 0, new_x = 0, new_y = 0;

// パソコンサイズの黒画面
cv::Mat img(cv::Size(1920, 1080), CV_8UC3, cv::Scalar(0, 0, 0));

// 線を描写
void draw_line(int event, int x, int y, int flags, void* userdata)
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
    addr.sin_port = htons(50008);  // 待ち受けポート番号設定
    addr.sin_addr.S_un.S_addr = inet_addr("192.168.11.12");  // 送信アドレスを設定

    // タイムアウト時間の設定
    // struct timeval tv;
    // tv.tv_sec = 0;
    // tv.tv_usec = 10;
    // if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv)) < 0) 
    // {
    //     perror("Error");
    // }

    // 今の座標と一つ前の座標を更新
    old_x = new_x;
    old_y = new_y;
    new_x = x;
    new_y = y;

    // 送信用のバッファ
    char old_xx[256];
    char old_yy[256];
    char new_xx[256];
    char new_yy[256];

    // 右クリック押す
    if (event == cv::EVENT_RBUTTONDOWN)
    {
        is_clicked = true;
    }

    // 右クリックされている状態に座標送信と点を描写
    if (is_clicked == true)
    {
        // 数値→文字列
        sprintf(old_xx, "%d", old_x);
        sprintf(old_yy, "%d", old_y);
        sprintf(new_xx, "%d", new_x);
        sprintf(new_yy, "%d", new_y);

        // データ送信
        // sendto(ソケット, 送信するデータ, データのバイト数, フラグ, アドレス情報, アドレス情報のサイズ);
        sendto(sock, old_xx, sizeof(old_xx), 0, (struct sockaddr*)&addr, sizeof(addr));
        sendto(sock, old_yy, sizeof(old_yy), 0, (struct sockaddr*)&addr, sizeof(addr));
        sendto(sock, new_xx, sizeof(new_xx), 0, (struct sockaddr*)&addr, sizeof(addr));
        sendto(sock, new_yy, sizeof(new_yy), 0, (struct sockaddr*)&addr, sizeof(addr));

        cv::line(img, cv::Point(old_x, old_y), cv::Point(new_x, new_y), cv::Scalar(0, 255, 0), 10, cv::LINE_AA);
    }

    // 右クリック離す
    if (event == cv::EVENT_RBUTTONUP)
    {
        is_clicked = false;
    }

    // socketの破棄
    closesocket(sock);

    // すべてのスレッドのwinsockを終了
    WSACleanup();
}


int main()
{

    // 表示するウィンドウに名前を付ける
    cv::namedWindow("Level2.1_Tx", cv::WINDOW_NORMAL);
    // フルスクリーン表示
    cv::setWindowProperty("Level2.1_Tx", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    // マウスイベントの登録
    cv::setMouseCallback("Level2.1_Tx", draw_line);

    // qが押されるまで表示を継続
    while (1)
    {
        cv::imshow("Level2.1_Tx", img);

        // キーイベント
        int key = cv::waitKey(1);
        // qキーが押されたときwhileループから抜ける(quit)
        if (key == 113)
        {
            break;
        }
        // rキーで全消し(reset)
        else if (key == 114)
        {
            cv::circle(img, cv::Point(new_x, new_y), 1920, cv::Scalar(0, 0, 0), -1, cv::LINE_AA);
        }
    }

    // ウィンドウを閉じる
    cv::destroyAllWindows();

    return 0;
}
