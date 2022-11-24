//甲州街道モデル
//衝突履歴実装ver
//壁に衝突する場合は反射係数eに依存，エージェント同士の衝突は衝突後，右方向に走って急ぐ想定
//関数でmain文簡略化

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <random>
#include <math.h>
#include <vector>
#include <time.h>
#include <iostream>
#include <chrono>
#define SIZE_X 800
#define SIZE_Y 400
#define MAX 1024
#define AGENT_N 50
using namespace std::chrono;
std::random_device rd;
std::mt19937 mt(rd());     //  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
std::default_random_engine eng(rd());

//乱数発生器 整数
int int_random(int a, int b) {      //aからbまでの乱数発生(0を除外する)
    std::uniform_int_distribution<> rand(a, b);        // [a,b] 範囲の一様乱数
    return rand(mt);
}

//乱数発生器 小数
float float_random(int min, int max) {
    std::uniform_real_distribution<> rand(min, max);
    return rand(eng);
}

class agent {
public:
    float e = -0.5; //反射係数
    float box;      //一時保存の箱

    //点(円)の初期座標設定
    float x = float_random(0, 20);
    float y = float_random(0, SIZE_Y);       //注：y座標は上から0っぽい y=0なら天井,y=500なら床

    //点の半径
    int r = 1;
    int eraser = r + 1;             //黒色の点(円)で上書き用(境界が残るため1サイズ大きめ)

    //x,y方向への速度
    float v = 1;
    float vx = float_random(1, v);
    float vy = float_random(-v, v);

    //保存用
    std::vector<int> another_agent;
    std::vector<unsigned long long int> ms;

    //新たに点(円)を赤色で描写
    void paint(cv::Mat img) {
        if (x < SIZE_X - r) {
            cv::circle(img, cv::Point(x, y), r, cv::Scalar(0, 0, 255), -1, cv::LINE_AA);
        }
    }

    //位置の更新
    void update(cv::Mat img) {
        cv::circle(img, cv::Point(x, y), eraser, cv::Scalar(0, 0, 0), -1, cv::LINE_AA); //描写した点（円）を消去(黒で塗りつぶし)する関数

        x += vx; //vxの速度でx軸方向に進む
        y += vy; //vyの速度でy軸方向に進む

        if (x <= r) {    // 左の壁に当たる場合
            vx = vx * e;
        }
        if (y <= r || y >= SIZE_Y - r) {    // 上下の壁に当たる場合
            vy = vy * e;
        }

        //壁に埋まらないように位置調整
        if (x < r) {
            x = r;
        }
        if (y < r) {
            y = r;
        }
        else if (y > SIZE_Y - r) {
            y = SIZE_Y - r;
        }
    }

    //エージェント同士の距離を返す
    float dist(agent& hoge) {
        float d_x = x - hoge.x;
        float d_y = y - hoge.y;
        float D = sqrt(d_x * d_x + d_y * d_y); //任意の二つのエージェント間の距離
        return D;
    }

    //エージェント同士が衝突した際  (弾性衝突) 
    void collision(cv::Mat img, agent& hoge, int i, int j, unsigned long long int ms0) {

        another_agent.push_back(j);      //自分に相手のIDを格納
        hoge.another_agent.push_back(i); //相手に自分のIDを格納

        uint64_t ms1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count(); //リアルタイムのエポックミリ秒
        ms1 -= ms0; //計測処理開始からの所要時間
        ms.push_back(ms1);     //自分に所要時間ミリ秒を格納
        hoge.ms.push_back(ms1);//相手に所要時間ミリ秒を格納

        //2つのエージェントを緑で光らせる
        cv::circle(img, cv::Point(x, y), hoge.r, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);
        cv::circle(img, cv::Point(hoge.x, hoge.y), hoge.r, cv::Scalar(0, 255, 0), -1, cv::LINE_AA);

        float box;
        box = vx;
        vx = hoge.vx;  //x方向への速度が入れ替わる
        hoge.vx = box;

        box = vy;
        vy = hoge.vy;  //y方向への速度が入れ替わる 
        hoge.vy = box;

        vx += float_random(5, 10);
        hoge.vx += float_random(5, 10);

    }

};

void flow(cv::Mat img, int count, int* N, agent hoge[]) {
    cv::namedWindow("Example", cv::WINDOW_AUTOSIZE);  //Window作成(forの中から出した)
    uint64_t ms0 = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();  //処理開始時点でのエポックミリ秒
    while (true) {
        count++;
        for (int i = 0; i < *N; i++) {
            hoge[i].paint(img);      //点(円)を任意の数だけ描写
        }
        for (int i = 0; i < *N; i++) {
            for (int j = i + 1; j < *N; j++) {
                if (hoge[i].dist(hoge[j]) <= hoge[i].r * 2) { //エージェントが衝突した場合(二つのエージェント間の距離が直径以下)
                    hoge[i].collision(img, hoge[j], i, j, ms0);  //弾性衝突
                }
            }
        }
        cv::imshow("Example", img);                      //Windowを表示(更新)
        cv::waitKey(100);                                //点(円)を一定時間の間だけ表示して待機
        for (int i = 0; i < *N; i++) {
            hoge[i].update(img);                       //点(円)の位置を更新
        }
        if (count > 200) { //終了条件
            break;
        }
        if (int_random(1, 10) == 1) {      // 1/10の確率でエージェント発生
            *N += int_random(1, AGENT_N);        // 1〜AGENT_Nの乱数でエージェント生成
        }
    }
}

void show_agent(int* N, agent hoge[]) {
    for (int i = 0; i < *N; i++) {
        for (int j = 0; j < hoge[i].another_agent.size(); j++) {  //エージェント衝突履歴一覧の表示
            printf("メインID:%d, 衝突相手のID:%d, 経過時間:%llu [ms]\n", i, hoge[i].another_agent[j], hoge[i].ms[j]);
        }
    }
}

int main(void) {
    cv::Mat img = cv::imread("back3.png", -1);        // 画像の読み込み
    int count = 0;            //while文のカウンタ
    int N = int_random(1, AGENT_N);    // 1〜AGENT_Nの乱数で最初のエージェントの数を決定
    agent person[MAX];

    flow(img, count, &N, person); //一定時間の間，エージェント発生,移動
    show_agent(&N, person);  //衝突履歴を表示

    return 0;
}
