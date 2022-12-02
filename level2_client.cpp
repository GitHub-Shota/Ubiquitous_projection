#include <iostream>
#include <string>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>

// UDP通信用ヘッダ
// #include <sys/sock.h> // linux
#include <WinSock2.h>    // windows
#include <Windows.h>
#include <Windowsx.h>

// UDP通信用ライブラリ
#pragma comment(lib, "ws2_32.lib")
// inet_addr()関数で警告が出る場合は以下で警告を無効化
#pragma warning(disable:4996)

/*
// 右クリックの状態（押された状態:true, 離した状態:false）
bool is_clicked = false;

// グローバル座標
int old_x = 0, old_y = 0, new_x = 0, new_y = 0;

// パソコンサイズの黒画面
cv::Mat img(cv::Size(1920, 1080), CV_8UC3, cv::Scalar(0, 0, 0));

// 線を描写
void draw_line(int event, int x, int y, int flags, void* userdata)
{
    old_x = new_x;
    old_y = new_y;
    new_x = x;
    new_y = y;

    // 右クリック押す
    if (event == cv::EVENT_RBUTTONDOWN)
    {
        is_clicked = true;
    }

    // 右クリックされている状態に点を描写
    if (is_clicked == true)
    {
        cv::line(img, cv::Point(old_x, old_y), cv::Point(new_x, new_y), cv::Scalar(0, 255, 0), 10, cv::LINE_AA);
    }

    // 右クリック離す
    if (event == cv::EVENT_RBUTTONUP)
    {
        is_clicked = false;
    }
}
*/

int main()
{
    /*
    // 表示するウィンドウに名前を付ける
    cv::namedWindow("Level2_client", cv::WINDOW_NORMAL);
    // フルスクリーン表示
    cv::setWindowProperty("Level2_client", cv::WND_PROP_FULLSCREEN, cv::WINDOW_FULLSCREEN);

    // マウスイベントの登録
    cv::setMouseCallback("Level2_client", draw_line);

    // qが押されるまで表示を継続
    while (1)
    {
        cv::imshow("Level2_client", img);

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
    */

    // ソケット通信winsockの立ち上げ
    // wsaDataはエラー取得時に使用
    WSAData wsaData;
    // MAKEWORD(2, 0)はwinsockのバージョン2.0
    WSAStartup(MAKEWORD(2, 0), &wsaData);

    
    // socket作成 
    // socketを通してデータのやり取りを行う
    // socket(アドレスファミリ, ソケットタイプ, プロトコル)
    // AF_INETはIPv4、SOCK_DGRAMはUDP通信、0は？
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);

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

    // 受信用のバッファ
    char old_xx[256];
    char old_yy[256];
    char new_xx[256];
    char new_yy[256];

    // データ受信
    // recv(ソケット, 受信するデータの格納先, データのバイト数, フラグ);
    // バインドしていない場合は recvfrom(sock, buf, 5, 0, (struct sockaddr *)&addr, sizeof(addr)); でもOK？
    for (int i = 0; i < 100; i++) 
    {
        /*  
        std::string rdata = udp0.recieve();
        printf("recv:%s\n", rdata.c_str());
        */

        memset(old_xx, 0, 100);
        recv(sock, old_xx, 100, 0);
        printf("recv : %s \n", old_xx);

        memset(old_yy, 0, 100);
        recv(sock, old_yy, 100, 0);
        printf("recv : %s \n", old_yy);

        memset(new_xx, 0, 100);
        recv(sock, new_xx, 100, 0);
        printf("recv : %s \n", new_xx);

        memset(new_yy, 0, 100);
        recv(sock, new_yy, 100, 0);
        printf("recv : %s \n", new_yy);

        /*
        if (recv(sock, &buf, 1, 0) >= 0)
        {
            printf("recv : %c \n", buf);
        }
        */
    }

    // socketの破棄
    closesocket(sock);

    // winsockの終了
    WSACleanup();

    return 0;
}
