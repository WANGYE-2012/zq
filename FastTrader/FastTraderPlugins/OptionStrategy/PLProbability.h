#pragma once
#include "NormalDistribution.h"
#include <math.h>

class CPLProbability
{
public:
    CPLProbability(void);
    ~CPLProbability(void);

    //����
    //mu: 		�������ۣ����õ�ǰ��
    //sigma:	�����۸��껯������
    //days:	    ������Ȩ����������
    CPLProbability(double mu,double sigma,int days)
        : m_mu(mu)
    {
        m_sigma=sigma/sqrt((double)250)*sqrt((double)days);
    };

    //ȡ��ǰ��ʾ�ļ۸����䡣ȡ������Ҫ������С�䶯��λ���е���
    void GetRange(double& MinPrice,double& MaxPrice) 
    { 
        MinPrice=m_mu-m_sigma*m_mu*2.58;  MaxPrice=m_mu+m_sigma*m_mu*2.58; 
    };

    //����۸��Ӧ�ĸ����ܶȡ���x=muʱ�������ܶ���󣬶�Ӧ��Y���Hei*90%�����Դ�ȷ�������귽����ʾ����
    double CalcuProbabilityDensity(double xPrice) 
    { 
        return CNormalDistribution:: Calcu_ProbabilityDensity(xPrice,m_mu,m_sigma*m_mu); 
    };

    //����۸��Ӧ���������������Ϊ-�������xPrice
    double CalcuDistribution(double xPrice) 
    { 
        return CNormalDistribution:: Calcu_Distribution(xPrice,m_mu,m_sigma*m_mu);
    }

private:
    double m_mu;
    double m_sigma;

};
