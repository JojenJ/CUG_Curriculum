#include "calculator.h"
#include "ui_calculator.h"
#include<QDebug>
#include<QIcon>
#include<QThread>
using namespace std;
 
#define pass (void)0

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Calculator)
{
    ui->setupUi(this);
    this->setWindowTitle("科学计算器");
    this->setWindowIcon(QIcon(":/image/Calculator.jpg"));
    ui->Button_trashsc->setIconSize(QSize(100,80));
    ui->Button_trashsc->setIcon(QIcon(":/image/trash.jpg"));
    clock = new AnalogClock(this);
    clock->setWindowTitle("时钟模式");
    ui->labelsc->setText(Inputnumsc);
    m_opr.push(make_unique<Hash>());
    m_cheopr.push(make_unique<Hash>());
}

Calculator::~Calculator()
{
    delete ui;
}

void Calculator::calinit(){
    m_num.clear();
    m_opr.clear();
    m_cheopr.clear();
    m_opr.push(make_unique<Hash>());
    m_cheopr.push(make_unique<Hash>());
}

double Calculator::readNum(string::const_iterator& it) {
   string t;
   while(isNum(it)){
       t.push_back(*it++);
   }
   return stod(t);
}

string Calculator::readStr(string::const_iterator& it) {
    string t;
    while (isStr(it)) {
        t.push_back(*it++);
    }
    return t;
}

string Calculator::readOpr(string::const_iterator& it) {
    string oo;
    while (isOpr(it)) {
        if (*it != '=')
            oo.push_back(*it++);
        else
            break;
    }
    return oo;
}

void Calculator::calculate() {			//添加异常处理
    double a[2] = { 0 };
    try {
        for (auto i = 0; i < m_opr.top()->numOprand(); ++i) {
            a[i] = m_num.top();
            m_num.pop();
        }
        m_num.push(m_opr.top()->get(a[1], a[0]));
    }  catch (const char* str) {
        throw str;
    }
    m_opr.pop();
}

double Calculator::doIt(const string& exp) {
    bool is_negative = 0;			//1表示下一个数是负数存入数堆栈
    bool nonumber = 1;
    for (auto it = exp.begin(); it != exp.end();) {
        if (isNum(it)){
            nonumber = 0;
            if (is_negative == 1) {
                m_num.push((-1) * readNum(it));
                is_negative = 0;
            }
            else
                m_num.push(readNum(it));
        }
        else {
            unique_ptr<Operator> oo;
            if (isStr(it)) {
                string f = readStr(it);
                if (f == "pi") {
                    m_num.push(PI);
                }
                else if (f == "e") {
                    m_num.push(e);
                }
                else if (f == "ans") {
                    m_num.push(ans);
                }
                else{
                    try {
                        auto oo = Factory::create_fun(f);
                    }
                    catch (const char* str) {
                        throw str;
                    }
                    auto oo = Factory::create_fun(f);
                    m_opr.push(std::move(oo));
                }

            }
            else if (isOpr(it)) {
                string o;
                o.push_back(*it++);
                if (o == "(")
                    nonumber = 1;
                try {
                    auto oo = Factory::create_opr(o);
                }
                catch (const char* str) {
                    throw str;
                }
                auto oo = Factory::create_opr(o);
                if ((oo->symbol() == "-" && m_opr.top()->symbol() == "#" && nonumber == 1) ||
                    (oo->symbol() == "-" && m_opr.top()->symbol() == "(" && nonumber == 1)) {
                    is_negative = 1;
                    continue;
                }

                while (oo->precedence() <= m_opr.top()->precedence()) {
                    if (m_opr.top()->symbol() == "#" || m_opr.top()->symbol() == "(")
                        break;
                    try {
                        calculate();
                    }
                    catch (const char* str) {								//跳出错误时需要对数据栈，操作符栈进行清空
                        throw str;
                    }
                }
                if (oo->symbol() == ")" && m_opr.top()->symbol() == "(") {
                    m_opr.pop();
                }
                if (oo->symbol() != "=" && oo->symbol() != ")")//等号和右括号不进栈
                    m_opr.push(std::move(oo));
            }
            else {
                throw "错误：未定义的表达式";
            }
        }
    }
    double result = m_num.top();
    m_num.pop();
    return result;
}

QString get_form(double result){    //设置小数点后最大输出长度
    QString rhs;
    int outputlength = 4;
    int inresult = int(result);
    double temp =inresult-result;
    if(abs(temp)<1.0e-6){
        result = int(result);
        rhs = QString::number(result,10,0);
    }
    else{
        rhs = QString::number(result,10,outputlength);
    }
    return rhs;
}




void Calculator::on_Science_Button_2nd1_clicked()
{
    ui->secondfunc->setCurrentIndex(1);
}


void Calculator::on_Science_Button_2nd2_clicked()
{
    ui->secondfunc->setCurrentIndex(0);
}

void Calculator::numberclicksc(QString && number){
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 1) {
        ui->expressionsc->setText(Inputexpsc);
        afequ_numsc = 0;
    }
    QString temp = number;
    if (Inputnumsc == "0") {
        Inputnumsc = temp;
        ui->labelsc->setText(Inputnumsc);
    }
    else if(afconstsc==1){
        Inputnumsc = temp;
        ui->labelsc->setText(Inputnumsc);
        afconstsc = 0;
    }
    else {
        Inputnumsc += temp;
        ui->labelsc->setText(Inputnumsc);
    }
    clicknumsc = 1;
}

void Calculator::on_Science_Button_0_clicked()//输入常数
{
    this->numberclicksc("0");
}


void Calculator::on_Science_Button_1_clicked()
{
    this->numberclicksc("1");
}


void Calculator::on_Science_Button_2_clicked()
{
    this->numberclicksc("2");
}


void Calculator::on_Science_Button_3_clicked()
{
    this->numberclicksc("3");
}


void Calculator::on_Science_Button_4_clicked()
{
    this->numberclicksc("4");
}


void Calculator::on_Science_Button_5_clicked()
{
    this->numberclicksc("5");
}


void Calculator::on_Science_Button_6_clicked()
{
    this->numberclicksc("6");
}


void Calculator::on_Science_Button_7_clicked()
{
    this->numberclicksc("7");
}


void Calculator::on_Science_Button_8_clicked()
{
    this->numberclicksc("8");
}


void Calculator::on_Science_Button_9_clicked()
{
    this->numberclicksc("9");
}


void Calculator::on_Science_Button_point_clicked()  //可以在等于号使用后直接使用，默认已输入数字为0,且能防止连续点击
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if(Inputnumsc.indexOf(".")==-1){
        QString temp;
        if (afequ_numsc == 1) {
            ui->expressionsc->setText(Inputexpsc);
            afequ_numsc = 0;
        }
        if (Inputnumsc.isEmpty() == 1)
            temp = "0.";
        else
            temp = ".";
        Inputnumsc += temp;
        ui->labelsc->setText(Inputnumsc);
        clicknumsc = 0;
    }
    else
        pass;
}


void Calculator::on_Science_Button_neg_pos_clicked()//可以在等于号使用后直接使用，默认已输入数字为0
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 1) {
        ui->expressionsc->setText(Inputexpsc);
        afequ_numsc = 0;
    }
    if (Inputnumsc == "0") {
        ui->labelsc->setText(Inputnumsc);
    }
    else if(Inputnumsc.isEmpty()==1){
        Inputnumsc = "0";
        ui->labelsc->setText(Inputnumsc);
    }
    else if(clicknumsc == 1) {
        string inputnumsc = Inputnumsc.toStdString();
        string::iterator it = inputnumsc.begin();
        double number = stod(inputnumsc);
        if (number < 0) {
            inputnumsc.erase(it);
            Inputnumsc = QString::fromStdString(inputnumsc);
        }
        else {
            inputnumsc = "-" + inputnumsc;
            Inputnumsc = QString::fromStdString(inputnumsc);
        }
        ui->labelsc->setText(Inputnumsc);
    }
    else
        pass;
}

void Calculator::on_Science_Button_equ_clicked()//不允许连续按等于号，且等号前需要输入数字，
{
    if(clicknumsc==1&&afequ_numsc==0){//上次输入为数字且不是等号
        Inputexpsc += Inputnumsc;//表达式在末尾添加新输入内容
        while(Lbranum>0){//括号自动补全
            Inputexpsc+=")";
            Lbranum--;
        }
        Inputexpsc += "=";
        ui->expressionsc->setText(Inputexpsc);
        string inputexpsc = Inputexpsc.toStdString();
        try {
            anssc = doIt(inputexpsc);//计算结果
            Inputnumsc = get_form(anssc);
            QString histsc = Inputexpsc+Inputnumsc;
            this->histsc(histsc);//更新历史记录
            ui->labelsc->setText(Inputnumsc);
            Inputexpsc = QString();
            afequ_numsc = 1;
            Inputnumsc = QString();
            Lbranum = 0;
            clickRba = 0;
        }  catch (const char* str) {
            errorflagsc = 1;
            Inputnumsc = QString::fromStdString(str);
            ui->labelsc->setText(Inputnumsc);
            Inputnumsc = QString();
            Inputexpsc = QString();
            clicknumsc = 0;
            afequ_numsc = 0;
            afconstsc = 0;
            Lbranum = 0;
            clickRba = 0;
            this->calinit();
        }

    }
    else
        pass;
}

void Calculator::operatesc(QString && opr){//连续输入运算符选取最后一个，且可以直接和上次运算值进行运算,且防止上次输入小数点为最后字符的情况
    if(clickRba == 1){
        clickRba=0;
        Inputnumsc = "0";
        ui->labelsc->setText(Inputnumsc);
        Inputexpsc+=opr;
        ui->expressionsc->setText(Inputexpsc);
        clicknumsc = 0;
    }
    else{
        if(clicknumsc==1){
            if (afequ_numsc == 0) {
                if(Inputnumsc.at(0)=='-'){
                    Inputnumsc = "("+Inputnumsc+")";
                }
                Inputexpsc+=Inputnumsc;
                Inputnumsc = QString();
                Inputexpsc+=opr;
                ui->expressionsc->setText(Inputexpsc);
            }
            else {
                Inputexpsc = get_form(anssc);
                Inputexpsc += opr;
                ui->expressionsc->setText(Inputexpsc);
            }
            clicknumsc = 0;
        }
        else{
            if(Inputnumsc.isEmpty()==0){
                string inputnumsc = Inputnumsc.toStdString();
                string::iterator it = inputnumsc.end()-1;
                if(*it =='.'){                  //小数点未输入完全的情况
                    inputnumsc.erase(it);
                    Inputnumsc = QString::fromStdString(inputnumsc);
                    Inputexpsc+=Inputnumsc;
                    Inputnumsc = QString();
                    Inputexpsc+=opr;
                    ui->expressionsc->setText(Inputexpsc);
                    clicknumsc = 0;
                }
                else
                    pass;
            }
            else{   //连续输入运算符情况或者上次有报错情况
                if(errorflagsc == 1){
                    errorflagsc = 0;
                    Inputnumsc = "0";
                    Inputexpsc = Inputnumsc+opr;
                    ui->labelsc->setText(Inputnumsc);
                    ui->expressionsc->setText(Inputexpsc);
                    clicknumsc = 0;
                }
                else{
                    string inputexpsc = Inputexpsc.toStdString();
                    string::iterator is = inputexpsc.end()-1;
                    inputexpsc.erase(is);
                    string sopr = opr.toStdString();
                    inputexpsc+=sopr;
                    Inputexpsc = QString::fromStdString(inputexpsc);
                    ui->expressionsc->setText(Inputexpsc);
                    clicknumsc = 0;
                }
            }
        }
    }
}

void Calculator::on_Science_Button_plus_clicked()
{
    this->operatesc("+");
}


void Calculator::on_Science_Button_minus_clicked()
{
    this->operatesc("-");
}


void Calculator::on_Science_Button_multi_clicked()
{
    this->operatesc("*");
}


void Calculator::on_Science_Button_division_clicked()
{
    this->operatesc("/");
}

void Calculator::on_Science_Button_Mod_clicked()
{
    this->operatesc("%");
}

void Calculator::on_Science_Button_power_clicked()
{
    this->operatesc("^");
}

void Calculator::on_Science_Button_factorial_clicked()//直接运算符,double直接转化为int负数和0默认计算结果为0
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 0) {
        if(Inputnumsc.isEmpty()==1){
            Inputnumsc = "0";
        }
        string inputnumsc = Inputnumsc.toStdString();
        double number = stod(inputnumsc);
        if (number == 0){
            Inputnumsc = "1";
            ui->labelsc->setText(Inputnumsc);
        }
        else{
            int item = int(number);
            int result = 1;
            for(int i =1;i<=item;++i){
                result *= i;
            }
            Inputnumsc = get_form(result);
            ui->labelsc->setText(Inputnumsc);
        }
    }
    else {
        if (anssc == 0){
            anssc = 1;
            Inputnumsc = "1";
            ui->labelsc->setText(Inputnumsc);
        }
        else{
            int item = int(anssc);
            int result = 1;
            for(int i =1;i<=item;++i){
                result *= i;
            }
            anssc = result;
            Inputnumsc = get_form(anssc);
            ui->labelsc->setText(Inputnumsc);
        }
    }
    afconstsc = 1;
    clicknumsc = 1;
}

void Calculator::Lba_like(QString && str){
    if(errorflagsc == 1)
        errorflagsc = 0;
    if(clickRba != 1){
        if (afequ_numsc == 1) {     //上次等号运算结束后
            Inputexpsc = str;
            ui->expressionsc->setText(Inputexpsc);
            Inputnumsc = "0";
            ui->labelsc->setText(Inputnumsc);
            afequ_numsc = 0;
        }
        else{   //如果正在输入数据，则将数据归0
            Inputnumsc = "0";
            ui->labelsc->setText(Inputnumsc);
            Inputexpsc += str;
            ui->expressionsc->setText(Inputexpsc);
        }
        Lbranum++;
    }
    else
        pass;

}

void Calculator::on_Science_Button_Lbra_clicked()
{
    Lba_like("(");
}


void Calculator::on_Science_Button_Rbra_clicked()//有左括号情况下才能添加右括号，且右括号前需要是数字，若前一位为左括号，则默认补充0
{
    if(Lbranum>0){
        if(clickRba == 1){//上次点击右括号，允许连续输入
            cout<<1<<endl;
            Inputexpsc+=")";
            ui->expressionsc->setText(Inputexpsc);
            clickRba = 1;
        }
        else{
            string inputexpsc = Inputexpsc.toStdString();
            string::iterator it = inputexpsc.end()-1;
            if(*it =='(' && Inputnumsc.isEmpty()==1){
                Inputexpsc += "0)";
                ui->expressionsc->setText(Inputexpsc);
                clickRba = 1;
            }
            if(clicknumsc==1){
                if(Inputnumsc.at(0)=='-'){
                    Inputnumsc = "("+Inputnumsc+")";//负数自动添加括号
                }
                Inputexpsc+=Inputnumsc;
                Inputnumsc = QString();
                Inputexpsc+=")";
                ui->expressionsc->setText(Inputexpsc);
                clickRba = 1;
            }
            else{
                if(Inputnumsc.isEmpty()==0){
                    string inputnumsc = Inputnumsc.toStdString();
                    string::iterator it = inputnumsc.end()-1;
                    if(*it =='.'){                  //小数点未输入完全的情况
                        inputnumsc.erase(it);
                        Inputnumsc = QString::fromStdString(inputnumsc);
                        Inputexpsc+=Inputnumsc;
                        Inputnumsc = QString();
                        Inputexpsc+=")";
                        ui->expressionsc->setText(Inputexpsc);
                        clickRba = 1;
                    }
                    else    //不允许右括号处有运算符
                        pass;
                }
                else{
                    pass;
                }
            }
        }

    }
    else
        pass;
    if(clickRba==1)
        Lbranum--;
}


void Calculator::on_Science_Button_ans_clicked()
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 1) {
        ui->expressionsc->setText(Inputexpsc);
        afequ_numsc = 0;
    }
    QString temp = get_form(anssc);
    Inputnumsc = temp;
    ui->labelsc->setText(Inputnumsc);
    clicknumsc = 1;
}


void Calculator::on_Science_Button_PI_clicked()
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 1) {
        ui->expressionsc->setText(Inputexpsc);
        afequ_numsc = 0;
    }
    QString temp = "pi";
    Inputnumsc = temp;
    ui->labelsc->setText(Inputnumsc);
    afconstsc = 1;
    clicknumsc = 1;
}


void Calculator::on_Science_Button_e_clicked()
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 1) {
        ui->expressionsc->setText(Inputexpsc);
        afequ_numsc = 0;
    }
    QString temp = "e";
    Inputnumsc = temp;
    ui->labelsc->setText(Inputnumsc);
    afconstsc = 1;
    clicknumsc = 1;
}

void Calculator::on_Science_Button_square_clicked() //直接对anscs计算，anscs会改变
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 0) {
        if(Inputnumsc.isEmpty()==1){
            Inputnumsc = "0";
        }
        string inputnumsc = Inputnumsc.toStdString();
        double number = stod(inputnumsc);
        number = pow(number,2);
        Inputnumsc = get_form(number);
        ui->labelsc->setText(Inputnumsc);
    }
    else {
        anssc = pow(anssc,2);
        Inputnumsc = get_form(anssc);
        ui->labelsc->setText(Inputnumsc);
    }
    afconstsc = 1;
    clicknumsc = 1;
}

void Calculator::on_Science_Button_recip_clicked()
{
    if(errorflagsc == 1)
        errorflagsc = 0;
    if (afequ_numsc == 0) {
        if(Inputnumsc.isEmpty()==1){
            Inputnumsc = "0";
        }
        string inputnumsc = Inputnumsc.toStdString();
        double number = stod(inputnumsc);
        if (number == 0){
            Inputnumsc = "除数不能为0!";
            ui->labelsc->setText(Inputnumsc);
            Inputnumsc = QString();
            afconstsc = 0;
            clicknumsc = 0;
            errorflagsc = 1;
        }
        else{
            number = 1/number;
            Inputnumsc = get_form(number);
            ui->labelsc->setText(Inputnumsc);
            afconstsc = 1;
            clicknumsc = 1;
        }
    }
    else {
        if (anssc == 0){
            Inputnumsc = "除数不能为0!";
            ui->labelsc->setText(Inputnumsc);
            Inputnumsc = QString();
            afconstsc = 0;
            clicknumsc = 0;
            errorflagsc = 1;
        }
        else{
            anssc = 1/anssc;
            Inputnumsc = get_form(anssc);
            ui->labelsc->setText(Inputnumsc);
            afconstsc = 1;
            clicknumsc = 1;
        }
    }

}

void Calculator::on_Science_Button_sqrt_clicked()
{
    Lba_like("sqrt(");
}


void Calculator::on_Science_Button_ce_clicked()
{
    errorflagsc = 0;
    Inputnumsc = "0";
    ui->labelsc->setText(Inputnumsc);
}


void Calculator::on_Science_Button_c_clicked()
{
    errorflagsc = 0;
    anssc = 0;
    Inputnumsc = "0";
    Inputexpsc = QString();
    ui->expressionsc->setText(Inputexpsc);
    ui->labelsc->setText(Inputnumsc);
}


void Calculator::on_Science_Button_sin_clicked()
{
    Lba_like("sin(");
}


void Calculator::on_Science_Button_cos_clicked()
{
    Lba_like("cos(");
}


void Calculator::on_Science_Button_tan_clicked()
{
    Lba_like("tan(");
}


void Calculator::on_Science_Button_log_clicked()
{
    Lba_like("log(");
}

void Calculator::on_Science_Button_dtor_clicked()
{
    Lba_like("dtor(");
}


void Calculator::on_Science_Button_asin_clicked()
{
    Lba_like("asin(");
}


void Calculator::on_Science_Button_acos_clicked()
{
    Lba_like("acos(");
}


void Calculator::on_Science_Button_atan_clicked()
{
    Lba_like("atan(");
}


void Calculator::on_Science_Button_ln_clicked()
{
    Lba_like("ln(");
}


void Calculator::on_Science_Button_rtod_clicked()
{
    Lba_like("rtod(");
}


void Calculator::histsc(QString & newexpression){
    QString h1 = ui->Button_histsc1->text();
    QString h2 = ui->Button_histsc2->text();
    QString h3 = ui->Button_histsc3->text();
    QString h4 = ui->Button_histsc4->text();
    QString h5 = ui->Button_histsc5->text();
    QString h6 = ui->Button_histsc6->text();
    QString h7 = ui->Button_histsc7->text();
    ui->Button_histsc8->setText(h7);
    ui->Button_histsc7->setText(h6);
    ui->Button_histsc6->setText(h5);
    ui->Button_histsc5->setText(h4);
    ui->Button_histsc4->setText(h3);
    ui->Button_histsc3->setText(h2);
    ui->Button_histsc2->setText(h1);
    ui->Button_histsc1->setText(newexpression);
}

void Calculator::clickhistsc(QString item){
    if(item.isEmpty()==0){
        string outcome;
        string inputnum = item.toStdString();
        size_t length = inputnum.length();
        size_t position = 0;
        for(auto it = inputnum.begin();it!=inputnum.end();++it){
            if(*it=='=')
                break;
            else
                position++;
        }
        outcome = inputnum.substr(position+1,length-position-1);
        inputnum = inputnum.substr(0,position+1);
        Inputnumsc = QString::fromStdString(outcome);
        Inputexpsc = QString::fromStdString(inputnum);
        ui->labelsc->setText(Inputnumsc);
        ui->expressionsc->setText(Inputexpsc);
        Inputexpsc = QString();
        afequ_numsc = 1;
        Inputnumsc = QString();
        errorflagsc = 0;
    }
    else
        pass;
}


void Calculator::on_Button_histsc1_clicked()
{
    QString item = ui->Button_histsc1->text();
    this->clickhistsc(item);
}


void Calculator::on_Button_histsc2_clicked()
{
    QString item = ui->Button_histsc2->text();
    this->clickhistsc(item);
}


void Calculator::on_Button_histsc3_clicked()
{
    QString item = ui->Button_histsc3->text();
    this->clickhistsc(item);
}


void Calculator::on_Button_histsc4_clicked()
{
    QString item = ui->Button_histsc4->text();
    this->clickhistsc(item);
}


void Calculator::on_Button_histsc5_clicked()
{
    QString item = ui->Button_histsc5->text();
    this->clickhistsc(item);
}


void Calculator::on_Button_histsc6_clicked()
{
    QString item = ui->Button_histsc6->text();
    this->clickhistsc(item);
}


void Calculator::on_Button_histsc7_clicked()
{
    QString item = ui->Button_histsc7->text();
    this->clickhistsc(item);
}


void Calculator::on_Button_histsc8_clicked()
{
    QString item = ui->Button_histsc8->text();
    this->clickhistsc(item);
}


void Calculator::on_actionClock_Mode_triggered()
{
    clock->show();//时钟模式显示
    this->hide();//隐藏计算器界面
    clock->exec();//等待时钟模式退出
    this->show();//显示计算器界面
}



void Calculator::on_Button_trashsc_clicked()
{
    QString st;
    ui->Button_histsc8->setText(st);
    ui->Button_histsc7->setText(st);
    ui->Button_histsc6->setText(st);
    ui->Button_histsc5->setText(st);
    ui->Button_histsc4->setText(st);
    ui->Button_histsc3->setText(st);
    ui->Button_histsc2->setText(st);
    ui->Button_histsc1->setText(st);
}
