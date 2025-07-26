
#include <kalman.h>
#include <stdio.h>
#include <stdlib.h>
 

 
/*
 * 函数：KalmanFilter_Init
 * 功能：初始化卡尔曼滤波器
 * 参数：
 *      kf      - 指向 KalmanFilter 结构体的指针
 *      init_x  - 初始状态估计
 *      init_P  - 初始估计误差协方差
 *      Q       - 过程噪声协方差
 *      R       - 测量噪声协方差
 */
void KalmanFilter_Init(KalmanFilter *kf, double init_x, double init_P, double Q, double R) {
    kf->x = init_x;
    kf->P = init_P;
    kf->Q = Q;
    kf->R = R;
    kf->K = 0.0;

}
 
/*
 * 函数：KalmanFilter_Update
 * 功能：使用卡尔曼滤波器对新的测量值进行更新
 * 参数：
 *      kf - 指向 KalmanFilter 结构体的指针
 *      z  - 新的测量值
 * 返回值：
 *      更新后的状态估计
 *
 * 实现步骤：
 *   1. 预测： x_prior = x,  P_prior = P + Q
 *   2. 计算卡尔曼增益： K = P_prior / (P_prior + R)
 *   3. 更新状态估计： x = x_prior + K * (z - x_prior)
 *   4. 更新估计误差协方差： P = (1 - K) * P_prior
 */
double KalmanFilter_Update(KalmanFilter *kf, double z) {
    // 预测步骤
    double x_prior = kf->x;
    double P_prior = kf->P + kf->Q;
    
    // 更新步骤
    kf->K = P_prior / (P_prior + kf->R);
    kf->x = x_prior + kf->K * (z - x_prior);
    kf->P = (1 - kf->K) * P_prior;
    
    return kf->x;
}
