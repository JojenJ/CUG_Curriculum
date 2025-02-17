#ifndef CALCULATOR_H
#define CALCULATOR_H
 
#include <QMainWindow>
#include<memory>
#include<string>
#include<QString>
#include<cmath>
#include"Operator.h"
#include"Factory.h"
#include"Stack.h"
#include "analogclock.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Calculator; }
QT_END_NAMESPACE

constexpr double PI = 3.14159265358979323846;
constexpr double e = 2.718281828459045;
extern double ans;
extern double anssc;


class Calculator : public QMainWindow
{
    Q_OBJECT
private:
    QString Inputnumsc = "0";
    QString Inputexpsc;
    Stack<double> m_num;
    Stack<unique_ptr<Operator>> m_opr;
    Stack<unique_ptr<Operator>> m_cheopr;
    double readNum(string::const_iterator& it);
    bool isNum(string::const_iterator& it) {					//判断是否是数字
        return (*it >= '0' && *it <= '9') || *it == '.';
    }
    string readStr(string::const_iterator& it);
    bool isStr(string::const_iterator& n) {						//判断是否是字符
        return *n >= 'a' && *n <= 'z';
    }
    bool isOpr(string::const_iterator& it) {					//判断是否是操作符
        string o;
        o.push_back(*it);
        for (auto it = Factory::ms_operator.begin(); it != Factory::ms_operator.end(); ++it) {
            if (it->first == o)
                return true;
        }
        return false;
    }
    string readOpr(string::const_iterator& it);					//读取连续运算符的最后一位
    void calculate();
    void numberclicksc(QString &&);//数字点击函数
    void operatesc(QString &&);
    void Lba_like(QString &&);
    void calinit();//计算器初始化
    void histsc(QString &);//更新历史记录
    void clickhistsc(QString);//点击历史记录
private:
    double doIt(const string& exp);
public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();
private slots:
    void on_Science_Button_2nd1_clicked();//"2nd"
    void on_Science_Button_2nd2_clicked();
    void on_Science_Button_0_clicked();//"0"
    void on_Science_Button_1_clicked();//"1"
    void on_Science_Button_2_clicked();//"2"
    void on_Science_Button_3_clicked();//"3"
    void on_Science_Button_4_clicked();//"4"
    void on_Science_Button_5_clicked();//"5"
    void on_Science_Button_6_clicked();//"6"
    void on_Science_Button_7_clicked();//"7"
    void on_Science_Button_8_clicked();//"8"
    void on_Science_Button_9_clicked();//"9"
    void on_Science_Button_point_clicked();//"."
    void on_Science_Button_neg_pos_clicked();//"-"
    void on_Science_Button_equ_clicked();//"="
    void on_Science_Button_plus_clicked();//"+"
    void on_Science_Button_minus_clicked();//"-"
    void on_Science_Button_multi_clicked();//"*"
    void on_Science_Button_division_clicked();//"/"
    void on_Science_Button_Mod_clicked();//"%"
    void on_Science_Button_power_clicked();//"^"
    void on_Science_Button_factorial_clicked();//"!"
    void on_Science_Button_Lbra_clicked();//"("
    void on_Science_Button_Rbra_clicked();//")"
    void on_Science_Button_ans_clicked();//"Ans"
    void on_Science_Button_PI_clicked();//"PI"
    void on_Science_Button_e_clicked();//"e"
    void on_Science_Button_square_clicked();//"x^2"
    void on_Science_Button_recip_clicked();//"1/x"
    void on_Science_Button_sqrt_clicked();//"sqrt()"
    void on_Science_Button_ce_clicked();//"CE"
    void on_Science_Button_c_clicked();//"C"
    void on_Science_Button_sin_clicked();//"sin()"
    void on_Science_Button_cos_clicked();//"cos()"
    void on_Science_Button_tan_clicked();//"tan()"
    void on_Science_Button_log_clicked();//"log()"
    void on_Science_Button_dtor_clicked();//"dtor()"
    void on_Science_Button_asin_clicked();//"asin()"
    void on_Science_Button_acos_clicked();//"acos()"
    void on_Science_Button_atan_clicked();//"atan()"
    void on_Science_Button_ln_clicked();//"ln()"
    void on_Science_Button_rtod_clicked();//"rtod()"
    void on_Button_histsc1_clicked();
    void on_Button_histsc2_clicked();
    void on_Button_histsc3_clicked();
    void on_Button_histsc4_clicked();
    void on_Button_histsc5_clicked();
    void on_Button_histsc6_clicked();
    void on_Button_histsc7_clicked();
    void on_Button_histsc8_clicked();
    void on_actionClock_Mode_triggered();
    void on_Button_trashsc_clicked();

private:
    Ui::Calculator *ui;
    AnalogClock *clock;

    bool clicknumsc = 1;//0代表上一次点击为操作符
    bool afequ_numsc = 0;//0代表未用等号处理
    bool afconstsc = 0; //0表示未输入常数
    int Lbranum = 0;
    bool clickRba = 0; //0代表为打右括号
    bool errorflagsc = 0;
};

QString get_form(double);           //设置最多位数

#endif // CALCULATOR_H
