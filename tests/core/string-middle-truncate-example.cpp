/*************************************************************************
> FileName: string-middle-truncate-example.cpp
> Author  : DingJing
> Mail    : dingjing@live.cn
> Created Time: 2021年04月22日 星期四 10时31分17秒
 ************************************************************************/
#include <QString>
#include <iostream>
#include "../../core/utils.h"

int main (int argc, char* argv[])
{
    QString test1 = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
    std::cout << dingjing::Utils::stringMiddleTruncate(test1, 10).toStdString() << std::endl;

    QString test2 = "事实上似乎似乎似乎四是似乎似乎是";
    std::cout << dingjing::Utils::stringMiddleTruncate(test2, 10).toStdString() << std::endl;

    QString test3 = "111111!@#$#$$%$^$&^**&(*)))_P{}?>K<H反反复复F";
    std::cout << dingjing::Utils::stringMiddleTruncate(test3, 10).toStdString() << std::endl;

    QString test4 = "11!@#$啊啊啊啊>K<HJ啊啊啊啊啊F";
    std::cout << dingjing::Utils::stringMiddleTruncate(test4, 10).toStdString() << std::endl;

    QString test5 = "11!@#$啊啊啊啊>K<HJ啊啊啊啊啊F";
    std::cout << dingjing::Utils::stringMiddleTruncate(test5, 0).toStdString() << std::endl;

    QString test6 = "11!@#$啊啊啊啊>K<HJ啊啊啊啊啊F";
    std::cout << dingjing::Utils::stringMiddleTruncate(test5, -1).toStdString() << std::endl;

    return 0;
}