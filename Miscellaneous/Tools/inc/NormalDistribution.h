/*
 * NormalDistribution.h
 *
 * ��̬�ֲ����߼���
 *
 */

#ifndef NORMALDISTRIBUTION_H_
#define NORMALDISTRIBUTION_H_


#ifdef WIN32
#pragma managed(push,off)
#endif

class CNormalDistribution
{
public:

    class CStandardNormalDistribution
    {
    public:

        //********************************************************************************************
        //�����׼��̬�ֲ����� 
        //      ��������: �������--x; 1/sqrt(2*pi)*exp(-t^2/2)dt; t=x
        //�ο���http://zh.wikipedia.org/wiki/%E5%B8%B8%E6%85%8B%E5%88%86%E4%BD%88
        //�ο���http://wenku.baidu.com/link?url=ygq3GC5X5x38PUMvOYz6sEvh12KM9ZDJHabYO-cpLHR1D6XvTpDzigKBNw7V0PBbFcnPA6FwNt2r4IbETUxlmyVxllJ7MZOCUFBoniDL0YO
        //�ο�����̬������ֵ�㷨��ʾ����,�����׼��̬�ֲ���,���С�ڰ����֮һ
        //      @author ŷ����,����ʦ����ѧ��������ѧԺ,QQ:78692844
        //********************************************************************************************
        static double Calcu_Distribution(double x);

    private:
	    //���ڼ����׼��̬�ֲ�����̬�ֲ���������ֵ��ʹ��6������ຯ��
	    static double Fi_erf_6(double x);
	    //���ڼ����׼��̬�ֲ�����̬�ֲ���������������ຯ��
	    static double erf_6(double x);
    };

    //********************************************************************************************
    //�����׼��̬�ֲ����� 
    //      ��������: �������--x; 1/sqrt(2*pi)*exp(-t^2/2)dt; t=x
    //�ο���http://zh.wikipedia.org/wiki/%E5%B8%B8%E6%85%8B%E5%88%86%E4%BD%88
    //�ο���http://wenku.baidu.com/link?url=ygq3GC5X5x38PUMvOYz6sEvh12KM9ZDJHabYO-cpLHR1D6XvTpDzigKBNw7V0PBbFcnPA6FwNt2r4IbETUxlmyVxllJ7MZOCUFBoniDL0YO
    //�ο�����̬������ֵ�㷨��ʾ����,�����׼��̬�ֲ���,���С�ڰ����֮һ
    //      @author ŷ����,����ʦ����ѧ��������ѧԺ,QQ:78692844
    //********************************************************************************************
    static double Calcu_Distribution(double x);

    //********************************************************************************************
    //������̬�ֲ������ܶ�  
    //      f(x)=1/sqrt(2*pi)*exp(-(x-mu)^2/(2*mu^2))
    //�ο���http://wenku.baidu.com/view/231fa2176edb6f1aff001f37.html
    //�ο���http://baike.baidu.com/view/45379.htm?fr=aladdin#7_1
    //********************************************************************************************
    static double Calcu_ProbabilityDensity(double x,double mu,double sigma);

    //********************************************************************************************
    //������̬�ֲ�����  u=(x-mu)/sigma, ����u���׼��̬�ֲ�
    //�ο���http://baike.baidu.com/view/45379.htm?fr=aladdin#7_1
    //********************************************************************************************
    static double Calcu_Distribution(double x,double mu,double sigma);

};


#ifdef WIN32
#pragma managed(pop)
#endif

#endif /* NORMALDISTRIBUTION_H_ */
