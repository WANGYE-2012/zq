/*
�жϽ����Ƿ����
*/

#pragma once

#include <string>
using std::string;


class CIsProcessExist
{
public:
    CIsProcessExist(const string& ProcessFileName);

    bool m_bExist;
};

