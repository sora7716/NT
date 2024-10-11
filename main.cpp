#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>//WinSockのヘッダファイル
#include <iostream>

#pragma comment(lib,"WSock32.lib")//WinSockライブラリを指定

/// <summary>
/// サーバー
/// </summary>
void Serve() {
	SOCKET listen_s;
	SOCKET s;
	SOCKADDR_IN saddr;
	SOCKADDR_IN from;
	u_short uport = 0;
	int fromlen = sizeof(from);
	printf("ポート番号を入力--->");
	scanf_s("%hu", &uport);
	//socket関数でソケットを作成
	listen_s = socket(AF_INET, SOCK_STREAM, 0);
	printf("リスンソケットをオープンしました\n");
	//bind関数でソケットに名前を付ける
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = INADDR_ANY;
	bind(listen_s, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("bind成功です");
	//listen関数で接続の待機状態にする
	listen(listen_s, 0);
	printf("listen成功です\n");
	//accept関数で接続を受け入れる
	printf("acceptで待機します");
	s = accept(listen_s, (struct sockaddr*)&saddr, &fromlen);
	while (1) {
		int nRcv;
		char szBuf[1024];
		printf("accept関数成功です\n");

		//recv関数でデータを受信
		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\n';
		printf("会話を開始\n");
		printf("受信を待っています\n");
		printf("受信 --> %s\n", szBuf);
		printf("送信 --> ");

		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		//send関数でデータを送信
		send(s, szBuf, (int)strlen(szBuf), 0);

	}
	//closesocketでソケットを閉じる
	closesocket(s);
}

/// <summary>
/// クライアント
/// </summary>
void Client() {
	SOCKET s;
	SOCKADDR_IN saddr;
	u_short uport = 0;
	char ipAdd[256];
	HOSTENT* ipHost;

	printf("ポート番号を入力 --> ");
	scanf_s("%hu", &uport);
	printf("サーバー名を入力 --> ");
	scanf_s("%s", &ipAdd, 256);
	ipHost = gethostbyname(ipAdd);
	//socketの作成
	s = socket(AF_INET, SOCK_STREAM, 0);
	//クライアントソケットをサーバーに接続
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = ipHost->h_addrtype;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = *((u_long*)ipHost->h_addr);
	//bind(s, (struct sockaddr*)&saddr, sizeof(saddr));
	//conect関数でサーバーに接続する
	//s = connect(listen_s, (SOCKADDR*)&saddr, sizeof(saddr));
	if (connect(s, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("サーバーと接続できませんでした\n");
		closesocket(s);
		return;
	}
	while (1) {
		printf("サーバーと接続できました\n");
		int nRcv;
		char szBuf[1024];

		printf("会話を開始\n");
		printf("送信 --> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		//send関数でデータを送信
		send(s, szBuf, (int)strlen(szBuf), 0);

		//recv関数でデータを受信
		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\n';
		printf("受信 --> %s\n", szBuf);
	}
	//closescocketでソケットを閉じる
	closesocket(s);
}

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//初期エラー
		std::cout << "WinSockの初期化に失敗しました" << std::endl;
		return 1;
	}
	int mode = -1;
	printf("モードを選択\n0:サーバー,1:クライアント\nモード --> ");
	scanf_s("%d", &mode);
	if (mode == 0) {
		Serve();
	}
	else {
		Client();
	}
	WSACleanup();
	return 0;
}