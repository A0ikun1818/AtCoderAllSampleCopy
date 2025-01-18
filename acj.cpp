#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <filesystem>
using namespace std;
enum Mode {
	SampleMode = 1,
	InputMode = 11,
	OutputMode = 12
};
void Replace(std::string& stringreplace, const std::string& origin, const std::string& dest)
{
    size_t pos = stringreplace.find(origin);  // 置換したい文字列が登場する位置を検索する
    size_t len = origin.length();    // 置換したい文字列の長さ
    stringreplace.replace(pos, len, dest);  // 置換
}// https://learningprog.com/cpp-replace/
int main(int argc, char *argv[])
{
	string TIMEOUT_BORDER = "2.2";
	long double TIME_LIMIT = 2000.0;
	if(argc>=2){
		long double TL = 2.0;
		try{
			TL = stold(argv[1]);
		}catch(exception e){
			TL = 2.0;
		}
		TIMEOUT_BORDER = to_string(TL*1.1);
		TIME_LIMIT = TL*1000;
	}

	string s;
	int mode = 0;
	string sampleNum = "";
	string inputStr = "", outputStr = "";
	string finalRes = "";
	while (1) {
		getline(cin, s);
		// cerr << s << endl;
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

				string cmd = "timeout " + TIMEOUT_BORDER + " ./a.out < in.txt > check.txt 2> /dev/null";
				
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
				
				if (durationTime > TIME_LIMIT) {
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
						system("paste check.txt out.txt > diff.txt");
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
				string res = "";
				res += "Sample " + sampleNum + ": " + judgeResStr + " (" + to_string(runRes) + ", " + to_string(durationTime) + "ms)\n";
				if(filesystem::is_regular_file("diff.txt")){
					ifstream diffFile("diff.txt");
					if(!diffFile.is_open()){
						res += "diff.txt open error\n";
					}else{
						string line;
						while(getline(diffFile, line)){
							if(line.find('.') != string::npos){
								int slashPos = line.find('\t');
								if(slashPos == string::npos){
									line = "line slash error\n";
								}else{
									string userOutput = line.substr(0, slashPos);
									string sampleOutput = line.substr(slashPos+1);
									int minlen = min(userOutput.size(), sampleOutput.size())-1;
									if(userOutput.substr(0, minlen) == sampleOutput.substr(0, minlen)){
										line += "\tNear \033[32mAC\033[m?";
									}
								}
							}
							res += line + "\n";
						}
						diffFile.close();
					}
				}
				//個別で出力
				cout << string(30, '=') << endl;
				cout << res;

				//それとは別に、最後にまとめて出力する用
				finalRes += res;
			}
			if (s == "$ === END === $") {
				// 処理終了
				#ifndef DEBUG
					remove("in.txt");
					remove("out.txt");
					remove("check.txt");
					remove("diff.txt");
				#endif
				break;
			}else{
				remove("in.txt");
				remove("out.txt");
				remove("check.txt");
				remove("diff.txt");
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
	cout << string(30, '=') << endl;
	cout << finalRes;
	return 0;
}
