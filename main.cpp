#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>//WinSock�̃w�b�_�t�@�C��
#include <iostream>

#pragma comment(lib,"WSock32.lib")//WinSock���C�u�������w��

/// <summary>
/// �T�[�o�[
/// </summary>
void Serve() {
	SOCKET listen_s;
	SOCKET s;
	SOCKADDR_IN saddr;
	SOCKADDR_IN from;
	u_short uport = 0;
	int fromlen = sizeof(from);
	printf("�|�[�g�ԍ������--->");
	scanf_s("%hu", &uport);
	//socket�֐��Ń\�P�b�g���쐬
	listen_s = socket(AF_INET, SOCK_STREAM, 0);
	printf("���X���\�P�b�g���I�[�v�����܂���\n");
	//bind�֐��Ń\�P�b�g�ɖ��O��t����
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = INADDR_ANY;
	bind(listen_s, (struct sockaddr*)&saddr, sizeof(saddr));
	printf("bind�����ł�");
	//listen�֐��Őڑ��̑ҋ@��Ԃɂ���
	listen(listen_s, 0);
	printf("listen�����ł�\n");
	//accept�֐��Őڑ����󂯓����
	printf("accept�őҋ@���܂�");
	s = accept(listen_s, (struct sockaddr*)&saddr, &fromlen);
	while (1) {
		int nRcv;
		char szBuf[1024];
		printf("accept�֐������ł�\n");

		//recv�֐��Ńf�[�^����M
		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\n';
		printf("��b���J�n\n");
		printf("��M��҂��Ă��܂�\n");
		printf("��M --> %s\n", szBuf);
		printf("���M --> ");

		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		//send�֐��Ńf�[�^�𑗐M
		send(s, szBuf, (int)strlen(szBuf), 0);

	}
	//closesocket�Ń\�P�b�g�����
	closesocket(s);
}

/// <summary>
/// �N���C�A���g
/// </summary>
void Client() {
	SOCKET s;
	SOCKADDR_IN saddr;
	u_short uport = 0;
	char ipAdd[256];
	HOSTENT* ipHost;

	printf("�|�[�g�ԍ������ --> ");
	scanf_s("%hu", &uport);
	printf("�T�[�o�[������� --> ");
	scanf_s("%s", &ipAdd, 256);
	ipHost = gethostbyname(ipAdd);
	//socket�̍쐬
	s = socket(AF_INET, SOCK_STREAM, 0);
	//�N���C�A���g�\�P�b�g���T�[�o�[�ɐڑ�
	memset(&saddr, 0, sizeof(SOCKADDR_IN));
	saddr.sin_family = ipHost->h_addrtype;
	saddr.sin_port = htons(uport);
	saddr.sin_addr.s_addr = *((u_long*)ipHost->h_addr);
	//bind(s, (struct sockaddr*)&saddr, sizeof(saddr));
	//conect�֐��ŃT�[�o�[�ɐڑ�����
	//s = connect(listen_s, (SOCKADDR*)&saddr, sizeof(saddr));
	if (connect(s, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR) {
		printf("�T�[�o�[�Ɛڑ��ł��܂���ł���\n");
		closesocket(s);
		return;
	}
	while (1) {
		printf("�T�[�o�[�Ɛڑ��ł��܂���\n");
		int nRcv;
		char szBuf[1024];

		printf("��b���J�n\n");
		printf("���M --> ");
		scanf_s("%s", szBuf, 1024);
		fflush(stdin);

		//send�֐��Ńf�[�^�𑗐M
		send(s, szBuf, (int)strlen(szBuf), 0);

		//recv�֐��Ńf�[�^����M
		nRcv = recv(s, szBuf, sizeof(szBuf) - 1, 0);
		szBuf[nRcv] = '\n';
		printf("��M --> %s\n", szBuf);
	}
	//closescocket�Ń\�P�b�g�����
	closesocket(s);
}

int main() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0) {
		//�����G���[
		std::cout << "WinSock�̏������Ɏ��s���܂���" << std::endl;
		return 1;
	}
	int mode = -1;
	printf("���[�h��I��\n0:�T�[�o�[,1:�N���C�A���g\n���[�h --> ");
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