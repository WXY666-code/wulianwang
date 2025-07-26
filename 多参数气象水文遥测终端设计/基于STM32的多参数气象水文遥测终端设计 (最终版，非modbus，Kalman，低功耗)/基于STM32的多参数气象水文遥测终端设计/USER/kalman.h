




 // 定义卡尔曼滤波器数据结构
typedef struct {
    double x;   // 状态估计
    double P;   // 估计误差协方差
    double Q;   // 过程噪声协方差
    double R;   // 测量噪声协方差
    double K;   // 卡尔曼增益
} KalmanFilter;



void KalmanFilter_Init(KalmanFilter *kf, double init_x, double init_P, double Q, double R);

double KalmanFilter_Update(KalmanFilter *kf, double z);
