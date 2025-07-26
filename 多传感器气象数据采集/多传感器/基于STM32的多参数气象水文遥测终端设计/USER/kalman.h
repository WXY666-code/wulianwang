




 // ���忨�����˲������ݽṹ
typedef struct {
    double x;   // ״̬����
    double P;   // �������Э����
    double Q;   // ��������Э����
    double R;   // ��������Э����
    double K;   // ����������
} KalmanFilter;



void KalmanFilter_Init(KalmanFilter *kf, double init_x, double init_P, double Q, double R);

double KalmanFilter_Update(KalmanFilter *kf, double z);
