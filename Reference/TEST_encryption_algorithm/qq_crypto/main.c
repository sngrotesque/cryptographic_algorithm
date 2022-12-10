#include "lib/S2048.h"
#include <windows.h>

#define FILE_PATH_N 512 // �ļ�·�����ֽڳ���

// ����ļ�·��Ϊ��ʱ
char *add_file_name(char *in_f)
{
    int len = strlen(in_f);
    char *res = (char *)malloc(len + 6);
    memcpy(res, in_f, len);
    res[len]   = '.';
    res[len+1] = 's';
    res[len+2] = 'e';
    res[len+3] = 'n';
    res[len+4] = 'c';
    res[len+6] = 0b0;
    return res;
}

void CHECK_CV_FILE(sn *data)
{
    if (CV_R(data) == EOF) {
        CV_INIT(data);
        CV_W(data);
        printf("�ѽ���Կ����������.\n");
    } else {
        CV_R(data);
        printf("�Ѷ�ȡ������Կ.\n");
    }
}

BOOL exists(FILE *stream)
{
    if (!stream) {
        return FALSE;
    } else {
        return TRUE;
    }
}

// ��������ĵ�
static void HELP_DOCUMENT()
{
    system("cls");
    printf(
        "|----�ĵ�-----��ʼ--------------------------------------------------------|\n"
        "|                                                                         |\n"
        "|    �˼����㷨ԭ����SN-Grotesque����                                     |\n"
        "|    Github: https://github.com/sngrotesque/sn_cipher                     |\n"
        "|                                                                         |\n"
        "|    ��ȷ����������������ļ�·��֮�󣬳��������һ�ݶ�������İ�ȫ��Կ   |\n"
        "|    �ᱣ������ǰĿ¼�£�����Ϊ'%s'�������Ʊ�����                  |\n"
        "|                                                                         |\n"
        "|    �����ļ�·��ʱ����'����'                                             |\n"
        "|    ���ܺ�ı���·�����Ϊ�յĻ�����ֱ�ӱ����ڵ�ǰĿ¼�С�               |\n"
        "|                                                                         |\n"
        "|    [!] �������·��������ͬ���Ƶ��ļ����ᱻֱ�Ӹ��ǵ� [!]               |\n"
        "|                                                                         |\n"
        "|----�ĵ�---------------------------------------------------------����----|\n"
        "\n"
    , CV_FILE_NAME);
}

int main(int argc, char **argv)
{
    sn *data = (sn *)malloc(sizeof(sn));
    char *user_in_file_path = (char *)calloc(512, 1);
    char *user_out_file_path = (char *)calloc(512, 1);
    char user_input_buff;
    int user_xx;
    FILE *f_ptr_in, *f_ptr_out;

    HELP_DOCUMENT();

    printf("�˵�:\n\t0. �˳�����.\n\t1. ��������.\n\t2. ��������.\n\t������: ");
    scanf("%d", &user_xx);
    scanf("%c", &user_input_buff);
    
    switch(user_xx) {
        case 0:
            printf("�˳�...\n");
            exit(0);
        case 1:
            printf("����...\n");
            CHECK_CV_FILE(data);

            do {
                printf("����������ܵ��ļ�·��: ");
                scanf("%[^\n]s", user_in_file_path);
                scanf("%c", &user_input_buff);
            } while(*user_in_file_path == 0);
            
            printf("��������������ļ�·��: ");
            scanf("%[^\n]s", user_out_file_path);
            scanf("%c", &user_input_buff);

            f_ptr_in = fopen(user_in_file_path, "rb");
            if (!f_ptr_in) {
                printf("ָ���ļ������ڻ�������������ռ�ã������ԡ�\n");
                return -1;
            }
            data->data = file_read(f_ptr_in);
            data->data_n = get_file_size(f_ptr_in);
            
            
            break;
        case 2:
            printf("����...\n");
            break;
        default:
            printf("��������.\n");
            break;
    }

    Sleep(3000);
    return 0;
}