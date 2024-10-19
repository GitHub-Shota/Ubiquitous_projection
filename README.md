# ペイントアプリ

https://github.com/user-attachments/assets/8889fd55-9bdf-406f-990f-eecebcbdca54

#### 詳細：

**機能**
* 線の描写<br>
    -マウスの右クリックを押してる状態のみ線を描写する。
* 線の色を変更<br>
    -eキーで線の色を黒色に変更する。<br>
    -wキーで線の色を白色に変更する。<br>
    -gキーで線の色を緑色に変更する。<br>
    -bキーで線の色を青色に変更する。
* 線の太さを変更<br>
    -pキーで線の太さを太くする。<br>
    -mキーで線の太さを細くする。
* 描写画面のリセット<br>
    -rキーで描写画面を黒色にリセットする。
* 描写画面の保存<br>
    -sキーで描写画面を"output.png"として保存する。
* udp通信とプログラムの終了<br>
    -qキーでudp通信とプログラムを終了する。

**実装**
* C++
* OpenCV

**各ファイルの説明**
* level1.cpp：右クリックを押してる状態で線を描写。
* level2.1_Rx.cpp：main文のwhileループ内で一つ前とと今の座標データを受信し、線を描写。
* level2.1_Tx.cpp：マウスコールバック関数内で一つ前の座標と今の座標を送信。
* level2.2_Rx.cpp：排他制御ありでマルチスレッドを実装。
* level2.2_Tx.cpp：level2.1_Tx.cppと同じ。
* level2.3_Rx.cpp：排他制御なしでマルチスレッドを実装。
* level2.3_Tx.cpp：level2.1_Tx.cppと同じ。
* level2.4_Rx.cpp：level2.3_Rx.cppに機能を追加。
* level2.4_Tx.cpp：level2.3_Tx.cppに機能を追加。
