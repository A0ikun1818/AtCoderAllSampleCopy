#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <chrono>
using namespace std;
enum Mode {
	SampleMode = 1,
	InputMode = 11,
	OutputMode = 12
};
int main()
{
	const string TIME_LIMIT = "2.2";

	string s;
	int mode = 0;
	string sampleNum = "";
	string inputStr = "", outputStr = "";
  string finalRes = "";
	while (1) {
		getline(cin, s);
    cerr << s << endl;
		if (s == "$ === Sample === $" || s == "$ === END === $") {
      // cerr << ":" << sampleNum << endl;
			mode = SampleMode;
			if (!inputStr.empty()) {
				// サンプル実行
				if (outputStr.empty()) {
					// 空出力
					outputStr = "\n";
				}
				ofstream inFile("in.txt"), outFile("out.txt");
				inFile << inputStr;
				outFile << outputStr;
				inFile.close();
				outFile.close();

				string cmd = "timeout " + TIME_LIMIT + " ./a.out < in.txt > check.txt 2> /dev/null";
        
        // 実行前の時刻
        auto startTime = chrono::high_resolution_clock::now();

        // プログラムの実行
				int runRes = system(cmd.c_str());

        // 実行後の時刻
        auto endTime = chrono::high_resolution_clock::now();

        // 差分から実行時間を求める
        chrono::duration<double, milli> elapsed = endTime - startTime;
        int durationTime = elapsed.count();
				int judgeRes = -1;
				string judgeResStr = "WJ";
        
				if (durationTime > 2000) {
          // 実行時間制限超過
					judgeRes = 1;
					judgeResStr = "TLE";
				}else if(runRes != 0){
          // 実行時エラー
					judgeRes = 1;
					judgeResStr = "RE";
        }else {
          // 正しく出力されているか確認
					ifstream ifstr1("out.txt", ios::binary);
					ifstream ifstr2("check.txt", ios::binary);
					// ファイルの比較
					if (std::equal(std::istreambuf_iterator<char>(ifstr1)
						, std::istreambuf_iterator<char>(), std::istreambuf_iterator<char>(ifstr2))) {
              // 正しい
						judgeRes = 0;
						judgeResStr = "AC";
					}
					else {
            // 間違い
						judgeRes = 1;
						judgeResStr = "WA";
					}
				}

        if(judgeRes == 0){
          // AC
          judgeResStr = "\033[32m" + judgeResStr + "\033[m";
        }else if(judgeRes == 1){
          // AC以外
          judgeResStr = "\033[33m" + judgeResStr + "\033[m";
        }

        // 最後にまとめて出力する
				finalRes += "Sample " + sampleNum + ": " + judgeResStr + " (" + to_string(runRes) + ", " + to_string(durationTime) + "ms)\n";
			}
			if (s == "$ === END === $") {
				// 処理終了
				break;
			}else{
        remove("in.txt");
        remove("out.txt");
        remove("check.txt");
      }
      inputStr = "";
      outputStr = "";
		}else if (s == "$ == INPUT == $") {
			mode = InputMode;
		}else if (s == "$ == OUTPUT == $") {
			mode = OutputMode;
		}
		else {
      // cerr << ":" << sampleNum << endl;
			switch (mode) {
			case SampleMode:
				sampleNum = s;
				break;
			case InputMode:
				inputStr += s + "\n";
				break;
			case OutputMode:
				outputStr += s + "\n";
				break;
			}
		}
	}
  cout << finalRes;
	return 0;
}
