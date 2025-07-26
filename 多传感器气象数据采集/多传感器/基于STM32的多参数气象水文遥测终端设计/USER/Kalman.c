
#include <kalman.h>
#include <stdio.h>
#include <stdlib.h>
 

 
/*
 * ������KalmanFilter_Init
 * ���ܣ���ʼ���������˲���
 * ������
 *      kf      - ָ�� KalmanFilter �ṹ���ָ��
 *      init_x  - ��ʼ״̬����
 *      init_P  - ��ʼ�������Э����
 *      Q       - ��������Э����
 *      R       - ��������Э����
 */
void KalmanFilter_Init(KalmanFilter *kf, double init_x, double init_P, double Q, double R) {
    kf->x = init_x;
    kf->P = init_P;
    kf->Q = Q;
    kf->R = R;
    kf->K = 0.0;

}
 
/*
 * ������KalmanFilter_Update
 * ���ܣ�ʹ�ÿ������˲������µĲ���ֵ���и���
 * ������
 *      kf - ָ�� KalmanFilter �ṹ���ָ��
 *      z  - �µĲ���ֵ
 * ����ֵ��
 *      ���º��״̬����
 *
 * ʵ�ֲ��裺
 *   1. Ԥ�⣺ x_prior = x,  P_prior = P + Q
 *   2. ���㿨�������棺 K = P_prior / (P_prior + R)
 *   3. ����״̬���ƣ� x = x_prior + K * (z - x_prior)
 *   4. ���¹������Э��� P = (1 - K) * P_prior
 */
double KalmanFilter_Update(KalmanFilter *kf, double z) {
    // Ԥ�ⲽ��
    double x_prior = kf->x;
    double P_prior = kf->P + kf->Q;
    
    // ���²���
    kf->K = P_prior / (P_prior + kf->R);
    kf->x = x_prior + kf->K * (z - x_prior);
    kf->P = (1 - kf->K) * P_prior;
    
    return kf->x;
}
