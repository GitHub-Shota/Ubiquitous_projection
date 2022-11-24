#define _USE_MATH_DEFINES
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <chrono>
#include <thread>
#include "Random.hpp"

// エージェントの個数
const static int N = 3000;
// 仮想空間のサイズ
const static int SIZE = 300;
// 点の基準速度
const static double SPEED = 3.0;
// 点の半径
const static int R = 1;
// 角度
double radian;
// 座標
int sx, sy, ax, ay, ax_v, ay_v;

/* 名前空間 */s
namespace mynamespace
{
	// 方角
	enum dir
	{
		nw,  // 北西
		sw,  // 南西
		ne,  // 北東
		se,  // 南東
	};

	// 点の色
	enum color
	{
		blue,
		green,
		red,
		yellow,
	};

	/* エージェントクラス */
	class Agent
	{
	private:
		int ID;        // エージェントID
		int x;         // x座標
		int y;         // y座標
		int x_v;       // x方向の速度
		int y_v;       // y方向の速度
		int duration;  // 発生から消失までの期間
		dir dir;       // 最初にいた方角
		color color;   // エージェントの色の状態
		cv::Scalar bgr_color;  // エージェントの色
		std::vector<int> contacted;  // 衝突したエージェントを格納する配列

	public:
		// コンストラクタ
		Agent(int ID_, int x_, int y_, mynamespace::dir dir_) : ID(ID_), x(x_), y(y_), dir(dir_)
		{
			// 非決定的な乱数生成器
			std::random_device rndo;
			//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
			std::mt19937 mt32(rndo());
			// [0.5, 1.5] 範囲の一様乱数
			std::uniform_real_distribution<> rand_speed(0.5, 1.5);
			// 乱数発生のための処理
			std::srand(unsigned int(time(NULL)));

			// 角度[rad]
			if (dir_ == nw)
			{
				radian = double(rnd(1, 90)) * (M_PI / 180);
			}
			else if (dir_ == sw)
			{
				radian = double(rnd(270, 360)) * (M_PI / 180);
			}
			else if (dir_ == ne)
			{
				radian = double(rnd(90, 180)) * (M_PI / 180);
			}
			else if (dir_ == se)
			{
				radian = double(rnd(180, 270)) * (M_PI / 180);
			}

			// x方向の速度
			x_v = int(cos(radian) * SPEED * rand_speed(mt32));
			// y方向の速度
			y_v = int(sin(radian) * SPEED * rand_speed(mt32));
			// エージェントの発生
			duration = 0;
			// 初期状態の色は緑
			color = green;
			// 緑
			bgr_color = cv::Scalar(0, 255, 0);
		}

		// デストラクタ
		~Agent() {}

		// IDを取得
		int get_ID()
		{
			return ID;
		}

		// x座標を取得
		int get_x()
		{
			return x;
		}

		// y座標を取得
		int get_y()
		{
			return y;
		}

		// x方向の速度を取得
		int get_x_v()
		{
			return x_v;
		}

		// y方向の速度を取得
		int get_y_v()
		{
			return y_v;
		}

		// 期間を取得
		int get_duration()
		{
			return duration;
		}

		// 方角と速度を再定義
		void set_dir(Agent* self)
		{
			// 非決定的な乱数生成器
			std::random_device rndo;
			//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
			std::mt19937 mt32(rndo());
			// [0, 3] 範囲の一様乱数
			std::uniform_int_distribution<> rand03(0, 3);
			int quarter = rand03(mt32);
			// [0.5, 1.5] 範囲の一様乱数
			std::uniform_real_distribution<> rand_speed(0.5, 1.5);

			if (quarter == 0)
			{
				self->dir = nw;
				radian = double(rnd(1, 90)) * (M_PI / 180);
			}
			else if (quarter == 1)
			{
				self->dir = sw;
				radian = double(rnd(270, 360)) * (M_PI / 180);
			}
			else if (quarter == 2)
			{
				self->dir = ne;
				radian = double(rnd(90, 180)) * (M_PI / 180);
			}
			else if (quarter == 3)
			{
				self->dir = se;
				radian = double(rnd(180, 270)) * (M_PI / 180);
			}

			self->x_v = int(cos(radian) * SPEED * rand_speed(mt32));
			self->y_v = int(sin(radian) * SPEED * rand_speed(mt32));
		}

		// 座標を再定義
		void set_xy(Agent *self)
		{
			// 非決定的な乱数生成器
			std::random_device rndo;
			//  メルセンヌ・ツイスタの32ビット版、引数は初期シード値
			std::mt19937 mt32(rndo());
			// [0, 1] 範囲の一様乱数
			std::uniform_int_distribution<> rand01(0, 1);
			int half = rand01(mt32);

			if (self->dir == nw)
			{
				if (half == 0)
				{
					self->x = rnd(1, 50);
					self->y = 1;
				}
				else if (half == 1)
				{
					self->x = 1;
					self->y = rnd(1, 50);
				}
			}
			else if (self->dir == sw)
			{
				if (half == 0)
				{
					self->x = rnd(1, 50);
					self->y = SIZE;
				}
				else if (half == 1)
				{
					self->x = 1;
					self->y = rnd(SIZE - 50, SIZE);
				}
			}
			else if (self->dir == ne)
			{
				if (half == 0)
				{
					self->x = rnd(SIZE - 50, SIZE);
					self->y = 1;
				}
				else if (half == 1)
				{
					self->x = SIZE;
					self->y = rnd(1, 50);
				}
			}
			else if (self->dir == se)
			{
				if (half == 0)
				{
					self->x = rnd(SIZE - 50, SIZE);
					self->y = SIZE - 1;
				}
				else if (half == 1)
				{
					self->x = SIZE - 1;
					self->y = rnd(SIZE - 50, SIZE);
				}
			}
		}

		// エージェントのxy座標を更新
		void update_xy()
		{
			// xy方向の速さ分だけxy座標を更新
			x += x_v;
			y += y_v;
		}

		// 点を表示
		void draw_circle(cv::Mat img_)
		{
			cv::circle(img_, cv::Point(x, y), R, bgr_color, -1);
		}

		// 点の色を変更
		void chenge_color(int b, int g, int r)
		{
			bgr_color = cv::Scalar(b, g, r);
		}

		// 衝突したエージェントをリストに追加
		void add_list(Agent* encountered)
		{
			contacted.push_back(encountered->get_ID());
		}

		// 衝突イベント
		void collision(Agent* self, std::vector<Agent*> agent)
		{
			// 自分の座標
			sx = self->x;
			sy = self->y;
			for (int i = 0; i < agent.size(); i++)
			{
				// 各エージェントの座標
				ax = agent[i]->get_x();
				ay = agent[i]->get_y();

				// エージェント同士が衝突した場合
				if ((((sx - ax) * (sx - ax)) + ((sy - ay) * (sy - ay))) < (R * R))
				{
					// 違うエージェントであることを判断
					if (self->get_ID() != agent[i]->get_ID())
					{
						// 衝突した座標とエージェントIDを表示
						std::cout << "Collision x座標:" << sx << " y座標:" << sy << std::endl;
						std::cout << self->ID << " & " << agent[i]->ID << std::endl;

						// 衝突したエージェントのIDをお互いに保存
						self->add_list(agent[i]);
						agent[i]->add_list(self);
						// エージェントを赤色に変更
						self->color = red;
						self->chenge_color(0, 0, 255);
					}
				}
			}
		}

		// 衝突したIDを出力
		void print_contacted_agents()
		{
			std::cout << "AgentID:" << ID << std::endl;
			std::cout << "ID:";
			for (int i = 0; i < contacted.size(); i++)
			{
				std::cout << contacted[i] << ", ";
			}
			std::cout << std::endl;
		}
	};
}


int main()
{
	using namespace std;
	using namespace cv;
	using namespace mynamespace;

	// エージェントを格納する可変配列
	vector<Agent*> Agents;
	// windowのタイトル
	namedWindow("Shibuya Crossing Simulation", WINDOW_AUTOSIZE);

	/*
	// 4角に2N/3
	for (int i = 0; i < 2 * N / 3; i += 4)
	{
		// 北西
		Agent* agent1 = new Agent(i, rnd(1, 50), rnd(1, 50), nw);
		Agents.push_back(agent1);

		// 南西
		Agent* agent2 = new Agent(i + 1, rnd(1, 50), rnd(SIZE - 50, SIZE), sw);
		Agents.push_back(agent2);

		// 北東
		Agent* agent3 = new Agent(i + 2, rnd(SIZE - 50, SIZE), rnd(1, 50), ne);
		Agents.push_back(agent3);

		// 南東
		Agent* agent4 = new Agent(i + 3, rnd(SIZE - 50, SIZE), rnd(SIZE - 50, SIZE), se);
		Agents.push_back(agent4);
	}	
	*/

	// 更新するまでの時間を設定
	this_thread::sleep_for(std::chrono::seconds(3));
	for (int i = 0; i < N; i += 5)
	{
		// エージェントを作成
		Agent* agent1 = new Agent(i, 1, 1, sw);
		// エージェントの方向を決定
		agent1->set_dir(agent1);
		// エージェントの座標を決定
		agent1->set_xy(agent1);
		// 追加したエージェントをAgentsに追加
		Agents.push_back(agent1);

		// エージェントを作成
		Agent* agent2 = new Agent(i, 1, 1, sw);
		// エージェントの方向を決定
		agent2->set_dir(agent2);
		// エージェントの座標を決定
		agent2->set_xy(agent2);
		// 追加したエージェントをAgentsに追加
		Agents.push_back(agent2);

		// エージェントを作成
		Agent* agent3 = new Agent(i, 1, 1, sw);
		// エージェントの方向を決定
		agent3->set_dir(agent3);
		// エージェントの座標を決定
		agent3->set_xy(agent3);
		// 追加したエージェントをAgentsに追加
		Agents.push_back(agent3);

		// エージェントを作成
		Agent* agent4 = new Agent(i, 1, 1, sw);
		// エージェントの方向を決定
		agent4->set_dir(agent4);
		// エージェントの座標を決定
		agent4->set_xy(agent4);
		// 追加したエージェントをAgentsに追加
		Agents.push_back(agent4);

		// エージェントを作成
		Agent* agent5 = new Agent(i, 1, 1, sw);
		// エージェントの方向を決定
		agent5->set_dir(agent5);
		// エージェントの座標を決定
		agent5->set_xy(agent5);
		// 追加したエージェントをAgentsに追加
		Agents.push_back(agent5);

		// windowのサイズと色
		Mat img(Size(SIZE, SIZE), CV_8UC3, Scalar(0, 0, 0));

		// 更新
		for (int j = 0; j < Agents.size(); j++)
		{
			// エージェントを表示
			Agents[j]->draw_circle(img);

			// エージェントの衝突
			Agents[j]->collision(Agents[j], Agents);

			// エージェントのxy座標を更新
			Agents[j]->update_xy();
			
			ax = Agents[j]->get_x();
			ax_v = Agents[j]->get_x_v();
			ay = Agents[j]->get_y();
			ay_v = Agents[j]->get_y_v();

			// x方向の壁に到着
			if ((ax + ax_v < 0) || (SIZE < ax + ax_v))
			{
				Agents.erase(Agents.begin() + j);
				Agents.shrink_to_fit();
			}
			// y方向の壁に到着
			else if ((ay + ay_v < 0) || (SIZE < ay + ay_v))
			{
				Agents.erase(Agents.begin() + j);
				Agents.shrink_to_fit();
			}
		}

		imshow("Shibuya Crossing Simulation", img);
		const int k = waitKey(20);
		// qを押したら終了
		if (k == 'q')
		{
			break;
		}
	}

	// windowを閉じる
	destroyAllWindows();

	// 衝突した情報を表示
	for (int j = 0; j < Agents.size(); j++)
	{
		Agents[j]->print_contacted_agents();
	}

	return 0;
}
